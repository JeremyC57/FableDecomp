// _global::_Adjust_heap(SKeyedFlag*, long, long, SKeyedFlag, SKeyLess) -- Dinkumware heap sift-down for 16-byte (unsigned __int64 key, bool) pairs ordered by the 64-bit key, then _Push_heap @ 0x00A7D6F0
// Flags: /Gr
// Leaf: _Adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00a7f0f0, 172 bytes (+ int3 padding).
// Callee: 0x00A7D6F0 _Push_heap (same instantiation).  first in ecx, hole in edx (link-time
// __fastcall); bottom, the 16-byte pair by value and the empty comparator follow (`ret 18h`).
// Shape: VC7.1 <algorithm> `_Adjust_heap` with a 16-byte stride.  The child test is the VC7.1
// unsigned 64-bit compare (`cmp hi ; ja ; jb ; cmp lo ; jae`), so the key is an unsigned __int64
// (8-aligned, hence the 16-byte element).  The argument slot gets the two key dwords and ONE BYTE at
// +8 (`mov al,[esp+34h] ; mov [esi+8],al`): a user copy constructor of a pair whose second member is
// a single bool, the rest being alignment padding; the element assignment is implicit (four dword
// moves, padding included).
// Label: the propagated pair<pair<long,long>, CEngineTextureDecal> cannot be right -- Ego_r.pdb
// gives CEngineTextureDecal 0x18 bytes (it holds a vector), and a signed pair<long,long> would
// compare first (+0) before second (+4) with signed jumps.
namespace FableHeap_00a7f0f0 {

struct SKeyedFlag {
    unsigned __int64 m_key;
    bool m_flag;
    SKeyedFlag(const SKeyedFlag& other) : m_key(other.m_key), m_flag(other.m_flag) { }
};

struct SKeyLess {
    char m_tag;
    bool operator()(const SKeyedFlag& a, const SKeyedFlag& b) const { return a.m_key < b.m_key; }
};

void __fastcall _Push_heap(SKeyedFlag* first, long hole, long top, SKeyedFlag value, SKeyLess pred);   // 0x00A7D6F0

void __fastcall _Adjust_heap(SKeyedFlag* first, long hole, long bottom, SKeyedFlag value, SKeyLess pred)
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