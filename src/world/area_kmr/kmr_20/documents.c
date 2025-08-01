#include "kmr_20.h"
#include "hud_element.h"
#include "ld_addrs.h"
#include "charset/charset.h"

#define PACKED_BYTE(base, i) \
    (i / 8) + EVT_INDEX_OF_GAME_BYTE(base)

#define GET_PACKED_FLAG(base, i) \
    get_global_byte((i / 8) + EVT_INDEX_OF_GAME_BYTE(base)) & (1 << (i % 8))

#define SET_PACKED_FLAG(base, i) \
    get_global_byte((i / 8) + EVT_INDEX_OF_GAME_BYTE(base)) | (1 << (i % 8))

extern HudScript HES_Item_Postcard;
extern HudScript HES_Item_Postcard_disabled;
extern HudScript HES_Item_EmptyBook;
extern HudScript HES_Item_EmptyBook_disabled;

BSS PopupMenu N(ChooseDocumentPopupMenu);
BSS IMG_BIN N(LetterBackgroundImg)[0x3D86] ALIGNED(16);
BSS PAL_BIN N(LetterBackgroundPal)[0x100];
BSS char N(static_pad)[0x8]; // padding
BSS IMG_BIN N(LetterPhotoImg)[0x3D86];
BSS PAL_BIN N(N(LetterPhotoPal))[0x100];
BSS s32 N(LetterWorkerID);
BSS s32 N(LetterAlpha);
BSS MessageImageData N(LetterImgData)[2];

#define NAME_SUFFIX _Unused
#include "common/foliage.inc.c"
#define NAME_SUFFIX

void N(worker_draw_letter)(void) {
    N(LetterAlpha) += 20;
    if (N(LetterAlpha) > 255) {
        N(LetterAlpha) = 255;
    }

    draw_ci_image_with_clipping(N(LetterImgData)[0].raster,
        N(LetterImgData)[0].width, N(LetterImgData)[0].height,
        N(LetterImgData)[0].format, N(LetterImgData)[0].bitDepth,
        N(LetterImgData)[0].palette,
        85, 97,
        0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
        N(LetterAlpha));

    draw_ci_image_with_clipping(N(LetterImgData)[1].raster,
        N(LetterImgData)[1].width, N(LetterImgData)[1].height,
        N(LetterImgData)[1].format, N(LetterImgData)[1].bitDepth,
        N(LetterImgData)[1].palette,
        160, 102,
        0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
        N(LetterAlpha));
}

s32 N(LetterStoryRequirements)[] = {
    STORY_INTRO,
    STORY_CH4_PULLED_SWITCH_SWITCH,
    STORY_CH1_STAR_SPIRIT_RESCUED,
    STORY_CH2_STAR_SPIRIT_RESCUED,
    STORY_CH3_STAR_SPIRIT_RESCUED,
    STORY_CH3_STAR_SPIRIT_RESCUED,
    STORY_CH4_STAR_SPIRIT_RESCUED,
    STORY_CH4_GAVE_CAKE_TO_GOURMET_GUY,
    STORY_CH5_RETURNED_TO_TOAD_TOWN,
    STORY_CH6_STAR_SPRIT_DEPARTED,
    STORY_CH4_GOT_TAYCE_TS_CAKE,
    STORY_CH7_STAR_SPIRIT_RESCUED,
};

s32 N(LetterSenderStringIDs)[] = {
    MSG_Document_Letter_Mario01_From,
    MSG_Document_Letter_Mario02_From,
    MSG_Document_Letter_Mario03_From,
    MSG_Document_Letter_Mario04_From,
    MSG_Document_Letter_Mario05_From,
    MSG_Document_Letter_Mario06_From,
    MSG_Document_Letter_Mario07_From,
    MSG_Document_Letter_Mario08_From,
    MSG_Document_Letter_Mario09_From,
    MSG_Document_Letter_Mario10_From,
    MSG_Document_Letter_Mario11_From,
    MSG_Document_Letter_Mario12_From,
};

