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
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
template <class T> struct CSpawnedFunc : CSpawnedFuncBase {
    void (T::*Func)();                                  // +0x34
    T* PClass;                                          // +0x38
    CSpawnedFunc(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) {}
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
public:
    void AddSpawnedFunction(CSpawnedFuncBase*, const CCharString&);   // 0x00CB7E50
    bool IsActiveThreadTerminating();   // 0x00CB7940
};
namespace NScript {
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