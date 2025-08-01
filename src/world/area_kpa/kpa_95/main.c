#include "kpa_95.h"

EvtScript N(EVS_OpenCellDoor) = {
    Call(DisablePlayerInput, true)
    Call(PlayerMoveTo, -30, 15, 20)
    Call(InterpPlayerYaw, 90, 2)
    Call(PlaySoundAtCollider, COLLIDER_deilittn, SOUND_METAL_GATE_OPEN, SOUND_SPACE_DEFAULT)
    Call(MakeLerp, 0, 140, 60, EASING_COS_IN_OUT)
    Label(0)
    Call(UpdateLerp)
    Call(RotateModel, MODEL_o199, LVar0, 0, 1, 0)
    Call(RotateModel, MODEL_g35, LVar0, 0, 1, 0)
    Wait(1)
    IfEq(LVar1, 1)
        Goto(0)
    EndIf
    Call(ModifyColliderFlags, MODIFY_COLLIDER_FLAGS_SET_BITS, COLLIDER_deilittn, COLLIDER_FLAGS_UPPER_MASK)
    Call(ModifyColliderFlags, MODIFY_COLLIDER_FLAGS_CLEAR_BITS, COLLIDER_o297, COLLIDER_FLAGS_UPPER_MASK)
    Call(DisablePlayerInput, false)
    Unbind
    Return
    End
};

EvtScript N(EVS_ExitWalk_kpa_94_0) = EVT_EXIT_WALK(60, kpa_95_ENTRY_0, "kpa_94", kpa_94_ENTRY_0);

MAP_RODATA_PAD(1, exits);

EvtScript N(EVS_BindExitTriggers) = {
    BindTrigger(Ref(N(EVS_ExitWalk_kpa_94_0)), TRIGGER_FLOOR_ABOVE, COLLIDER_deilie, 1, 0)
    Return
    End
};

EvtScript N(EVS_Main) = {
    Set(GB_WorldLocation, LOCATION_BOWSERS_CASTLE)
    Call(SetSpriteShading, SHADING_NONE)
    SetUP_CAMERA_ALT_NO_LEAD()
    Call(MakeNpcs, true, Ref(N(DefaultNPCs)))
    ExecWait(N(EVS_MakeEntities))
    Set(LVar0, Ref(N(EVS_BindExitTriggers)))
    Exec(EnterWalk)
    Wait(1)
    Exec(N(EVS_SetupMusic))
    Call(ModifyColliderFlags, MODIFY_COLLIDER_FLAGS_SET_BITS, COLLIDER_o297, COLLIDER_FLAGS_UPPER_MASK)
    Return
    End
};
