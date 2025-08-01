EvtScript N(EVS_NpcInteract_Toad_03) = {
    Switch(GB_StoryProgress)
#if VERSION_JP
        CaseLt(STORY_CH0_MET_STAR_SPIRITS)
            Set(LVar0, MSG_MAC_Bridge_00A0)
        CaseLt(STORY_CH1_MERLIN_REVEALED_KOOPA_BROS)
            Set(LVar0, MSG_MAC_Bridge_00A1)
        CaseLt(STORY_CH1_STAR_SPRIT_DEPARTED)
            Set(LVar0, MSG_MAC_Bridge_00A2)
#endif
        CaseLt(STORY_CH2_STAR_SPRIT_DEPARTED)
            IfEq(GF_MAC03_BombedRock, false)
                Set(LVar0, MSG_MAC_Bridge_006B)
            Else
                Set(LVar0, MSG_MAC_Bridge_006C)
            EndIf
        CaseLt(STORY_CH3_STAR_SPIRIT_RESCUED)
            Set(LVar0, MSG_MAC_Bridge_006D)
        CaseLt(STORY_CH4_STAR_SPRIT_DEPARTED)
            IfEq(GF_MAC03_ShyGuyBlockingTracks, false)
                Set(LVar0, MSG_MAC_Bridge_006E)
            Else
                IfEq(GF_MAC03_ShyGuyChasedOff, false)
                    Set(LVar0, MSG_MAC_Bridge_006F)
                Else
                    Set(LVar0, MSG_MAC_Bridge_0070)
                EndIf
            EndIf
        CaseLt(STORY_CH5_DEFEATED_FUZZIPEDE)
            Set(LVar0, MSG_MAC_Bridge_0071)
        CaseLt(STORY_CH5_STAR_SPRIT_DEPARTED)
            Set(LVar0, MSG_MAC_Bridge_0072)
        CaseLt(STORY_CH6_ARRIVED_AT_FLOWER_FIELDS)
            Set(LVar0, MSG_MAC_Bridge_0073)
            Set(GF_MAC02_HeardAboutMinhT, true)
        CaseLt(STORY_CH6_STAR_SPIRIT_RESCUED)
            Set(LVar0, MSG_MAC_Bridge_0074)
        CaseLt(STORY_CH7_INVITED_TO_STARBORN_VALLEY)
            Set(LVar0, MSG_MAC_Bridge_0075)
        CaseLt(STORY_CH7_STAR_SPRIT_DEPARTED)
            Set(LVar0, MSG_MAC_Bridge_0076)
        CaseLt(STORY_EPILOGUE)
            Set(LVar0, MSG_MAC_Bridge_0077)
    EndSwitch
    Call(SpeakToPlayer, NPC_Toad_03, ANIM_Toad_Yellow_Talk, ANIM_Toad_Yellow_Idle, 0, LVar0)
    Return
    End
};

EvtScript N(EVS_NpcInit_Toad_03) = {
    Call(BindNpcInteract, NPC_SELF, Ref(N(EVS_NpcInteract_Toad_03)))
    Return
    End
};
