// _global::_Push_heap(long*, long, long, long, less<long>) -- heap sift-up of signed longs (max-heap)
// Leaf: _Push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00776750, 75 bytes, `ret 0xc`.  No callees.
// Label correction: the catalog "_Push_heap<float*,int,float,std::less<float>>" is propagated; the
// compare is the integer `cmp [parent],value ; jge` (signed less), so the elements are 4-byte signed
// integers (the heap is built in the CTCShop TU, 0x007771E0..0x00777F70, callers SellItem/CTCShop).
// Link-time __fastcall (/Gr algorithm TU): first in ecx, hole in edx, then top, the value and the
// empty std::less (4 + 4 + 4 = 0xc); value is held in edi across the loop.
// Shape: SGI/STLport `__push_heap`; the drop is tail-duplicated into both exits.
struct SLongLess {                              // std::less<long>
    char m_tag;
    bool operator()(const long& a, const long& b) const { return a < b; }
};

void __fastcall _Push_heap(long* first, long hole, long top, long value, SLongLess comp)
{
    long parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}