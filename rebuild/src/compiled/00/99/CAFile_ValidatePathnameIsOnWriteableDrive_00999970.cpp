// CAFile::ValidatePathnameIsOnWriteableDrive(const CWideString& path) -- debug-only check: when read-only drive emulation is on, split the drive letter off the full path (the check itself is compiled out)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00999970, 192 bytes. FableWin.pdb
// ?ValidatePathnameIsOnWriteableDrive@CAFile@@SIXABVCWideString@@@Z: a static `__fastcall` (path in ecx), plain `ret`.
// Built /GS (cookie 0x0139C8A8, __security_check_cookie 0x00BFE9F9).
// Callees: CCharString() @ 0x0099E4B0, CAFile::GetCurrentPath() @ 0x00997F80, CAFile::ExpandPath @ 0x00998C90,
// ~CWideString @ 0x0099B510 (x2), CWideString::ToCharString() const @ 0x0099B2C0, CCharString::operator const char*()
// const @ 0x0099E4C0, _splitpath @ 0x00BFED52, CCharString::operator=(const char*) @ 0x0099EFE0, ~CCharString @ 0x0099EAE0 (x2).
// Globals: CAFile::DebugUseReadOnlyDrives (bool) @ 0x013BC9F8 (set by SetDebugUseReadOnlyDrives).
// Shape: the inline wide ConvertPathToFullPath (landed 0x00999110) is expanded, its result narrowed into a named
// local and destroyed at once; the split buffers sit in an inner block (the drive buffer reuses the dead wide
// temporary's frame word); the drive string is assigned and nothing reads it in the retail build.
// Leaf: ?ValidatePathnameIsOnWriteableDrive@CAFile
// Flags: /GS
typedef unsigned short wchar16;          // wchar_t is not a built-in type under the retail flags

extern "C" void __cdecl _splitpath(const char* path, char* drive, char* dir, char* fname, char* ext);   // 0x00BFED52

class CWideStringData;
class CCharStringData;
class CCharString
{
public:
    CCharStringData* PStringData;                       // +0x00
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const char* text);     // 0x0099EFE0
    operator const char*() const;                       // 0x0099E4C0
};

class CWideString
{
public:
    CWideStringData* PStringData;                       // +0x00
    ~CWideString();                                     // 0x0099B510
    CCharString ToCharString() const;                   // 0x0099B2C0
};

class CAFile
{
public:
    static bool DebugUseReadOnlyDrives;                                                         // 0x013BC9F8
    static CWideString __fastcall GetCurrentPath();                                             // 0x00997F80
    static CWideString __fastcall ExpandPath(const CWideString& path, const CWideString& base);  // 0x00998C90
    static CWideString __fastcall ConvertPathToFullPath(const CWideString& path)                // 0x00999110 (inlined here)
    {
        return ExpandPath(path, GetCurrentPath());
    }
    static void __fastcall ValidatePathnameIsOnWriteableDrive(const CWideString& path);
};

void __fastcall CAFile::ValidatePathnameIsOnWriteableDrive(const CWideString& path)
{
    if (DebugUseReadOnlyDrives) {
        CCharString drive;
        CCharString fullPath = ConvertPathToFullPath(path).ToCharString();
        {
            char driveText[3];
            char dir[256];
            char name[256];
            char ext[256];
            _splitpath(fullPath, driveText, dir, name, ext);
            drive = driveText;
        }
    }
}