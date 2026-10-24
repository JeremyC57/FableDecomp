// _global::_Adjust_heap(SKeyedPair*, long, long, SKeyedPair, SKeyGreater) -- Dinkumware heap sift-down for 8-byte (long key, dword value) pairs ordered by a GREATER-key comparator, then _Push_heap @ 0x0088C7C0
// Flags: /Gr
// Leaf: _Adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088ca20, 106 bytes.
// Callee: 0x0088C7C0 _Push_heap (same instantiation; its loop also tests `jle`, i.e. moves the parent
// down while parent.key > value.key).  Callers 0x0088CB2E.. are the make_heap/pop_heap/sort_heap of
// the same instantiation.  first arrives in ecx, hole in edx (link-time __fastcall); bottom, the pair
// by value (8 bytes, built in its argument slot by the user copy constructor: `sub esp,8 ; mov eax,esp`)
// and the empty comparator (4-byte slot) follow on the stack (`ret 10h`).
// Shape: VC7.1 <algorithm> `_Adjust_heap`: idx = 2*idx+2 (`lea eax,[eax+eax+2]`), step to the left
// child when pred(right, left), take the lone child at the bottom, push the saved value back up.
// The compare is a SIGNED dword `cmp ; jle` that decrements when right.key > left.key: the key is a
// long and the comparator is "greater" (a min-heap), not CKeyPairCompareLess<float,...> as the
// propagated label pair<float,EConversationAttitude> claims.
namespace FableHeap_0088ca20 {

struct SKeyedPair {
    long m_key;
    unsigned long m_value;
    SKeyedPair(const SKeyedPair& other) : m_key(other.m_key), m_value(other.m_value) { }
};

struct SKeyGreater {
    char m_tag;
    bool operator()(const SKeyedPair& a, const SKeyedPair& b) const { return a.m_key > b.m_key; }
};

void __fastcall _Push_heap(SKeyedPair* first, long hole, long top, SKeyedPair value, SKeyGreater pred);   // 0x0088C7C0

void __fastcall _Adjust_heap(SKeyedPair* first, long hole, long bottom, SKeyedPair value, SKeyGreater pred)
{
    long top = hole;
    long idx = 2 * hole + 2;
    for (; idx < bottom; idx = 2 * idx + 2) {
        if (pred(first[idx], first[idx - 1]))
            --idx;
        first[hole] = first[idx], hole = idx;
    }
    if (idx == bottom) {
        first[hole] = first[idx - 1];
        hole = idx - 1;
    }
    _Push_heap(first, hole, top, value, pred);
}

}