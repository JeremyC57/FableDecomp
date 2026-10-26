// _global::Copy_PairULongEventVector_006fbc50(SAnimEventPair*, SAnimEventPair*, SAnimEventPair*, SRandomAccessIteratorTag, long*) -- SGI __copy over pair<unsigned long, CSmallVector<NAnimationEvents::CEvent,32> >, returns the advanced result
// Leaf: Copy_PairULongEventVector_006fbc50
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FBC50, length 105 bytes.
// Callees: CSmallVector<NAnimationEvents::CEvent,32>::Copy(const CSmallVector&) @ 0x006FB5C0.
// SGI-STL `__copy(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first in
// ecx, last in edx, result + the two tag arguments on the stack, `ret 0Ch`). The signed count (last - first) / 12
// is computed once and counted down in the dead `result` argument slot (result itself lives in ebp). Element (the
// manifest label, 12 bytes): { unsigned long key; CSmallVector<CEvent,32> { CEvent* data; unsigned long count; } }.
// The pair assignment moves the key, then CSmallVector::operator= tests `&other != this` (scheduled before the key
// move: `cmp edi,ebx` on the strength-reduced src+4/dst+4 pointers) around the out-of-line Copy. Same model as the
// landed 0x006FB960 (__unguarded_linear_insert over the same pairs).
namespace NAnimationEvents { class CEvent { public: long m_a, m_b; }; }

struct CSmallVector_CEvent_32 {
    NAnimationEvents::CEvent* m_pData;          // +0x00
    unsigned long             m_count;          // +0x04
    void Copy(const CSmallVector_CEvent_32& other);         // 0x006FB5C0
    CSmallVector_CEvent_32& operator=(const CSmallVector_CEvent_32& other)
    {
        if (&other != this)
            Copy(other);
        return *this;
    }
};

struct SAnimEventPair {                         // pair<unsigned long, CSmallVector<CEvent,32>>, 12 bytes
    unsigned long          first;               // +0x00
    CSmallVector_CEvent_32 second;              // +0x04
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

SAnimEventPair* __fastcall Copy_PairULongEventVector_006fbc50(SAnimEventPair* first, SAnimEventPair* last, SAnimEventPair* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}