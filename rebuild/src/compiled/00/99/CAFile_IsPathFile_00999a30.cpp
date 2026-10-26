// CAFile::IsPathFile(const CWideString& path) -- true when the path exists and its expanded form is not a directory
// Retail: Fable.exe (TLC, VC7.1) @ 0x00999A30, 123 bytes. FableWin.pdb ?IsPathFile@CAFile@@SI_NABVCWideString@@@Z:
// a static `__fastcall` (path in ecx), plain `ret`.
// Callees: CWideString() @ 0x0099AED0, CAFile::GetCurrentPath() @ 0x00997F80, CAFile::ExpandPath @ 0x00998C90,
// CWideString::operator=(const CWideString&) @ 0x0099B7D0, ~CWideString @ 0x0099B510, CAFile::PathExists(const
// CWideString&) @ 0x00999230 (static `__fastcall`, called with the ORIGINAL path), CWideString::operator const
// wchar_t*() const @ 0x0099AF00, GetFileAttributesW (IAT 0x0143FDBC).
// Shape: the landed PathExists prologue; the two tests share one false exit and each return carries its own
// out-of-line destructor call (`mov al,1` / `xor al,al` after it).
// Leaf: ?IsPathFile@CAFile
typedef unsigned short wchar16;          // wchar_t is not a built-in type under the retail flags

extern "C" __declspec(dllimport) unsigned long __stdcall GetFileAttributesW(const wchar16* fileName);   // IAT 0x0143FDBC

class CWideStringData;
class CWideString
{
public:
    CWideStringData* PStringData;                       // +0x00
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    operator const wchar16*() const;                    // 0x0099AF00
};

class CAFile
{
public:
    static CWideString __fastcall GetCurrentPath();                                             // 0x00997F80
    static CWideString __fastcall ExpandPath(const CWideString& path, const CWideString& base);  // 0x00998C90
    static bool __fastcall PathExists(const CWideString& path);                                 // 0x00999230
    static bool __fastcall IsPathFile(const CWideString& path);
};

bool __fastcall CAFile::IsPathFile(const CWideString& path)
{
    CWideString fullPath;
    fullPath = ExpandPath(path, GetCurrentPath());
    if (PathExists(path) && !(GetFileAttributesW(fullPath) & 0x10))   // FILE_ATTRIBUTE_DIRECTORY
        return true;
    return false;
}