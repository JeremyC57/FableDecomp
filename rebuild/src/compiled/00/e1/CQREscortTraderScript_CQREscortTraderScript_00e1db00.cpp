// CQR_EscortTraderScript::CQR_EscortTraderScript(void* owner, CGameScriptInterface* game) -- quest-script constructor: script base, owner/interface pair, vtable 0x012E12E0, empty bonus-object string, four registered timers, fifteen empty speech strings, two arrays of three empty route vectors and two empty region strings
// Leaf: ??0CQR_EscortTraderScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E1DB00, length 291 bytes, `ret 8`.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110, CCharString::CCharString() @ 0x0099E4B0 (BonusObject, the two
// counted array loops of 10 and 5, the two region strings), CGameScriptInterface::RegisterTimer (slot 87, +0x15c)
// through the interface global @ 0x0143E8F8 (four CTimer members).
// Layout: Ego_r.pdb CQR_EscortTraderScript - 0x14, retail STLport vectors 12 bytes: BonusObject +0x74, the four timers
// +0x78..+0x84, RandomSpeechFollow[10] +0x88, RandomSpeechWait[5] +0xb0, TradeRoute +0xc4, RouteDynamic[3] +0xc8,
// RouteStatic[3] +0xec, PreviousLoadedRegion +0x110, CurrentLoadedRegion +0x114, RegionChangeDetected +0x118,
// QuestStage +0x11c.
// Shape: the landed CQ_ArenaScript ctor model (0x00CFA660) -- novtable script-object base, CTimer registering itself
// in its inline ctor, the STLport _Vector_base/_STLP_alloc_proxy vector (the two vector arrays unroll inline).
class CCharString { public:
    void* PStringData;
    CCharString();                                   // 0x0099E4B0
};

class CGameScriptInterface { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual long RegisterTimer();                                        // slot 87 (+0x15c)
};
extern CGameScriptInterface* g_pGameScriptInterface;  // 0x0143E8F8

class CTimer { public:
    long Index;
    CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { }
};

class CScriptBase { public:
    virtual void v00();
    unsigned char _body[0x3c];                       // +0x04..+0x40
    CScriptBase();                                   // 0x00CB8110
};

struct __declspec(novtable) CScriptObject : CScriptBase {
    CGameScriptInterface* m_pGame;                   // +0x40
    void* m_pOwner;                                  // +0x44
    CScriptObject(void* owner, CGameScriptInterface* game) : CScriptBase(), m_pGame(game), m_pOwner(owner) { }
};

template <class T>
struct allocator {
    allocator() { }
};

template <class T, class A>
struct _STLP_alloc_proxy : A {                       // STLport: the end-of-storage pointer over an empty allocator
    T _M_data;
    _STLP_alloc_proxy(const A& a, T p) : A(a), _M_data(p) { }
};

template <class T>
struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, allocator<T> > _M_end_of_storage;
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) { }
};

template <class T>
struct CRetailVector : _Vector_base<T> {             // STLport vector<T>: three pointers (12 bytes in retail)
    CRetailVector() : _Vector_base<T>(allocator<T>()) { }
};

struct CQR_EscortTraderScript : CScriptObject {      // vtable 0x012E12E0
    bool MissionSucceeded;                           // +0x48
    bool MissionFailed;                              // +0x49
    bool HiredByTrader;                              // +0x4a
    bool TraderAtDestination;                        // +0x4b
    bool TraderDepartedLost;                         // +0x4c
    bool GreetedHero;                                // +0x4d
    bool TraderKilled;                               // +0x4e
    bool TraderHit;                                  // +0x4f
    bool TraderFollowing;                            // +0x50
    bool TraderDeparted;                             // +0x51
    long BanditSpawnCount;                           // +0x54
    long BanditKillCount;                            // +0x58
    bool TraderIntroDone;                            // +0x5c
    bool DoneExpressions;                            // +0x5d
    bool TraderCreated;                              // +0x5e
    bool WrongTurnDetected;                          // +0x5f
    bool TheBlackGateIsClosed;                       // +0x60
    bool AddedQuestInfo;                             // +0x61
    bool GoneWrongWay;                               // +0x62
    bool TotallyLost;                                // +0x63
    long TraderHitCount;                             // +0x64
    long TraderHealthBarID;                          // +0x68
    float TraderInitialHealth;                       // +0x6c
    bool ExpressionDetectedFollow;                   // +0x70
    bool ExpressionDetectedWait;                     // +0x71
    bool AddedBonusObject;                           // +0x72
    CCharString BonusObject;                         // +0x74
    CTimer TraderComplainTimer;                      // +0x78
    CTimer TraderExpressionDetectTimer;              // +0x7c
    CTimer TraderHitDetectTimer;                     // +0x80
    CTimer TraderRandomSpeechTimer;                  // +0x84
    CCharString RandomSpeechFollow[10];              // +0x88
    CCharString RandomSpeechWait[5];                 // +0xb0
    long TradeRoute;                                 // +0xc4 (ETradeRoutes)
    CRetailVector<CCharString> RouteDynamic[3];      // +0xc8
    CRetailVector<CCharString> RouteStatic[3];       // +0xec
    CCharString PreviousLoadedRegion;                // +0x110
    CCharString CurrentLoadedRegion;                 // +0x114
    bool RegionChangeDetected;                       // +0x118
    long QuestStage;                                 // +0x11c (EscortTraderQuestStage)
    virtual void v00();
    CQR_EscortTraderScript(void* owner, CGameScriptInterface* game);
};

CQR_EscortTraderScript::CQR_EscortTraderScript(void* owner, CGameScriptInterface* game)
    : CScriptObject(owner, game)
{
}