// std::fill(pair<EPSCType, CComponentInstance>* first, pair<EPSCType, CComponentInstance>* last, const pair<EPSCType, CComponentInstance>& val) -- assign val to every entry of a particle system's component map
// Leaf: fill
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe @ 0x00AF6D90, 128 bytes, `ret 4`: __fastcall (first ecx -> ebp, last edx -- spilled to the `push ecx`
// frame word -- and the value reference on the stack -> edi). Only call site 0x00AF7684.
// Identity: the catalog's pair<long, CCountedPointer<CContainmentVolume> > (12 bytes) is propagated; the body strides 24
// bytes and never writes +4, the vptr of a polymorphic second member: pair<NParticleEngine::EPSCType,
// CComponentInstance> (Ego_r.pdb; the element of the _Destroy_range twin 0x00AF6F90): first EPSCType +0, second
// CComponentInstance +4 = {vptr, Component CCountedPointer<CPSCBase> +8 / +0xC, Enabled +0x10, InstanceName +0x14}.
// Callees: operator delete @ 0x00BFE9BC (the count block).
// Shape: the pair's implicit copy assignment: the key, then CComponentInstance's implicit (vptr-preserving) assignment:
// the inlined CCountedPointer operator= (both source words read first -- Data parked in the dead value-pointer slot
// `[esp+18h]` -- old block released, new words stored, count bumped), Enabled and InstanceName. The count-block address
// is a second induction register (`lea esi,[ebp+0Ch]`).
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
}
using NParticleEngine::CComponentInstance;
using NParticleEngine::EPSCType;

namespace std {
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
};
template <class ForwardIter, class T>
void __fastcall fill(ForwardIter first, ForwardIter last, const T& val)
{
    for (; first != last; ++first)
        *first = val;
}
typedef pair<EPSCType, CComponentInstance> CComponentEntry;
template void __fastcall fill(CComponentEntry*, CComponentEntry*, const CComponentEntry&);
}