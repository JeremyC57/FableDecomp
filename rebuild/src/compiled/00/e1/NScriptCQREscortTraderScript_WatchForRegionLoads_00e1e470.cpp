// NScript::CQR_EscortTraderScript::WatchForRegionLoads() -- spawned watcher: whenever a region finishes loading, shift CurrentLoadedRegion into PreviousLoadedRegion, store the new one and flag the change
// Leaf: ?WatchForRegionLoads@CQR_EscortTraderScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E1E470, 165 bytes, thiscall, plain `ret`.
// Identity (proven): the landed NScript::CQR_EscortTraderScript::Main 0x00E1E110 spawns this address as the
//   CSpawnedFunc<CQR_EscortTraderScript> named "WatchForRegionLoads" (pointer stored at 0x00E1E414); Ego_r.pdb has
//   ?WatchForRegionLoads@CQR_EscortTraderScript@NScript@@QAEXXZ.
// Layout: Ego_r.pdb CQR_EscortTraderScript members, each at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: CCharString() 0x0099E4B0, CCharString::operator=(const CCharString&) 0x0099EFB0, ~CCharString 0x0099EAE0,
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940; interface slots 22 MsgOnRegionLoaded(CCharString&), 7 NewScriptFrame.
// Members: Ego_r PreviousLoadedRegion +0x13c / CurrentLoadedRegion +0x140 / RegionChangeDetected +0x144 sit at retail
//   +0x110 / +0x114 / +0x118 (- 0x14 for CScriptBase, - 4 for each of the six Dinkumware route vectors).
class CCharString {
public:
    void* PData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
class CTimer {                                          // Ego_r.pdb: one long TimerIndex
public:
    long TimerIndex;
};
enum ETradeRoutes { };
enum EscortTraderQuestStage { };
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {   // STLport 4.6: start, finish, end of storage (0xC)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
}
class CGameScriptInterface {                     // the script interface vtable (slot names: ghidra_out/cgsi_slots_aligned.tsv)
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;                                            // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021();
    virtual bool MsgOnRegionLoaded(CCharString& a0) const;   // slot 22 (+0x58)
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
    bool IsActiveThreadTerminating();                   // 0x00CB7940
    unsigned char _body[0x3c];
};
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    TData*                MasterScriptData;             // +0x44
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQR_EscortTraderScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool MissionSucceeded;                              // +0x48
    bool MissionFailed;                                 // +0x49
    bool HiredByTrader;                                 // +0x4a
    bool TraderAtDestination;                           // +0x4b
    bool TraderDepartedLost;                            // +0x4c
    bool GreetedHero;                                   // +0x4d
    bool TraderKilled;                                  // +0x4e
    bool TraderHit;                                     // +0x4f
    bool TraderFollowing;                               // +0x50
    bool TraderDeparted;                                // +0x51
    long BanditSpawnCount;                              // +0x54
    long BanditKillCount;                               // +0x58
    bool TraderIntroDone;                               // +0x5c
    bool DoneExpressions;                               // +0x5d
    bool TraderCreated;                                 // +0x5e
    bool WrongTurnDetected;                             // +0x5f
    bool TheBlackGateIsClosed;                          // +0x60
    bool AddedQuestInfo;                                // +0x61
    bool GoneWrongWay;                                  // +0x62
    bool TotallyLost;                                   // +0x63
    long TraderHitCount;                                // +0x64
    long TraderHealthBarID;                             // +0x68
    float TraderInitialHealth;                          // +0x6c
    bool ExpressionDetectedFollow;                      // +0x70
    bool ExpressionDetectedWait;                        // +0x71
    bool AddedBonusObject;                              // +0x72
    CCharString BonusObject;                            // +0x74
    CTimer TraderComplainTimer;                         // +0x78
    CTimer TraderExpressionDetectTimer;                 // +0x7c
    CTimer TraderHitDetectTimer;                        // +0x80
    CTimer TraderRandomSpeechTimer;                     // +0x84
    CCharString RandomSpeechFollow[10];                 // +0x88
    CCharString RandomSpeechWait[5];                    // +0xb0
    ETradeRoutes TradeRoute;                            // +0xc4
    std::vector<CCharString> RouteDynamic[3];           // +0xc8
    std::vector<CCharString> RouteStatic[3];            // +0xec
    CCharString PreviousLoadedRegion;                   // +0x110
    CCharString CurrentLoadedRegion;                    // +0x114
    bool RegionChangeDetected;                          // +0x118
    EscortTraderQuestStage QuestStage;                  // +0x11c
    void WatchForRegionLoads();
};

void CQR_EscortTraderScript::WatchForRegionLoads()
{
    CCharString region;
    while (!IsActiveThreadTerminating()) {
        while (!Game->MsgOnRegionLoaded(region)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        PreviousLoadedRegion = CurrentLoadedRegion;
        CurrentLoadedRegion = region;
        RegionChangeDetected = true;
        Game->NewScriptFrame();
    }
}
}