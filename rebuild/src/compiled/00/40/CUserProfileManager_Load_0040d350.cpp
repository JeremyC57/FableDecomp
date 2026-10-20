// CUserProfileManager::Load() -- when the profile's file name is set, exists and passes the signature check, open it and read the game state from it
// Leaf: Load
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040D350, length 164 bytes (176 with padding).
// Callees: the profile path builder @ 0x004082D0 (CWideString by hidden pointer; manifest "GetDefaultVal<...>" is
// propagated), CWideString::operator!=(const wchar_t*) @ 0x0099B150, PathExists @ 0x00999230 and VerifySignature
// @ 0x00409730 (`__fastcall`), CDiskFileWin32::CDiskFileWin32(const CWideString&, FileAccessType, unsigned long)
// @ 0x0099AD80 (landed), CUserProfileManager::LoadGameState(CAFile*) @ 0x0040BCA0, CDiskFileWin32::Close @ 0x0099A920
// (landed), ~CWideString @ 0x0099B510, CBase::~CBase @ 0x0099A300. Literal L"" @ 0x0122D70C.
// Layout: Ego_r.pdb CUserProfileManager ProfileName (CWideString) +0x08; CDiskFileWin32 (0x1c bytes, vtable 0x0122D06C)
// Opened +0x14, Pathname +0x18 (as the landed constructor 0x0099AD80).
// Shape: the three tests chain in one `if`; the file is a block-scoped local whose inline virtual destructor stores the
// vtable, calls Close directly and destroys the pathname before the base.
typedef unsigned short wchar16;                             // wchar_t under the era defaults
struct CWideString {
    void* PStringData;                                      // +0
    ~CWideString();                                         // 0x0099B510
    bool operator!=(const wchar16* text) const;             // 0x0099B150
};
bool __fastcall PathExists(const CWideString& path);       // 0x00999230
bool __fastcall VerifySignature(const CWideString& path);  // 0x00409730
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };

struct CBase {
    virtual ~CBase();                                       // 0x0099A300
};
struct CAFile : CBase {
    long AccessType;                                        // +0x04 FileAccessType
    long Position;                                          // +0x08
    long Length;                                            // +0x0c
};
struct CDiskFileWin32 : CAFile {                            // vtable 0x0122D06C
    void*       WinFileHandle;                              // +0x10
    bool        Opened;                                     // +0x14
    CWideString Pathname;                                   // +0x18
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099AD80
    virtual ~CDiskFileWin32() { Close(); }
    virtual void Close();                                   // slot 2, 0x0099A920
};

struct CUserProfileManager {
    void*       m_vtable;                                   // +0x00
    unsigned char _pad04[4];
    CWideString ProfileName;                                // +0x08
    bool        UseNewProfileValues;                        // +0x0c
    CWideString GetProfilePathname(const CWideString& name, bool forSaving);   // 0x004082D0 (descriptive)
    void LoadGameState(CAFile* file);                       // 0x0040BCA0
    void Load();
};

void CUserProfileManager::Load()
{
    CWideString path = GetProfilePathname(ProfileName, false);
    if (path != (const wchar16*)L"" && PathExists(path) && VerifySignature(path)) {
        CDiskFileWin32 file(path, FILE_ACCESS_READ, 2);
        if (file.Opened) {
            LoadGameState(&file);
            file.Close();
        }
    }
}