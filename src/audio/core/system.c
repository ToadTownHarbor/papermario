#include "audio.h"
#include "audio/core.h"

NOP_FIX

u8 nuAuPreNMI = 0;
NUAuPreNMIFunc nuAuPreNMIFunc = NULL;
s32 nuAuDmaNext = 0;
u32 nuAuFrameCounter = 0;
u8 nuAuTaskStop = NU_AU_TASK_RUN;
u8 volatile AuSynUseStereo = true;

BSS u16 AuInitialGlobalVolume;
BSS OSThread nuAuMgrThread;
BSS char D_800A1108[0x408];
BSS u64 AuStack[NU_AU_STACK_SIZE / sizeof(u64)];
BSS Acmd* AlCmdListBuffers[3];
BSS NUScTask nuAuTasks[3];
BSS u8* D_800A3628[3];
BSS s32 AlFrameSize;
BSS s32 AlMinFrameSize;
BSS s32 D_800A363C;
BSS OSMesgQueue nuAuDmaMesgQ;
BSS OSMesg nuAuDmaMesgBuf[50];
BSS OSIoMesg nuAuDmaIOMesgBuf[50];
BSS NUDMAState nuAuDmaState;
BSS s32 D_800A3BDC;
BSS NUDMABuffer nuAuDmaBufList[50];

ALHeap nuAuHeap;
AuSynDriver auSynDriver;
#if !VERSION_IQUE
u8 rspbootUcodeBuffer[0x100] ALIGNED(16);
#endif

extern u64 n_aspMain_text_bin[];
extern u64 n_aspMain_data_bin[];

extern u8 AuHeapBase[AUDIO_HEAP_SIZE];

void create_audio_system(void) {
    u32 i;
    u32 outputRate, frameSize;
    ALConfig config;

    nuAuTaskStop = NU_AU_TASK_RUN;
    nuAuPreNMI = 0;
    alHeapInit(&nuAuHeap, AuHeapBase, AUDIO_HEAP_SIZE);
    config.num_pvoice = 24;
    config.num_bus = 4;
    outputRate = osAiSetFrequency(HARDWARE_OUTPUT_RATE);
    frameSize = (nusched.retraceCount * outputRate + (VIDEO_FRAMES_PER_SECOND - 1)) / VIDEO_FRAMES_PER_SECOND;
    config.outputRate = outputRate;
    config.unused_0C = 0;
    config.heap = &nuAuHeap;
    config.dmaNew = nuAuDmaNew;
    AlFrameSize = ((frameSize / AUDIO_SAMPLES) + 1) * AUDIO_SAMPLES;
    AlMinFrameSize = AlFrameSize - AUDIO_MAX_SAMPLES;

    for (i = 0; i < ARRAY_COUNT(AlCmdListBuffers); i++) {
        AlCmdListBuffers[i] = alHeapAlloc(config.heap, 1, AUDIO_COMMAND_LIST_BUFFER_SIZE);
    }

    for (i = 0; i < ARRAY_COUNT(nuAuTasks); i++) {
        nuAuTasks[i].next = NULL;
        nuAuTasks[i].msg = 0;
        nuAuTasks[i].list.t.type = M_AUDTASK;
#if VERSION_IQUE
        nuAuTasks[i].list.t.ucode_boot = (u64*) rspbootTextStart;
        nuAuTasks[i].list.t.ucode_boot_size = (u32) rspbootTextEnd - (u32) rspbootTextStart;
#else
        nuAuTasks[i].list.t.ucode_boot = (u64*) rspbootUcodeBuffer;
        nuAuTasks[i].list.t.ucode_boot_size = 0x100;
#endif
        nuAuTasks[i].list.t.ucode = n_aspMain_text_bin;
        nuAuTasks[i].list.t.ucode_data = n_aspMain_data_bin;
        nuAuTasks[i].list.t.ucode_data_size = SP_UCODE_DATA_SIZE;
        nuAuTasks[i].list.t.dram_stack = NULL;
        nuAuTasks[i].list.t.dram_stack_size = 0;
        nuAuTasks[i].list.t.output_buff = NULL;
        nuAuTasks[i].list.t.output_buff_size = 0;
        nuAuTasks[i].list.t.yield_data_ptr = NULL;
        nuAuTasks[i].list.t.yield_data_size = 0;
    }

    for (i = 0; i < ARRAY_COUNT(D_800A3628); i++) {
        D_800A3628[i] = alHeapAlloc(config.heap, 1, AlFrameSize * 4);
    }

    nuAuDmaBufList[0].node.next = nuAuDmaBufList[0].node.prev = NULL;
    for (i = 0; i < ARRAY_COUNT(nuAuDmaBufList) - 1; i++) {
        alLink(&nuAuDmaBufList[i+1].node, &nuAuDmaBufList[i].node);
        nuAuDmaBufList[i].ptr = alHeapAlloc(config.heap, 1, 0x500);
    }
    nuAuDmaBufList[i].ptr = alHeapAlloc(config.heap, 1, 0x500);

    osCreateMesgQueue(&nuAuDmaMesgQ, nuAuDmaMesgBuf, 50);
    nuAuPreNMIFunc = nuAuPreNMIProc;
    au_driver_init(&auSynDriver, &config);
    au_engine_init(config.outputRate);
    osCreateThread(&nuAuMgrThread, THREAD_ID_AUDIO, nuAuMgr, NULL, &AuStack[NU_AU_STACK_SIZE / sizeof(u64)], NU_AU_MGR_THREAD_PRI);
    osStartThread(&nuAuMgrThread);
}

