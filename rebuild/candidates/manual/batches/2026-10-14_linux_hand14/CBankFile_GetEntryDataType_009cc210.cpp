// CBankFile::GetEntryDataType(unsigned long) -- the entry's data type from the packed array or the runtime record
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CC210.
// Callees: CPackedUIntArray::operator[] @ 0x00A629F0 (tail jump, the argument slot rewritten to index-1).
//
// Retail bytes:  mov eax,[ecx+5Ch] ; test eax,eax ; mov eax,[esp+4] ; je R ; dec eax ; mov [esp+4],eax ; add ecx,58h ; jmp operator[] ; R: lea edx,[eax+eax*2] ; mov eax,[ecx+20h] ; movzx eax,byte ptr [eax+edx*4+8] ; ret 4
// Packed banks forward to PackedDataType[index-1] as a sibling call (same one-argument __thiscall
// shape, so VC7.1 rewrites the argument slot and jumps); unpacked banks widen the 1-based runtime
// record's DataType byte (movzx: an unsigned char member returned as unsigned long).
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
    unsigned long GetEntryDataType(unsigned long index) const;   // 0x009CC210
};

unsigned long CBankFile_Retail::GetEntryDataType(unsigned long index) const
{
    if (PackedDataType.IsPacked())
        return PackedDataType[index - 1];
    return RuntimeData[index].DataType;
}
