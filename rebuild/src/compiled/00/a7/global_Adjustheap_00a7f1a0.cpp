// _global::_Adjust_heap(SKeyedValue*, long, long, SKeyedValue, SKeyLess) -- Dinkumware heap sift-down for 16-byte (unsigned __int64 key, dword) pairs ordered by the 64-bit key, then _Push_heap @ 0x00A7D7A0
// Flags: /Gr
// Leaf: _Adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00a7f1a0, 172 bytes.
// Callee: 0x00A7D7A0 _Push_heap (same instantiation).  first in ecx, hole in edx (link-time
// __fastcall); bottom, the 16-byte pair by value and the empty comparator follow (`ret 18h`).
// Shape: VC7.1 <algorithm> `_Adjust_heap` with a 16-byte stride.  The child test is the VC7.1
// unsigned 64-bit compare (`cmp hi ; ja ; jb ; cmp lo ; jae`), so the key is an unsigned __int64
// (8-aligned, hence the 16-byte element).  The argument slot gets three dwords (`mov esi,[esp+34h] ;
// mov [eax+8],esi` for the third): a user copy constructor of a pair whose second member is one
// dword; the element assignment is implicit (four dword moves, padding included).  Twin of the
// bool-valued 0x00A7F0F0 next to it.
// Label: the propagated pair<pair<long,long>, CEngineTextureDecal> cannot be right -- Ego_r.pdb
// gives CEngineTextureDecal 0x18 bytes (it holds a vector), and a signed pair<long,long> would
// compare first (+0) before second (+4) with signed jumps.
namespace FableHeap_00a7f1a0 {

struct SKeyedValue {
    unsigned __int64 m_key;
    unsigned long m_value;
    SKeyedValue(const SKeyedValue& other) : m_key(other.m_key), m_value(other.m_value) { }
};

struct SKeyLess {
    char m_tag;
    bool operator()(const SKeyedValue& a, const SKeyedValue& b) const { return a.m_key < b.m_key; }
};

void __fastcall _Push_heap(SKeyedValue* first, long hole, long top, SKeyedValue value, SKeyLess pred);   // 0x00A7D7A0

void __fastcall _Adjust_heap(SKeyedValue* first, long hole, long bottom, SKeyedValue value, SKeyLess pred)
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