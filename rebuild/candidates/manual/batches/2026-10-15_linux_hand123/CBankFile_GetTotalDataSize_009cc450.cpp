// CBankFile::GetTotalDataSize() const -- sum of the data sizes of every live entry of the bank
// Retail: Fable.exe (TLC, VC7.1) @ 0x009CC450, 112 bytes, __thiscall, no arguments (CBankFile::GetUnusedSpace
// 0x009CC4C0 calls it).
// Callee: CPackedUIntArray::operator[] 0x00A629F0 (twice: the packed validity flags and the packed sizes).
// Layout: the retail CBankFile layout of the landed rows (EntryExists 0x009CC190, GetEntryDataSize 0x009CC240):
// engine/CBankFile.h is the PDB layout, every CArray there is 4 bytes larger, so from +0x38 on each member
// sits 0x10 lower -- Size +4, RuntimeData (1-based 12-byte records: DataSize +4, Valid +9) +0x20,
// PackedDataSize +0x48, PackedValid +0x68 (a packed array is in use when its Size word, +4, is non-zero).
// Shape: both landed accessors inlined in the loop over 0..Size-1: EntryExists (`index != 0`, strength-
// reduced to the record offset in ebx, then `index < Size` with the Size re-read every pass) and
// GetEntryDataSize; the packed validity result is normalised with `test ; setne`, the record's Valid byte
// is used as is.
template <class T> struct CArray {                 // retail CArray: 12 bytes
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T& operator[](unsigned long index) const { return m_pFirst[index]; }
};

struct CRuntimeData {                              // CBankFile::CRuntimeData, 12 bytes, RuntimeData[0] is unused
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

struct CBankFile_Retail {                               // the retail layout (engine/CBankFile.h disagrees with the bytes)
    void*                     __vftable;           // +0x00
    unsigned long             Size;                // +0x04 entry count (index 0 is reserved)
    unsigned char             Symbols[0x0c];       // +0x08 CArray<CCharString>
    unsigned char             Checksums[0x0c];     // +0x14 CArray<unsigned long>
    CArray<CRuntimeData>      RuntimeData;         // +0x20
    unsigned char             UpdateData[0x0c];    // +0x2c CArray<CBankFileEntryUpdateData*>
    CPackedUIntArray          PackedDataOffset;    // +0x38
    CPackedUIntArray          PackedDataSize;      // +0x48
    CPackedUIntArray          PackedDataType;      // +0x58
    CPackedUIntArray          PackedValid;         // +0x68
    bool EntryExists(unsigned long index) const    // 0x009CC190 (landed)
    {
        if (index != 0 && index < Size) {
            if (PackedValid.IsPacked())
                return PackedValid[index - 1] != 0;
            return RuntimeData[index].Valid;
        }
        return false;
    }
    unsigned long GetEntryDataSize(unsigned long index) const   // 0x009CC240 (landed)
    {
        if (PackedDataSize.IsPacked())
            return PackedDataSize[index - 1];
        return RuntimeData[index].DataSize;
    }
    unsigned long GetTotalDataSize() const;
};

unsigned long CBankFile_Retail::GetTotalDataSize() const
{
    unsigned long total = 0;
    for (unsigned long index = 0; index < Size; ++index)
        if (EntryExists(index))
            total += GetEntryDataSize(index);
    return total;
}
