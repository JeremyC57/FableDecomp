// CAFile::GetFileModifiedTime(const CWideString& path) -- the expanded path's last-write time as a CDateAndTime
// Retail: Fable.exe (TLC, VC7.1) @ 0x009992A0, 119 bytes. FableWin.pdb ?GetFileModifiedTime@CAFile@@SI?AVCDateAndTime@@ABVCWideString@@@Z:
// a static `__fastcall` returning CDateAndTime by value (hidden result pointer in ecx, path in edx), plain `ret`.
// Callees: CWideString() @ 0x0099AED0, CAFile::GetCurrentPath() @ 0x00997F80, CAFile::ExpandPath @ 0x00998C90,
// CWideString::operator=(const CWideString&) @ 0x0099B7D0, ~CWideString @ 0x0099B510, CWideString::operator const
// wchar_t*() const @ 0x0099AF00, GetFileAttributesExW (IAT 0x0143FDB8, GetFileExInfoStandard = 0),
// CDateAndTime::CDateAndTime(const FILETIME&) @ 0x009E9DB0 (built straight in the result slot).
// Layout: WIN32_FILE_ATTRIBUTE_DATA (0x24 bytes) with ftLastWriteTime at +0x14; the frame is the local string +
// two by-value temporaries + the attribute data (0x30).
// Shape: the landed PathExists (0x00999230) prologue; the GetFileAttributesExW result is ignored.
// Leaf: ?GetFileModifiedTime@CAFile
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

class CDateAndTime
{
public:
    long          Year;                                 // +0x00
    unsigned long Month;                                // +0x04
    unsigned long Day;                                  // +0x08
    unsigned long Hour;                                 // +0x0c
    unsigned long Minute;                               // +0x10
    unsigned long Second;                               // +0x14
    unsigned long Millisecond;                          // +0x18
    CDateAndTime(const _FILETIME& fileTime);            // 0x009E9DB0
};

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
    static CDateAndTime __fastcall GetFileModifiedTime(const CWideString& path);
};

CDateAndTime __fastcall CAFile::GetFileModifiedTime(const CWideString& path)
{
    CWideString fullPath;
    fullPath = ExpandPath(path, GetCurrentPath());
    _WIN32_FILE_ATTRIBUTE_DATA data;
    GetFileAttributesExW(fullPath, 0, &data);           // GetFileExInfoStandard
    return CDateAndTime(data.ftLastWriteTime);
}