// std::__introsort_loop(unsigned long* first, unsigned long* last, unsigned long*, int depth_limit, CBankFile::CompareDataOffset comp) -- introsort recursion of a bank's entry-index sort by data offset: median-of-three partition until 16 indices remain, partial (heap) sort when the depth budget runs out
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D48C0, 136 bytes, `ret 0Ch` (/Gr algorithm TU: first in ecx, last in
//   edx; the value-type tag pointer, the depth limit and the one-pointer comparator on the stack).
// Identity: called by the CBankFile entry sort 0x009D06DA (`__lg(n) * 2`) and recursively by itself; comparator
//   CBankFile::CompareDataOffset (Ego_r.pdb; the landed relatives 0x009D2ED0 / 0x009D3750 / 0x009D3ED0 hold the model).
//   The catalog label "introsort_partial_uint_quicksort" is descriptive.
// Callees: __median(a, b, c, comp) @ 0x009D2E40 (out of line: the inlined GetEntryDataOffset is too big for six
//   copies), __unguarded_partition(first, last, *median, comp) @ 0x009D2ED0 (landed), __partial_sort(first, last,
//   last, (_Tp*)0, comp) @ 0x009D4620 (this chunk), itself.
// Shape: the shape of the turning-animation loop 0x0087E530: `while (last - first > 16)`, the comparator push hoisted
//   above the depth test, `--depth_limit` written back to its argument slot, `last = cut`.
template <class T> struct CArray {                 // retail CArray: 12 bytes
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T& operator[](unsigned long index) const { return m_pFirst[index]; }
};

struct CRuntimeData {                              // CBankFile::CRuntimeData, 12 bytes
    unsigned long DataOffset;                      // +0x00
    unsigned long DataSize;                        // +0x04
    unsigned char DataType;                        // +0x08
    bool          Valid;                           // +0x09
};

struct CPackedUIntArray {                          // 16 bytes
    unsigned long* PackedInts;                     // +0x00
    unsigned long  Size;                           // +0x04 (0: not packed, use RuntimeData)
    unsigned char  Bits;                           // +0x08
    unsigned long  Bias;                           // +0x0C
    unsigned long operator[](unsigned long index) const;   // 0x00A629F0
    bool IsPacked() const { return Size != 0; }
};

struct CBankFile_Retail {                          // the retail layout (engine/CBankFile.h disagrees with the bytes)
    void*                     __vftable;           // +0x00
    unsigned long             Size;                // +0x04
    unsigned char             Symbols[0x0c];       // +0x08
    unsigned char             Checksums[0x0c];     // +0x14
    CArray<CRuntimeData>      RuntimeData;         // +0x20
    unsigned char             UpdateData[0x0c];    // +0x2c
    CPackedUIntArray          PackedDataOffset;    // +0x38
    unsigned long GetEntryDataOffset(unsigned long index) const
    {
        if (PackedDataOffset.IsPacked())
            return PackedDataOffset[index - 1];
        return RuntimeData[index].DataOffset;
    }
};

struct CompareDataOffset {                         // CBankFile::CompareDataOffset: holds the bank
    const CBankFile_Retail* m_pBank;
    bool operator()(unsigned long a, unsigned long b) const
    {
        return m_pBank->GetEntryDataOffset(a) < m_pBank->GetEntryDataOffset(b);
    }
};

namespace std {
const int __stl_threshold = 16;

template <class _Tp, class _Compare>
const _Tp& __fastcall __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp);   // 0x009D2E40
template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot,
                                                   _Compare __comp);   // 0x009D2ED0
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp);   // 0x009D4620
template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, (unsigned long*)0, __comp);
}
template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit,
                                 _Compare __comp)
{
    while (__last - __first > __stl_threshold) {
        if (__depth_limit == 0) {
            partial_sort(__first, __last, __last, __comp);
            return;
        }
        --__depth_limit;
        _RandomAccessIter __cut =
            __unguarded_partition(__first, __last,
                                  _Tp(__median(*__first, *(__first + (__last - __first) / 2), *(__last - 1), __comp)),
                                  __comp);
        __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp);
        __last = __cut;
    }
}
template void __fastcall __introsort_loop<unsigned long*, unsigned long, int, CompareDataOffset>(unsigned long*,
                                                                                               unsigned long*,
                                                                                               unsigned long*, int,
                                                                                               CompareDataOffset);
}