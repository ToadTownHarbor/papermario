#include "machi.h"

FollowAnims N(GoombariaAnims) = {
    ANIM_Goombaria_Walk,
    ANIM_Goombaria_Jump,
    ANIM_Goombaria_Fall,
    ANIM_Goombaria_Idle,
    ANIM_Goombaria_Idle,
    ANIM_Goombaria_Run,
};

API_CALLABLE(N(func_802401B0_7E7550)) {
    Npc* npc = get_npc_unsafe(script->owner1.enemy->npcID);

    if (rand_int(1000) < 500) {
        npc_follow_init(npc,
            NPC_SELF,
            &N(GoombariaAnims),
            rand_int(4) + 1,
            rand_int(3) + 5,
            rand_int(50) + 50,
            rand_int(100) + 100);
    } else {
        npc_follow_init(npc,
            rand_int(3) + NPC_Goompa,
             &N(GoombariaAnims),
            rand_int(4) + 1,
            rand_int(3) + 5,
            rand_int(50) + 50,
            rand_int(100) + 10);
    }
    return ApiStatus_DONE2;
}

API_CALLABLE(N(func_802402EC_7E768C)) {
    Npc* npc = get_npc_unsafe(script->owner1.enemy->npcID);

    npc_update_npc_tracking(npc);
    return ApiStatus_DONE2;
}

API_CALLABLE(N(func_80240318_7E76B8)) {
    Npc* npc = get_npc_unsafe(script->owner1.enemy->npcID);

    npc_follow_npc(npc);
    return ApiStatus_DONE2;
}

API_CALLABLE(N(SetupChapter0)) {
    PlayerData* playerData = &gPlayerData;
    s32 i;

    playerData->curHP = 5;
    playerData->curMaxHP = 5;
    playerData->bootsLevel = 0;
    playerData->hammerLevel = -1;
    playerData->fortressKeyCount = 0;

    for (i = 0; i < ARRAY_COUNT(playerData->partners); i++) {
        playerData->partners[i].enabled = false;
    }
    switch_to_partner(PARTNER_NONE);
    return ApiStatus_DONE2;
}

API_CALLABLE(N(SetupStarRodPaletteCycling)) {
    Npc* npc = get_npc_unsafe(script->owner1.enemy->npcID);

    npc_set_palswap_mode_A(npc, NPC_PAL_ADJUST_BLEND_PALETTES_VARYING_INTERVALS);
    npc_set_palswap_1(npc, 0, 1, 30, 10);
    npc_set_palswap_2(npc, 60, 20, 0, 0);
    return ApiStatus_DONE2;
}

API_CALLABLE(N(SetupChapter1)) {
    PlayerData* playerData = &gPlayerData;
    s32 i;

    playerData->curHP = 8;
    playerData->curMaxHP = 8;
    playerData->bootsLevel = 0;
    playerData->hammerLevel = 0;
    playerData->fortressKeyCount = 0;
    playerData->partners[PARTNER_NONE].enabled = true;

    for (i = 1; i < ARRAY_COUNT(playerData->partners); i++) {
        playerData->partners[i].enabled = false;
    }
    return ApiStatus_DONE2;
}

API_CALLABLE(N(SetupChapter2)) {
    PlayerData* playerData = &gPlayerData;
    s32 i;

    playerData->curHP = 11;
    playerData->curMaxHP = 11;
    playerData->bootsLevel = 0;
    playerData->hammerLevel = 0;
    playerData->fortressKeyCount = 0;

    for (i = 0; i < ARRAY_COUNT(playerData->partners); i++) {
        playerData->partners[i].enabled = false;
    }

    playerData->partners[PARTNER_NONE].enabled = true;
    playerData->partners[PARTNER_GOOMBARIO].enabled = true;
    playerData->partners[PARTNER_BOMBETTE].enabled = true;
    return ApiStatus_DONE2;
}

EvtScript N(EVS_NpcAuxAI_Goompa) = {
    Return
    End
};

EvtScript N(EVS_802411A8) = {
    Call(N(func_802401B0_7E7550))
    Label(0)
        Call(N(func_802402EC_7E768C))
        Call(N(func_80240318_7E76B8))
        Wait(1)
        Goto(0)
    Return
    End
};

EvtScript N(EVS_NpcInteract_Goompa) = {
    Return
    End
};

