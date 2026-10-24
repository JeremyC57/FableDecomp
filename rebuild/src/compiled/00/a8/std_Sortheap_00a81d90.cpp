// std::_Sort_heap(SKeyedFlag* first, SKeyedFlag* last, SKeyLess pred) -- Dinkumware _Sort_heap over 16-byte (unsigned __int64 key, bool) pairs: pop the root to the back until one element remains
// Leaf: _Sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A81D90, length 141 bytes (+ int3 padding), `ret 4`.
// Identity: the catalog label `_Sort_heap<pair<long,CRandomAppearanceMorph::CSkeletalMorph>*,...>` is propagated over
// the byte-identical skeleton. The only callee is the landed Dinkumware `_Adjust_heap` @ 0x00A7F0F0 for 16-byte
// (unsigned __int64 key, bool) pairs (unsigned 64-bit key compare, a single flag byte at +8, the rest padding);
// element model reused from it. Twin of the dword-valued 0x00A81E80.
// Callees: _Adjust_heap @ 0x00A7F0F0 (out of line, __fastcall: first in ecx, hole 0 in edx, `ret 18h`).
// Shape: /Gr Dinkumware TU with _Pop_heap_0 / _Pop_heap inline and the loop calling _Pop_heap_0 directly (16-byte
// stride). The `_Ty(*(_Last - 1))` temporary keeps the low key dword in ecx and spills the high dword and the flag
// BYTE to the frame (the pair's memberwise user copy constructor copies only the bool, not the padding);
// `*_Dest = *_First` is the implicit assignment (four dword moves, padding included), and the value argument gets
// the two key dwords and the flag byte.
struct SKeyedFlag {
    unsigned __int64 m_key;
    bool m_flag;
    SKeyedFlag(const SKeyedFlag& other) : m_key(other.m_key), m_flag(other.m_flag) { }
};

struct SKeyLess {
    char m_tag;
    bool operator()(const SKeyedFlag& a, const SKeyedFlag& b) const { return a.m_key < b.m_key; }
};

void __fastcall _Adjust_heap(SKeyedFlag* first, long hole, long bottom, SKeyedFlag value, SKeyLess pred);   // 0x00A7F0F0

inline void _Pop_heap(SKeyedFlag* first, SKeyedFlag* last, SKeyedFlag* dest, SKeyedFlag value, SKeyLess pred, long*)
{
    *dest = *first;
    _Adjust_heap(first, 0, long(last - first), value, pred);
}

inline void _Pop_heap_0(SKeyedFlag* first, SKeyedFlag* last, SKeyLess pred, SKeyedFlag*)
{
    _Pop_heap(first, last - 1, last - 1, SKeyedFlag(*(last - 1)), pred, (long*)0);
}

void __fastcall _Sort_heap(SKeyedFlag* first, SKeyedFlag* last, SKeyLess pred)
{
    for (; 1 < last - first; --last)
        _Pop_heap_0(first, last, pred, (SKeyedFlag*)0);
}