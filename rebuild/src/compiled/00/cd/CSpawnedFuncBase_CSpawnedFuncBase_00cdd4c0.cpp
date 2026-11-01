// CSpawnedFuncBase::~CSpawnedFuncBase() -- the compiler-generated destructor: release PPredicate, destroy AssociatedRegionName and Name, then the CSuspendableProcessMicrothread base
// Leaf: ??1CSpawnedFuncBase@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CDD4C0, 35 bytes, thiscall, tail `jmp ~CSuspendableProcessMicrothread`.
// Identity (proven): the ctor 0x00CDD450 stores vtable 0x012C2F9C (engine/CSpawnedFuncBase.h) whose slot-0 `??_G` 0x00CDD4A0
//   calls this body; the other 314 callers are the per-instantiation `??_G`s of CSpawnedFunc<T> (implicit dtors, no members
//   with destructors, so each calls the base destructor). Ego_r.pdb and FableWin.pdb both spell ??1CSpawnedFuncBase@@UAE@XZ,
//   and FableWin's donor order ??1CSpawnedFuncBase 0x03B0A3A0 -> ??1?$CCountedPointer@$$CBVCPredicateBase@NScript@@@@QAE@XZ
//   0x03B0A460 is retail's 0x00CDD4C0 -> 0x00CDD4F0. The members are Ego_r's Name (+0x20), AssociatedRegionName (+0x24),
//   PPredicate (CCountedPointer<const NScript::CPredicateBase>, +0x2C). No vtable store: the destructor is the implicit one.
//   The catalog's "_Dest_val<allocator<CMacro>,CMacro>" is propagated; the landed binding ?AutoTinyCompositeDestructor@@YIXPAX@Z
//   (caller 0x00D30000) is invented.
// Callees: CCountedPointer<const NScript::CPredicateBase>::~CCountedPointer 0x00CDD4F0 (out of line in retail),
//   CCharString::~CCharString 0x0099EAE0 (twice), CSuspendableProcessMicrothread::~CSuspendableProcessMicrothread 0x00A44620
//   (protected virtual, ??1CSuspendableProcessMicrothread@@MAE@XZ).
// Shape: members destroyed in reverse declaration order, then the base. DestroyOne (a non-virtual destruction by name) makes
//   VC7.1 emit this out-of-line implicit `??1`, nothing more.
// Layout: local CSpawnedFuncBase -- engine/CSpawnedFuncBase.h declares a user destructor (a user `~T() {}` would add the
//   vtable store retail lacks) and flattens Name / AssociatedRegionName / PPredicate into raw pointers; the base comes from
//   the shared engine/CSuspendableProcessMicrothread.h (protected virtual destructor, PMicrothread +0x10). CCountedPointer is
//   declared locally with a declaration-only destructor: retail calls the out-of-line
//   ??1?$CCountedPointer@$$CBVCPredicateBase@NScript@@@@QAE@XZ 0x00CDD4F0 here, while engine/CCountedPointer.h's inline body
//   is expanded by VC7.1 (76v35, probe 3).
class CCharStringData;
class CCPPointerInfo;

class CCharString {
public:
    CCharStringData* PStringData;
    ~CCharString();                               // 0x0099EAE0
};

template <class T> class CCountedPointer {
public:
    T*              Data;                         // +0x00
    CCPPointerInfo* Info;                         // +0x04
    ~CCountedPointer();                           // CCountedPointer<const NScript::CPredicateBase>: 0x00CDD4F0
};

namespace NScript { class CPredicateBase; }

#include "engine/CSuspendableProcessMicrothread.h"

class CSpawnedFuncBase : public CSuspendableProcessMicrothread {   // vtable 0x012C2F9C
public:
    CSpawnedFuncBase(const CCharString&, unsigned long);           // 0x00CDD450
    unsigned long ID;                                              // +0x14
    long          LastScriptFrame;                                 // +0x18
    long          CurrentScriptFrame;                              // +0x1c
    CCharString   Name;                                            // +0x20
    CCharString   AssociatedRegionName;                            // +0x24
    bool          FuncPaused;                                      // +0x28
    bool          Interrupted;                                     // +0x29
    CCountedPointer<const NScript::CPredicateBase> PPredicate;     // +0x2c
};

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit `??1` (chunk 656/657).
void DestroyOne(CSpawnedFuncBase* func)
{
    func->CSpawnedFuncBase::~CSpawnedFuncBase();
}