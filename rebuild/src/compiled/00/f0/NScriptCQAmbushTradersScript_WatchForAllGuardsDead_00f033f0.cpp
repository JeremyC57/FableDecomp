// NScript::CQ_AmbushTradersScript::WatchForAllGuardsDead() -- thread: wait until DesertedFarm and GibbetWoods are loaded, then until no trader guard is left alive (GuardAliveCount 0), and raise AllGuardsDead
// Leaf: ?WatchForAllGuardsDead@CQ_AmbushTradersScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F033F0, 344 bytes, thiscall, plain `ret`.
// Identity (proven): the row is referenced once, at 0x00F02B19 inside the quest's Main: the CSpawnedFunc member-pointer
//   store (`mov [edi+34h], 0F033F0h`) right after the spawned thread's name "WatchForAllGuardsDead" 0x012F7CAC is built for
//   the CSpawnedFuncBase ctor 0x00CDD450; Ego_r.pdb / FableWin.pdb ?WatchForAllGuardsDead@CQ_AmbushTradersScript@NScript@@QAEXXZ.
//   The body consumes GuardAliveCount, which the landed EntityCountDecrease 0x00F088E0 lowers. Catalog label sub_00f033f0.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CCharString(const char*, long) 0x0099EBF0, ~CCharString
//   0x0099EAE0; Game (+0x40) slots 7 NewScriptFrame, 12 IsRegionLoaded.
// Literals: "DesertedFarm" 0x012C7778, "GibbetWoods" 0x012C776C.
// Layout (retail = Ego_r.pdb - 0x14, the landed EntityCountDecrease model): AllGuardsDead +0x54, GuardAliveCount +0x70.
// Shape: two region WAIT macros, each `while (!cond) { NewScriptFrame(); if (terminating) return; }` plus a trailing
//   termination test, then a lone test; the guard wait is NOT rotated in retail (test at the loop top, the success arm out of
//   line after the loop's own `ret`): the chunk 576 do-while form `do { if (cond) { ...; return; } NewScriptFrame(); }
//   while (!terminating);` -- the plain `while (GuardAliveCount != 0)` rotates it (test duplicated, 347v344).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CGameScriptInterfaceBase { public:                // slots: ghidra_out/cgsi_slots_aligned.tsv (CGameScriptInterface vtable 0x01260F0C)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual bool IsRegionLoaded(const CCharString& name) const;   // slot 12 (+0x30)
};
class CPersistContext;
class CScriptBase {                                     // retail 0x40 bytes (Ego_r 0x54: debug containers)
public:
    virtual ~CScriptBase();                             // slot 0
protected:
    virtual void RegisterMain();                        // slot 1
    virtual void Main();                                // slot 2
    virtual void Init();                                // slot 3
public:
    virtual void OnPersist(CPersistContext& context);   // slot 4
    unsigned char _body[0x3c];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    TData*                    MasterScriptData;         // +0x44
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_AmbushTradersScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool QuestFailed;                                   // +0x48
    bool QuestFinished;                                 // +0x49
    bool QuestSucceeded;                                // +0x4a
    bool QuestOver;                                     // +0x4b
    bool RegionInvalid;                                 // +0x4c
    bool BanditIntroDone;                               // +0x4d
    bool TraderEscaped;                                 // +0x4e
    bool NoEnemiesLeft;                                 // +0x4f
    bool PhaseComplete;                                 // +0x50
    bool ConvoyTrigger2;                                // +0x51
    bool AllBanditsDead;                                // +0x52
    bool AllTradersDead;                                // +0x53
    bool AllGuardsDead;                                 // +0x54
    bool ContactDead;                                   // +0x55
    bool ContactAttackedByHero;                         // +0x56
    bool HeroDead;                                      // +0x57
    bool SpyDead;                                       // +0x58
    bool SentryAttackedByHero;                          // +0x59
    bool SpySeenBySentry;                               // +0x5a
    bool SpyFollowing;                                  // +0x5b
    bool SpyFollowingHero;                              // +0x5c
    bool SpyHeroProximity;                              // +0x5d
    bool SpyWanderReset;                                // +0x5e
    bool SpyContactMade;                                // +0x5f
    bool SpyEscaped;                                    // +0x60
    bool SpyExited;                                     // +0x61
    bool SpyReachedNoEntryMarker;                       // +0x62
    bool AnyTraderEscaped;                              // +0x63
    bool RegionClear;                                   // +0x64
    bool QuestSuccessSpeechComplete;                    // +0x65
    bool ActivatedPhase3;                               // +0x66
    bool BanditContactIntroComplete;                    // +0x67
    bool BanditContactKilledBeforeQuestEnd;             // +0x68
    bool EnableSpeechSpy;                               // +0x69
    bool EnableSpeechBandit;                            // +0x6a
    long TraderAliveCount;                              // +0x6c
    long GuardAliveCount;                               // +0x70
    void WatchForAllGuardsDead();
};

void CQ_AmbushTradersScript::WatchForAllGuardsDead()
{
    while (!Game->IsRegionLoaded("DesertedFarm")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!Game->IsRegionLoaded("GibbetWoods")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (IsActiveThreadTerminating())
        return;
    do {
        if (GuardAliveCount == 0) {
            if (IsActiveThreadTerminating())
                return;
            AllGuardsDead = true;
            if (IsActiveThreadTerminating())
                return;
            return;
        }
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}
}   // namespace NScript