void nuAuPreNMIFuncSet(NUAuPreNMIFunc func) {
    OSIntMask mask = osSetIntMask(OS_IM_NONE);

    nuAuPreNMIFunc = func;
    osSetIntMask(mask);
}

void nuAuMgr(void* arg) {
    NUScClient auClient;
    OSMesgQueue auMesgQ;
    OSMesg auMsgBuf[NU_AU_MESG_MAX];
    OSMesgQueue auRtnMesgQ;
    OSMesg auRtnMesgBuf;
    NUScMsg* mesg_type;
    s32 cmdList_len;
    Acmd* cmdListAfter_ptr;
    s32 cmdListIndex;
    s32 bufferIndex;
    s32 sampleSize;
    Acmd* cmdListBuf;
    u8* bufferPtr;
    s32 samples;
    s32 cond;

    osCreateMesgQueue(&auMesgQ, auMsgBuf, NU_AU_MESG_MAX);
    osCreateMesgQueue(&auRtnMesgQ, &auRtnMesgBuf, 1);
    nuScAddClient(&auClient, &auMesgQ, NU_SC_RETRACE_MSG | NU_SC_PRENMI_MSG);

    cmdList_len = 0;
    cmdListIndex = 0;
    bufferIndex = 0;
    samples = 0;
    cmdListBuf = AlCmdListBuffers[0];
    bufferPtr = D_800A3628[0];
    while (true) {
        osRecvMesg(&auMesgQ, (OSMesg*)&mesg_type, OS_MESG_BLOCK);
        switch (*mesg_type) {
            case NU_SC_RETRACE_MSG:
                if (cmdList_len != 0 && nuAuTaskStop == NU_AU_TASK_RUN) {
                    nuAuTasks[cmdListIndex].msgQ = &auRtnMesgQ;
                    nuAuTasks[cmdListIndex].list.t.data_ptr = (u64*)cmdListBuf;
                    nuAuTasks[cmdListIndex].list.t.data_size = (cmdListAfter_ptr - cmdListBuf) * sizeof(Acmd);
                    osSendMesg(&nusched.audioRequestMQ, &nuAuTasks[cmdListIndex], OS_MESG_BLOCK);
                    nuAuCleanDMABuffers();
                    osRecvMesg(&auRtnMesgQ, NULL, 1);
                    if (++bufferIndex == 3) {
                        bufferIndex = 0;
                    }
                    if (++cmdListIndex == 3) {
                        cmdListIndex = 0;
                    }
                }
                if (osAiGetStatus() & AI_STATUS_FIFO_FULL) {
                    cond = false;
                    continue;
                }
                sampleSize = osAiGetLength() >> 2;
                if (cmdList_len != 0 && nuAuTaskStop == NU_AU_TASK_RUN) {
                    osAiSetNextBuffer(bufferPtr, samples * 4);
                    cmdListBuf = AlCmdListBuffers[cmdListIndex];
                    bufferPtr = D_800A3628[bufferIndex];
                }
                if (sampleSize < AUDIO_MAX_SAMPLES || cond) {
                    samples = AlFrameSize;
                    cond = false;
                } else {
                    samples = AlMinFrameSize;
                    cond = true;
                }
                cmdListAfter_ptr = alAudioFrame(cmdListBuf, &cmdList_len, (s16*)osVirtualToPhysical(bufferPtr), samples);
                if (nuAuPreNMIFunc != 0 && nuAuPreNMI != 0) {
                    nuAuPreNMIFunc(NU_SC_RETRACE_MSG, nuAuPreNMI);
                    nuAuPreNMI++;
                }

                break;
            case NU_SC_PRENMI_MSG:
                if (nuAuPreNMIFunc) {
                    nuAuPreNMIFunc(NU_SC_PRENMI_MSG, nuAuPreNMI);
                }
                nuAuPreNMI++;
                break;
        }
    }
}

