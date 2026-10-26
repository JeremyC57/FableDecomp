// std::__make_heap(unsigned long* first, unsigned long* last, CBankFile::CompareDataOffset comp, unsigned long*, int*) -- STLport heap build over a bank's entry indices by data offset: sift every parent down from (len - 2) / 2 to 0
// Leaf: ??$__make_heap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D3CF0, 77 bytes (+ int3 padding to 80), `__fastcall` (/Gr algorithm TU: first in
//   ecx, last in edx; the 4-byte comparator, the _Tp* and _Distance* tags on the stack), `ret 0Ch`.
// Identity (proven): _RandomAccessIterator = unsigned long*, _Compare = CBankFile::CompareDataOffset, _Tp = unsigned long, _Distance = int. its callers are the __partial_sort 0x009D4620 of the introsort 0x009D48C0 (H008's sort_heap 0x009D42B0 is the same sort) and 0x009D3F60, its callee the __adjust_heap 0x009D3750; both PDB name sets nest the comparator in CBankFile as a class (VCompareDataOffset@CBankFile@@; operator() ??RCompareDataOffset@CBankFile@@QAE_NKK@Z), so the catalog's global struct spelling `UCompareDataOffset@@` is corrected here.
// Label: the manifest's "std::make_heap" is propagated across byte-identical bodies (family J012: six rows that differ only in
//   their call displacement; retail has no identical-code folding, so each address is one instantiation).
// Callee: __adjust_heap(first, parent, len, value, comp) @ 0x009D3750 (this instantiation's own COMDAT, __fastcall `ret 0Ch`).
// Shape: STLport 4.6 _heap.c `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) {
//   __adjust_heap(first, parent, len, _Tp(*(first + parent)), comp); if (parent == 0) return; parent--; }`; the signed
//   halving is `cdq / sub / sar`. Model: the landed 0x005176D0 (CThingBuilding*, CIsHouseLargerThan).
class CBankFile {
public:
    class CompareDataOffset {                       // Ego_r.pdb: nested class, 4 bytes
    public:
        CBankFile* BankFile;                        // +0x00
        bool operator()(unsigned long a, unsigned long b);   // 0x009CC640
    };
};
typedef CBankFile::CompareDataOffset TComp;
namespace std {

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);   // 0x009D3750

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*)
{
    if (__last - __first < 2) return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;
    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0) return;
        __parent--;
    }
}
template void __fastcall __make_heap<unsigned long*, TComp, unsigned long, int>(unsigned long*, unsigned long*, TComp, unsigned long*, int*);
}   // namespace std