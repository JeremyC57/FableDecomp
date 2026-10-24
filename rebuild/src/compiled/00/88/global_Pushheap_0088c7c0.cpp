// _global::_Push_heap(SKeyedPair*, long, long, SKeyedPair, SKeyGreater) -- heap sift-up for 8-byte (long key, dword value) pairs under a GREATER-key comparator (min-heap)
// Leaf: _Push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088C7C0, 79 bytes, `ret 0x10`.  No callees.
// Called by the landed _Adjust_heap @ 0x0088CA20 (same instantiation; its model is reused here).
// Label correction: the propagated pair<float,EConversationAttitude> label is wrong -- the compare is
// a SIGNED dword `cmp [parent],ebx ; jle`: the parent moves down while parent.key > value.key.
// Link-time __fastcall (/Gr algorithm TU): first in ecx, hole in edx, then top, the pair by value
// (8 bytes) and the empty comparator (4 + 8 + 4 = 0x10).
// Shape: SGI/STLport `__push_heap` (`parent = (hole - 1) / 2; while (hole > top && comp(first[parent], value))`).
namespace FableHeap_0088c7c0 {
struct SKeyedPair {
    long m_key;
    unsigned long m_value;
    SKeyedPair(const SKeyedPair& other) : m_key(other.m_key), m_value(other.m_value) { }
};

struct SKeyGreater {
    char m_tag;
    bool operator()(const SKeyedPair& a, const SKeyedPair& b) const { return a.m_key > b.m_key; }
};

void __fastcall _Push_heap(SKeyedPair* first, long hole, long top, SKeyedPair value, SKeyGreater comp)
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