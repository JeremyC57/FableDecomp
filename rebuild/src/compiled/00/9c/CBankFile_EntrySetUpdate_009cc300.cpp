// CBankFile::EntrySetUpdate(unsigned long, bool) -- swap the entry's update flag, returning the old one
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CC300.
// Callees: none.
//
// Retail bytes:  mov eax,[ecx+20h] ; mov edx,[ecx+24h] ; sub edx,eax ; mov eax,2AAAAAABh ; imul edx ; mov eax,[esp+4] ; sar edx,1 ; push esi ; mov esi,edx ; shr esi,1Fh ; add esi,edx ; cmp eax,esi ; jae F ; mov esi,[ecx+20h] ; push ebx ; lea edx,[eax+eax*2] ; mov bl,[esi+edx*4+9] ; test bl,bl ; pop ebx ; je F ; mov ecx,[ecx+2Ch] ; mov ecx,[ecx+eax*4] ; mov dl,[esp+0Ch] ; mov al,[ecx+14h] ; mov [ecx+14h],dl ; pop esi ; ret 8 ; F: xor al,al ; pop esi ; ret 8
// The bound is the RuntimeData element count (a signed 12-byte pointer difference: the 0x2AAAAAAB
// reciprocal with the sign fix-up) compared unsigned against the index; a valid record's update
// object (UpdateData[index], a pointer array) has its byte flag at +0x14 exchanged for the argument.
// Retail CBankFile layout (see the landed CBankFile::Align @ 0x009CBB90): in the shipped binary every
// CArray / std::vector / map is 4 bytes smaller than in Ego_r.pdb (engine/CBankFile.h), so from +0x38
// on each member sits 0x10 lower than the PDB offset.  The PDB member names are kept at their retail
// offsets; the header itself cannot be included because its offsets disagree with the bytes.
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
    bool EntrySetUpdate(unsigned long index, bool update);   // 0x009CC300
};

class CBankFileEntryUpdateData { public:
    unsigned char _pad[0x14];
    bool m_update;                 // +0x14
};

bool CBankFile_Retail::EntrySetUpdate(unsigned long index, bool update)
{
    if (index < (unsigned long)RuntimeData.GetCount() && RuntimeData[index].Valid) {
        CBankFileEntryUpdateData* data = UpdateData[index];
        bool old = data->m_update;
        data->m_update = update;
        return old;
    }
    return false;
}