/// DMA callback for audio sample streaming; manages a DMA buffer cache.
s32 nuAuDmaCallBack(s32 addr, s32 len, void *state, u8 useDma) {
    NUDMABuffer* dmaPtr;
    NUDMABuffer* freeBuffer;
    OSIoMesg* mesg;
    s32 delta;
    u32 startAddr;
    s32 addrEnd, buffEnd;
    NUDMABuffer* lastDmaPtr;

    if (!useDma) {
        return osVirtualToPhysical((void*)addr);
    }

    lastDmaPtr = NULL;
    dmaPtr = nuAuDmaState.firstUsed;
    addrEnd = addr + len;

    while (dmaPtr != NULL) {
        startAddr = dmaPtr->startAddr;
        buffEnd = dmaPtr->startAddr + 0x500;
        if (addr >= startAddr && buffEnd >= addrEnd) {
            dmaPtr->frameCnt = nuAuFrameCounter;
            freeBuffer = (NUDMABuffer*)(dmaPtr->ptr + addr - dmaPtr->startAddr);
            return osVirtualToPhysical(freeBuffer);
        } else if (addr < startAddr) {
            break;
        }
        lastDmaPtr = dmaPtr;
        dmaPtr = (NUDMABuffer*)dmaPtr->node.next;
    }

    dmaPtr = nuAuDmaState.firstFree;
    if (dmaPtr == NULL) {
        return osVirtualToPhysical(nuAuDmaState.firstUsed);
    }

    nuAuDmaState.firstFree = (NUDMABuffer*)dmaPtr->node.next;
    alUnlink(&dmaPtr->node);

    if (lastDmaPtr != NULL) {
        alLink(&dmaPtr->node, &lastDmaPtr->node);
    } else if (nuAuDmaState.firstUsed != NULL){
        lastDmaPtr = nuAuDmaState.firstUsed;
        nuAuDmaState.firstUsed = dmaPtr;
        dmaPtr->node.next = &lastDmaPtr->node;
        dmaPtr->node.prev = NULL;
        lastDmaPtr->node.prev = &dmaPtr->node;
    } else {
        nuAuDmaState.firstUsed = dmaPtr;
        dmaPtr->node.next = NULL;
        dmaPtr->node.prev = NULL;
    }

    freeBuffer = (NUDMABuffer*)dmaPtr->ptr;
    delta = addr & 1;
    addr -= delta;
    dmaPtr->startAddr = addr;
    dmaPtr->frameCnt = nuAuFrameCounter;

    mesg = &nuAuDmaIOMesgBuf[nuAuDmaNext++];
    mesg->hdr.pri = OS_MESG_PRI_NORMAL;
    mesg->hdr.retQueue = &nuAuDmaMesgQ;
    mesg->dramAddr = freeBuffer;
    mesg->devAddr = addr;
    mesg->size = 0x500;
    osEPiStartDma(nuPiCartHandle, mesg, 0);
    return osVirtualToPhysical(freeBuffer) + delta;
}

