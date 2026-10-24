// _global::_Push_heap(SPair*, long, long, SPair, SLess) -- heap sift-up for 8-byte (float key, EConversationAttitude) pairs
// Leaf: _Push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004CFD80, 86 bytes, `ret 0x10`.  No callees.
// Called by the landed _Adjust_heap @ 0x004D04F0 (same instantiation; its model is reused here).
// Element: std::pair<float, EConversationAttitude> (8 bytes); CKeyPairCompareLess<float,
// EConversationAttitude> is `a.first < b.first` (`fld [parent] ; fcomp [value] ; test ah,5 ; jp`).
// Link-time __fastcall (/Gr algorithm TU): first in ecx, hole in edx, then top, the pair by value and
// the empty comparator (4 + 8 + 4 = 0x10).  Moves are two dword copies through eax.
// Shape: SGI/STLport `__push_heap` (`parent = (hole - 1) / 2; while (hole > top && comp(first[parent], value))`).
namespace N004cfd80 {
enum EConversationAttitude { CONVERSATION_ATTITUDE_NONE };

struct SPair {
    float m_first;
    EConversationAttitude m_second;
    SPair(const SPair& other) : m_first(other.m_first), m_second(other.m_second) { }
};

struct SLess {
    char m_tag;
    bool operator()(const SPair& a, const SPair& b) const { return a.m_first < b.m_first; }
};

void __fastcall _Push_heap(SPair* first, long hole, long top, SPair value, SLess comp)
{
    long parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
}