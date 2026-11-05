// std::vector<std::pair<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance> >::resize(unsigned int newSize) -- STLport one-argument resize: resize(newSize, value_type()) with a default (type 0, no component, enabled) instance built and destroyed in the frame
// Leaf: resize
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pCount->Info are the header CCountedPointer's (same offsets); the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF7A10, 120 bytes (`ret 4`).
// Identity: the manifest label "CThingCreatureBase::GetAnimsWithNameStartingWith" is propagated. The only caller
//   (0x00AF4F19, in CParticleSystem::DatabaseCopy) resizes the Components CVectorMap<EPSCType, CComponentInstance>
//   vector; the callee 0x00AF7970 (manifest "vector<CUIMiniMapMarker>::resize", propagated) strides 24 bytes
//   (`imul 2AAAAAABh ; sar 2`, `lea [n+n*2]*8`) = pair<EPSCType, CComponentInstance>. The frame vtable 0x0129F478 is
//   NParticleEngine::CComponentInstance (RTTI locator at vtable-4), its base CBaseClass (vtable 0x01231710).
// Callees: CBaseClass::CBaseClass() @ 0x0099A2D0, resize(unsigned int, const value_type&) @ 0x00AF7970, the component's
//   count block destroy function (+4, object +8), operator delete @ 0x00BFE9BC, CBaseClass::~CBaseClass() @ 0x0099A2E0.
// Layout (Ego_r.pdb = retail): pair.first (EPSCType) +0x00; CComponentInstance (20 bytes) +0x04: vptr +0x00,
//   Component (CCountedPointer<CPSCBase>) +0x04/+0x08, Enabled +0x0c (true by default), InstanceName +0x10 (left unset).
// Shape: the landed 0x0056A230 / 0x00BBDCB0 resize(n) form; the CComponentInstance ctor's own vtable store follows the
//   out-of-line base ctor; the counted-pointer dtor clears both words; the base dtor is out of line.
void __cdecl operator delete(void* block);              // 0x00BFE9BC

#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
namespace NParticleEngine {
enum EPSCType { PSC_NONE = 0 };
class CPSCBase;
class CComponentInstance : public CBaseClass {  // vtable 0x0129F478
public:
    CCountedPointer<CPSCBase> Component;        // +0x04
    bool Enabled;                               // +0x0c
    unsigned long InstanceName;                 // +0x10
    CComponentInstance() : Enabled(true) { }
};
}
struct SComponentPair {                         // std::pair<EPSCType, CComponentInstance>
    NParticleEngine::EPSCType first;
    NParticleEngine::CComponentInstance second;
    SComponentPair() : first(NParticleEngine::EPSCType()), second() { }
};

struct CComponentInstanceVector {               // std::vector<pair<EPSCType, CComponentInstance> >: first/last/end
    SComponentPair* first;
    SComponentPair* last;
    SComponentPair* end_of_storage;
    void resize(unsigned int newSize, const SComponentPair& value);     // 0x00AF7970
    void resize(unsigned int newSize);
};

void CComponentInstanceVector::resize(unsigned int newSize)
{
    resize(newSize, SComponentPair());
}