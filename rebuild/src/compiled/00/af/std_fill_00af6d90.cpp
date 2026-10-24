// std::fill(pair<EPSCType, CComponentInstance>* first, pair<EPSCType, CComponentInstance>* last, const pair<EPSCType, CComponentInstance>& val) -- assign val to every entry of a particle system's component map
// Leaf: fill
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
struct SCountBlock {
    long m_references;                          // +0x00
    void (__fastcall* m_pfnDestroy)(void*);     // +0x04
    void* m_pObject;                            // +0x08
};

template <class T>
struct CCountedPointer {
    T* Data;                                    // +0x00
    SCountBlock* Info;                          // +0x04
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        SCountBlock* info = other.Info;         // both source words first (retail: edi / the frame word)
        T* data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->m_references;
                if (Info->m_references == 0) {
                    Info->m_pfnDestroy(Info->m_pObject);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->m_references;
        }
        return *this;
    }
};

struct CBaseClass {
    virtual ~CBaseClass();                      // 0x0099A2E0
};
namespace NParticleEngine {
enum EPSCType { PSC_NONE };
class CPSCBase;
struct CComponentInstance : CBaseClass {        // 20 bytes
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