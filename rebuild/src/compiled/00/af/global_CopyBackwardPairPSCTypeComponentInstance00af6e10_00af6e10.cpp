// _global::CopyBackward_PairPSCTypeComponentInstance_00af6e10(pair<NParticleEngine::EPSCType,NParticleEngine::CComponentInstance>*, pair<..>*, pair<..>*, const random_access_iterator_tag&, int*) -- SGI __copy_backward over the particle system's (component type, component instance) pairs
// Leaf: CopyBackward_PairPSCTypeComponentInstance_00af6e10
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF6E10, length 181 bytes (+ int3 padding).
// SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first in
// ecx, last in edx, result + the tag reference + the distance pointer on the stack, `ret 0Ch`); count = (last - first)
// / 0x18, kept in a frame word; the result pointer stays in ecx and is spilled to its argument slot across the
// operator delete call; the moved-back result is returned.
// Identity: the manifest's `pair<long,CCountedPointer<CNavConnectorNode> >` label is propagated (that pair is 12
// bytes). The 0x18-byte element skips the word at +4 when it copies -- a vtable pointer -- and the TU neighbour
// 0x00AF6ED0 is `_Move<pair<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance>*>` over the same stride.
// Ego_r.pdb CComponentInstance (0x14): vtable +0, CCountedPointer<CPSCBase> Component +4, bool Enabled +0xC,
// unsigned long InstanceName +0x10; so the element is pair<EPSCType, CComponentInstance>. Its implicit operator=
// copies the key, leaves the vtable alone, runs the inlined CCountedPointer::operator= (source fields read first;
// when the count blocks differ release the old block -- destroy function at +4 with the object, then operator delete
// @ 0x00BFE9BC -- adopt both words and add a reference; landed model 0x0084F130), then Enabled and InstanceName.
struct SCountBlock {
    long m_references;                       // +0
    void (__fastcall* m_pfnDestroy)(void*);  // +4
    void* m_pObject;                         // +8
};

struct CCountedPointer {                     // CCountedPointer<NParticleEngine::CPSCBase>, 8 bytes
    void*        m_pObject;                  // +0
    SCountBlock* m_pCount;                   // +4
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        SCountBlock* count = other.m_pCount;
        void* object = other.m_pObject;
        if (m_pCount != count) {
            if (m_pCount) {
                --m_pCount->m_references;
                if (m_pCount->m_references == 0) {
                    m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                    delete m_pCount;
                }
            }
            m_pObject = object;
            m_pCount = count;
            if (count)
                ++count->m_references;
        }
        return *this;
    }
};

namespace NParticleEngine {
enum EPSCType { PSC_TYPE_NONE = 0 };

class CComponentInstance {                   // Ego_r.pdb, 0x14 bytes
public:
    virtual void v00();                      // +0x00 vtable (not copied by the implicit operator=)
    CCountedPointer Component;               // +0x04 CCountedPointer<CPSCBase>
    bool            Enabled;                 // +0x0C
    unsigned long   InstanceName;            // +0x10
};
}

struct PairPSCTypeComponentInstance {        // std::pair<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance>
    NParticleEngine::EPSCType           first;   // +0x00
    NParticleEngine::CComponentInstance second;  // +0x04
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

PairPSCTypeComponentInstance* __fastcall CopyBackward_PairPSCTypeComponentInstance_00af6e10(PairPSCTypeComponentInstance* first, PairPSCTypeComponentInstance* last, PairPSCTypeComponentInstance* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}