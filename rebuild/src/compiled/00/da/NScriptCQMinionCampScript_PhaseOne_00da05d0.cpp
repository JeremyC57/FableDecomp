// CQ_MinionCampScript::PhaseOne() -- the minion camp's first phase: wait for the intro, start the first-wave control thread, wait for Briar to reach the altar and start the spawn-point control thread, then hold the minion cap at three while the chant runs until the portal is sealed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DA05D0, length 585 bytes.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940, CSpawnedFuncBase::CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc vtable
//   0x012D6CAC), CScriptBase::AddSpawnedFunction 0x00CB7E50, RunFirstWaveControl 0x00DA0820, RunSpawnPointControl
//   0x00DA0C40; the game script interface's NewScriptFrame (slot 7); the timer service (global 0x0143E8F8) GetTimer
//   (slot 90) through an inline CTimer::GetTime.
// Layout: the landed CQ_MinionCampScript ctor 0x00D9E210 (retail = Ego_r.pdb - 0x14).
// Shape: the landed CinemaTest Main spawn model (flag word in ebx guarding the name temporaries).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
struct CSpawnedFuncBase {                               // retail 0x34 bytes
    virtual ~CSpawnedFuncBase();
    virtual void RunSuspendableProcess();
    virtual void SuspendSuspendableProcess(bool suspend);
    virtual void Restart();
    virtual void SuspendableProcess() = 0;
    unsigned char _body[0x30];                          // +0x04 .. +0x34
    CSpawnedFuncBase(const CCharString& name, unsigned long id);   // 0x00CDD450
};
template <class T> struct CSpawnedFunc : CSpawnedFuncBase {
    void (T::*Func)();                                  // +0x34
    T* PClass;                                          // +0x38
    __forceinline CSpawnedFunc(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) {}
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};

struct CGameScriptInterface {                       // the quest's game script interface (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c) NewScriptFrame
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
    virtual void v088(); virtual void v089();
    virtual long GetTimer(long timer) const;   // slot 90 (+0x168) GetTimer
};
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
struct CTimer {                                         // a script timer: the interface's timer index
    long Index;
    long GetTime() const { return g_pGameScriptInterface->GetTimer(Index); }
};
namespace NScript {
struct CScriptBase {                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                                                 // 0x00CB7940
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
};
class CQ_MinionCampScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void* Data;                                         // +0x44
    bool QuestStartScreenShown;                         // +0x48
    bool IntroDone;                                     // +0x49
    bool BriarRoseAtAltar;                              // +0x4a
    bool MinionsSummoned;                               // +0x4b
    bool PortalSealed;                                  // +0x4c
    bool MissionFailed;                                 // +0x4d
    long NumberOfMinions;                               // +0x50
    long MaxNumberOfMinions;                            // +0x54
    float MinMinionSpawnPeriod;                         // +0x58
    float MaxMinionSpawnPeriod;                         // +0x5c
    CTimer ChantTimer;                                  // +0x60
    long ChantTimerBar;                                 // +0x64
    bool ChantTimerPaused;                              // +0x68
    long PausedTime;                                    // +0x6c
    CTimer MinionSpawnTimer;                            // +0x70
    long CurrentStage;                                  // +0x74 (MissionStage)
    void PhaseOne();
    void RunFirstWaveControl();                         // 0x00DA0820
    void RunSpawnPointControl();                        // 0x00DA0C40
};
void CQ_MinionCampScript::PhaseOne()
{
    while (!IntroDone && !PortalSealed && !MissionFailed) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CurrentStage = 2;
    SpawnFunction(new CSpawnedFunc<CQ_MinionCampScript>("RunFirstWaveControl", 0, this, &CQ_MinionCampScript::RunFirstWaveControl));
    while (!BriarRoseAtAltar && !PortalSealed && !MissionFailed) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (BriarRoseAtAltar) {
        if (IsActiveThreadTerminating())
            return;
        CurrentStage = 3;
        SpawnFunction(new CSpawnedFunc<CQ_MinionCampScript>("RunSpawnPointControl", 0, this, &CQ_MinionCampScript::RunSpawnPointControl));
    }
    while (!PortalSealed && !MissionFailed) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (ChantTimer.GetTime() == 0x30) {
            if (IsActiveThreadTerminating())
                return;
            MaxNumberOfMinions = 3;
        }
        if (ChantTimer.GetTime() == 0x18) {
            if (IsActiveThreadTerminating())
                return;
            MaxNumberOfMinions = 3;
        }
    }
    if (IsActiveThreadTerminating())
        return;
    if (MissionFailed)
        return;
    if (IsActiveThreadTerminating())
        return;
    CurrentStage = 4;
}
}