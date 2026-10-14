// CBankFile::CountEntries() -- how many of the Size slots hold a live entry
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CC3B0.
// Callees: CPackedUIntArray::operator[] @ 0x00A629F0 (through the inlined EntryExists @ 0x009CC190).
//
// Retail bytes:  push ebp ; push esi ; mov esi,ecx ; mov eax,[esi+4] ; push edi ; xor ebp,ebp ; xor edi,edi ; test eax,eax ; jbe E ; push ebx ; xor ebx,ebx ; L: test ebx,ebx ; je X ; cmp edi,eax ; jae X ; mov eax,[esi+6Ch] ; test eax,eax ; je R ; lea eax,[edi-1] ; push eax ; lea ecx,[esi+68h] ; call operator[] ; test eax,eax ; setne al ; jmp T ; R: mov ecx,[esi+20h] ; mov al,[ecx+ebx+9] ; T: test al,al ; je X ; inc ebp ; X: mov eax,[esi+4] ; inc edi ; add ebx,0Ch ; cmp edi,eax ; jb L ; pop ebx ; E: pop edi ; pop esi ; mov eax,ebp ; pop ebp ; ret
// Unsigned count up to Size with EntryExists inlined (the `index != 0` test is taken on the
// strength-reduced record offset in ebx, Size is re-read after the opaque call each turn).
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
    bool EntryExists(unsigned long index) const      // 0x009CC190, inlined
    {
        if (index != 0 && index < Size) {
            if (PackedValid.IsPacked())
                return PackedValid[index - 1] != 0;
            return RuntimeData[index].Valid;
        }
        return false;
    }
    unsigned long CountEntries() const;   // 0x009CC3B0
};

unsigned long CBankFile_Retail::CountEntries() const
{
    unsigned long count = 0;
    for (unsigned long index = 0; index < Size; ++index) {
        if (EntryExists(index))
            ++count;
    }
    return count;
}
