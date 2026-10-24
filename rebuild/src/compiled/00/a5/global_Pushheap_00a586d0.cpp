// _global::_Push_heap(float*, long, long, float, SFloatLess) -- heap sift-up over a float array (less-than, max-heap)
// Leaf: _Push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A586D0, 72 bytes, `ret 0xc`.  No callees.
// Called by the landed _Adjust_heap @ 0x00A5A580 (same instantiation; its model is reused here).
// Label correction: the catalog pair<float,CCharString> label is propagated; the elements are 4-byte
// floats (the value is loaded onto the FPU once, `fld [esp+8]`, and compared in place with
// `fcom [parent] ; test ah,41h ; jne` = `parent < value`; the final drop is `fstp`).
// Link-time __fastcall (/Gr algorithm TU): first in ecx, hole in edx, then top, the float value and
// the empty std::less (4 + 4 + 4 = 0xc).
// Shape: SGI/STLport `__push_heap` (`parent = (hole - 1) / 2; while (hole > top && comp(first[parent], value))`).
struct SFloatLess {
    char m_tag;
    bool operator()(float a, float b) const { return a < b; }
};

void __fastcall _Push_heap(float* first, long hole, long top, float value, SFloatLess comp)
{
    long parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}