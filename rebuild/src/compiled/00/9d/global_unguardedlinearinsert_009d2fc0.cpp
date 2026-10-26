// _global::__unguarded_linear_insert(unsigned long* last, unsigned long value, CEntryDataOffsetLess comp) -- SGI insertion-sort step over a bank's entry indices, ordered by each entry's data offset (packed or from the runtime record)
// Retail: Fable.exe (TLC, VC7.1) @ 0x009D2FC0, 127 bytes (+1 int3), __fastcall (/Gr algorithm TU: `last` in ecx,
// `value` in edx, the one-pointer comparator by value on the stack), ret 4.
// Leaf: __unguarded_linear_insert
// The catalog label (CBankFile::InsertionSort_PackedEntry) describes it; the body is the SGI STL helper
// instantiated for the CBankFile entry sort (GOTCHAS: the retail algorithms are SGI and their TUs use /Gr).
// Callee: CPackedUIntArray::operator[] 0x00A629F0.
// Layout: the comparator holds the bank; retail CBankFile layout of the landed rows: RuntimeData +0x20
// (12-byte records, DataOffset +0), PackedDataOffset +0x38 (packed when its Size word, +4, is non-zero).
// Shape: `next = last - 1` in ebx, `last` and `value` spilled to the frame (two words), the comparator's
// GetEntryDataOffset inlined twice (value first, then *next, read into edi before the packed test), the
// unsigned `<` (`jae` exits), the element moved through eax and `last` re-pointed via the frame slot.
template <class T> class CArray { public:                 // retail CArray: 12 bytes
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

struct CEntryDataOffsetLess {
    const CBankFile_Retail* m_pBank;
    bool operator()(unsigned long a, unsigned long b) const
    {
        return m_pBank->GetEntryDataOffset(a) < m_pBank->GetEntryDataOffset(b);
    }
};

template <class RandomIt, class T, class Compare>
void __fastcall __unguarded_linear_insert(RandomIt last, T value, Compare comp)
{
    RandomIt next = last;
    --next;
    while (comp(value, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = value;
}

template void __fastcall __unguarded_linear_insert<unsigned long*, unsigned long, CEntryDataOffsetLess>(unsigned long*, unsigned long, CEntryDataOffsetLess);