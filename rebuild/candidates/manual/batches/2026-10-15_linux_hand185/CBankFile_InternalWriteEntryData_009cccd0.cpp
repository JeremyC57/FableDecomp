// CBankFile::InternalWriteEntryData(unsigned long index, unsigned long start, const void* data, unsigned long size) -- write part of an entry's data: on the first write reopen the bank file for writing at the same position, seek to the entry offset + start, write, and when the write ends the entry pad it to the alignment and clear the entry's pending-update flag
// Leaf: InternalWriteEntryData
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CCCD0, length 229 bytes (+11 int3), ret 0x10.
// Callees: CPackedUIntArray::operator[] @ 0x00A629F0 (inline GetEntryDataSize / GetEntryDataOffset, as the landed
// 0x009CC240 / 0x009CC270), the disk file's (+0x7c) virtual slots 7 (+0x1c, GetPosition), 2 (+0x08, Close), 1 (+0x04,
// Open(const CWideString&, access 2, flags 4)), 5 (+0x14, Seek) and 4 (+0x10, Write(data, size, 0)); CBankFile::WritePadding
// (unsigned long) @ 0x009CCB20.
// Shape: entry size (ebp) then entry offset (ebx) through the inline getters; the reopen block keeps the saved position in
// edi; the padding is `(size + Alignment - 1) / Alignment * Alignment - size` (unsigned div). UpdateData[index]->RequiresUpdate (+0x14) is the
// entry's pending flag.
// Layout: the landed retail CBankFile model (0x009CC240), plus Filename +0x84 (CWideString), WrittenToFile +0x8e,
// Alignment +0x98.
struct CWideString { void* PStringData; };
struct CDiskFileCalls {
    virtual void v00();
    virtual void Open(const CWideString& pathname, long access, unsigned long flags);   // slot 1 (+0x04)
    virtual void Close();                          // slot 2 (+0x08)
    virtual void v03();
    virtual void Write(const void* data, unsigned long size, long flags);   // slot 4 (+0x10)
    virtual void Seek(unsigned long position);     // slot 5 (+0x14)
    virtual void v06();
    virtual unsigned long GetPosition();           // slot 7 (+0x1c)
};
struct CBankFileEntryUpdateData {
    unsigned char _pad[0x14];
    bool RequiresUpdate;                           // +0x14
};

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

template <class T> struct CArray {      // retail CArray: 12 bytes
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
    CWideString               Filename;             // +0x84
    void*                     BankHandle_PStringData; // +0x88 CCharString
    bool                      RetailMode;           // +0x8c
    bool                      ReadOnly;             // +0x8d
    bool                      WrittenToFile;        // +0x8e
    bool                      Solid;                // +0x8f
    unsigned long             ChangeCount;          // +0x90
    bool                      FileValid;            // +0x94
    bool                      UpdatingFlag;         // +0x95
    unsigned long             Alignment;            // +0x98
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
    void WritePadding(unsigned long size);          // 0x009CCB20
    void InternalWriteEntryData(unsigned long index, unsigned long start, const void* data, unsigned long size);
};

void CBankFile_Retail::InternalWriteEntryData(unsigned long index, unsigned long start, const void* data, unsigned long size)
{
    unsigned long entrySize = GetEntryDataSize(index);
    unsigned long entryOffset = GetEntryDataOffset(index);
    if (!WrittenToFile) {
        unsigned long position = BankFile_Data->GetPosition();
        BankFile_Data->Close();
        BankFile_Data->Open(Filename, 2, 4);
        BankFile_Data->Seek(position);
        WrittenToFile = true;
    }
    BankFile_Data->Seek(entryOffset + start);
    BankFile_Data->Write(data, size, 0);
    if (start + size == entrySize) {
        WritePadding((entrySize + Alignment - 1) / Alignment * Alignment - entrySize);
        UpdateData[index]->RequiresUpdate = false;
    }
}
