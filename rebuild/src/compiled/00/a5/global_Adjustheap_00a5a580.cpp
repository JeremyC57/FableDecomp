// _global::_Adjust_heap(float*, long, long, float, SFloatLess) -- Dinkumware heap sift-down over a float array (less-than), then _Push_heap @ 0x00A586D0
// Flags: /Gr
// Leaf: _Adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5A580, 82 bytes.
// Callee: 0x00A586D0 _Push_heap (same instantiation).  Link-time __fastcall: first in ecx, hole in
// edx, then bottom, the float value (one dword slot) and the empty comparator; `ret 0xc`.
// The propagated label (CAStarState** / CAStarStateTotalGreaterThan) is wrong: the elements are
// 4-byte floats compared directly (`fld [first+idx*4] ; fcomp [first+idx*4-4] ; test ah,5 ; jp`
// = `first[idx] < first[idx-1]`), i.e. a max-heap of floats under a less-than predicate.
// Shape: VC7.1 <algorithm> `_Adjust_heap`; the bottom case reuses idx (`first[idx - 1]`).
struct SFloatLess {
    char m_tag;
    bool operator()(float a, float b) const { return a < b; }
};

void __fastcall _Push_heap(float* first, long hole, long top, float value, SFloatLess pred);   // 0x00A586D0

void __fastcall _Adjust_heap(float* first, long hole, long bottom, float value, SFloatLess pred)
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