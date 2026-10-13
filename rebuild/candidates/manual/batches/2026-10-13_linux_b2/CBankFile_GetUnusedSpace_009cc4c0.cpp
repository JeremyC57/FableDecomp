// CBankFile::GetUnusedSpace()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CC4C0, oracle length 38 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push esi ; push edi ; mov edi,ecx ; mov esi,[edi+0A8h] ; call <entry data total> ; mov ecx,[edi+0A4h] ; mov edx,[edi+0A0h] ; sub esi,ecx ; sub esi,edx ; sub esi,eax ; pop edi ; mov eax,esi ; pop esi ; ret
// Unused bytes = FileSize - FileInformationSize - HeaderSize - (total size of entry data).
// The helper at 0x009CC450 is __thiscall with no arguments (manifest label unverified).

// Retail CBankFile layout. In the shipped binary every std::vector / CArray / map / CVectorMap is
// 4 bytes smaller than in Ego_r.pdb (engine/CBankFile.h), so each container before a member moves
// it 4 bytes earlier: the four CArrays before +0x48 give PDB-0x10 from there on, and the three
// maps + CVectorMap + CArray between +0xAC and +0x110 give PDB-0x24 for ProgressCallback and
// WarningFlag. The members below are the PDB names at their retail offsets.
struct CDiskFileWin32Calls;
struct CProgressCallbackCalls;
struct CBankFile_Retail {
    void*                   __vftable;                 // +0x00
    unsigned long           Size;                      // +0x04
    unsigned char           Symbols[0x0C];             // +0x08 CArray<CCharString>        (retail 12 bytes)
    unsigned char           Checksums[0x0C];           // +0x14 CArray<unsigned long>      (retail 12 bytes)
    unsigned char           RuntimeData[0x0C];         // +0x20 CArray<CRuntimeData>       (retail 12 bytes)
    unsigned char           UpdateData[0x0C];          // +0x2C CArray<CBankFileEntryUpdateData*> (retail 12 bytes)
    unsigned char           PackedDataOffset[0x10];    // +0x38 CPackedUIntArray   (PDB +0x48)
    unsigned char           PackedDataSize[0x10];      // +0x48 CPackedUIntArray   (PDB +0x58)
    unsigned char           PackedDataType[0x10];      // +0x58 CPackedUIntArray   (PDB +0x68)
    unsigned char           PackedValid[0x10];         // +0x68 CPackedUIntArray   (PDB +0x78)
    unsigned long           OpenFlags;                 // +0x78                    (PDB +0x88)
    CDiskFileWin32Calls*    BankFile_Data;             // +0x7C CCountedPointer<CDiskFileWin32>::Data (PDB +0x8C)
    void*                   BankFile_Info;             // +0x80
    void*                   Filename_PStringData;      // +0x84 CWideString
    void*                   BankHandle_PStringData;    // +0x88 CCharString
    bool                    RetailMode;                // +0x8C
    bool                    ReadOnly;                  // +0x8D
    bool                    WrittenToFile;             // +0x8E
    bool                    Solid;                     // +0x8F
    unsigned long           ChangeCount;               // +0x90
    bool                    FileValid;                 // +0x94
    bool                    UpdatingFlag;              // +0x95
    unsigned char           _pad_0x96[2];
    unsigned long           Alignment;                 // +0x98                    (PDB +0xA8)
    unsigned long           BankFormatVersion;         // +0x9C
    unsigned long           HeaderSize;                // +0xA0                    (PDB +0xB0)
    unsigned long           FileInformationSize;       // +0xA4                    (PDB +0xB4)
    unsigned long           FileSize;                  // +0xA8                    (PDB +0xB8)
    unsigned char           SymbolIndices[0x0C];       // +0xAC map<CCharString,ulong>      (retail 12 bytes)
    unsigned char           OldSymbolIndices[0x0C];    // +0xB8 map<CCharString,ulong>      (retail 12 bytes)
    unsigned char           FilenameIndices[0x0C];     // +0xC4 multimap<CCharString,ulong> (retail 12 bytes)
    unsigned char           SymbolCRCIndices[0x10];    // +0xD0 CVectorMap<ulong,ulong>     (retail 16 bytes)
    unsigned char           BlockOrder[0x0C];          // +0xE0 CArray<unsigned long>       (retail 12 bytes)
    CProgressCallbackCalls* ProgressCallback;          // +0xEC                    (PDB +0x110)
    void*                   HeaderEnumerationType_PStringData; // +0xF0 CCharString
    bool                    WarningFlag;               // +0xF4                    (PDB +0x118)
    unsigned long GetTotalEntryDataSize();     // 0x009CC450
    unsigned long GetUnusedSpace();
};

unsigned long CBankFile_Retail::GetUnusedSpace()
{
    // Lane feedback: retail loads FileSize into esi BEFORE calling GetTotalEntryDataSize and
    // subtracts the two header sizes and then the call result; that order only falls out when
    // FileSize is read into a local first (a one-expression return calls first). RELOCATION_MATCH.
    unsigned long fileSize = this->FileSize;
    return fileSize - this->FileInformationSize - this->HeaderSize - GetTotalEntryDataSize();
}
