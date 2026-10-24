// CAFile::ConvertPathToFullPath(const CWideString& path, CWideString& fullPath) -- expand `path` against the current path into `fullPath`; always succeeds
// Retail: Fable.exe (TLC, VC7.1) @ 0x00999140, 64 bytes. FableWin.pdb ?ConvertPathToFullPath@CAFile@@SI_NABVCWideString@@AAV2@@Z:
// a static `__fastcall` (path in ecx, fullPath in edx), plain `ret`.
// Callees: CAFile::GetCurrentPath() @ 0x00997F80 and CAFile::ExpandPath(const CWideString&, const CWideString&) @ 0x00998C90
// (both static `__fastcall` returning CWideString through the hidden pointer in ecx), CWideString::operator=(const
// CWideString&) @ 0x0099B7D0, ~CWideString @ 0x0099B510.
// Shape: one assignment statement; the ExpandPath result and the current-path temporary die at its end (expand result
// first), then `mov al,1`. The bool overload of the landed 0x00999110 (by-value) form.
// Layout: CWideString is one pointer to the shared CWideStringData block.
// Leaf: ?ConvertPathToFullPath@CAFile
typedef unsigned short wchar16;          // wchar_t is not a built-in type under the retail flags

struct CWideStringData;
class CWideString
{
public:
    CWideStringData* PStringData;                       // +0x00
    ~CWideString();                                     // 0x0099B510
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};

class CAFile
{
public:
    static CWideString __fastcall GetCurrentPath();                                             // 0x00997F80
    static CWideString __fastcall ExpandPath(const CWideString& path, const CWideString& base);  // 0x00998C90
    static bool __fastcall ConvertPathToFullPath(const CWideString& path, CWideString& fullPath);
};

bool __fastcall CAFile::ConvertPathToFullPath(const CWideString& path, CWideString& fullPath)
{
    fullPath = ExpandPath(path, GetCurrentPath());
    return true;
}