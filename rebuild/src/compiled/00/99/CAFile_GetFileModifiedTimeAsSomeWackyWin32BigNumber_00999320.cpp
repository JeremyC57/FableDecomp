// CAFile::GetFileModifiedTimeAsSomeWackyWin32BigNumber(const CWideString& path) -- the expanded path's last-write FILETIME as one 64-bit count
// Retail: Fable.exe (TLC, VC7.1) @ 0x00999320, 137 bytes. FableWin.pdb
// ?GetFileModifiedTimeAsSomeWackyWin32BigNumber@CAFile@@SI_KABVCWideString@@@Z: a static `__fastcall` (path in ecx)
// returning unsigned __int64 in edx:eax, plain `ret`.
// Callees: CWideString() @ 0x0099AED0, CAFile::GetCurrentPath() @ 0x00997F80, CAFile::ExpandPath @ 0x00998C90,
// CWideString::operator=(const CWideString&) @ 0x0099B7D0, ~CWideString @ 0x0099B510, CWideString::operator const
// wchar_t*() const @ 0x0099AF00, GetFileAttributesExW (IAT 0x0143FDB8), __allmul @ 0x00BFEB50.
// Shape: the high word is scaled by an explicit 64-bit multiply by 2^32 (VC7.1 keeps the __allmul call for a 64-bit
// constant multiply instead of a shift) and the low word added with add/adc; the sum is held in edi:esi across the
// local's destructor.
// Leaf: ?GetFileModifiedTimeAsSomeWackyWin32BigNumber@CAFile
typedef unsigned short wchar16;          // wchar_t is not a built-in type under the retail flags

struct _FILETIME {
    unsigned long dwLowDateTime;
    unsigned long dwHighDateTime;
};
struct _WIN32_FILE_ATTRIBUTE_DATA {
    unsigned long dwFileAttributes;                     // +0x00
    _FILETIME     ftCreationTime;                       // +0x04
    _FILETIME     ftLastAccessTime;                     // +0x0c
    _FILETIME     ftLastWriteTime;                      // +0x14
    unsigned long nFileSizeHigh;                        // +0x1c
    unsigned long nFileSizeLow;                         // +0x20
};
extern "C" __declspec(dllimport) int __stdcall GetFileAttributesExW(const wchar16* fileName, int infoLevelId, void* fileInformation);   // IAT 0x0143FDB8

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
    static unsigned __int64 __fastcall GetFileModifiedTimeAsSomeWackyWin32BigNumber(const CWideString& path);
};

unsigned __int64 __fastcall CAFile::GetFileModifiedTimeAsSomeWackyWin32BigNumber(const CWideString& path)
{
    CWideString fullPath;
    fullPath = ExpandPath(path, GetCurrentPath());
    _WIN32_FILE_ATTRIBUTE_DATA data;
    GetFileAttributesExW(fullPath, 0, &data);           // GetFileExInfoStandard
    return (unsigned __int64)data.ftLastWriteTime.dwHighDateTime * 0x100000000ui64 + data.ftLastWriteTime.dwLowDateTime;
}