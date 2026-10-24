// std::_Sort_heap(SKeyedValue* first, SKeyedValue* last, SKeyLess pred) -- Dinkumware _Sort_heap over 16-byte (unsigned __int64 key, dword) pairs: pop the root to the back until one element remains
// Leaf: _Sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A81E80, length 141 bytes (+ int3 padding), `ret 4`.
// Identity: the catalog label `_Sort_heap<pair<long,CRandomAppearanceMorph::CSkeletalMorph>*,...>` is propagated over
// the byte-identical skeleton. The only callee is the landed Dinkumware `_Adjust_heap` @ 0x00A7F1A0 for 16-byte
// (unsigned __int64 key, dword) pairs (unsigned 64-bit key compare, one dword at +8, the rest padding);
// element model reused from it. Twin of the bool-valued 0x00A81D90.
// Callees: _Adjust_heap @ 0x00A7F1A0 (out of line, __fastcall: first in ecx, hole 0 in edx, `ret 18h`).
// Shape: /Gr Dinkumware TU with _Pop_heap_0 / _Pop_heap inline and the loop calling _Pop_heap_0 directly (16-byte
// stride). The `_Ty(*(_Last - 1))` temporary keeps the low key dword in ecx and spills the high dword and the value
// dword to the frame (the pair's memberwise user copy constructor skips the padding); `*_Dest = *_First` is the
// implicit assignment (four dword moves, padding included), and the value argument gets three dwords.
struct SKeyedValue {
    unsigned __int64 m_key;
    unsigned long m_value;
    SKeyedValue(const SKeyedValue& other) : m_key(other.m_key), m_value(other.m_value) { }
};

struct SKeyLess {
    char m_tag;
    bool operator()(const SKeyedValue& a, const SKeyedValue& b) const { return a.m_key < b.m_key; }
};

void __fastcall _Adjust_heap(SKeyedValue* first, long hole, long bottom, SKeyedValue value, SKeyLess pred);   // 0x00A7F1A0

inline void _Pop_heap(SKeyedValue* first, SKeyedValue* last, SKeyedValue* dest, SKeyedValue value, SKeyLess pred, long*)
{
    *dest = *first;
    _Adjust_heap(first, 0, long(last - first), value, pred);
}

inline void _Pop_heap_0(SKeyedValue* first, SKeyedValue* last, SKeyLess pred, SKeyedValue*)
{
    _Pop_heap(first, last - 1, last - 1, SKeyedValue(*(last - 1)), pred, (long*)0);
}

void __fastcall _Sort_heap(SKeyedValue* first, SKeyedValue* last, SKeyLess pred)
{
    for (; 1 < last - first; --last)
        _Pop_heap_0(first, last, pred, (SKeyedValue*)0);
}