// CBankFile::GetMaxEntryIndex() -- the highest live entry index, -1 when there is none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CC350.
// Callees: CPackedUIntArray::operator[] @ 0x00A629F0 (through the inlined EntryExists @ 0x009CC190).
//
// Retail bytes:  push esi ; push edi ; mov edi,ecx ; mov esi,[edi+4] ; dec esi ; js N ; push ebx ; lea ebx,[esi+esi*2] ; shl ebx,2 ; L: test esi,esi ; je X ; cmp esi,[edi+4] ; jae X ; mov eax,[edi+6Ch] ; test eax,eax ; je R ; lea eax,[esi-1] ; push eax ; lea ecx,[edi+68h] ; call operator[] ; test eax,eax ; setne al ; jmp T ; R: mov ecx,[edi+20h] ; mov al,[ecx+ebx+9] ; T: test al,al ; jne D ; X: dec esi ; sub ebx,0Ch ; test esi,esi ; jge L ; D: pop ebx ; pop edi ; mov eax,esi ; pop esi ; ret ; N: pop edi ; mov eax,esi ; pop esi ; ret
// Walks down from Size-1 (signed: the `dec ; js` is the rotated `i >= 0` entry test) until
// EntryExists(i) holds; the inlined predicate keeps the 12-byte record offset in ebx (strength
// reduced alongside i) and its packed branch folds `!= 0` into `setne al`.
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
    bool EntryExists(unsigned long index) const      // 0x009CC190, inlined
    {
        if (index != 0 && index < Size) {
            if (PackedValid.IsPacked())
                return PackedValid[index - 1] != 0;
            return RuntimeData[index].Valid;
        }
        return false;
    }
    long GetMaxEntryIndex() const;   // 0x009CC350
};

long CBankFile_Retail::GetMaxEntryIndex() const
{
    long index;
    for (index = Size - 1; index >= 0; --index) {
        if (EntryExists(index))
            break;
    }
    return index;
}