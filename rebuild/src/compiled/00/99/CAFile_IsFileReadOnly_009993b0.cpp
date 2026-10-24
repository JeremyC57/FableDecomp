// CAFile::IsFileReadOnly(const CWideString& path) -- true when the expanded path's file attributes carry FILE_ATTRIBUTE_READONLY
// Retail: Fable.exe (TLC, VC7.1) @ 0x009993B0, 103 bytes. FableWin.pdb ?IsFileReadOnly@CAFile@@SI_NABVCWideString@@@Z:
// a static `__fastcall` (path in ecx), plain `ret`.
// Callees: CWideString() @ 0x0099AED0, CAFile::GetCurrentPath() @ 0x00997F80, CAFile::ExpandPath @ 0x00998C90 (static
// `__fastcall`, hidden result in ecx), CWideString::operator=(const CWideString&) @ 0x0099B7D0, ~CWideString @ 0x0099B510,
// CWideString::operator const wchar_t*() const @ 0x0099AF00, GetFileAttributesW through the IAT slot 0x0143FDBC.
// Shape: the landed PathExists (0x00999230) prologue -- default-constructed local assigned from ExpandPath; the
// read-only bit is kept in a callee-saved byte (`mov ebx,eax ; and bl,1`) across the local's destructor, so the
// result is a named bool computed before the scope ends.
// Leaf: ?IsFileReadOnly@CAFile
typedef unsigned short wchar16;          // wchar_t is not a built-in type under the retail flags

extern "C" __declspec(dllimport) unsigned long __stdcall GetFileAttributesW(const wchar16* fileName);   // IAT 0x0143FDBC

struct CWideStringData;
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
    static bool __fastcall IsFileReadOnly(const CWideString& path);
};

bool __fastcall CAFile::IsFileReadOnly(const CWideString& path)
{
    CWideString fullPath;
    fullPath = ExpandPath(path, GetCurrentPath());
    bool readOnly = (GetFileAttributesW(fullPath) & 1) != 0;   // FILE_ATTRIBUTE_READONLY
    return readOnly;
}