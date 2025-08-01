#include "common.h"

// LVar0 wall 1 model ID
// LVar1 wall 2 model ID
// LVar2 tex panner ID
EvtScript N(EVS_GlassShimmer) = {
    Call(SetTexPanner, LVar0, LVar2)
    Call(SetTexPanner, LVar1, LVar2)
    Loop(0)
        Call(EnableModel, LVar0, true)
        Call(EnableModel, LVar1, true)
        Set(LVarA, 0)
        Loop(20)
            Call(SetTexPanOffset, LVar2, 0, LVarA, 0)
            Add(LVarA, 3000)
            Wait(1)
        EndLoop
        Call(EnableModel, LVar0, false)
        Call(EnableModel, LVar1, false)
        Wait(160)
    EndLoop
    Return
    End
};
