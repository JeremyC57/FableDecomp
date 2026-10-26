// NScript::CV_KnotholeGladeGatesScript::PhaseOne() -- spawns the DoPanicBanter thread in KnotholeGlade, runs the initial balverine attack and two rounds of ControlMainBalverines, then marks BalvsKilled and waits frame by frame until GatesOpened
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA1630, 232 bytes (padded to 240), plain `ret`.
//   PDB name ?PhaseOne@CV_KnotholeGladeGatesScript@NScript@@QAEXXZ.
// Callees: operator new 0x00BFEA1A; CSpawnedFuncBase::CSpawnedFuncBase(const CCharString&, unsigned long) 0x00CDD450
//   (CSpawnedFunc<CV_KnotholeGladeGatesScript> vtable 0x012ED9E8, 0x3c bytes: Func +0x34, PClass +0x38);
//   CScriptBase::AddSpawnedFunction(CSpawnedFuncBase*, const CCharString&) 0x00CB7E50; DoPanicBanter 0x00EA1EC0 (the
//   stored member-function pointer); DoInitialBalverineAttack 0x00EA1720; ControlMainBalverines 0x00EA2080;
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940; CGameScriptInterface slot 7 NewScriptFrame (+0x1c);
//   CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0.
// Strings: "KnotholeGlade" 0x012C513C, "DoPanicBanter" 0x012EDBF8.
// Shape: the region name is an implicit conversion temporary (built before operator new, i.e. before every other
//   argument); the thread name exists only when operator new succeeded, so VC guards its destructor with a flag in ebx
//   (landed CQ_FireHeartScript::CheckCompletion 0x00D3AD60 model); the waits are the script yield idiom
//   `if (IsActiveThreadTerminating()) return;`.
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, GatesOpened +0x4d, BalvsKilled +0x51.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
class CSpawnedFuncBase { public:                               // retail 0x34 bytes
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
    CSpawnedFunc(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) {}
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};
class CGameScriptInterface { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame();   // slot 7 (+0x1c)
};
namespace NScript {
struct CScriptBase {                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
};
class CV_KnotholeGladeGatesScript : public CScriptBase { public:
    CGameScriptInterface* Game;                         // +0x40
    void* Data;                                         // +0x44
    bool MissionSucceeded, MissionFailed, MissionOver, Cutscene1Start, HeroAlertedToProblem;   // +0x48..+0x4c
    bool GatesOpened;                                   // +0x4d
    bool SaidBefore, HeroKilledAVillager, IntroductionFinished;   // +0x4e..+0x50
    bool BalvsKilled;                                   // +0x51
    void PhaseOne();
    void DoPanicBanter();                               // 0x00EA1EC0
    void DoInitialBalverineAttack();                    // 0x00EA1720
    void ControlMainBalverines();                       // 0x00EA2080
};
void CV_KnotholeGladeGatesScript::PhaseOne()
{
    AddSpawnedFunction(new CSpawnedFunc<CV_KnotholeGladeGatesScript>("DoPanicBanter", 0, this,
                                                                    &CV_KnotholeGladeGatesScript::DoPanicBanter),
                       "KnotholeGlade");
    DoInitialBalverineAttack();
    for (long round = 0; round < 2; ++round) {
        if (IsActiveThreadTerminating())
            return;
        ControlMainBalverines();
    }
    if (IsActiveThreadTerminating())
        return;
    BalvsKilled = true;
    while (!GatesOpened) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
}
}