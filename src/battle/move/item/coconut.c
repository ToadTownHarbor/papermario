#include "common.h"
#include "script_api/battle.h"
#include "entity.h"
#include "ld_addrs.h"
#include "sprite/player.h"
#include "include_asset.h"

#define NAMESPACE battle_item_coconut

#include "battle/common/move/ItemRefund.inc.c"
#include "battle/common/move/UseItem.inc.c"

static s32 _pad = 0;

#include "battle/move/item/coconut.png.h"
INCLUDE_IMG("battle/move/item/coconut.png", battle_item_coconut_png);
INCLUDE_PAL("battle/move/item/coconut.pal", battle_item_coconut_pal);

/// 32x32 square.
Vtx N(model)[] = {
    { .v = {{ -16, -16, 0 }, false, { 0,    0    }, { 0, 0, 0, 255 }}},
    { .v = {{ 15,  -16, 0 }, false, { 1024, 0    }, { 0, 0, 0, 255 }}},
    { .v = {{ 15,  15,  0 }, false, { 1024, 1024 }, { 0, 0, 0, 255 }}},
    { .v = {{ -16, 15,  0 }, false, { 0,    1024 }, { 0, 0, 0, 255 }}},
};

Gfx N(displayList)[] = {
    gsDPPipeSync(),
    gsSPTexture(-1, -1, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
    gsDPSetTexturePersp(G_TP_PERSP),
    gsDPSetTextureDetail(G_TD_CLAMP),
    gsDPSetTextureLOD(G_TL_TILE),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetTextureFilter(G_TF_AVERAGE),
    gsDPSetTextureConvert(G_TC_FILT),
    gsDPSetTextureLUT(G_TT_RGBA16),
    gsDPLoadTLUT_pal16(0, battle_item_coconut_pal),
    gsDPLoadTextureTile_4b(battle_item_coconut_png, G_IM_FMT_CI, battle_item_coconut_png_width, battle_item_coconut_png_height, 0, 0, battle_item_coconut_png_width - 1, battle_item_coconut_png_height - 1, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD),
    gsSPClearGeometryMode(G_LIGHTING),
    gsSPClearGeometryMode(G_SHADING_SMOOTH),
    gsSPVertex(&N(model), ARRAY_COUNT(N(model)), 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSP1Triangle(0, 2, 3, 0),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};

EntityModelScript N(modelCommandList) = STANDARD_ENTITY_MODEL_SCRIPT(N(displayList), RENDER_MODE_ALPHATEST);

EvtScript N(EVS_UseItem) = {
    SetConst(LVarA, ITEM_COCONUT)
    ExecWait(N(UseItemWithEffect))
    Call(UseBattleCamPreset, BTL_CAM_VIEW_ENEMIES)
    Call(MoveBattleCamOver, 15)
    Call(SetAnimation, ACTOR_PLAYER, 0, ANIM_Mario1_Throw)
    Call(PlaySound, SOUND_THROW)
    Wait(3)
    Call(CreateVirtualEntity, LVarA, Ref(N(modelCommandList)))
    SetF(LVar0, Float(1.0))
    Call(MultiplyByActorScale, LVar0)
    Call(SetVirtualEntityScale, LVarA, LVar0, LVar0, LVar0)
    Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
    Set(LVar3, 20)
    Set(LVar4, 42)
    Set(LVar5, 5)
    Call(MultiplyVec3ByActorScale, LVar3, LVar4, LVar5)
    Add(LVar0, LVar3)
    Add(LVar1, LVar4)
    Add(LVar2, LVar5)
    Call(SetVirtualEntityPosition, LVarA, LVar0, LVar1, LVar2)
    Call(InitTargetIterator)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Thread
        Set(LVar0, 0)
        Loop(18)
            Add(LVar0, -60)
            Call(SetVirtualEntityRotation, LVarA, 0, 0, LVar0)
            Wait(1)
        EndLoop
    EndThread
    Call(SetVirtualEntityJumpGravity, LVarA, Float(0.8))
    Add(LVar2, 5)
    Call(VirtualEntityJumpTo, LVarA, LVar0, LVar1, LVar2, 18)
    Call(GetItemPower, ITEM_COCONUT, LVar3, LVar4)
    Call(ApplyShrinkFromOwner, LVar3)
    Call(ItemDamageEnemy, LVar9, DAMAGE_TYPE_IGNORE_DEFENSE | DAMAGE_TYPE_NO_CONTACT, 0, LVar3, BS_FLAGS1_TRIGGER_EVENTS)
    Add(LVar0, 60)
    Add(LVar1, 0)
    Call(VirtualEntityJumpTo, LVarA, LVar0, LVar1, LVar2, 16)
    Call(DeleteVirtualEntity, LVarA)
    ExecWait(N(PlayerGoHome))
    Return
    End
};
