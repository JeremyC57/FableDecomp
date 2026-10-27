// NScript::CV_IntroductionToTrophiesScript::Init() -- reset the Briar Rose trophies introduction: speech flags, hit count, opinions and both timers
// Leaf: ?Init@CV_IntroductionToTrophiesScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E9C3B0, 93 bytes, thiscall, plain `ret`.
// Identity (proven): slot 3 (Init) of the RTTI-less level-script vtable 0x012ED558, which its constructor 0x00E9C360 (and Alloc 0x00E9FB40) stores;
//   level-script slots are 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist (FableWin.pdb CScriptBase order, the landed
//   level Init / OnPersist rows). Ego_r.pdb has ?Init@CV_IntroductionToTrophiesScript@NScript@@UAEXXZ.
// Layout: Ego_r.pdb CV_IntroductionToTrophiesScript members, each at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Identity, second source: Ego_r.pdb CV_IntroductionToTrophiesScript is the only level script with CTimers at +0x78/+0x7c
//   after bools and two BriarRoseOpinion values; slot 2 (Main 0x00E9C4B0) binds "BriarRose" / "TrophiesIntroWatcher".
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
enum BriarRoseOpinion { };                              // Ego_r.pdb enum (value 0 unnamed here)
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
class CV_IntroductionToTrophiesScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool             DoneBriarIntroSpeech;              // +0x48
    bool             DoneBriarTrophySpeech;             // +0x49
    bool             AttackedByHero;                    // +0x4a
    bool             DoneApproachMessage1;              // +0x4b
    bool             DoneApproachMessage2;              // +0x4c
    bool             AttackSpeechComplete;              // +0x4d
    long             HitByHeroCount;                    // +0x50
    bool             HitWarningComplete;                // +0x54
    bool             InsideTavern;                      // +0x55
    bool             QuestComplete;                     // +0x56
    BriarRoseOpinion OpinionCurrent;                    // +0x58
    BriarRoseOpinion OpinionRequired;                   // +0x5c
    bool             CutsceneStarted;                   // +0x60
    bool             OpinionTimerReset;                 // +0x61
    bool             OutsideTavernTimerReset;           // +0x62
    CTimer           OpinionTimer;                      // +0x64
    CTimer           OutsideTavernTimer;                // +0x68
    virtual void Init();
};

void CV_IntroductionToTrophiesScript::Init()
{
    DoneBriarIntroSpeech = false;
    DoneBriarTrophySpeech = false;
    AttackedByHero = false;
    DoneApproachMessage1 = false;
    DoneApproachMessage2 = false;
    AttackSpeechComplete = false;
    HitByHeroCount = 0;
    HitWarningComplete = true;
    InsideTavern = false;
    CutsceneStarted = false;
    OpinionCurrent = (BriarRoseOpinion)0;
    OpinionRequired = (BriarRoseOpinion)0;
    OpinionTimerReset = false;
    OutsideTavernTimerReset = false;
    QuestComplete = false;
    OpinionTimer = 0;
    OutsideTavernTimer = 0;
}
}