s32 N(LetterBodyStringIDs)[] = {
    MSG_Document_Letter_Mario01_Body,
    MSG_Document_Letter_Mario02_Body,
    MSG_Document_Letter_Mario03_Body,
    MSG_Document_Letter_Mario04_Body,
    MSG_Document_Letter_Mario05_Body,
    MSG_Document_Letter_Mario06_Body,
    MSG_Document_Letter_Mario07_Body,
    MSG_Document_Letter_Mario08_Body,
    MSG_Document_Letter_Mario09_Body,
    MSG_Document_Letter_Mario10_Body,
    MSG_Document_Letter_Mario11_Body,
    MSG_Document_Letter_Mario12_Body,
};

// TODO mix of image / palette pointers
IMG_PTR N(LetterDmaOffsets)[] = {
    charset_postcard_OFFSET, (IMG_PTR) charset_postcard_pal_OFFSET,
    charset_letter_content_1_OFFSET, (IMG_PTR) charset_letter_content_1_pal_OFFSET,
    charset_letter_content_2_OFFSET, (IMG_PTR) charset_letter_content_2_pal_OFFSET,
    charset_letter_content_3_OFFSET, (IMG_PTR) charset_letter_content_3_pal_OFFSET,
    charset_letter_content_4_OFFSET, (IMG_PTR) charset_letter_content_4_pal_OFFSET,
    charset_letter_content_5_OFFSET, (IMG_PTR) charset_letter_content_5_pal_OFFSET,
    charset_letter_content_6_OFFSET, (IMG_PTR) charset_letter_content_6_pal_OFFSET,
    charset_letter_content_7_OFFSET, (IMG_PTR) charset_letter_content_7_pal_OFFSET,
    charset_letter_content_8_OFFSET, (IMG_PTR) charset_letter_content_8_pal_OFFSET,
    charset_letter_content_9_OFFSET, (IMG_PTR) charset_letter_content_9_pal_OFFSET,
    charset_letter_content_10_OFFSET, (IMG_PTR) charset_letter_content_10_pal_OFFSET,
    charset_letter_content_11_OFFSET, (IMG_PTR) charset_letter_content_11_pal_OFFSET,
    charset_letter_content_12_OFFSET, (IMG_PTR) charset_letter_content_12_pal_OFFSET,
};

API_CALLABLE(N(InitLetters)){
    script->functionTemp[3] = 0;
    return ApiStatus_DONE2;
}

