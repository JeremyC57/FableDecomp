// _global::_Adjust_heap(SKeyedPair*, long, long, SKeyedPair, SKeyLess) -- Dinkumware heap sift-down for 8-byte (unsigned long key, dword value) pairs ordered by an UNSIGNED less-key comparator, then _Push_heap @ 0x009B1D10
// Flags: /Gr
// Leaf: _Adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009b2ff0, 106 bytes (+ int3 padding).
// Callee: 0x009B1D10 _Push_heap (same instantiation).  first arrives in ecx, hole in edx (link-time
// __fastcall); bottom, the pair by value (8 bytes, built in its argument slot by the user copy
// constructor: `sub esp,8 ; mov eax,esp`) and the empty comparator (4-byte slot) follow (`ret 10h`).
// Shape: VC7.1 <algorithm> `_Adjust_heap`: idx = 2*idx+2, step to the left child when
// pred(right, left), take the lone child at the bottom, push the saved value back up.  The compare
// is an UNSIGNED `cmp ; jae` (key < key) over an 8-byte element: a (unsigned long key, dword value)
// pair.  The propagated label pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<..>>
// cannot be right as written: the PDB gives CParticleUpdateParamDesc<ulong,ulong> 12 bytes, which
// would make a 16-byte element; the 8-byte stride says the second member is a single dword.
namespace FableHeap_009b2ff0 {

struct SKeyedPair {
    unsigned long m_key;
    unsigned long m_value;
    SKeyedPair(const SKeyedPair& other) : m_key(other.m_key), m_value(other.m_value) { }
};

struct SKeyLess {
    char m_tag;
    bool operator()(const SKeyedPair& a, const SKeyedPair& b) const { return a.m_key < b.m_key; }
};

void __fastcall _Push_heap(SKeyedPair* first, long hole, long top, SKeyedPair value, SKeyLess pred);   // 0x009B1D10

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