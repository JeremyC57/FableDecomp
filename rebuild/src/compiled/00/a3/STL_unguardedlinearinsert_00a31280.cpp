// std::__unguarded_linear_insert(pair<unsigned long, SRecordInfo>* last, pair<unsigned long, SRecordInfo> val, CKeyPairCompareLess comp) -- insertion-sort inner step without a lower bound check: shift every larger element up one slot and drop the value into the gap
// Leaf: __unguarded_linear_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A31280, 93 bytes (+ int3 padding), `ret 18h` (__fastcall: last in ecx, the
//   20-byte value and the empty comparator on the stack).
// Identity: called from the landed __linear_insert 0x00A33780 (which names it) and the 20-byte sort helpers; the
//   catalog's "std_deque_rotate" is descriptive. Element: STLport pair<unsigned long, SRecordInfo> (20 bytes, the model of
//   the landed 0x00A33780: a 16-byte second with a memberwise user copy constructor), ordered by the unsigned key (`jb`).
// Shape: SGI `next = last; --next; while (comp(val, *next)) { *last = *next; last = next; --next; } *last = val;` -- the
//   key of val stays in edx, the loop is rotated with the next key preloaded (`mov esi,[eax-14h]`), the implicit pair
//   assignment copies five dwords, and the final store reads val's words from the argument slots.
namespace std {

struct SRecordInfo {                                   // 16 bytes
    unsigned long m_a, m_b, m_c, m_d;
    SRecordInfo(const SRecordInfo& o) : m_a(o.m_a), m_b(o.m_b), m_c(o.m_c), m_d(o.m_d) { }
};

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& p) : first(p.first), second(p.second) { }
};

typedef pair<unsigned long, SRecordInfo> TEntry;       // 20 bytes

struct CKeyPairCompareLess {                           // CKeyPairCompareLess<unsigned long, SRecordInfo>
    bool operator()(const TEntry& a, const TEntry& b) const { return a.first < b.first; }
};

void __fastcall __unguarded_linear_insert(TEntry* last, TEntry val, CKeyPairCompareLess comp)
{
    TEntry* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}

}