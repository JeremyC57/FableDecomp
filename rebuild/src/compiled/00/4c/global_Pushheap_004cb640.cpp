// _global::_Push_heap(SPair*, long, long, SPair, SLess) -- heap sift-up for 8-byte (long key, dword value) pairs
// Leaf: _Push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004CB640, 79 bytes, `ret 0x10`.  No callees.
// Called by the landed _Adjust_heap @ 0x004CBED0 (same instantiation; its model is reused here).
// Label correction: the propagated pair<float,EConversationAttitude> label is wrong -- the compare is
// a SIGNED dword `cmp [parent],ebx ; jge` (value.first loaded once into ebx), so the key is a long.
// Link-time __fastcall (/Gr algorithm TU): first in ecx, hole in edx, then top, the pair by value
// (8 bytes) and the empty comparator (4 + 8 + 4 = 0x10).  Moves are two dword copies; the drop
// stores ebx and the value's second word.  Byte twin of 0x00680460.
// Shape: SGI/STLport `__push_heap` (`parent = (hole - 1) / 2; while (hole > top && comp(first[parent], value))`).
namespace N004cb640 {
struct SPair {
    long m_first;
    unsigned long m_second;
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