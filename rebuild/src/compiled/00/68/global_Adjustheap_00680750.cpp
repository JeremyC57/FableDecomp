// _global::_Adjust_heap(SPair*, long, long, SPair, SLess) -- Dinkumware heap sift-down for 8-byte (long key, dword value) pairs, then _Push_heap @ 0x00680460
// Flags: /Gr
// Leaf: _Adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00680750, 106 bytes (+ int3 padding).
// Callee: 0x00680460 _Push_heap (same instantiation).  Caller: 0x00680E00 sort_heap (landed),
// which passes first in ecx, hole in edx (link-time __fastcall), then bottom, the pair by value
// (8 bytes, built in its slot by the user copy constructor) and the empty comparator (4-byte slot).
// Shape: VC7.1 <algorithm> `_Adjust_heap`: walk the hole down to the larger child while
// `idx = 2*idx+2 < bottom` (`lea eax,[eax+eax+2]`), take the only child at the bottom, then push
// the saved value back up from `top`.  The comparison is a SIGNED dword `jge`, so the key is a
// long, not a float as the propagated label pair<float,EConversationAttitude> says.
struct SPair {
    long m_first;
    unsigned long m_second;
    SPair(const SPair& other) : m_first(other.m_first), m_second(other.m_second) { }
};

struct SLess {
    char m_tag;
    bool operator()(const SPair& a, const SPair& b) const { return a.m_first < b.m_first; }
};

void __fastcall _Push_heap(SPair* first, long hole, long top, SPair value, SLess pred);   // 0x00680460

void __fastcall _Adjust_heap(SPair* first, long hole, long bottom, SPair value, SLess pred)
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