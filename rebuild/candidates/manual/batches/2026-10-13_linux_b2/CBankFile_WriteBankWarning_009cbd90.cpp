// CBankFile::WriteBankWarning(const CCharString&)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CBD90, oracle length 34 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  push esi ; mov esi,ecx ; mov ecx,[esi+0ECh] ; test ecx,ecx ; je L ; mov edx,[esp+8] ; mov eax,[ecx] ; push edx ; call [eax+0Ch] ; L: mov byte ptr [esi+0F4h],1 ; pop esi ; ret 4
// Forwards the warning text to the progress callback's virtual slot 3 when one is attached and
// latches WarningFlag so the bank reports that a warning was raised.

struct CCharString;

// Retail CBankFile layout. Ego_r.pdb (engine/CBankFile.h) is an editor build: it carries
// `CArray UpdateData` at +0x38 (0x10 bytes) and `CVectorMap SymbolCRCIndices` at +0xEC (0x14 bytes)
// that the shipped executable does not have, so every retail offset from +0x38 is PDB-0x10 and
// from +0xEC is PDB-0x24. The members below are the PDB names at their retail offsets.
struct CDiskFileWin32Calls;
struct CProgressCallbackCalls;
struct CBankFile_Retail {
    void*                   __vftable;                 // +0x00
    unsigned long           Size;                      // +0x04
    unsigned char           Symbols[0x10];             // +0x08 CArray<CCharString>
    unsigned char           Checksums[0x10];           // +0x18 CArray<unsigned long>
    unsigned char           RuntimeData[0x10];         // +0x28 CArray<CRuntimeData>
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
    unsigned char           SymbolIndices[0x10];       // +0xAC map<CCharString,ulong>
    unsigned char           OldSymbolIndices[0x10];    // +0xBC map<CCharString,ulong>
    unsigned char           FilenameIndices[0x10];     // +0xCC multimap<CCharString,ulong>
    unsigned char           BlockOrder[0x10];          // +0xDC CArray<unsigned long>
    CProgressCallbackCalls* ProgressCallback;          // +0xEC                    (PDB +0x110)
    void*                   HeaderEnumerationType_PStringData; // +0xF0 CCharString
    bool                    WarningFlag;               // +0xF4                    (PDB +0x118)
    void WriteBankWarning(const CCharString& message);
};

struct CProgressCallbackCalls {
    virtual void v00(); virtual void v04(); virtual void v08();
    virtual void Warning(const CCharString& message);     // slot 3 (+0x0C)
};

void CBankFile_Retail::WriteBankWarning(const CCharString& message)
{
    CProgressCallbackCalls* callback = this->ProgressCallback;
    if (callback)
        callback->Warning(message);
    this->WarningFlag = true;
}
