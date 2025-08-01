#include "kkj_17.h"
#include "hud_element.h"
#include "sprite/player.h"

extern IconHudScriptPair gItemHudScripts[];

// ------------------------------------------------------
// begin modified Chest.inc.c
// differences are:
// - inclusion of N(ChestItems)
// - removal of N(EVS_Chest_GetItem)

#include "world/common/todo/StashVars.inc.c"
#include "world/common/todo/GetItemName.inc.c"
#include "world/common/todo/SomeItemEntityFunc.inc.c"
#include "world/common/todo/IsItemBadge.inc.c"

s32** N(varStash) = NULL;

EvtScript N(EVS_Chest_ShowGotItem) = {
    SetGroup(EVT_GROUP_NEVER_PAUSE)
    Call(SetTimeFreezeMode, TIME_FREEZE_FULL)
    Wait(40)
    Call(ShowGotItem, LVar0, false, 0)
    Call(SetTimeFreezeMode, TIME_FREEZE_NONE)
    Return
    Return
    End
};

s32 N(ChestItems)[] = {
    ITEM_POWER_RUSH,
    ITEM_SHOOTING_STAR,
    ITEM_DEEP_FOCUS_A,
    ITEM_LAST_STAND,
    ITEM_JAMMIN_JELLY,
};

// end modified Chest.inc.c
// ------------------------------------------------------

//TODO this whole file is probably an include shared with kkj_17, but the temp required in this function prevents deduplication
API_CALLABLE(N(ChestItemPrompt)) {
    PopupMenu *menu;
    s32 menuIdx;
    s32 selectIdx;
    s32 canUseItem;
    s32 itemUsedBefore;
    s32 i;

    if (isInitialCall) {
        menu = heap_malloc(sizeof(*menu));
        script->functionTempPtr[2] = menu;
        script->varTable[10] = script->varTable[0];

        // NOTE: identical to ChestItemPrompt in hos_06 EXCEPT here, where varTable[0] is required instead of varTable[10]
        if (script->varTable[0] == 0) {
            // storing items
            script->varTable[1] = GF_KKJ16_Item_PowerRush;
            script->varTable[2] = GF_KKJ_Stored_PowerRush;
            menu->popupType = POPUP_MENU_USEKEY;
        } else {
            // retrieving items
            script->varTable[1] = GF_KKJ_Stored_PowerRush;
            script->varTable[2] = GF_KKJ_Retrieved_PowerRush;
            menu->popupType = POPUP_MENU_TAKE_FROM_CHEST;
        }

        menuIdx = 0;
        for (i = 0; i < ARRAY_COUNT(N(ChestItems)); i++) {
            // meaning of 'can use' and 'used before' depends on type of chest interaction
            canUseItem = evt_get_variable(NULL, script->varTable[1] + i);
            itemUsedBefore = evt_get_variable(NULL, script->varTable[2] + i);
            if (canUseItem && !itemUsedBefore) {
                ItemData* item = &gItemTable[N(ChestItems)[i]];
                IconHudScriptPair* itemHudScripts = &gItemHudScripts[item->hudElemID];
                menu->ptrIcon[menuIdx] = itemHudScripts->enabled;
                menu->userIndex[menuIdx] = i;
                menu->enabled[menuIdx] = true;
                menu->nameMsg[menuIdx] = item->nameMsg;
                menu->descMsg[menuIdx] = item->shortDescMsg;
                menuIdx++;
            }
        }

        if (menuIdx == 0) {
            script->varTable[0] = 0;
            return ApiStatus_DONE1;
        }

        menu->numEntries = menuIdx;
        menu->initialPos = 0;
        create_standard_popup_menu(menu);
        script->functionTemp[0] = 0;
    }

    menu = script->functionTempPtr[2];
    if (script->functionTemp[0] == 0) {
        script->functionTemp[1] = menu->result;
        if (script->functionTemp[1] == POPUP_RESULT_CHOOSING) {
            return ApiStatus_BLOCK;
        }
        hide_popup_menu();
    }

    script->functionTemp[0]++;
    if (script->functionTemp[0] < 15) {
        return ApiStatus_BLOCK;
    }

    destroy_popup_menu();

    if (script->functionTemp[1] == POPUP_RESULT_CANCEL) {
        script->varTable[0] = -1;
    } else {
        selectIdx = menu->userIndex[script->functionTemp[1] - 1];
        script->varTable[0] = N(ChestItems)[selectIdx];
        if (script->varTable[10] == 0) {
            evt_set_variable(NULL, script->varTable[2] + selectIdx, 1);
        }
        heap_free(script->functionTempPtr[2]);
    }

    return ApiStatus_DONE2;
}

