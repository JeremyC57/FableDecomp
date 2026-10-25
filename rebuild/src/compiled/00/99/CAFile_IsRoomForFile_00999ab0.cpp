// CAFile::IsRoomForFile(long size, const CWideString& pathname) -- true when the drive holding pathname (its directory created if missing) has at least size bytes free
// Leaf: ?IsRoomForFile@CAFile@@
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00999AB0, 415 bytes (+ int3 padding). FableWin.pdb `?IsRoomForFile@CAFile@@SI_NJABVCWideString@@@Z`:
//   a static `__fastcall` (size in ecx, pathname in edx), plain `ret`, /GS frame (the four _wsplitpath buffers) with
//   the cookie check 0x00BFE9F9 on both returns.
// Callees: CWideString() 0x0099AED0, ~CWideString 0x0099B510, CWideString(const wchar_t*) 0x0099B6B0, CWideString::
//   operator=(const wchar_t*) 0x0099B800, CWideString::operator const wchar_t*() 0x0099AF00, CAFile::GetCurrentPath
//   0x00997F80, CAFile::ExpandPath 0x00998C90, CAFile::MakePath 0x00997780, CAFile::PathExists 0x00999230,
//   CAFile::CreateDirectoryW 0x009994A0 (all static __fastcall, CWideString results through ecx), _wsplitpath thunk
//   0x00BFED4C, GetDiskFreeSpaceExW (kernel32 IAT 0x0143FDB0). Data: L"" 0x0122D70C.
// Shape: the landed CAFile::CreateDirectoryForFile 0x00999620 split: the full path is ExpandPath(pathname,
//   GetCurrentPath()), split into drive / directory (buffers drive[3], dir[256], fname[256], ext[256]; the full path
//   dies after the two assignments), rejoined as MakePath(drive, dir, L"", L"") through the landed sibling's
//   forced-inline helper with NAMED empty strings (extension built first; unnamed temporaries are built and pushed one
//   at a time), the directory created when missing, then the free byte count compared unsigned 64-bit against the
//   sign-extended size: three separate `return false` (the merged false block on the fall-through, true at the end).
typedef unsigned short wchar;

class CWideString
{
public:
    void* PStringData;                                          // +0x00
    CWideString();                                              // 0x0099AED0
    CWideString(const wchar* text);                             // 0x0099B6B0
    ~CWideString();                                             // 0x0099B510
    CWideString& operator=(const wchar* text);                  // 0x0099B800
    operator const wchar*() const;                              // 0x0099AF00
};

union ULARGE_INTEGER {
    struct { unsigned long LowPart; unsigned long HighPart; };
    unsigned __int64 QuadPart;
};
extern "C" __declspec(dllimport) int __stdcall GetDiskFreeSpaceExW(const wchar* directory, ULARGE_INTEGER* freeBytesAvailable,
                                                                   ULARGE_INTEGER* totalBytes, ULARGE_INTEGER* totalFreeBytes);
extern "C" void __cdecl _wsplitpath(const wchar* path, wchar* drive, wchar* dir, wchar* fname, wchar* ext);   // 0x00BFED4C

class CAFile
{
public:
    static CWideString __fastcall GetCurrentPath();                                             // 0x00997F80
    static CWideString __fastcall ExpandPath(const CWideString& path, const CWideString& base); // 0x00998C90
    static CWideString __fastcall MakePath(const CWideString& drive, const CWideString& dir,
                                           const CWideString& fname, const CWideString& ext);    // 0x00997780
    static bool __fastcall PathExists(const CWideString& path);                                 // 0x00999230
    static void __fastcall CreateDirectoryW(const CWideString& path);                           // 0x009994A0
    static bool __fastcall IsRoomForFile(long size, const CWideString& pathname);
};

// The directory part of a split path rejoined with an empty name and extension (both named, extension built first).
static __forceinline CWideString MakeDirectoryPath(const CWideString& drive, const CWideString& dir)
{
    CWideString extension(L""), name(L"");
    return CAFile::MakePath(drive, dir, name, extension);
}

bool __fastcall CAFile::IsRoomForFile(long size, const CWideString& pathname)
{
    CWideString drive, dir;
    {
        CWideString fullPath = ExpandPath(pathname, GetCurrentPath());
        wchar driveBuffer[3], dirBuffer[256], nameBuffer[256], extBuffer[256];
        _wsplitpath(fullPath, driveBuffer, dirBuffer, nameBuffer, extBuffer);
        drive = driveBuffer;
        dir = dirBuffer;
    }
    CWideString root = MakeDirectoryPath(drive, dir);
    if (!PathExists(root)) {
        CreateDirectoryW(root);
        if (!PathExists(root))
            return false;
    }
    ULARGE_INTEGER freeBytes, totalBytes;
    if (!GetDiskFreeSpaceExW(root, &freeBytes, &totalBytes, 0))
        return false;
    if (freeBytes.QuadPart < (unsigned __int64)(__int64)size)
        return false;
    return true;
}