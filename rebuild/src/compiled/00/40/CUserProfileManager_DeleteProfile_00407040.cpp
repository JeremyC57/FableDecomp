// CUserProfileManager::DeleteProfile(const CWideString& profileName) -- delete every entry of the profile's save directory that exists and is a file, then delete the directory itself
// Leaf: DeleteProfile
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00407040, 384 bytes, /GS cookie frame (0x230), `ret 4`.
// Callees: GetValidProfileName(const CWideString&) @ 0x00406350 and NResourceDirectoryNames::GetSaveDir() @ 0x0041A540
//          (both `__fastcall`, hidden result in ecx), operator+(CWideString, CWideString) @ 0x0099BE70 / (CWideString,
//          const wchar_t*) @ 0x0099BF30, CWideString(const wchar_t*) @ 0x0099B6B0, ~CWideString @ 0x0099B510, the CAFile
//          find-first @ 0x00999760 / find-next @ 0x00999850, PathExists @ 0x00999230, IsPathFile @ 0x00999A30 (all
//          `__fastcall`), CDriveManager::DeleteFile @ 0x009E4BC0 and DeleteDirectory @ 0x009E4B70 on the drive manager held
//          by the static counted pointer @ 0x013CAA28, __security_check_cookie @ 0x00BFE9F9. Signatures from FableWin.pdb.
// Layout: the find handle is 8 bytes and the find data 0x20C (MAX_PATH wide name + a dword), as the landed
//          CMainGameComponent::GenerateMetFilesFromLugFiles (0x00418C3B); wchar_t is `unsigned short` in this lane.
// Shape: the directory name is one expression `GetSaveDir() + GetValidProfileName(name) + L"\\"` (right operand first);
//          the file tests are a named bool built from implicit-conversion temporaries (construction flags in ebx); the
//          entry and the directory are passed as implicit conversions too.
class CWideStringData;
class CWideString { public:
    CWideStringData* PStringData;
    CWideString(const unsigned short* text);    // 0x0099B6B0
    ~CWideString();                             // 0x0099B510
};
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);         // 0x0099BE70
CWideString __fastcall operator+(const CWideString& a, const unsigned short* b);      // 0x0099BF30
CWideString __fastcall GetValidProfileName(const CWideString& name);                 // 0x00406350
namespace NResourceDirectoryNames {
    CWideString __fastcall GetSaveDir();                                              // 0x0041A540
}
struct CFindFileData {
    unsigned short FileName[260];
    unsigned long  Attributes;
};
struct CFindFileHandle {
    long _body[2];
};
bool __fastcall FindFirstFile(const CWideString& pattern, CFindFileHandle& handle, CFindFileData& data);   // 0x00999760
bool __fastcall FindNextFile(const CWideString& pattern, CFindFileHandle& handle, CFindFileData& data);    // 0x00999850
class CAFile {
public:
    static bool __fastcall PathExists(const CWideString&);   // 0x00999230
};                                 // 0x00999230
bool __fastcall IsPathFile(const CWideString& path);                                 // 0x00999A30

class CSystemManager;
class CDriveManager { public:
    void*           __vftable;                  // +0x00
    CSystemManager* SystemManager;              // +0x04
    long DeleteFile(const CWideString& path);       // 0x009E4BC0
    long DeleteDirectory(const CWideString& path);  // 0x009E4B70
};
struct CDriveManagerPointer {                   // static CCountedPointer<CDriveManager>: object, count block
    CDriveManager* Object;
    long*          Count;
    CDriveManager* operator->() const { return Object; }
};
extern CDriveManagerPointer g_pDriveManager;    // 0x013CAA28

class CUserProfileManager {
public:
    void DeleteProfile(const CWideString& profileName);
};

void CUserProfileManager::DeleteProfile(const CWideString& profileName)
{
    CWideString directory = NResourceDirectoryNames::GetSaveDir() + GetValidProfileName(profileName) + (const unsigned short*)L"\\";
    CWideString pattern = directory + (const unsigned short*)L"*";
    CFindFileHandle handle;
    CFindFileData data;
    if (FindFirstFile(pattern, handle, data)) {
        do {
            bool isFile = CAFile::PathExists(data.FileName) && IsPathFile(data.FileName);
            if (isFile)
                g_pDriveManager->DeleteFile(data.FileName);
        } while (FindNextFile(pattern, handle, data));
    }
    g_pDriveManager->DeleteDirectory(directory);
}