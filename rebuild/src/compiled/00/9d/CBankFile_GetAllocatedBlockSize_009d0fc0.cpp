// CBankFile::GetAllocatedBlockSize(unsigned long index) -- the file space an entry owns: from its offset up to the next block in offset order, or its data size when it is the last block (0 for an entry without data)
// Leaf: GetAllocatedBlockSize
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D0FC0, length 196 bytes (+12 int3), ret 4.
// Callees: CBankFile::UpdateBlockOrder @ 0x009D0630, CPackedUIntArray::operator[] @ 0x00A629F0 (inline
// GetEntryDataOffset / GetEntryDataSize, as the landed 0x009CC270 / 0x009CC240).
// Shape: the entry offset is homed in the index-argument slot (index stays in edi); `found` is an int register flag (`mov ebx,1`;
// a bool gives `bl`) set when the walk passes the entry; each block index is read once into a local before the `found` test, and
// the next block's offset minus ours replaces the size with a `break` to the single `return size` (the size stays in ebp).
// Layout: the landed retail CBankFile model (0x009CC240) plus BlockOrder +0xe0.
struct CDiskFileCalls;
class CBankFileEntryUpdateData;

struct CRuntimeData {                   // CBankFile::CRuntimeData, 12 bytes, RuntimeData[0] is unused
    unsigned long DataOffset;           // +0x00
    unsigned long DataSize;             // +0x04
    unsigned char DataType;             // +0x08
    bool          Valid;                // +0x09
    unsigned char _pad[2];
};

struct CPackedUIntArray {               // 16 bytes; m_pData == 0 means "not packed, use RuntimeData"
    unsigned long  m_bitsPerValue;      // +0x00
    unsigned long* m_pData;             // +0x04
    unsigned long  m_count;             // +0x08
    unsigned long  m_capacity;          // +0x0c
    unsigned long operator[](unsigned long index) const;   // 0x00A629F0
    bool IsPacked() const { return m_pData != 0; }
};

template <class T> class CArray { public:      // retail CArray: 12 bytes
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    long GetCount() const { return m_pLast - m_pFirst; }
    T& operator[](unsigned long index) const { return m_pFirst[index]; }
};

struct CBankFile_Retail {        // the retail layout; engine/CBankFile.h (PDB) disagrees with the bytes
    void*                     __vftable;            // +0x00
    unsigned long             Size;                 // +0x04 entry count (index 0 is reserved)
    unsigned char             Symbols[0x0c];        // +0x08 CArray<CCharString>
    unsigned char             Checksums[0x0c];      // +0x14 CArray<unsigned long>
    CArray<CRuntimeData>      RuntimeData;          // +0x20 1-based: entry i is RuntimeData[i]
    CArray<CBankFileEntryUpdateData*> UpdateData;   // +0x2c 0-based
    CPackedUIntArray          PackedDataOffset;     // +0x38 0-based: entry i is [i-1]
    CPackedUIntArray          PackedDataSize;       // +0x48
    CPackedUIntArray          PackedDataType;       // +0x58
    CPackedUIntArray          PackedValid;          // +0x68
    unsigned long             OpenFlags;            // +0x78
    CDiskFileCalls*           BankFile_Data;        // +0x7c CCountedPointer<CDiskFileWin32>::Data
    void*                     BankFile_Info;        // +0x80
    void*                     Filename_PStringData; // +0x84 CWideString
    void*                     BankHandle_PStringData; // +0x88 CCharString
    bool                      RetailMode;           // +0x8c
    bool                      ReadOnly;             // +0x8d
    bool                      WrittenToFile;        // +0x8e
    bool                      Solid;                // +0x8f
    unsigned long             ChangeCount;          // +0x90
    bool                      FileValid;            // +0x94
    bool                      UpdatingFlag;         // +0x95
    unsigned long             Alignment;            // +0x98
    unsigned long             BankFormatVersion;    // +0x9c
    unsigned long             HeaderSize;           // +0xa0
    unsigned long             FileInformationSize;  // +0xa4
    unsigned long             FileSize;             // +0xa8
    unsigned char             _padac[0x34];         // +0xac .. +0xdf (symbol maps / CRC vector map)
    CArray<unsigned long>     BlockOrder;           // +0xe0 entry indices sorted by data offset
    unsigned long GetEntryDataSize(unsigned long index) const
    {
        if (PackedDataSize.IsPacked())
            return PackedDataSize[index - 1];
        return RuntimeData[index].DataSize;
    }
    unsigned long GetEntryDataOffset(unsigned long index) const
    {
        if (PackedDataOffset.IsPacked())
            return PackedDataOffset[index - 1];
        return RuntimeData[index].DataOffset;
    }
    unsigned long AlignOffset(unsigned long value) const { return (value + Alignment - 1) / Alignment * Alignment; }
    void UpdateBlockOrder();                        // 0x009D0630
    unsigned long GetAllocatedBlockSize(unsigned long index);
};

unsigned long CBankFile_Retail::GetAllocatedBlockSize(unsigned long index)
{
    unsigned long offset = GetEntryDataOffset(index);
    if (offset == 0)
        return 0;
    UpdateBlockOrder();
    int found = 0;
    unsigned long size = GetEntryDataSize(index);
    for (unsigned long* it = BlockOrder.m_pFirst; it != BlockOrder.m_pLast; ++it) {
        unsigned long entry = *it;
        if (found) {
            size = GetEntryDataOffset(entry) - offset;
            break;
        }
        if (entry == index)
            found = 1;
    }
    return size;
}