API_CALLABLE(N(ReadLetters)){
    PopupMenu* menu = &N(ChooseDocumentPopupMenu);
    s32 temp;
    s32 userIndex;
    s32 storyReq;
    s32 flags;
    s32 cond;
    s32 numEntries;
    s32 i;

    if (isInitialCall) {
        script->functionTemp[0] = 0;
    }

    switch (script->functionTemp[0]) {
        case 0:
            numEntries = 0;

            for (i = 0; i < ARRAY_COUNT(N(LetterStoryRequirements)); i++) {
                cond = false;
                storyReq = N(LetterStoryRequirements)[i];
                switch (storyReq) {
                    case STORY_CH4_PULLED_SWITCH_SWITCH:
                        if (gPlayerData.quizzesCorrect != 0) {
                            cond = true;
                        }
                        break;
                    case STORY_CH4_GAVE_CAKE_TO_GOURMET_GUY:
                        if (evt_get_variable(NULL, GF_MAC02_TayceT_HasCookbook) != 0) {
                            cond = true;
                        }
                        break;
                    case STORY_CH4_GOT_TAYCE_TS_CAKE:
                        temp = false;
                        if (evt_get_variable(NULL, GB_StoryProgress) >= STORY_CH4_BEGAN_PEACH_MISSION) {
                            temp = (evt_get_variable(NULL, GF_MAC01_BoughtBadgeFromRowf) != 0);
                        }
                        if (temp) {
                            cond = true;
                        }
                        break;
                    default:
                        if (evt_get_variable(NULL, GB_StoryProgress) >= storyReq) {
                            cond = true;
                        }
                        break;
                }

                if (cond) {
                    menu->ptrIcon[numEntries] = &HES_Item_Postcard;
                    menu->userIndex[numEntries] = i;
                    menu->enabled[numEntries] = true;
                    menu->nameMsg[numEntries] = N(LetterSenderStringIDs)[i];
                    menu->descMsg[numEntries] = 0;
                    temp = GET_PACKED_FLAG(GB_KMR20_MarioReadLetterFlags_00, i);

                    menu->value[numEntries] = 0;
                    if (temp) {
                        menu->value[numEntries] = 1;
                        menu->ptrIcon[numEntries] = &HES_Item_Postcard_disabled;
                    }
                    numEntries++;
                }
            }

            if (numEntries == 0) {
                script->varTable[0] = 0;
                return ApiStatus_DONE2;
            }
            menu->popupType = POPUP_MENU_READ_POSTCARD;
            menu->numEntries = numEntries;
            menu->initialPos = script->functionTemp[3];
            create_standard_popup_menu(menu);
            status_bar_respond_to_changes();
            close_status_bar();
            script->functionTemp[1] = 0;
            script->functionTemp[0] = 1;
            break;
        case 1:
            if (script->functionTemp[1] == 0) {
                script->functionTemp[2] = menu->result;
                if (script->functionTemp[2] != POPUP_RESULT_CHOOSING) {
                    hide_popup_menu();
                } else {
                    break;
                }
            }

            script->functionTemp[1]++;
            if (script->functionTemp[1] >= 15) {
                destroy_popup_menu();
                if (script->functionTemp[2] == POPUP_RESULT_CANCEL) {
                    script->varTable[0] = -1;
                    return ApiStatus_DONE2;
                }
                script->functionTemp[3] = script->functionTemp[2] - 1;
                userIndex = menu->userIndex[script->functionTemp[2] - 1];
                script->varTable[0] = N(LetterBodyStringIDs)[userIndex];

                flags = SET_PACKED_FLAG(GB_KMR20_MarioReadLetterFlags_00, userIndex);
                set_global_byte(PACKED_BYTE(GB_KMR20_MarioReadLetterFlags_00, userIndex), flags);
                if (userIndex == 3) {
                    evt_set_variable(NULL, GF_KMR20_ReadThankYouLetterFromKoopaVillage, true);
                }

                dma_copy(charset_ROM_START + (s32) N(LetterDmaOffsets)[0],
                         charset_ROM_START + (s32) N(LetterDmaOffsets)[0] + sizeof(N(LetterBackgroundImg)),
                         N(LetterBackgroundImg));
                dma_copy(charset_ROM_START + (s32) N(LetterDmaOffsets)[1],
                         charset_ROM_START + (s32) N(LetterDmaOffsets)[1] + sizeof(N(LetterBackgroundPal)),
                         N(LetterBackgroundPal));

                N(LetterImgData)[0].raster = N(LetterBackgroundImg);
                N(LetterImgData)[0].palette = N(LetterBackgroundPal);
                N(LetterImgData)[0].width = 150;
                N(LetterImgData)[0].height = 105;
                N(LetterImgData)[0].format = G_IM_FMT_CI;
                N(LetterImgData)[0].bitDepth = G_IM_SIZ_4b;

                dma_copy(charset_ROM_START + (s32) N(LetterDmaOffsets)[(userIndex * 2) + 2],
                         charset_ROM_START + (s32) N(LetterDmaOffsets)[(userIndex * 2) + 2] + sizeof(N(LetterPhotoImg)),
                         N(LetterPhotoImg));
                dma_copy(charset_ROM_START + (s32) N(LetterDmaOffsets)[(userIndex * 2) + 3],
                         charset_ROM_START + (s32) N(LetterDmaOffsets)[(userIndex * 2) + 3] + sizeof(N(N(LetterPhotoPal))),
                         N(N(LetterPhotoPal)));

                N(LetterImgData)[1].raster = N(LetterPhotoImg);
                N(LetterImgData)[1].palette = N(N(LetterPhotoPal));
                N(LetterImgData)[1].width = 70;
                N(LetterImgData)[1].height = 95;
                N(LetterImgData)[1].format = G_IM_FMT_CI;
                N(LetterImgData)[1].bitDepth = G_IM_SIZ_8b;

                N(LetterWorkerID) = create_worker_frontUI(NULL, N(worker_draw_letter));
                N(LetterAlpha) = 0;
                return ApiStatus_DONE2;
            }
            break;
    }
    return ApiStatus_BLOCK;
}

