// CUserProfileManager::GetPathNameFromFileName(const CWideString&, bool) const -- a file name carrying a save extension (.qs/.cp/.rl) is put under the profile's directory; any other name under the path of the profile's save game (opened, or created when asked), "" when that fails
// Leaf: GetPathNameFromFileName
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00408120, 426 bytes (432 with padding), /GS cookie frame (0x21c), hidden result, `ret 0Ch`.
// Callees: CWideString(const wchar_t*) 0x0099B6B0 / (const CWideString&) 0x0099B720 / ~CWideString 0x0099B510,
//   CWideString::Contains 0x0099AF20, CUserProfileManager::GetDirectoryNameFromProfileName 0x004071C0,
//   operator+(const CWideString&, const CWideString&) 0x0099BE70 and operator+(const wchar_t*, const CWideString&)
//   0x0099BFF0 (`__fastcall`, hidden result in ecx), CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   CreateSaveGame 0x00406E30 (`__fastcall`, 0 = success), __security_check_cookie 0x00BFE9F9. Signatures from FableWin.pdb.
// Strings: L".qs" 0x0122DEB0, L".cp" 0x0122DEB8, L".rl" 0x0122DEC0, "U:\\" 0x0122E05C, L"" 0x0122D70C.
// Layout: Ego_r.pdb CUserProfileManager: ProfileName (CWideString) +0x08.
// Shape: the extension test is a named bool over three implicit wide-string temporaries (ebx construction flags); the
//   profile directory is a named local; the disposition an if-assigned local;
//   open result is a named bool (neg/sbb/inc); the joined path is a named local copied into the result (no NRVO).
struct CCharString {
    void* m_pStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CWideStringData;
struct CWideString {
    CWideStringData* PStringData;
    CWideString(const unsigned short* text);            // 0x0099B6B0
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    bool Contains(const CWideString& text) const;       // 0x0099AF20
};
CWideString __fastcall operator+(const CWideString& left, const CWideString& right);         // 0x0099BE70
CWideString __fastcall operator+(const unsigned short* left, const CWideString& right);      // 0x0099BFF0
long __fastcall CreateSaveGame(const CCharString& rootName, const CWideString& saveGameName, long creationDisposition,
                               long flags, unsigned short* pathBuffer, long pathSize);   // 0x00406E30

class CUserProfileManager {
public:
    void*       __vftable;                              // +0x00
    long        _unknown04;                             // +0x04
    CWideString ProfileName;                            // +0x08
    CWideString GetDirectoryNameFromProfileName() const;   // 0x004071C0
    CWideString GetPathNameFromFileName(const CWideString& fileName, bool create) const;
};

CWideString CUserProfileManager::GetPathNameFromFileName(const CWideString& fileName, bool create) const
{
    unsigned short path[260];
    bool isSave = fileName.Contains((const unsigned short*)L".qs") || fileName.Contains((const unsigned short*)L".cp")
        || fileName.Contains((const unsigned short*)L".rl");
    if (isSave) {
        CWideString directory = GetDirectoryNameFromProfileName();
        return directory + fileName;
    }
    long disposition = 3;
    if (create)
        disposition = 4;
    bool opened = CreateSaveGame("U:\\", ProfileName, disposition, 0, path, 260) == 0;
    if (opened) {
        CWideString full = path + fileName;
        return full;
    }
    return (const unsigned short*)L"";
}
