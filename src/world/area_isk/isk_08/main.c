#include "isk_08.h"

EvtScript N(EVS_ExitWalk_isk_09_0) = EVT_EXIT_WALK(40, isk_08_ENTRY_0, "isk_09", isk_09_ENTRY_0);
EvtScript N(EVS_ExitWalk_isk_10_0) = EVT_EXIT_WALK(40, isk_08_ENTRY_1, "isk_10", isk_10_ENTRY_0);
EvtScript N(EVS_ExitWalk_isk_07_0) = EVT_EXIT_WALK(40, isk_08_ENTRY_2, "isk_07", isk_07_ENTRY_0);
EvtScript N(EVS_ExitWalk_isk_11_0) = EVT_EXIT_WALK(40, isk_08_ENTRY_3, "isk_11", isk_11_ENTRY_0);

EvtScript N(EVS_BindExitTriggers) = {
    BindTrigger(Ref(N(EVS_ExitWalk_isk_09_0)), TRIGGER_FLOOR_ABOVE, COLLIDER_deilinw, 1, 0)
    BindTrigger(Ref(N(EVS_ExitWalk_isk_10_0)), TRIGGER_FLOOR_ABOVE, COLLIDER_deiliw, 1, 0)
    BindTrigger(Ref(N(EVS_ExitWalk_isk_07_0)), TRIGGER_FLOOR_ABOVE, COLLIDER_deiline, 1, 0)
    BindTrigger(Ref(N(EVS_ExitWalk_isk_11_0)), TRIGGER_FLOOR_ABOVE, COLLIDER_deilie, 1, 0)
    Return
    End
};

EvtScript N(EVS_Main) = {
    Set(GB_WorldLocation, LOCATION_DRY_DRY_RUINS)
    Call(SetSpriteShading, SHADING_ISK_08)
    SetUP_CAMERA_NO_LEAD()
    Call(MakeNpcs, true, Ref(N(DefaultNPCs)))
    ExecWait(N(EVS_SetupFlames))
    ExecWait(N(EVS_SetupBombableWall))
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
    Set(LVar0, Ref(N(EVS_BindExitTriggers)))
    Exec(EnterWalk)
    Return
    End
};
