// CBankFile::ReadEntryData(unsigned long, unsigned long, void*, unsigned long) -- seek to the entry (+offset) and read  bytes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CC2A0.
// Callees: CPackedUIntArray::operator[] @ 0x00A629F0 (x2, through the inlined GetEntryDataSize/GetEntryDataOffset), CDiskFile virtual slots 5 (Seek) and 3 (Read).
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+4Ch] ; test eax,eax ; push edi ; mov edi,[esp+0Ch] ; je A ; lea eax,[edi-1] ; push eax ; lea ecx,[esi+48h] ; call operator[] ; A: mov eax,[esi+3Ch] ; test eax,eax ; je B ; dec edi ; push edi ; lea ecx,[esi+38h] ; call operator[] ; jmp C ; B: mov edx,[esi+20h] ; lea ecx,[edi+edi*2] ; mov eax,[edx+ecx*4] ; C: mov edi,[esp+10h] ; mov ecx,[esi+7Ch] ; mov edx,[ecx] ; add eax,edi ; push eax ; call [edx+14h] ; mov edx,[esp+18h] ; mov ecx,[esi+7Ch] ; mov eax,[ecx] ; push 0 ; push edx ; mov edx,[esp+1Ch] ; push edx ; call [eax+0Ch] ; pop edi ; pop esi ; ret 10h
// The entry size is computed first and never used (its packed-array call survives as an opaque
// call while the unpacked record read is dropped: an assertion on offset+size compiled out of the
// retail build), then the entry offset (both getters inlined), then the bank's disk file is
// positioned at entryOffset + offset through virtual slot 5 and read into the buffer through
// virtual slot 3 (buffer, size, 0).
// Retail CBankFile layout (see the landed CBankFile::Align @ 0x009CBB90): in the shipped binary every
// CArray / std::vector / map is 4 bytes smaller than in Ego_r.pdb (engine/CBankFile.h), so from +0x38
// on each member sits 0x10 lower than the PDB offset.  The PDB member names are kept at their retail
// offsets; the header itself cannot be included because its offsets disagree with the bytes.
struct CDiskFileCalls;
struct CBankFileEntryUpdateData;

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
    void*                     Filename_PStringData; // +0x84 CWideString
    void*                     BankHandle_PStringData; // +0x88 CCharString
    bool                      RetailMode;           // +0x8c
    bool                      ReadOnly;             // +0x8d
    bool                      WrittenToFile;        // +0x8e
    bool                      Solid;                // +0x8f
    unsigned long GetEntryDataSize(unsigned long index) const      // 0x009CC240, inlined
    {
        if (PackedDataSize.IsPacked())
            return PackedDataSize[index - 1];
        return RuntimeData[index].DataSize;
    }
    unsigned long GetEntryDataOffset(unsigned long index) const    // 0x009CC270, inlined
    {
        if (PackedDataOffset.IsPacked())
            return PackedDataOffset[index - 1];
        return RuntimeData[index].DataOffset;
    }
    void ReadEntryData(unsigned long index, unsigned long offset, void* buffer, unsigned long size);   // 0x009CC2A0
};

struct CDiskFileCalls {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void Read(void* buffer, unsigned long size, unsigned long flags);   // slot 3 (+0x0c)
    virtual void v04();
    virtual void Seek(unsigned long position);                                  // slot 5 (+0x14)
};

void CBankFile_Retail::ReadEntryData(unsigned long index, unsigned long offset, void* buffer, unsigned long size)
{
    unsigned long dataSize = GetEntryDataSize(index);
    unsigned long dataOffset = GetEntryDataOffset(index);
    BankFile_Data->Seek(dataOffset + offset);
    BankFile_Data->Read(buffer, size, 0);
}