API_CALLABLE(N(CleanupLetters)){
    free_worker(N(LetterWorkerID));
    return ApiStatus_DONE2;
}

EvtScript N(EVS_Inspect_Letters) = {
    Call(DisablePlayerInput, true)
    IfLt(GB_StoryProgress, STORY_CH0_TWINK_GAVE_LUCKY_STAR)
        Set(GF_KMR20_CheckedDeskForMail, true)
        Call(ShowMessageAtScreenPos, MSG_CH0_0102, 160, 40)
        Call(DisablePlayerInput, false)
        Return
    Else
        IfEq(GF_KMR20_CheckedDeskForMail, false)
            Set(GF_KMR20_CheckedDeskForMail, true)
            Call(ShowMessageAtScreenPos, MSG_CH0_0101, 160, 40)
            Wait(5)
        EndIf
    EndIf
    Call(N(InitLetters))
    Label(0)
        Call(N(ReadLetters))
        IfEq(LVar0, -1)
            Goto(10)
        EndIf
        IfEq(LVar0, 0)
            Goto(10)
        EndIf
    Call(ShowMessageAtScreenPos, LVar0, 160, 40)
    Call(N(CleanupLetters))
    Goto(0)
    Label(10)
    Call(DisablePlayerInput, false)
    Return
    End
};

s32 N(DiaryStoryRequirements)[] = {
    STORY_CH3_GOT_SUPER_BOOTS,
    STORY_CH3_GOT_SUPER_BOOTS,
    STORY_CH4_STAR_SPRIT_DEPARTED,
    STORY_CH4_STAR_SPRIT_DEPARTED,
    STORY_CH5_TRADED_VASE_FOR_SEED,
    STORY_CH5_TRADED_VASE_FOR_SEED,
    STORY_CH6_RETURNED_TO_TOAD_TOWN,
    STORY_CH6_RETURNED_TO_TOAD_TOWN,
    STORY_CH7_STAR_SPRIT_DEPARTED,
    STORY_CH7_STAR_SPRIT_DEPARTED,
    STORY_EPILOGUE,
    STORY_EPILOGUE,
};

s32 N(DiaryEntryStringIDs)[] = {
    MSG_Document_LuigisDiary_01,
    MSG_Document_LuigisDiary_02,
    MSG_Document_LuigisDiary_03,
    MSG_Document_LuigisDiary_04,
    MSG_Document_LuigisDiary_05,
    MSG_Document_LuigisDiary_06,
    MSG_Document_LuigisDiary_07,
    MSG_Document_LuigisDiary_08,
    MSG_Document_LuigisDiary_09,
    MSG_Document_LuigisDiary_10,
    MSG_Document_LuigisDiary_11,
    MSG_Document_LuigisDiary_12,
};

API_CALLABLE(N(InitDiary)){
    script->functionTemp[3] = 0;
    return ApiStatus_DONE2;
}

