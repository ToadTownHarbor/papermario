#include "kkj_17.h"

// immediately remove the item from Peach's inventory, keeping only the flag
EvtScript N(EVS_AutoRemoveDeepFocus) = {
    Loop(0)
        IfEq(GF_KKJ17_Item_DeepFocus, true)
            BreakLoop
        EndIf
        Wait(1)
    EndLoop
    Wait(10)
    Call(RemoveBadge, ITEM_DEEP_FOCUS_A)
    Return
    End
};

EvtScript N(EVS_MakeEntities) = {
    Call(MakeItemEntity, ITEM_DEEP_FOCUS_A, 100, 0, -30, ITEM_SPAWN_MODE_FIXED_NEVER_VANISH, GF_KKJ17_Item_DeepFocus)
    IfLt(GB_StoryProgress, STORY_CH8_REACHED_PEACHS_CASTLE)
        IfEq(GF_KKJ17_Item_DeepFocus, false)
            Exec(N(EVS_AutoRemoveDeepFocus))
        EndIf
    EndIf
    Return
    End
};
