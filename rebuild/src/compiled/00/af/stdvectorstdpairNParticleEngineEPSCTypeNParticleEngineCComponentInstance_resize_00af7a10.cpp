// std::vector<std::pair<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance> >::resize(unsigned int newSize) -- STLport one-argument resize: resize(newSize, value_type()) with a default (type 0, no component, enabled) instance built and destroyed in the frame
// Leaf: resize
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

struct SCountBlock {
    long m_references;                          // +0
    void (__fastcall* m_pfnDestroy)(void*);     // +4 per-type deleter, object in ecx
    void* m_pObject;                            // +8
};
template <class T> class CCountedPointer { public:
    T* Data;
    SCountBlock* m_pCount;
    CCountedPointer() : Data(0), m_pCount(0) { }
    ~CCountedPointer()
    {
        if (m_pCount && --m_pCount->m_references == 0) {
            m_pCount->m_pfnDestroy(m_pCount->m_pObject);
            operator delete(m_pCount);
        }
        Data = 0;
        m_pCount = 0;
    }
};
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