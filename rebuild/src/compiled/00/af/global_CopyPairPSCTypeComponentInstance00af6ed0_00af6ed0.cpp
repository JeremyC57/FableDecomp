// _global::Copy_PairPSCTypeComponentInstance_00af6ed0(const PairPSCTypeComponentInstance* first, const PairPSCTypeComponentInstance* last, PairPSCTypeComponentInstance* result, const random_access_iterator_tag&, int*) -- assign [first, last) of (component type, component instance) pairs onto result, return the advanced result
// Leaf: Copy_PairPSCTypeComponentInstance_00af6ed0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF6ED0, length 181 bytes (`ret 0Ch`).
// Identity: `__copy<std::pair<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance>*>` -- the catalog element is right
//   (Ego_r.pdb, 0x18 bytes; the word at +4 is CComponentInstance's vtable and is not copied); same model as the landed
//   __copy_backward TU neighbour 0x00AF6E10.
// Shape: SGI `__copy(first, last, result, random_access_iterator_tag, Distance*)` (the counted form), `__fastcall`: first in
//   ecx, last in edx, result + the tag reference + the distance pointer on the stack (`ret 0Ch`); the signed count is
//   (last - first) / sizeof(T) (reciprocal multiply), counted down; an empty range returns the untouched result.
//   The CCountedPointer assignment is inlined: both source words read first (the object word spilled to a frame slot),
//   release the old block at zero (destroy function at +4 with the object in ecx, then operator delete 0x00BFE9BC),
//   adopt both words, add a reference.
//   Result rides in ecx and is re-read from its argument slot after the operator delete call.
struct random_access_iterator_tag { random_access_iterator_tag() { } };   // empty tag, passed by const reference

struct SCountBlock {                            // shared count block of CCountedPointer
    long m_references;                          // +0
    void (__fastcall* m_pfnDestroy)(void*);     // +4
    void* m_pObject;                            // +8
};

struct CCountedPointer {                        // 8 bytes; operator= as the landed 0x0084F130 / 0x00AF6E10
    void*        m_pObject;                     // +0
    SCountBlock* m_pCount;                      // +4
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        SCountBlock* count = other.m_pCount;    // both source words are read first
        void* object = other.m_pObject;
        if (m_pCount != count) {
            if (m_pCount) {
                --m_pCount->m_references;
                if (m_pCount->m_references == 0) {
                    m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                    delete m_pCount;            // operator delete 0x00BFE9BC
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

class CComponentInstance {                      // Ego_r.pdb, 0x14 bytes
public:
    virtual void v00();                         // +0x00 vtable (not copied by the implicit operator=)
    CCountedPointer Component;                  // +0x04 CCountedPointer<CPSCBase>
    bool            Enabled;                    // +0x0C
    unsigned long   InstanceName;               // +0x10
};
}

struct PairPSCTypeComponentInstance {           // std::pair<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance>
    NParticleEngine::EPSCType           first;  // +0x00
    NParticleEngine::CComponentInstance second; // +0x04
};

PairPSCTypeComponentInstance* __fastcall Copy_PairPSCTypeComponentInstance_00af6ed0(const PairPSCTypeComponentInstance* first, const PairPSCTypeComponentInstance* last, PairPSCTypeComponentInstance* result, const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}