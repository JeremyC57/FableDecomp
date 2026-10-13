// CBankFile::GetBankPathname()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CBF10, oracle length 20 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov ecx,[ecx+7Ch] ; mov eax,[ecx] ; push esi ; mov esi,[esp+8] ; push esi ; call [eax+2Ch] ; mov eax,esi ; pop esi ; ret 4
// Returns the pathname of the underlying disk file: virtual slot 11 of the CDiskFileWin32 held by
// the BankFile counted pointer, returning CWideString through the caller's hidden result pointer
// (which VC7.1 forwards straight to the callee and hands back in eax).

struct CWideString {
    void* PStringData;        // +0x00
    CWideString();
    ~CWideString();
};

struct CDiskFileWin32Calls {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28();
    virtual CWideString GetPathname();     // slot 11 (+0x2C)
};

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
    CWideString GetBankPathname();
};

CWideString CBankFile_Retail::GetBankPathname()
{
    return this->BankFile_Data->GetPathname();
}
