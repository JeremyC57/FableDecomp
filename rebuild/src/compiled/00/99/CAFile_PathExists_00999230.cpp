// CAFile::PathExists(const CWideString& path) -- true when the expanded path names an existing file or directory
// Retail 0x00999230, 111 bytes (Ego_r.pdb ?PathExists@CAFile@@SI_NABVCWideString@@@Z: static __fastcall, path in ecx).
// Body: full = ExpandPath(path, GetCurrentPath()) assigned into a default-constructed local, then
// GetFileAttributesW(full) != INVALID_FILE_ATTRIBUTES (0xFFFFFFFF) through the dllimport thunk 0x0143FDBC.
// Callees: CWideString() @ 0x0099AED0, CAFile::GetCurrentPath() @ 0x00997F80 and CAFile::ExpandPath(const
// CWideString&, const CWideString&) @ 0x00998C90 (static __fastcall, hidden result in ecx, first argument in edx),
// CWideString::operator= @ 0x0099B7D0, ~CWideString @ 0x0099B510, operator const wchar_t* @ 0x0099AF00.
// Shape: the two by-value temporaries die at the end of the assignment statement (expand result first);
// the local's out-of-line destructor is called once per return path, so each `return` carries its own
// `call ~CWideString` before the `mov al,1` / `xor al,al` (the `!= INVALID` test returning true first
// lays the true exit first; `== INVALID ... return false` swaps the two exits).
// Leaf: ?PathExists@CAFile

typedef unsigned short wchar16;          // wchar_t is not a built-in type under the retail flags

extern "C" __declspec(dllimport) unsigned long __stdcall GetFileAttributesW(const wchar16* fileName);   // IAT 0x0143FDBC

class CWideString
{
public:
    void* PData;
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    operator const wchar16*() const;                    // 0x0099AF00
};

class CAFile
{
public:
    static CWideString __fastcall GetCurrentPath();                                             // 0x00997F80
    static CWideString __fastcall ExpandPath(const CWideString& path, const CWideString& base);  // 0x00998C90
    static bool __fastcall PathExists(const CWideString& path);
};

bool __fastcall CAFile::PathExists(const CWideString& path)
{
    CWideString fullPath;
    fullPath = ExpandPath(path, GetCurrentPath());
    if (GetFileAttributesW(fullPath) != 0xFFFFFFFF)
        return true;
    return false;
}
