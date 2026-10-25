// std::__partial_sort(unsigned long* first, unsigned long* middle, unsigned long* last, unsigned long*, CBankFile::CompareDataOffset comp) -- introsort depth-limit fallback of a bank's entry-index sort: heap the prefix, pop every index whose data offset is lower from the rest into it, then heap-sort the prefix
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D4620, 168 bytes (+ int3 padding), `ret 0Ch` (/Gr algorithm TU: first
//   in ecx, middle in edx; last, the value-type tag pointer and the one-pointer comparator on the stack).
// Identity: called by the __introsort_loop @ 0x009D48C0 (`partial_sort(first, last, last, comp)`) of the CBankFile entry
//   sort (CBankFile::CompareDataOffset, Ego_r.pdb; landed relatives __unguarded_partition 0x009D2ED0, __adjust_heap
//   0x009D3750, __push_heap 0x009D3040, __insertion_sort 0x009D3B90, __final_insertion_sort 0x009D3ED0). The catalog
//   label "heapsort_and_extract_min" is descriptive.
// Callees: __make_heap(first, middle, comp, _Tp*, _Distance*) @ 0x009D3CF0 (`push 0` x2), CPackedUIntArray::operator[]
//   @ 0x00A629F0 (the inlined GetEntryDataOffset), __adjust_heap @ 0x009D3750, sort_heap @ 0x009D42B0.
// Layout: the retail CBankFile model of the landed siblings (RuntimeData +0x20, 12-byte records with DataOffset +0;
//   PackedDataOffset +0x38, packed when its Size word, +4, is non-zero); engine/CBankFile.h's default layout disagrees.
// Shape: STLport make_heap / __pop_heap inline; the comparator inline (left offset parked in the comparator's dead
//   argument slot, the bank in esi); `middle` spilled to the `push ecx` word because `i` takes ebx.
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
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);   // 0x009D3750
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*,
                            _Distance*);   // 0x009D3CF0
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x009D42B0

template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, (unsigned long*)0, (int*)0);
}
template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                       _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, (int*)0);
    sort_heap(__first, __middle, __comp);
}
template void __fastcall __partial_sort<unsigned long*, unsigned long, CompareDataOffset>(unsigned long*, unsigned long*,
                                                                                       unsigned long*, unsigned long*,
                                                                                       CompareDataOffset);
}