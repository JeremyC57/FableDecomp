// CSpawnedFunc2<NScript::CHeroBoastsScript,long,CCharString>::SuspendableProcess() -- run the spawned call: invoke the bound member callback with the stored long and a copy of the stored string, then flag the process to stop
// Leaf: SuspendableProcess
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CE1C70, 31 bytes, `__fastcall`, `ret`; vtable slot at 0x012C36C8. FableWin.pdb
//   ?SuspendableProcess@?$CSpawnedFunc2@VCHeroBoastsScript@NScript@@JVCCharString@@@@UAEXXZ.
// Callees: CCharString(const CCharString&) 0x0099EC30 for Par2 (pushed first, right to left), then Par1 and the member-pointer call.
// Layout: Ego_r.pdb CSpawnedFunc2<CHeroBoastsScript,long,CCharString> 0x44: Func +0x34, PClass +0x38, Par1 +0x3C, Par2 +0x40;
//   StopRunning +0x05. Local polymorphic model (engine/CSpawnedFuncBase.h flattens the vptr).
class CCharString {                                       // one pointer to the shared string data
public:
    void* PStringData;                                    // +0x00
    CCharString();                                        // 0x0099E4B0
    CCharString(const CCharString& other);                // 0x0099EC30
    ~CCharString();                                       // 0x0099EAE0
};
namespace NScript { class CPredicateBase; }
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T* Data;                                              // +0x00
    CCPPointerInfo* Info;                                 // +0x04
    CCountedPointer() : Data(0), Info(0) { }
};
// The 0x14-byte process base: CASuspendableProcess (vptr, Running +4, StopRunning +5, TerminateUsingExceptions +6,
// TimeProcessSliceStarted +8, TimesliceLength +0xC) plus the dword at +0x10 its (bool, long, float) ctor 0x00A44740 zeroes.
class CScriptProcessBase {
public:
    bool Running;                                         // +0x04
    bool StopRunning;                                     // +0x05
    bool TerminateUsingExceptions;                        // +0x06
    float TimeProcessSliceStarted;                        // +0x08
    float TimesliceLength;                                // +0x0C
    unsigned long ProcessState;                           // +0x10
    CScriptProcessBase(bool running, long stackSize, float timesliceFraction);   // 0x00A44740
    virtual ~CScriptProcessBase();
    virtual void SuspendableProcess() = 0;
};
class CSpawnedFuncBase : public CScriptProcessBase {      // Ego_r.pdb: 0x34 bytes
public:
    unsigned long ID;                                     // +0x14
    long LastScriptFrame;                                 // +0x18
    long CurrentScriptFrame;                              // +0x1C
    CCharString Name;                                     // +0x20
    CCharString AssociatedRegionName;                     // +0x24
    bool FuncPaused;                                      // +0x28
    bool Interrupted;                                     // +0x29
    CCountedPointer<const NScript::CPredicateBase> PPredicate;   // +0x2C
    CSpawnedFuncBase(const CCharString& name, unsigned long id);
    virtual ~CSpawnedFuncBase();
};
template <class T, class P1, class P2> class CSpawnedFunc2 : public CSpawnedFuncBase {   // Ego_r.pdb: 0x44 bytes
public:
    void (T::*Func)(P1, P2);                              // +0x34
    T* PClass;                                            // +0x38
    P1 Par1;                                              // +0x3C
    P2 Par2;                                              // +0x40
    virtual void SuspendableProcess();
};
template <class T, class P1, class P2> void CSpawnedFunc2<T, P1, P2>::SuspendableProcess()
{
    (PClass->*Func)(Par1, Par2);
    StopRunning = true;
}
namespace NScript { class CHeroBoastsScript { public: void Boast(long n, CCharString text); }; }
template void CSpawnedFunc2<NScript::CHeroBoastsScript, long, CCharString>::SuspendableProcess();