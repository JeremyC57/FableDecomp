// NParticleEngine::CComponentInstance::`scalar deleting destructor'(unsigned int flags) -- run the implicit destructor inline (release the counted pointer at +0x04, clearing Data then Info; then CBaseClass::~CBaseClass 0x0099A2E0) and free the object when bit 0 of the flags is set
// Leaf: ??_GCComponentInstance@NParticleEngine@@UAEPAXI@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00af6b90, 81 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): vtable 0x0129F478 (RTTI .?AVCComponentInstance@NParticleEngine@@, base CBaseClass) slot 0; Ego_r.pdb CComponentInstance::Component (CCountedPointer<NParticleEngine::CPSCBase>, +4), Enabled, InstanceName.
// Callees: the CCPPointerInfo deleter (fastcall), operator delete 0x00BFE9BC, CBaseClass::~CBaseClass 0x0099A2E0.
// Shape (family L036): ~CCountedPointer inline (`--Info->RefCount`, deleter, delete Info, Data = Info = 0), then the
//   base or first-member destructor out of line; `new` emits the vtable and the ??_G, the explicit `p->X::~X()` keeps the ??_G inlining the implicit destructor.
void __cdecl operator delete(void* p);                     // 0x00BFE9BC
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass {
public:
    CBaseClassNonCopyable();
    virtual ~CBaseClassNonCopyable();                     // 0x0099A300
};
namespace NParticleEngine {
class CPSCBase;
class CComponentInstance : public CBaseClass {             // Ego_r.pdb, 20 bytes
public:
    CCountedPointer<CPSCBase> Component;                  // +0x04
    bool Enabled;                                         // +0x0C
    unsigned long InstanceName;                           // +0x10
};
}
NParticleEngine::CComponentInstance* MakeCComponentInstance() { return new NParticleEngine::CComponentInstance; }
void KillCComponentInstance(NParticleEngine::CComponentInstance* p) { p->NParticleEngine::CComponentInstance::~CComponentInstance(); }