API_CALLABLE(N(ReadDiary)){
    PopupMenu* menu = &N(ChooseDocumentPopupMenu);
    s32 userIndex;
    s32 numEntries;
    s32 flags;
    s32 i;

    if (isInitialCall) {
        script->functionTemp[0] = 0;
    }

    switch (script->functionTemp[0]) {
        case 0:
            numEntries = 0;
            for (i = 0; i < ARRAY_COUNT(N(DiaryStoryRequirements)); i++) {
                if (evt_get_variable(NULL, GB_StoryProgress) >= N(DiaryStoryRequirements)[i]) {
                    menu->ptrIcon[numEntries] = &HES_Item_EmptyBook;
                    menu->userIndex[numEntries] = i;
                    menu->enabled[numEntries] = true;
                    menu->nameMsg[numEntries] = 0;
                    menu->descMsg[numEntries] = 0;
                    flags = GET_PACKED_FLAG(GB_KMR20_MarioReadDiaryFlags_00, i);

                    menu->value[numEntries] = 0;
                    if (flags) {
                        menu->value[numEntries] = 1;
                        menu->ptrIcon[numEntries] = &HES_Item_EmptyBook_disabled;
                    }
                    numEntries++;
                }
            }

            if (numEntries == 0) {
                script->varTable[0] = 0;
                return ApiStatus_DONE2;
            }
            menu->popupType = POPUP_MENU_READ_DIARY_PAGE;
            menu->numEntries = numEntries;
            menu->initialPos = script->functionTemp[3];
            create_standard_popup_menu(menu);
            status_bar_respond_to_changes();
            close_status_bar();
            script->functionTemp[1] = 0;
            script->functionTemp[0] = 1;
            break;
        case 1:
            if (script->functionTemp[1] == 0) {
                script->functionTemp[2] = menu->result;
                if (script->functionTemp[2] != POPUP_RESULT_CHOOSING) {
                    hide_popup_menu();
                } else {
                    break;
                }
            }
            script->functionTemp[1]++;
            if (script->functionTemp[1] >= 15) {
                destroy_popup_menu();
                if (script->functionTemp[2] == POPUP_RESULT_CANCEL) {
                    script->varTable[0] = -1;
                    return ApiStatus_DONE2;
                }
                script->functionTemp[3] = script->functionTemp[2] - 1;
                userIndex = menu->userIndex[script->functionTemp[2] - 1];
                script->varTable[0] = N(DiaryEntryStringIDs)[userIndex];
                // separate lines required to match, cant combine macro
                flags = SET_PACKED_FLAG(GB_KMR20_MarioReadDiaryFlags_00, userIndex);
                set_global_byte(PACKED_BYTE(GB_KMR20_MarioReadDiaryFlags_00, userIndex), flags);
                return ApiStatus_DONE2;
            }
            break;
    }
    return ApiStatus_BLOCK;
}

API_CALLABLE(N(MuteAmbienceVolume_Documents)){
    snd_ambient_set_volume(0, 1000, 1);
    return ApiStatus_DONE2;
}

EvtScript N(EVS_Inspect_LuigisDiary) = {
    Call(DisablePlayerInput, true)
    IfLt(GB_StoryProgress, STORY_CH3_STAR_SPIRIT_RESCUED)
        IfEq(AF_KMR_0C, true)
            Call(ShowMessageAtScreenPos, MSG_CH0_00EC, 160, 40)
            Call(DisablePlayerInput, false)
            Return
        EndIf
    EndIf
    IfEq(GF_KMR20_FoundLuigisDiary, false)
        Set(GF_KMR20_FoundLuigisDiary, true)
        Call(ShowMessageAtScreenPos, MSG_CH0_0100, 160, 40)
        Wait(5)
    EndIf
    Call(N(InitDiary))
    Label(0)
        Call(N(ReadDiary))
        IfEq(LVar0, -1)
            Goto(10)
        EndIf
        IfEq(LVar0, 0)
            Goto(10)
        EndIf
        Call(ShowMessageAtScreenPos, LVar0, 160, 40)
        Goto(0)
    Label(10)
    Call(DisablePlayerInput, false)
    Return
    End
};

EvtScript N(EVS_Shake_Mailbox) = {
    Call(PlaySoundAtCollider, COLLIDER_o305, SOUNC_CHECK_MAILBOX, SOUND_SPACE_DEFAULT)
    Call(TranslateModel, MODEL_o222, 2, 0, 0)
    Call(TranslateModel, MODEL_o223, 3, 0, 0)
    Call(TranslateModel, MODEL_o224, 1, 0, 0)
    Wait(1)
    Call(TranslateModel, MODEL_o222, -3, 0, 0)
    Call(TranslateModel, MODEL_o223, -3, 0, 0)
    Call(TranslateModel, MODEL_o224, -2, 0, 0)
    Wait(1)
    Call(TranslateModel, MODEL_o222, 2, 0, 0)
    Call(TranslateModel, MODEL_o223, 3, 0, 0)
    Call(TranslateModel, MODEL_o224, 1, 0, 0)
    Wait(1)
    Call(TranslateModel, MODEL_o222, -2, 0, 0)
    Call(TranslateModel, MODEL_o223, -3, 0, 0)
    Call(TranslateModel, MODEL_o224, -1, 0, 0)
    Wait(1)
    Call(TranslateModel, MODEL_o222, 0, 0, 0)
    Call(TranslateModel, MODEL_o223, 0, 0, 0)
    Call(TranslateModel, MODEL_o224, 0, 0, 0)
    Return
    End
};

