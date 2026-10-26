// CTCCreatureGroupBoastingCrowd::FrameUpdate() -- kill the crowd once the hero is gone or dead; when an off-podium state's clear frame has passed, clear every opinion source pushed for the hero and return to state 0; when the post-boast state's leave frame has passed, go back to the encouragement state (boasts left) or to 7/8 (good/bad boast)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00784500, 245 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3), CThing::Kill(bool) @ 0x004C9B80, GFGetCurrentFrame @
//   0x0049D870 (__fastcall), CTCBase::GetWorld @ 0x00686A70 (manifest "CViewBase::GetDisplayEngine" is propagated),
//   COpinionReactionManager::ClearOpinionSource(CThing*, bool) @ 0x007008C0, EnterCrowdState @ 0x00784260.
// Globals: the quest manager @ 0x013B89FC; its retail +0x68 member is a single-pointer STLport list (boasts).
// Layout: retail = engine/CTCCreatureGroupBoastingCrowd.h - 4 (the retail intelligent pointer / member map is 4 bytes
//   shorter, as the landed siblings 0x00784600 / 0x00784920): PHero +0x24, CrowdState +0x2c, CurrentEncouragementState
//   +0x30, HeroMadeGoodBoast +0x35, HeroOpinionSourcesPushed +0x3c, FrameToClearRemainingOpinionSources +0x40,
//   FrameToLeavePostBoastState +0x44; CWorld's opinion reaction manager at +0x74. Local model (the header is flat).
// Shape: the hero test is an inline bool helper `IsHeroAlive()` (the `!PHero.GetPItem() || ...` spelled inline sends the
//   kill block to the end; the helper keeps it in place with its own early `ret`); CurrentStateIsOffPodiumType (landed
//   0x007841C0) must be `__forceinline` to be inlined into the guard chain; the source clearing is a while loop (rotated:
//   pre-test, `dec ; test ; jg`) whose manager is a named loop local (edi pushed around the loop only);
//   `HeroMadeGoodBoast ? 7 : 8` gives `neg ; sbb ; add 8`.
class CThing { public:
    unsigned char _pad00[0x91];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    void Kill(bool immediately);                // 0x004C9B80
};
class COpinionReactionManager { public:
    void ClearOpinionSource(CThing* source, bool immediately);   // 0x007008C0
};
class CWorld { public:
    unsigned char _pad00[0x74];
    COpinionReactionManager* POpinionReactionManager;   // +0x74
};
struct SListNode { SListNode* next; SListNode* prev; };
class CQuestManager { public:
    unsigned char _pad00[0x68];
    SListNode* m_boasts;                        // +0x68 list head (retail 4-byte STLport list)
    bool BoastsEmpty() const { return m_boasts->next == m_boasts; }
};
extern CQuestManager* g_pQuestManager;          // 0x013B89FC
long __fastcall GFGetCurrentFrame();            // 0x0049D870

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
class CTCCreatureGroupBoastingCrowd { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x24 - 0x08];          // CTCBase tail + CTCCreatureGroup members
    CBaseIntelligentPointer PHero;              // +0x24 CIntelligentPointer<CThing>
    long CrowdState;                            // +0x2c
    long CurrentEncouragementState;             // +0x30
    bool HeroBoasted;                           // +0x34
    bool HeroMadeGoodBoast;                     // +0x35
    long CurrentOpinionDeedHandle;              // +0x38
    long HeroOpinionSourcesPushed;              // +0x3c
    long FrameToClearRemainingOpinionSources;   // +0x40
    long FrameToLeavePostBoastState;            // +0x44
    CWorld* GetWorld();                         // 0x00686A70 (CTCBase)
    void EnterCrowdState(long state);           // 0x00784260
    __forceinline bool CurrentStateIsOffPodiumType()
    {
        long state = CrowdState;
        return state == 0 || state == 1 || state == 7 || state == 8 || state == 9 || state == 10;
    }
    bool IsHeroAlive() const { return PHero.GetPItem() && !PHero.GetPItem()->IsDead(); }
    void FrameUpdate();
};

void CTCCreatureGroupBoastingCrowd::FrameUpdate()
{
    if (!IsHeroAlive()) {
        ParentThing->Kill(true);
        return;
    }
    if (HeroOpinionSourcesPushed > 0 && CurrentStateIsOffPodiumType() && FrameToClearRemainingOpinionSources > 0 &&
        GFGetCurrentFrame() > FrameToClearRemainingOpinionSources) {
        FrameToClearRemainingOpinionSources = 0;
        while (HeroOpinionSourcesPushed > 0) {
            COpinionReactionManager* manager = GetWorld()->POpinionReactionManager;
            manager->ClearOpinionSource(PHero.GetPItem(), true);
            --HeroOpinionSourcesPushed;
        }
        EnterCrowdState(0);
    }
    if ((CrowdState == 5 || CrowdState == 6) && GFGetCurrentFrame() > FrameToLeavePostBoastState) {
        FrameToLeavePostBoastState = 0;
        if (!g_pQuestManager->BoastsEmpty())
            EnterCrowdState(CurrentEncouragementState);
        else
            EnterCrowdState(HeroMadeGoodBoast ? 7 : 8);
    }
}