// assumes itemID on LVar0, sets GF_KKJ_Retrieved_* based on item list position
API_CALLABLE(N(SetItemRetrieved)) {
    s32 found = false;
    s32 i;

    for (i = 0; i < ARRAY_COUNT(N(ChestItems)); i++) {
        s32 listItemID = N(ChestItems)[i];

        if (script->varTable[0] == listItemID) {
            found = true;
            break;
        }
    }

    if (found) {
        evt_set_variable(NULL, GF_KKJ_Retrieved_PowerRush + i, true);
    }

    return ApiStatus_DONE2;
}

#include "world/common/todo/GetItemEmptyCount.inc.c"

EvtScript N(EVS_OpenChest) = {
    Call(PlaySoundAtCollider, COLLIDER_o89, SOUND_OPEN_MAGIC_CHEST, 0)
    Call(MakeLerp, 0, 80, 20, EASING_QUADRATIC_OUT)
    Loop(0)
        Call(UpdateLerp)
        Call(RotateGroup, MODEL_g21, LVar0, -1, 0, 0)
        Wait(1)
        IfEq(LVar1, 0)
            BreakLoop
        EndIf
    EndLoop
    Return
    End
};

EvtScript N(EVS_CloseChest) = {
    Call(MakeLerp, 80, 0, 20, EASING_QUADRATIC_OUT)
    Loop(0)
        Call(UpdateLerp)
        Call(RotateGroup, MODEL_g21, LVar0, -1, 0, 0)
        Wait(1)
        IfEq(LVar1, 0)
            BreakLoop
        EndIf
    EndLoop
    Call(PlaySoundAtCollider, COLLIDER_o89, SOUND_CLOSE_MAGIC_CHEST, 0)
    Return
    End
};

EvtScript N(EVS_FindMagicChest) = {
    Call(SpeakToPlayer, NPC_PARTNER, ANIM_Twink_Talk, ANIM_Twink_Idle, 0, MSG_Peach_018E)
    Call(SetPlayerAnimation, ANIM_Peach2_RaiseArms)
    Wait(10)
    Call(SetPlayerAnimation, ANIM_Peach2_TalkIdle)
    Call(SpeakToNpc, NPC_PLAYER, ANIM_Peach2_Talk, ANIM_Peach2_TalkIdle, 0, NPC_PARTNER, MSG_Peach_018F)
    Call(SpeakToPlayer, NPC_PARTNER, ANIM_Twink_Talk, ANIM_Twink_Idle, 0, MSG_Peach_0190)
    Call(SpeakToNpc, NPC_PLAYER, ANIM_Peach2_Talk, ANIM_Peach2_TalkIdle, 0, NPC_PARTNER, MSG_Peach_0191)
    Call(SpeakToPlayer, NPC_PARTNER, ANIM_Twink_Talk, ANIM_Twink_Idle, 0, MSG_Peach_0192)
    Call(SpeakToNpc, NPC_PLAYER, ANIM_Peach2_Talk, ANIM_Peach2_TalkIdle, 0, NPC_PARTNER, MSG_Peach_0193)
    Call(SetPlayerAnimation, ANIM_Peach2_LowerArms)
    Wait(10)
    Call(SetPlayerAnimation, ANIM_Peach1_Idle)
    Return
    End
};