EvtScript N(EVS_NpcHit_Goompa) = {
    Return
    End
};

EvtScript N(EVS_NpcDefeat_Goompa) = {
    Return
    End
};

EvtScript N(EVS_NpcAI_Unused) = {
    Return
    End
};

EvtScript N(EVS_NpcAuxAI_Unused) = {
    Return
    End
};

EvtScript N(EVS_NpcInteract_Unused) = {
    Return
    End
};

EvtScript N(EVS_NpcHit_Unused) = {
    Call(StartBattle)
    Return
    End
};

EvtScript N(EVS_NpcDefeat_Unused) = {
    Call(GetBattleOutcome, LVar0)
    Switch(LVar0)
        CaseEq(OUTCOME_PLAYER_WON)
            Call(DoNpcDefeat)
        CaseEq(OUTCOME_PLAYER_LOST)
        CaseEq(OUTCOME_PLAYER_FLED)
    EndSwitch
    Return
    End
};

EvtScript N(EVS_NpcInteract_StarRod) = {
    Call(BringPartnerOut, PARTNER_PARAKARRY)
    Wait(200)
    Call(PutPartnerAway)
    Return
    End
};

EvtScript N(EVS_NpcAI_StarRod) = {
    Return
    End
};

EvtScript N(EVS_NpcAuxAI_StarRod) = {
    Call(N(SetupStarRodPaletteCycling))
    Return
    End
};

EvtScript N(EVS_NpcInteract_Koopa) = {
    Call(N(SetupChapter1))
    Return
    End
};

EvtScript N(EVS_NpcAI_Koopa) = {
    Return
    End
};

EvtScript N(EVS_NpcInteract_Kolorado) = {
    Call(N(SetupChapter2))
    Return
    End
};

EvtScript N(EVS_NpcAI_Kolorado) = {
    Return
    End
};

NpcSettings N(NpcSettings_StarRod) = {
    .defaultAnim = ANIM_StarRod_Still,
    .height = 24,
    .radius = 24,
    .otherAI = &N(EVS_NpcAuxAI_StarRod),
    .onInteract = &N(EVS_NpcInteract_StarRod),
    .ai = &N(EVS_NpcAI_StarRod),
    .flags = ENEMY_FLAG_PASSIVE,
};

NpcSettings N(NpcSettings_Koopa) = {
    .defaultAnim = ANIM_Koopa_Walk,
    .height = 24,
    .radius = 24,
    .onInteract = &N(EVS_NpcInteract_Koopa),
    .ai = &N(EVS_NpcAI_Koopa),
    .flags = ENEMY_FLAG_PASSIVE,
};

NpcSettings N(NpcSettings_Kolorado) = {
    .defaultAnim = ANIM_Kolorado_Walk,
    .height = 24,
    .radius = 24,
    .onInteract = &N(EVS_NpcInteract_Kolorado),
    .ai = &N(EVS_NpcAI_Kolorado),
    .flags = ENEMY_FLAG_PASSIVE,
};

NpcSettings N(NpcSettings_Goompa) = {
    .defaultAnim = ANIM_Goompa_Walk,
    .height = 24,
    .radius = 24,
    .otherAI = &N(EVS_NpcAuxAI_Goompa),
    .onInteract = &N(EVS_NpcInteract_Goompa),
    .onHit = &N(EVS_NpcHit_Goompa),
    .flags = ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_IGNORE_PLAYER_COLLISION | ENEMY_FLAG_IGNORE_ENTITY_COLLISION | ENEMY_FLAG_FLYING | ENEMY_FLAG_SKIP_BATTLE,
};

NpcData N(NpcData_05)[] = {
    {
        .id = NPC_05,
        .pos = { 16.0f, 0.0f, -411.0f },
        .yaw = 0,
        .init = (void*) 0x1F64,
        .settings = &N(NpcSettings_StarRod),
        .flags = 0,
        .animations = {},
    },
    {
        .id = NPC_06,
        .pos = { 389.0f, 0.0f, -278.0f },
        .yaw = 0,
        .init = (void*) 0x1F64,
        .settings = &N(NpcSettings_Koopa),
        .flags = 0,
        .animations = {},
    },
    {
        .id = NPC_07,
        .pos = { 481.0f, 0.0f, -44.0f },
        .yaw = 0,
        .init = (void*) 0x1F64,
        .settings = &N(NpcSettings_Kolorado),
        .flags = 0,
        .animations = {},
    },
};

