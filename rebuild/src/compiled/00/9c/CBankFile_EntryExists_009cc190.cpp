// CBankFile::EntryExists(unsigned long) -- is entry `index` a live entry of the bank
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CC190.
// Callees: CPackedUIntArray::operator[] @ 0x00A629F0.
//
// Retail bytes:  mov eax,[esp+4] ; test eax,eax ; je F ; cmp eax,[ecx+4] ; jae F ; mov edx,[ecx+6Ch] ; test edx,edx ; je R ; dec eax ; push eax ; add ecx,68h ; call operator[] ; neg eax ; sbb eax,eax ; neg eax ; ret 4 ; R: mov ecx,[ecx+20h] ; lea eax,[eax+eax*2] ; mov al,[ecx+eax*4+9] ; ret 4 ; F: xor al,al ; ret 4
// Index 0 is reserved and indices run up to Size (unsigned).  A bank whose validity flags were
// packed (PackedValid has data) answers from the packed array at index-1 (`!= 0` on a dword gives
// the neg/sbb/neg form), otherwise from the 1-based RuntimeData record's Valid byte, returned as is.
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
    bool EntryExists(unsigned long index) const;   // 0x009CC190
};

bool CBankFile_Retail::EntryExists(unsigned long index) const
{
    if (index != 0 && index < Size) {
        if (PackedValid.IsPacked())
            return PackedValid[index - 1] != 0;
        return RuntimeData[index].Valid;
    }
    return false;
}