EvtScript N(EVS_UseMagicChest_Peach) = {
    Set(LVar0, 0)
    Call(N(ChestItemPrompt))
    Switch(LVar0)
        CaseEq(-1)
        CaseEq(0)
            Wait(10)
            Call(SpeakToPlayer, NPC_PARTNER, ANIM_Twink_Talk, ANIM_Twink_Idle, 0, MSG_Peach_0194)
            Call(SetPlayerAnimation, ANIM_Peach2_RaiseArms)
            Wait(10)
            Call(SetPlayerAnimation, ANIM_Peach2_TalkIdle)
            Call(SpeakToNpc, NPC_PLAYER, ANIM_Peach2_Talk, ANIM_Peach2_TalkIdle, 0, NPC_PARTNER, MSG_Peach_0195)
            Call(SpeakToPlayer, NPC_PARTNER, ANIM_Twink_Talk, ANIM_Twink_Idle, 0, MSG_Peach_0196)
            Call(SpeakToNpc, NPC_PLAYER, ANIM_Peach2_Talk, ANIM_Peach2_TalkIdle, 0, NPC_PARTNER, MSG_Peach_0197)
            Call(SetPlayerAnimation, ANIM_Peach2_LowerArms)
            Wait(10)
            Call(SetPlayerAnimation, ANIM_Peach1_Idle)
        CaseDefault
            Wait(10)
            Set(LVar9, LVar0)
            ExecWait(N(EVS_OpenChest))
            Call(N(GetItemName), LVar9)
            Call(SetMessageText, LVar9, 0)
            Call(ShowMessageAtScreenPos, MSG_Peach_0198, 160, 40)
            Wait(10)
            ExecWait(N(EVS_CloseChest))
    EndSwitch
    Return
    End
};

EvtScript N(EVS_UseMagicChest_Mario) = {
    ExecWait(N(EVS_OpenChest))
    Set(LVar0, 1)
    Call(N(ChestItemPrompt))
    Switch(LVar0)
        CaseEq(-1)
        CaseEq(0)
            Call(ShowMessageAtScreenPos, MSG_Menus_00D4, 160, 40)
        CaseOrEq(269)
        CaseOrEq(297)
        CaseOrEq(273)
            SetGroup(EVT_GROUP_NEVER_PAUSE)
            Call(SetTimeFreezeMode, TIME_FREEZE_FULL)
            Call(ShowGotItem, LVar0, false, 0)
            Call(SetTimeFreezeMode, TIME_FREEZE_NONE)
            Call(AddBadge, LVar0, LVar1)
            Call(N(SetItemRetrieved))
        EndCaseGroup
        CaseDefault
            Call(N(GetItemEmptyCount))
            IfLe(LVar1, 0)
                Call(ShowMessageAtScreenPos, MSG_Menus_00D5, 160, 40)
            Else
                Call(ShowGotItem, LVar0, false, ITEM_PICKUP_FLAG_NO_ANIMS)
                Call(AddItem, LVar0, LVar1)
                Call(N(SetItemRetrieved))
            EndIf
    EndSwitch
    ExecWait(N(EVS_CloseChest))
    Return
    End
};

EvtScript N(EVS_Interact_MagicChest) = {
    Call(DisablePlayerInput, true)
    Call(DisablePartnerAI, 1)
    IfLt(GB_StoryProgress, STORY_CH8_REACHED_PEACHS_CASTLE)
        IfEq(GF_KKJ17_FoundMagicChest, false)
            ExecWait(N(EVS_FindMagicChest))
            Set(GF_KKJ17_FoundMagicChest, true)
        Else
            ExecWait(N(EVS_UseMagicChest_Peach))
        EndIf
    Else
        ExecWait(N(EVS_UseMagicChest_Mario))
    EndIf
    Call(EnablePartnerAI)
    Call(DisablePlayerInput, false)
    Return
    End
};

EvtScript N(EVS_SetupMagicChest) = {
    BindTrigger(Ref(N(EVS_Interact_MagicChest)), TRIGGER_WALL_PRESS_A, COLLIDER_o89, 1, 0)
    Return
    End
};
