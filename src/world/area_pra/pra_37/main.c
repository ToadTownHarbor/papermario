#include "pra_37.h"

s32 N(map_init)(void) {
    gGameStatusPtr->playerSpriteSet = PLAYER_SPRITES_MARIO_REFLECT_FLOOR;
    sprintf(wMapShapeName, "pra_10_shape");
    sprintf(wMapHitName, "pra_10_hit");
    return false;
}

#include "../common/Reflection.inc.c"
#include "../common/Reflection.data.inc.c"

EvtScript N(EVS_ExitDoors_pra_22_1) = {
    SetGroup(EVT_GROUP_EXIT_MAP)
    Call(DisablePlayerInput, true)
    Set(LVar0, pra_37_ENTRY_0)
    Set(LVar1, COLLIDER_deilittsw)
    Set(LVar2, MODEL_o772)
    Set(LVar3, MODEL_o768)
    Set(LVar4, MODEL_o844)
    Set(LVar5, MODEL_o846)
    Exec(ExitSplitDoubleDoor)
    Wait(17)
    Call(GotoMap, Ref("pra_22"), pra_22_ENTRY_1)
    Wait(100)
    Return
    End
};

EvtScript N(EVS_ExitDoors_pra_28_0) = {
    SetGroup(EVT_GROUP_EXIT_MAP)
    Call(DisablePlayerInput, true)
    Set(LVar0, pra_37_ENTRY_1)
    Set(LVar1, COLLIDER_deilittse)
    Set(LVar2, MODEL_o1019)
    Set(LVar3, MODEL_o1021)
    Set(LVar4, MODEL_o1020)
    Set(LVar5, MODEL_o1022)
    Exec(ExitSplitDoubleDoor)
    Wait(17)
    Call(GotoMap, Ref("pra_28"), pra_28_ENTRY_0)
    Wait(100)
    Return
    End
};

EvtScript N(EVS_BindExitTriggers) = {
    BindTrigger(Ref(N(EVS_ExitDoors_pra_22_1)), TRIGGER_WALL_PRESS_A, COLLIDER_deilittsw, 1, 0)
    BindTrigger(Ref(N(EVS_ExitDoors_pra_28_0)), TRIGGER_WALL_PRESS_A, COLLIDER_deilittse, 1, 0)
    Return
    End
};

EvtScript N(EVS_EnterMap) = {
    Call(GetEntryID, LVar0)
    Switch(LVar0)
        CaseEq(pra_37_ENTRY_0)
            Set(LVar2, MODEL_o772)
            Set(LVar3, MODEL_o768)
            Set(LVar4, MODEL_o844)
            Set(LVar5, MODEL_o846)
            ExecWait(EnterSplitDoubleDoor)
        CaseEq(pra_37_ENTRY_1)
            Set(LVar2, MODEL_o1019)
            Set(LVar3, MODEL_o1021)
            Set(LVar4, MODEL_o1020)
            Set(LVar5, MODEL_o1022)
            ExecWait(EnterSplitDoubleDoor)
    EndSwitch
    Exec(N(EVS_BindExitTriggers))
    Return
    End
};

EvtScript N(EVS_Main) = {
    Set(GB_WorldLocation, LOCATION_CRYSTAL_PALACE)
    Call(SetSpriteShading, SHADING_NONE)
    Call(SetCamPerspective, CAM_DEFAULT, CAM_UPDATE_FROM_ZONE, 25, 16, 4096)
    Call(SetCamBGColor, CAM_DEFAULT, 24, 24, 40)
    Call(SetCamLeadPlayer, CAM_DEFAULT, false)
    Call(SetCamEnabled, CAM_DEFAULT, true)
    Call(MakeNpcs, true, Ref(N(DefaultNPCs)))
    Exec(N(EVS_SetupMusic))
    Set(LVar0, REFLECTION_FLOOR_ONLY)
    Set(LVar1, GF_PRA_BrokeIllusion)
    Exec(N(EVS_SetupReflections))
    Exec(N(EVS_EnterMap))
    Wait(1)
    Return
    End
};