/// Initializes the audio DMA state and returns the DMA callback.
/// Called once per AuPVoice initialization in au_driver_init to assign callbacks to them.
ALDMAproc nuAuDmaNew(NUDMAState** state) {
    if (!nuAuDmaState.initialized) {
        nuAuDmaState.firstFree = &nuAuDmaBufList[0];
        nuAuDmaState.firstUsed = NULL;
        nuAuDmaState.initialized = true;
    }

    nuAuDmaNext = 0;
    *state = &nuAuDmaState;
    return (ALDMAproc)nuAuDmaCallBack;
}

/// Recycles DMA buffers which are no longer in use (based on frame count).
void nuAuCleanDMABuffers(void) {
    NUDMAState* state = &nuAuDmaState;
    NUDMABuffer* dmaPtr = state->firstUsed;

    // A bit odd, this
    do {
        NUDMAState* state = &nuAuDmaState;
        NUDMABuffer* nextPtr;
        u32* frameCounter;

        while (dmaPtr != NULL) {
            nextPtr = (NUDMABuffer*)dmaPtr->node.next;

            if (dmaPtr->frameCnt + 1 < nuAuFrameCounter) {
                if (state->firstUsed == dmaPtr) {
                    state->firstUsed = nextPtr;
                }

                alUnlink(&dmaPtr->node);

                if (state->firstFree != NULL) {
                    alLink(&dmaPtr->node, &state->firstFree->node);
                } else {
                    state->firstFree = dmaPtr;
                    dmaPtr->node.next = NULL;
                    dmaPtr->node.prev = NULL;
                }
            }

            dmaPtr = nextPtr;
        }

        nuAuDmaNext = 0;
        frameCounter = &nuAuFrameCounter;
        (*frameCounter)++;
    } while (0);
}

/// Handles global audio fade-out during system resets (NMI).
void nuAuPreNMIProc(NUScMsg mesg_type, u32 frameCounter) {
    s16 maxVol;
    s32 vol;

    switch (mesg_type) {
        case NU_SC_PRENMI_MSG:
            AuInitialGlobalVolume = au_get_global_volume();
            au_use_global_volume();
            break;
        case NU_SC_RETRACE_MSG:
            maxVol = AuInitialGlobalVolume;
            vol = maxVol - (maxVol / 20) * frameCounter;

            if (vol < 0) {
                vol = 0;
            }

            vol = SQ(vol) >> 15;
            au_set_global_volume(vol);

            if (vol == 0) {
                nuAuTaskStop = NU_AU_TASK_STOP;
            }
            break;
    }
}

/// Links a new element into a doubly-linked list.
void alLink(ALLink* element, ALLink* after) {
    element->next = after->next;
    element->prev = after;

    if (after->next != NULL) {
        after->next->prev = element;
    }
    after->next = element;
}

/// Unlinks a list element from a doubly-linked list.
void alUnlink(ALLink* element) {
    if (element->next != NULL) {
        element->next->prev = element->prev;
    }

    if (element->prev != NULL) {
        element->prev->next = element->next;
    }
}
