// std::_Destroy_range(pair<EPSCType, CComponentInstance>* first, pair<EPSCType, CComponentInstance>* last, const allocator&) -- run the element destructor on [first, last) of a particle system's component map
// Leaf: _Destroy_range
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe @ 0x00AF6F90, 85 bytes, `ret 4`: __fastcall (first ecx, last edx, the empty allocator by const
// reference on the stack, never read). Eleven call sites 0x00AF451D .. 0x00AF79B7; the landed
// NParticleEngine::CParticleSystem destructor 0x00AF5900 calls it for its Components CVectorMap.
// Types (Ego_r.pdb): pair<NParticleEngine::EPSCType, CComponentInstance> (24 bytes) = first EPSCType +0, second
//   CComponentInstance +4 (20 bytes: vptr +0, Component CCountedPointer<CPSCBase> +4/+8, Enabled +0xC, InstanceName +0x10).
//   The catalog's `pair<long, CCompositeSurfaceInfo>` is propagated.
// Callees: CBaseClass::~CBaseClass @ 0x0099A2E0 (out of line, stores the base vtable), operator delete @ 0x00BFE9BC.
// Shape: CComponentInstance is polymorphic with a compiler-generated destructor, so VC7.1 drops its own vtable reset:
// the Component release (count-block induction register `lea esi,[edi+0Ch]`, both words zeroed) is followed directly by
// the base destructor call on the vptr word (`lea ecx,[esi-8]`).
#include "engine/CCountedPointer.h"


#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
namespace NParticleEngine {
enum EPSCType { PSC_NONE };
class CPSCBase;
class CComponentInstance : public CBaseClass { public:        // 20 bytes
    CCountedPointer<CPSCBase> Component;        // +0x04
    bool                      Enabled;          // +0x0C
    unsigned long             InstanceName;     // +0x10
};
struct SComponentPair {                         // std::pair<EPSCType, CComponentInstance>, 24 bytes
    EPSCType           first;                   // +0x00
    CComponentInstance second;                  // +0x04
};
}
using NParticleEngine::SComponentPair;

namespace std {
template <class T> struct allocator { allocator() {} };
template <class T>
void __fastcall _Destroy_range(T* first, T* last, const allocator<T>&)
{
    for (; first != last; ++first)
        first->~T();
}
template void __fastcall _Destroy_range(SComponentPair*, SComponentPair*, const allocator<SComponentPair>&);
}