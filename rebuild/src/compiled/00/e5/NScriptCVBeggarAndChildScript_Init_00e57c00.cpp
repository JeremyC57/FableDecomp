// NScript::CV_BeggarAndChildScript::Init() -- clear the beggar-and-child flags and start both timers at 5
// Leaf: ?Init@CV_BeggarAndChildScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E57C00, 86 bytes, thiscall, plain `ret`.
// Identity (proven): slot 3 (Init) of the RTTI-less level-script vtable 0x012E65E0, which its constructor 0x00E57BB0 (and Alloc 0x00E5D0B0) stores;
//   level-script slots are 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist (FableWin.pdb CScriptBase order, the landed
//   level Init / OnPersist rows). Ego_r.pdb has ?Init@CV_BeggarAndChildScript@NScript@@UAEXXZ.
// Layout: Ego_r.pdb CV_BeggarAndChildScript members, each at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Identity, second source: Ego_r.pdb CV_BeggarAndChildScript is the only level script with nine bools, a long, three
//   bools and two CTimers at +0x5c..+0x74; slot 2 (Main 0x00E57E60) binds "LookoutPointBeggar" / "BeggarBully".
// Callees: CTimer::operator=(long) inline: g_pGameScriptInterface 0x0143E8F8 slot 89 SetTimer.
class CGameScriptInterface {                     // the script interface vtable (slot names: ghidra_out/cgsi_slots_aligned.tsv)
public:
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
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void v088();
    virtual void SetTimer(long timer, long time);                                   // slot 89 (+0x164)
};
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8 (the global script interface, bound name)
class CTimer {                                          // Ego_r.pdb: one long TimerIndex
public:
    long TimerIndex;
    CTimer& operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
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
class CV_BeggarAndChildScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool   BeggarHit;                                   // +0x48
    bool   BeggarLeft;                                  // +0x49
    bool   BullyLeft;                                   // +0x4a
    bool   BullyHit;                                    // +0x4b
    bool   TaughtBelch;                                 // +0x4c
    bool   TaughtSneer;                                 // +0x4d
    bool   TaughtThanks;                                // +0x4e
    bool   TaughtBattleCry;                             // +0x4f
    bool   BullyHasTaunted;                             // +0x50
    long   BelchedAtBeggar;                             // +0x54
    bool   BelchedAtBully;                              // +0x58
    bool   QuestCardGiven;                              // +0x59
    bool   ExpressionTutorialShown;                     // +0x5a
    CTimer TauntTimer;                                  // +0x5c
    CTimer BeggarReplyTimer;                            // +0x60
    virtual void Init();
};

void CV_BeggarAndChildScript::Init()
{
    BeggarHit = false;
    BeggarLeft = false;
    BullyLeft = false;
    BullyHit = false;
    TaughtBelch = false;
    TaughtSneer = false;
    TaughtThanks = false;
    TaughtBattleCry = false;
    BullyHasTaunted = false;
    BelchedAtBeggar = 0;
    BelchedAtBully = false;
    QuestCardGiven = false;
    ExpressionTutorialShown = false;
    TauntTimer = 5;
    BeggarReplyTimer = 5;
}
}