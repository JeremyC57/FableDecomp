// CASoundBank::`scalar deleting destructor'(unsigned int flags) -- run the implicit destructor inline (release the counted pointer at +0x04, clearing Data then Info; then CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300) and free the object when bit 0 of the flags is set
// Leaf: ??_GCASoundBank@@UAEPAXI@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Naming (naming wave 24): ~CBaseClassNonCopyable 0x0099A300 is protected virtual (Ego_r.pdb / FableWin.pdb ??1CBaseClassNonCopyable@@MAE@XZ, engine/CBaseClassNonCopyable.h).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a0bd80, 81 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): vtable 0x0129C9AC (RTTI .?AVCASoundBank@@, bases CBaseClassNonCopyable, CBaseClass) slot 0; Ego_r.pdb CASoundBank::PSymbolMap (CCountedPointer<CCRCSymbolMap>, +4); the implicit destructor stores no vtable of its own.
// Callees: the CCPPointerInfo deleter (fastcall), operator delete 0x00BFE9BC, CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300.
// Shape (family L036): ~CCountedPointer inline (`--Info->RefCount`, deleter, delete Info, Data = Info = 0), then the
//   base or first-member destructor out of line; `new` emits the vtable and the ??_G, the explicit `p->X::~X()` keeps the ??_G inlining the implicit destructor.
void __cdecl operator delete(void* p);                     // 0x00BFE9BC
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass {
public:
    CBaseClassNonCopyable();
    protected:
    virtual ~CBaseClassNonCopyable();                     // 0x0099A300
    public:
};
class CCRCSymbolMap;
class CASoundBank : public CBaseClassNonCopyable {         // Ego_r.pdb, 12 bytes
public:
    CCountedPointer<CCRCSymbolMap> PSymbolMap;            // +0x04
};
CASoundBank* MakeCASoundBank() { return new CASoundBank; }
void KillCASoundBank(CASoundBank* p) { p->CASoundBank::~CASoundBank(); }