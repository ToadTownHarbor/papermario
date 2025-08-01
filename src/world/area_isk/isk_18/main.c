#include "isk_18.h"

EvtScript N(EVS_ExitWalk_isk_10_2) = EVT_EXIT_WALK(40, isk_18_ENTRY_0, "isk_10", isk_10_ENTRY_2);
EvtScript N(EVS_ExitWalk_isk_12_2) = EVT_EXIT_WALK(40, isk_18_ENTRY_1, "isk_12", isk_12_ENTRY_2);

EvtScript N(EVS_BindExitTriggers) = {
    BindTrigger(Ref(N(EVS_ExitWalk_isk_10_2)), TRIGGER_FLOOR_ABOVE, COLLIDER_deiliw, 1, 0)
    BindTrigger(Ref(N(EVS_ExitWalk_isk_12_2)), TRIGGER_FLOOR_ABOVE, COLLIDER_deilie, 1, 0)
    Return
    End
};

EvtScript N(EVS_Main) = {
    Set(GB_WorldLocation, LOCATION_DRY_DRY_RUINS)
    Call(SetSpriteShading, SHADING_ISK_18)
    SetUP_CAMERA_ALT_NO_LEAD()
    Call(MakeNpcs, true, Ref(N(DefaultNPCs)))
    ExecWait(N(EVS_SetupFlames))
#if VERSION_PAL
    Call(SetMusic, 0, SONG_DRY_DRY_RUINS, 0, VOL_LEVEL_FULL)
#else
    Switch(GB_StoryProgress)
        CaseLt(STORY_CH2_SOLVED_ARTIFACT_PUZZLE)
            Call(SetMusic, 0, SONG_DRY_DRY_RUINS, 0, VOL_LEVEL_FULL)
        CaseLt(STORY_CH2_DEFEATED_TUTANKOOPA)
            Call(SetMusic, 0, SONG_RUINS_BASEMENT, 0, VOL_LEVEL_FULL)
        CaseGe(STORY_CH2_DEFEATED_TUTANKOOPA)
            Call(SetMusic, 0, SONG_DRY_DRY_RUINS, 0, VOL_LEVEL_FULL)
    EndSwitch
#endif
    IfLt(GB_StoryProgress, STORY_CH2_DRAINED_THIRD_SAND_ROOM)
        Call(EnableModel, MODEL_o1956, false)
        Call(EnableModel, MODEL_o1957, false)
        Call(EnableModel, MODEL_o1958, false)
        Call(ModifyColliderFlags, MODIFY_COLLIDER_FLAGS_SET_BITS, COLLIDER_deilitte, COLLIDER_FLAGS_UPPER_MASK)
    EndIf
    Set(LVar0, Ref(N(EVS_BindExitTriggers)))
    Exec(EnterWalk)
    Return
    End
};
