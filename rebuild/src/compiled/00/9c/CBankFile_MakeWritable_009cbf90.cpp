// CBankFile::MakeWritable() -- reopen the bank's disk file read/write at the same position (once)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CBF90.
// Callees: CDiskFile virtual slots 7 (GetPosition), 2 (Close), 1 (Open), 5 (Seek).
//
// Retail bytes:  push esi ; mov esi,ecx ; mov al,[esi+8Eh] ; test al,al ; jne E ; mov ecx,[esi+7Ch] ; mov eax,[ecx] ; push edi ; call [eax+1Ch] ; mov ecx,[esi+7Ch] ; mov edx,[ecx] ; mov edi,eax ; call [edx+8] ; mov ecx,[esi+7Ch] ; mov eax,[ecx] ; push 4 ; push 2 ; lea edx,[esi+84h] ; push edx ; call [eax+4] ; mov ecx,[esi+7Ch] ; mov eax,[ecx] ; push edi ; call [eax+14h] ; mov byte ptr [esi+8Eh],1 ; pop edi ; E: pop esi ; ret
// Guarded by the byte at +0x8e (PDB name WrittenToFile at the retail offset): remember the file
// position, close, reopen the Filename (CWideString at +0x84) with mode 2 / share 4, seek back and
// set the flag.  The disk file is reached through the counted pointer's object every time.
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
    void MakeWritable();   // 0x009CBF90
};

struct CWideString;

struct CDiskFileCalls {
    virtual void v00();
    virtual void Open(const CWideString& filename, unsigned long mode, unsigned long share);   // slot 1 (+0x04)
    virtual void Close();                                                                       // slot 2 (+0x08)
    virtual void v03();
    virtual void v04();
    virtual void Seek(unsigned long position);                                                  // slot 5 (+0x14)
    virtual void v06();
    virtual unsigned long GetPosition();                                                        // slot 7 (+0x1c)
};

void CBankFile_Retail::MakeWritable()
{
    if (!WrittenToFile) {
        unsigned long position = BankFile_Data->GetPosition();
        BankFile_Data->Close();
        BankFile_Data->Open(*(const CWideString*)&Filename_PStringData, 2, 4);
        BankFile_Data->Seek(position);
        WrittenToFile = true;
    }
}