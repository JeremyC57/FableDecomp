// __unguarded_linear_insert(SAnimEventPair* last, SAnimEventPair value, CPairFirstLess comp) -- SGI insertion-sort step over pair<unsigned long, CSmallVector<NAnimationEvents::CEvent,32>>: shift the larger keys up one slot, drop `value` into the hole, destroy the by-value copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FB960, length 88 bytes (96 with padding).
// Leaf: __unguarded_linear_insert
// The chunk label (CSmallVector::Erase) is propagated garbage. The retail STL algorithms are SGI and their
// TUs are /Gr (GOTCHAS: free template functions are __fastcall): `last` in ecx, the 12-byte value and the
// empty comparator on the stack (`ret 10h`).
// Callees: 0x006FB5C0 (CSmallVector<CEvent,32>::Copy(const CSmallVector&): delete[] the old block, new[] the
// count, copy the 8-byte events), 0x00BFEB1C (operator delete[], the import thunk; `pop ecx`).
// Layout: the 12-byte element is { unsigned long key; CSmallVector { CEvent* m_pData; unsigned long m_count; } }
// (landed relative 0x006FB710 reads the same pair).
// Shape: SGI `next = last; --next; while (comp(value, *next)) { *last = *next; last = next; --next; } *last = value;`
// with the comparator inlined to an unsigned key compare (`cmp ebx,eax ; jae/jb`). The value's key lives in
// ebx across the Copy calls and is stored from ebx at the end: it is read into a LOCAL first (comparing
// `value.first` directly makes the lane keep it in edx and reload the parameter slot after each call). The
// pair assignment as key move + the CSmallVector operator= self-test (`&other != this`) around the out-of-line
// Copy, `last` in ecx and `next` / `&next->events` strength-reduced in esi / edi; finally the by-value
// parameter's destructor (`delete[] m_pData`) runs in the callee.
namespace NAnimationEvents { struct CEvent { long m_a, m_b; }; }

struct CSmallVector_CEvent_32 {
    NAnimationEvents::CEvent* m_pData;          // +0x00
    unsigned long             m_count;          // +0x04
    void Copy(const CSmallVector_CEvent_32& other);         // 0x006FB5C0
    CSmallVector_CEvent_32(const CSmallVector_CEvent_32& other);
    CSmallVector_CEvent_32& operator=(const CSmallVector_CEvent_32& other)
    {
        if (&other != this)
            Copy(other);
        return *this;
    }
    ~CSmallVector_CEvent_32() { delete[] m_pData; }
};

struct SAnimEventPair {                         // pair<unsigned long, CSmallVector<CEvent,32>>, 12 bytes
    unsigned long          first;               // +0x00
    CSmallVector_CEvent_32 second;              // +0x04
    SAnimEventPair& operator=(const SAnimEventPair& other)
    {
        first = other.first;
        second = other.second;
        return *this;
    }
};

struct CPairFirstLess {                         // compares the keys only
    bool operator()(unsigned long a, const SAnimEventPair& b) const { return a < b.first; }
};

void __fastcall __unguarded_linear_insert(SAnimEventPair* last, SAnimEventPair value, CPairFirstLess comp)
{
    unsigned long key = value.first;
    SAnimEventPair* next = last;
    --next;
    while (comp(key, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    last->first = key;
    last->second = value.second;
}
