// _STL::__unguarded_partition(unsigned long* first, unsigned long* last, unsigned long pivot, CBankFile::CompareDataOffset comp) -- STLport quicksort partition of a bank's entry indices around a pivot index, ordered by each entry's data offset (packed array or runtime record); returns the split point
// Leaf: __unguarded_partition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D2ED0, 234 bytes (+ int3 padding), `ret 8` (__fastcall: first in ecx,
// last in edx, the pivot index and the one-pointer comparator by value on the stack -- the SGI/STLport algorithm TUs
// are /Gr). Caller: 0x009D490A (__introsort_loop of the CBankFile entry sort; __final_insertion_sort is 0x009D3ED0).
// Callee: 0x00A629F0 CPackedUIntArray::operator[](unsigned long) (four inlined GetEntryDataOffset reads).
// Identity: catalogued as "CBankFile_QuicksortPartition" (descriptive); the body is STLport's __unguarded_partition
// (Hoare partition, single iterator result) instantiated for CBankFile::CompareDataOffset.
// Layout: retail CBankFile model of the landed siblings 0x009D2FC0 / 0x009D3B90 / 0x009D3750 (RuntimeData +0x20,
// 12-byte records with DataOffset +0; PackedDataOffset +0x38, packed when its Size word, +4, is non-zero);
// engine/CBankFile.h's default layout disagrees with the bytes.
// Shape: `first` and `last` live in the two `sub esp,8` frame words, the bank in esi, the pivot in ebp; each comparison
// inlines GetEntryDataOffset twice (left operand first); the `--last` loop reads *last before the pivot's offset;
// iter_swap moves both elements through eax/edi; unsigned `<` (`jae` / `jb`).
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

template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    unsigned long __tmp = *__i1;
    *__i1 = *__i2;
    *__i2 = __tmp;
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}

template unsigned long* __fastcall __unguarded_partition<unsigned long*, unsigned long, CompareDataOffset>(unsigned long*, unsigned long*, unsigned long, CompareDataOffset);