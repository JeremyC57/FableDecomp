// CUserProfileManager::GetDirectoryNameFromProfileName() const -- open (or create) the profile's save game under "U:\", take the directory part of the returned path and strip the leading ".\<save dir>" prefix (7 characters) when it is there; "\" when the save game cannot be opened
// Leaf: GetDirectoryNameFromProfileName
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x004071C0, 368 bytes, /GS cookie frame (0x218), hidden result, `ret 4`.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CreateSaveGame(const CCharString&,
//          const CWideString&, long, long, wchar_t*, long) @ 0x00406E30 (`__fastcall`, XCreateSaveGame emulation: 0 =
//          success), CWideString() @ 0x0099AED0 / (const wchar_t*) @ 0x0099B6B0 / (const CWideString&) @ 0x0099B720 /
//          ~ @ 0x0099B510, CAFile::SplitPath @ 0x00997620 (static `__fastcall`),
//          NResourceDirectoryNames::GetSaveDirWithoutTrailingBlackslash @ 0x0041A4F0 (`__fastcall`; manifest "GetSaveDir"
//          is the neighbour 0x0041A540), operator+(const wchar_t*, const CWideString&) @ 0x0099BFF0, CWideString::GetLength
//          @ 0x0099B220, GetAsCharArray() @ 0x0099AF10, GetSubString @ 0x0099BDB0, wcsncmp @ 0x00BFEB0A,
//          __security_check_cookie @ 0x00BFE9F9. Signatures from FableWin.pdb.
// Layout: Ego_r.pdb CUserProfileManager: ProfileName (CWideString) +0x08.
// Shape: the open result is a named bool (neg/sbb/inc) that outlives the key string and the success path is nested (the
//          failure return is laid out last); the substring is a named local
//          copied into the result (no NRVO in VC7.1), the full path is returned through the wide-string ctor.
class CCharString { public:
    void* m_pStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CWideStringData;
class CWideString { public:
    CWideStringData* PStringData;
    CWideString();                                      // 0x0099AED0
    CWideString(const unsigned short* text);            // 0x0099B6B0
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    long GetLength() const;                             // 0x0099B220
    const unsigned short* GetAsCharArray() const;             // 0x0099AF10
    CWideString GetSubString(unsigned long start, unsigned long end) const;   // 0x0099BDB0
};
CWideString __fastcall operator+(const unsigned short* left, const CWideString& right);   // 0x0099BFF0
long __fastcall CreateSaveGame(const CCharString& rootName, const CWideString& saveGameName, long creationDisposition,
                               long flags, unsigned short* pathBuffer, long pathSize);   // 0x00406E30
namespace NResourceDirectoryNames {
    CWideString __fastcall GetSaveDirWithoutTrailingBlackslash();   // 0x0041A4F0
}
class CAFile { public:
    static void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* directory,
                                     CWideString* name, CWideString* extension);   // 0x00997620
};
extern "C" int __cdecl wcsncmp(const unsigned short* a, const unsigned short* b, unsigned int count);   // 0x00BFEB0A

class CUserProfileManager {
public:
    void*       __vftable;                              // +0x00
    long        _unknown04;                             // +0x04
    CWideString ProfileName;                            // +0x08
    CWideString GetDirectoryNameFromProfileName() const;
};

CWideString CUserProfileManager::GetDirectoryNameFromProfileName() const
{
    unsigned short path[260];
    bool opened = CreateSaveGame("U:\\", ProfileName, 4, 0, path, 260) == 0;
    if (opened) {
        CWideString directory;
        CAFile::SplitPath(CWideString(path), 0, &directory, 0, 0);
        CWideString prefix = (const unsigned short*)L".\\" + NResourceDirectoryNames::GetSaveDirWithoutTrailingBlackslash();
        if (wcsncmp(directory.GetAsCharArray(), prefix.GetAsCharArray(), prefix.GetLength()) == 0) {
            CWideString name = directory.GetSubString(7, directory.GetLength());
            return name;
        }
        return path;
    }
    return (const unsigned short*)L"\\";
}