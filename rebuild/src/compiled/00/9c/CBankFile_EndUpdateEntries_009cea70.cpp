// CBankFile::EndUpdateEntries() -- finish an update pass: a retail-mode bank just closes its file; otherwise write the file information block, delete the crash-recovery file if one exists and clear the solid / updating flags
// Retail: Fable.exe (TLC, VC7.1) @ 0x009CEA70, 108 bytes (+4 int3), __fastcall, no arguments.
// Callees: CDiskFile virtual slot 2 (Close), CBankFile::WriteFileInformation(bool) 0x009CE370,
// CBankFile::GetRecoveryFilePath() const 0x009CBFE0 (CWideString by hidden pointer), PathExists(const
// CWideString&) 0x00999230 (__fastcall, bool), CWideString::operator const wchar_t*() 0x0099AF10, kernel32
// DeleteFileW (IAT 0x0143FE34), CWideString::~CWideString 0x0099B510.
// Layout: the retail CBankFile layout of the landed rows (engine/CBankFile.h is the PDB layout, 0x10 higher
// from +0x38 on): BankFile_Data +0x7C, RetailMode +0x8C, Solid +0x8F, UpdatingFlag +0x95.
// Shape: the retail-mode arm returns early with its own epilogue; the recovery path is a named local in
// the `push ecx` frame word, scoped so its destructor runs before the two flag stores.
typedef unsigned short wchar16;
extern "C" __declspec(dllimport) int __stdcall DeleteFileW(const wchar16* fileName);

class CWideString {
public:
    void* m_pData;
    CWideString(const CWideString& other);
    ~CWideString();                                 // 0x0099B510
    operator const wchar16*() const;                // 0x0099AF10
};

bool __fastcall PathExists(const CWideString& path);   // 0x00999230

struct CDiskFileCalls {
    virtual void v00();
    virtual void v01();
    virtual void Close();                           // slot 2 (+0x08)
};

struct CBankFile_Retail {                        // the retail layout; engine/CBankFile.h (PDB) disagrees with the bytes
    void*                     __vftable;            // +0x00
    unsigned long             Size;                 // +0x04
    unsigned char             _arrays[0x70];        // +0x08 Symbols .. PackedValid
    unsigned long             OpenFlags;            // +0x78
    CDiskFileCalls*           BankFile_Data;        // +0x7c CCountedPointer<CDiskFileWin32>::Data
    void*                     BankFile_Info;        // +0x80
    void*                     Filename_PStringData; // +0x84 CWideString
    void*                     BankHandle_PStringData; // +0x88 CCharString
    bool                      RetailMode;           // +0x8c
    bool                      ReadOnly;             // +0x8d
    bool                      WrittenToFile;        // +0x8e
    bool                      Solid;                // +0x8f
    unsigned long             ChangeCount;          // +0x90
    bool                      FileValid;            // +0x94
    bool                      UpdatingFlag;         // +0x95
    void WriteFileInformation(bool flush);          // 0x009CE370
    CWideString GetRecoveryFilePath() const;        // 0x009CBFE0
    void EndUpdateEntries();
};

void CBankFile_Retail::EndUpdateEntries()
{
    if (RetailMode) {
        BankFile_Data->Close();
        UpdatingFlag = false;
        return;
    }
    WriteFileInformation(true);
    {
        CWideString recoveryPath = GetRecoveryFilePath();
        if (PathExists(recoveryPath))
            DeleteFileW(recoveryPath);
    }
    Solid = false;
    UpdatingFlag = false;
}