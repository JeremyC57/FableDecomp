// _global::Copy_PairUInt64CountedPointer_00cbb120(PairUInt64CountedPointer*, PairUInt64CountedPointer*, PairUInt64CountedPointer*, SRandomAccessIteratorTag, long*) -- SGI __copy over 16-byte (64-bit key, counted pointer) pairs, returns the advanced result
// Leaf: Copy_PairUInt64CountedPointer_00cbb120
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBB120, length 143 bytes.
// Callees: operator delete @ 0x00BFE9BC (the count block's destroy function is called through its slot +4).
// SGI-STL `__copy(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first in
// ecx, last in edx, result + the two tag arguments on the stack, `ret 0Ch`). The signed count (last - first) / 16 (a plain `sar 4`)
// is computed once and counted down in the dead `result` argument slot (result itself lives in ebp). Element:
// 16 bytes, a 64-bit key + the counted pointer at +8 (the manifest label says pair<long, CCountedPointer<CParticleEmitter> >, which is 12 bytes: propagated); the pair's
// implicit operator= copies the key, then runs the inlined CCountedPointer::operator= -- the source's count block
// and object are read first (retail keeps them in ebx and the `push ecx` frame word across the release), when
// the blocks differ the destination's block is released (destroy function at +4 with the object at +8 in ecx,
// then operator delete) and both words plus a new reference are adopted. Same model as the landed 0x0084F130.
struct SCountBlock {
    long m_references;                       // +0
    void (__fastcall* m_pfnDestroy)(void*);  // +4
    void* m_pObject;                         // +8
};

struct CCountedPointer {                     // CCountedPointer<T>, 8 bytes
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

struct PairUInt64CountedPointer {              // std::pair<unsigned __int64, CCountedPointer<T> >
    unsigned __int64 first;                  // +0x00
    CCountedPointer second;                  // +0x08
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

PairUInt64CountedPointer* __fastcall Copy_PairUInt64CountedPointer_00cbb120(PairUInt64CountedPointer* first, PairUInt64CountedPointer* last, PairUInt64CountedPointer* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}