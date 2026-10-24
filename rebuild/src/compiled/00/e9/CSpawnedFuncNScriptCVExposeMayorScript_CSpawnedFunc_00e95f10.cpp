// CSpawnedFunc<NScript::CV_ExposeMayorScript>::CSpawnedFunc(const CCharString& name, unsigned long id, NScript::CV_ExposeMayorScript* pclass, void (NScript::CV_ExposeMayorScript::*func)()) -- build a spawned script function bound to a script object and its member callback
// Leaf: ??0?$CSpawnedFunc@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E95F10, 112 bytes, `__fastcall`, `ret 10h`; vtable 0x012ECBA8. FableWin.pdb has the shape
//   ??0?$CSpawnedFunc@V...@@@@QAE@ABVCCharString@@KPAV...@P8...@AEXXZ@Z for every script T; the only caller 0x00E94C0B passes
//   CV_ExposeMayorScript::DoMission 0x00E953E0, so T is CV_ExposeMayorScript (the manifest's CExpression_FollowScript is propagated).
// Callees: the inlined CSpawnedFuncBase ctor (0x00CDD450 out of line): process base ctor 0x00A44740 (1, 0x40, 0.1f),
//   CCharString(const CCharString&) 0x0099EC30, CCharString() 0x0099E4B0.
// Layout: Ego_r.pdb CSpawnedFunc<T> 0x3C = CSpawnedFuncBase (0x34) + Func (+0x34, single-inheritance member pointer) + PClass (+0x38).
//   Local polymorphic model (engine/CSpawnedFuncBase.h flattens the vptr).
// Shape: the base ctor is an inline definition in this TU; initialisers Func(func), PClass(pclass) -- VC7.1 schedules the PClass store
//   before the derived vtable store and Func after it.
class CCharString {                                       // one pointer to the shared string data
public:
    void* PStringData;                                    // +0x00
    CCharString();                                        // 0x0099E4B0
    CCharString(const CCharString& other);                // 0x0099EC30
    ~CCharString();                                       // 0x0099EAE0
};
namespace NScript { class CPredicateBase; }
struct CCPPointerInfo;
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
inline CSpawnedFuncBase::CSpawnedFuncBase(const CCharString& name, unsigned long id)
    : CScriptProcessBase(true, 0x40, 0.1f), ID(id), LastScriptFrame(-1), CurrentScriptFrame(-1), Name(name),
      FuncPaused(false), Interrupted(false)
{
}
template <class T> class CSpawnedFunc : public CSpawnedFuncBase {   // Ego_r.pdb: 0x3C bytes
public:
    void (T::*Func)();                                    // +0x34
    T* PClass;                                            // +0x38
    CSpawnedFunc(const CCharString& name, unsigned long id, T* pclass, void (T::*func)());
    virtual void SuspendableProcess();
};
template <class T> CSpawnedFunc<T>::CSpawnedFunc(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
    : CSpawnedFuncBase(name, id), Func(func), PClass(pclass)
{
}
namespace NScript { class CV_ExposeMayorScript { public: void DoMission(); }; }
template CSpawnedFunc<NScript::CV_ExposeMayorScript>::CSpawnedFunc(const CCharString& name, unsigned long id, NScript::CV_ExposeMayorScript* pclass, void (NScript::CV_ExposeMayorScript::*func)());