EvtScript N(EVS_SecretPanel_Flip) = {
    Call(ModifyColliderFlags, MODIFY_COLLIDER_FLAGS_SET_BITS, COLLIDER_o252, COLLIDER_FLAGS_UPPER_MASK)
    Call(PlaySoundAtCollider, COLLIDER_o252, SOUND_FLIP_PANEL, SOUND_SPACE_DEFAULT)
    Call(MakeLerp, 0, 2160, 60, EASING_QUADRATIC_OUT)
    Loop(0)
        Call(UpdateLerp)
        Call(RotateGroup, MODEL_g61, LVar0, 1, 0, 0)
        Wait(1)
        IfEq(LVar1, 0)
            BreakLoop
        EndIf
    EndLoop
    Call(PlaySoundAtCollider, COLLIDER_o252, SOUND_CLOSE_PANEL, SOUND_SPACE_DEFAULT)
    Call(ModifyColliderFlags, MODIFY_COLLIDER_FLAGS_CLEAR_BITS, COLLIDER_o252, COLLIDER_FLAGS_UPPER_MASK)
    Return
    End
};

EvtScript N(EVS_Setup_SecretPanel) = {
    Call(GetPlayerActionState, LVar0)
    IfNe(LVar0, ACTION_STATE_SPIN_POUND)
        IfNe(LVar0, ACTION_STATE_TORNADO_POUND)
            Return
        EndIf
    EndIf
    Call(DisablePlayerInput, true)
    Call(N(MuteAmbienceVolume_Documents))
    IfLt(GB_StoryProgress, STORY_CH3_STAR_SPIRIT_RESCUED)
        IfEq(AF_KMR_0C, true)
            Call(EnableModel, MODEL_o200, false)
        EndIf
    EndIf
    Loop(0)
        Wait(1)
        Call(GetPlayerActionState, LVar0)
        IfEq(LVar0, ACTION_STATE_IDLE)
            BreakLoop
        EndIf
    EndLoop
    Exec(N(EVS_SecretPanel_Flip))
    Wait(1)
    Call(SetPlayerPos, -150, 30, -35)
    Loop(0)
        Wait(1)
        Call(GetPlayerActionState, LVar0)
        IfEq(LVar0, ACTION_STATE_IDLE)
            BreakLoop
        EndIf
    EndLoop
    Call(DisablePlayerInput, false)
    IfEq(MF_LuigiInBasement, true)
        Set(MF_LuigiInBasement, false)
        Exec(N(EVS_Scene_CaughtLuigiInBasement))
    EndIf
    Return
    End
};

EvtScript N(EVS_Setup_Interactables) = {
    BindTrigger(Ref(N(EVS_Inspect_Letters)), TRIGGER_WALL_PRESS_A, COLLIDER_o251, 1, 0)
    BindTrigger(Ref(N(EVS_Inspect_LuigisDiary)), TRIGGER_WALL_PRESS_A, COLLIDER_o240, 1, 0)
    BindTrigger(Ref(N(EVS_Shake_Mailbox)), TRIGGER_WALL_PRESS_A, COLLIDER_o305, 1, 0)
    BindTrigger(Ref(N(EVS_Setup_SecretPanel)), TRIGGER_FLOOR_TOUCH, COLLIDER_o252, 1, 0)
    BindTrigger(Ref(N(EVS_Inspect_Records)), TRIGGER_WALL_PRESS_A, COLLIDER_o355, 1, 0)
    Return
    End
};
