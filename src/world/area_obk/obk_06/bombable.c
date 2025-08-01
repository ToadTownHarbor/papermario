#include "obk_06.h"
#include "effects.h"

BombTrigger N(BombPos_Wall) = {
    .pos = { -237.0f, 0.0f, 50.0f },
    .diameter = 0.0f
};

EvtScript N(EVS_OnBlast_Wall) = {
    PlayEffect(EFFECT_BOMBETTE_BREAKING, 1, 23, 26, 1, 10, 30)
    Loop(10)
        Call(EnableModel, MODEL_bom_ato, false)
        Call(EnableModel, MODEL_bom1, true)
        Wait(1)
        Call(EnableModel, MODEL_bom_ato, true)
        Call(EnableModel, MODEL_bom1, false)
        Wait(1)
    EndLoop
    Call(ModifyColliderFlags, MODIFY_COLLIDER_FLAGS_SET_BITS, COLLIDER_tt1, COLLIDER_FLAGS_UPPER_MASK)
    Set(GF_OBK06_BombedWall, true)
    Unbind
    Return
    End
};

EvtScript N(EVS_SetupBombables) = {
    IfEq(GF_OBK06_BombedWall, false)
        BindTrigger(Ref(N(EVS_OnBlast_Wall)), TRIGGER_POINT_BOMB, Ref(N(BombPos_Wall)), 1, 0)
        Call(EnableModel, MODEL_bom_ato, false)
    Else
        Call(EnableModel, MODEL_bom1, false)
        Call(ModifyColliderFlags, MODIFY_COLLIDER_FLAGS_SET_BITS, COLLIDER_tt1, COLLIDER_FLAGS_UPPER_MASK)
    EndIf
    Return
    End
};