NpcData N(NpcData_GoombaFamily) = {
    .id = NPC_Goompa,
    .pos = { 0.0f, 0.0f, 0.0f },
    .yaw = 0,
    .init = (void*) 0x1F64,
    .settings = &N(NpcSettings_Goompa),
    .flags = ENEMY_FLAG_ENABLE_HIT_SCRIPT,
    .animations = {
    },
};

NpcData N(NpcData_0B)[] = {
    {
        .id = NPC_0B,
        .pos = { 0.0f, 0.0f, 0.0f },
        .yaw = 0,
        .init = (void*) 0x1F64,
        .settings = &N(NpcSettings_Goompa),
        .flags = ENEMY_FLAG_ENABLE_HIT_SCRIPT,
        .animations = {},
    },
    {
        .id = NPC_0C,
        .pos = { 0.0f, 0.0f, 0.0f },
        .yaw = 0,
        .init = (void*) 0x1F64,
        .settings = &N(NpcSettings_Goompa),
        .flags = ENEMY_FLAG_ENABLE_HIT_SCRIPT,
        .animations = {},
    },
    {
        .id = NPC_0D,
        .pos = { 0.0f, 0.0f, 0.0f },
        .yaw = 0,
        .init = (void*) 0x1F64,
        .settings = &N(NpcSettings_Goompa),
        .flags = ENEMY_FLAG_ENABLE_HIT_SCRIPT,
        .animations = {},
    },
    {
        .id = NPC_0E,
        .pos = { 0.0f, 0.0f, 0.0f },
        .yaw = 0,
        .init = (void*) 0x1F64,
        .settings = &N(NpcSettings_Goompa),
        .flags = ENEMY_FLAG_ENABLE_HIT_SCRIPT,
        .animations = {},
    },
    {
        .id = NPC_0F,
        .pos = { 0.0f, 0.0f, 0.0f },
        .yaw = 0,
        .init = (void*) 0x1F64,
        .settings = &N(NpcSettings_Goompa),
        .flags = ENEMY_FLAG_ENABLE_HIT_SCRIPT,
        .animations = {},
    },
    {
        .id = NPC_10,
        .pos = { 0.0f, 0.0f, 0.0f },
        .yaw = 0,
        .init = (void*) 0x1F64,
        .settings = &N(NpcSettings_Goompa),
        .flags = ENEMY_FLAG_ENABLE_HIT_SCRIPT,
        .animations = {},
    },
    {
        .id = NPC_11,
        .pos = { 0.0f, 0.0f, 0.0f },
        .yaw = 0,
        .init = (void*) 0x1F64,
        .settings = &N(NpcSettings_Goompa),
        .flags = ENEMY_FLAG_ENABLE_HIT_SCRIPT,
        .animations = {},
    },
    {
        .id = NPC_12,
        .pos = { 0.0f, 0.0f, 0.0f },
        .yaw = 0,
        .init = (void*) 0x1F64,
        .settings = &N(NpcSettings_Goompa),
        .flags = ENEMY_FLAG_ENABLE_HIT_SCRIPT,
        .animations = {},
    },
    {
        .id = NPC_13,
        .pos = { 0.0f, 0.0f, 0.0f },
        .yaw = 0,
        .init = (void*) 0x1F64,
        .settings = &N(NpcSettings_Goompa),
        .flags = ENEMY_FLAG_ENABLE_HIT_SCRIPT,
        .animations = {},
    },
};

NpcData N(NpcData_GoombaKing) = {
    .id = NPC_GoombaKing,
    .pos = { 12.0f, 0.0f, -13.0f },
    .yaw = 0,
    .init = (void*) 0x4003,
    .settings = &N(NpcSettings_GoombaKing),
    .flags = 0,
    .animations = {}
};

NpcGroupList N(DefaultNPCs) = {
    NPC_GROUP(N(NpcData_05), BTL_KMR_1_FORMATION_08),
    NPC_GROUP(N(NpcData_GoombaFamily), BTL_KMR_1_FORMATION_08),
    NPC_GROUP(N(NpcData_Misstar)),
    {}
};
