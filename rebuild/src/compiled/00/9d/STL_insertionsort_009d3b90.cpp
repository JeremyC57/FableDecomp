// std::__insertion_sort(unsigned long* first, unsigned long* last, CBankFile::CompareDataOffset comp) -- STLport insertion sort of a bank's entry indices by each entry's data offset (packed array or runtime record), __linear_insert inlined: an index whose data comes first shifts the run up one slot with memmove and goes first, anything else goes through the out-of-line __unguarded_linear_insert
// Leaf: __insertion_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D3B90, 217 bytes, `ret 4` (__fastcall: first in ecx, last in edx, the
// one-pointer comparator on the stack -- the SGI/STLport algorithm TUs are /Gr).
// Callees: 0x00A629F0 CPackedUIntArray::operator[](unsigned long), 0x00BFEAE6 memmove, 0x009D2FC0
// __unguarded_linear_insert(last, value, comp) (landed as global_unguardedlinearinsert_009d2fc0, whose retail CBankFile
// model -- RuntimeData +0x20, PackedDataOffset +0x38, packed when its Size word is non-zero -- this source reuses;
// engine/CBankFile.h's default donor layout disagrees with the bytes and its FABLE_RETAIL_BANK_FILE view is opaque).
// Label: catalogued as `_Insertion_sort1<unsigned long*, CBankFile::CompareDataOffset, unsigned long>` (Dinkumware
// name); the code is STLport's `__insertion_sort` with `__linear_insert(first, i, *i, comp)` inlined.
// Shape: the comparator's bank pointer lives in edi and its dead argument slot is reused for the running byte count
// `(i - first) * 4` (strength-reduced, `add edx,4` per pass); GetEntryDataOffset is inlined twice (value first, the
// first element read into ebp up front); unsigned `<` (`jae`); copy_backward of a scalar element is STLport's
// __copy_trivial_backward over void*, counted in BYTES (`n = (char*)last - (char*)first ; if (n > 0) memmove((char*)
// result - n, first, n)` -- an element count adds `sar eax,2 ; shl eax,2`); `*first = val` re-reads `first`.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6

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

struct CBankFile_Retail {                          // the retail layout (as the landed 0x009D2FC0)
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

void __fastcall __unguarded_linear_insert(unsigned long* last, unsigned long value, CompareDataOffset comp);   // 0x009D2FC0

__forceinline void* __copy_trivial_backward(const void* first, const void* last, void* result)
{
    const int num = (const char*)last - (const char*)first;   // STLport counts the trivial copy in bytes
    return (num > 0) ? memmove((char*)result - num, first, num) : result;
}

__forceinline unsigned long* copy_backward(unsigned long* first, unsigned long* last, unsigned long* result)
{
    return (unsigned long*)__copy_trivial_backward(first, last, result);
}

__forceinline void __linear_insert(unsigned long* first, unsigned long* last, unsigned long value, CompareDataOffset comp)
{
    if (comp(value, *first)) {
        copy_backward(first, last, last + 1);
        *first = value;
    }
    else
        __unguarded_linear_insert(last, value, comp);
}

void __fastcall __insertion_sort(unsigned long* first, unsigned long* last, CompareDataOffset comp)
{
    if (first == last)
        return;
    for (unsigned long* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}