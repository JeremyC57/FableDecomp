// _global::Copy_PairLongCountedPointer_00828cd0(const SPairLongCountedPointer* first, const SPairLongCountedPointer* last, SPairLongCountedPointer* result, const random_access_iterator_tag&, int*) -- assign [first, last) of (key, counted emitter pointer) pairs onto result, return the advanced result
// Leaf: Copy_PairLongCountedPointer_00828cd0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00828CD0, length 145 bytes (`ret 0Ch`).
// Identity: `__copy<std::pair<long, CCountedPointer<...> >*>` -- 12-byte {long, CCountedPointer} elements as the catalog says
//   (`_Move` is the Dinkumware spelling). Callers: CWorldMap region-graph load 0x00828710, the landed buffer-owner destructor
//   0x00828F10, CEngineWaterRenderer::DestroyWaterGenerator 0x00829160 and a heap helper.
// Shape: SGI `__copy(first, last, result, random_access_iterator_tag, Distance*)` (the counted form), `__fastcall`: first in
//   ecx, last in edx, result + the tag reference + the distance pointer on the stack (`ret 0Ch`); the signed count is
//   (last - first) / sizeof(T) (reciprocal multiply), counted down; an empty range returns the untouched result.
//   The CCountedPointer assignment is inlined: both source words read first (the object word spilled to a frame slot),
//   release the old block at zero (destroy function at +4 with the object in ecx, then operator delete 0x00BFE9BC),
//   adopt both words, add a reference.
//   The count is kept in the dead `result` argument slot, result in ebp, the count-block word addressed by esi.
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

struct SPairLongCountedPointer {                // std::pair<long, CCountedPointer<NParticleEngine::CParticleEmitter> >, 12 bytes
    long            first;                      // +0x00
    CCountedPointer second;                     // +0x04
};

SPairLongCountedPointer* __fastcall Copy_PairLongCountedPointer_00828cd0(const SPairLongCountedPointer* first, const SPairLongCountedPointer* last, SPairLongCountedPointer* result, const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}