#include "isk_06.h"

EvtScript N(EVS_ExitWalk_isk_04_2) = EVT_EXIT_WALK(40, isk_06_ENTRY_0, "isk_04", isk_04_ENTRY_2);
EvtScript N(EVS_ExitWalk_isk_04_3) = EVT_EXIT_WALK(40, isk_06_ENTRY_1, "isk_04", isk_04_ENTRY_3);

EvtScript N(EVS_BindExitTriggers) = {
    BindTrigger(Ref(N(EVS_ExitWalk_isk_04_2)), TRIGGER_FLOOR_ABOVE, COLLIDER_deiline, 1, 0)
    BindTrigger(Ref(N(EVS_ExitWalk_isk_04_3)), TRIGGER_FLOOR_ABOVE, COLLIDER_deilie, 1, 0)
    Return
    End
};

EvtScript N(EVS_Main) = {
    Set(GB_WorldLocation, LOCATION_DRY_DRY_RUINS)
    Call(SetSpriteShading, SHADING_ISK_06)
    SetUP_CAMERA_ALT_NO_LEAD()
    IfLt(GB_StoryProgress, STORY_CH2_DRAINED_SECOND_SAND_ROOM)
        Call(SetZoneEnabled, ZONE_o2016, false)
    Else
        Call(SetZoneEnabled, ZONE_C2, false)
        Call(SetZoneEnabled, ZONE_o2012, false)
    EndIf
    ExecWait(N(EVS_MakeEntities))
    Exec(N(EVS_SetupSand))
    Exec(N(EVS_SetupSwitch))
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
