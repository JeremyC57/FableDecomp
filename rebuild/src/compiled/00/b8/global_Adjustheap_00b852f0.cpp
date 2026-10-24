// _global::_Adjust_heap(SKeyedPair*, long, long, SKeyedPair, SKeyLess) -- Dinkumware heap sift-down for 8-byte (unsigned long key, dword value) pairs ordered by an UNSIGNED less-key comparator, then _Push_heap @ 0x00B84FD0
// Flags: /Gr
// Leaf: _Adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00b852f0, 98 bytes.
// Callee: 0x00B84FD0 _Push_heap (same instantiation).  first in ecx, hole in edx (link-time __fastcall);
// bottom, the pair by value and the empty comparator follow (`ret 10h`).  Unlike the sibling rows the
// pair is pushed dword by dword (`mov eax,[esp+18h] ; push eax` x3): the element is a POD with no
// user copy constructor here.
// Shape: VC7.1 <algorithm> `_Adjust_heap`; UNSIGNED `cmp ; jae` = key < key on an 8-byte element.
namespace FableHeap_00b852f0 {

struct SKeyedPair {
    unsigned long m_key;
    unsigned long m_value;
};

struct SKeyLess {
    char m_tag;
    bool operator()(const SKeyedPair& a, const SKeyedPair& b) const { return a.m_key < b.m_key; }
};

void __fastcall _Push_heap(SKeyedPair* first, long hole, long top, SKeyedPair value, SKeyLess pred);   // 0x00B84FD0

void __fastcall _Adjust_heap(SKeyedPair* first, long hole, long bottom, SKeyedPair value, SKeyLess pred)
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