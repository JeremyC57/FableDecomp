// CAFile::GetDiskSectorSize(const CWideString& path) -- split the drive off the expanded path and return the (fixed) hard-disk sector size 0x800
// Retail: Fable.exe (TLC, VC7.1) @ 0x00999180, 161 bytes. FableWin.pdb ?GetDiskSectorSize@CAFile@@SIJABVCWideString@@@Z:
// a static `__fastcall` (path in ecx) returning long, plain `ret`.
// Built /GS (cookie 0x0139C8A8, __security_check_cookie 0x00BFE9F9).
// Callees: CAFile::GetCurrentPath() @ 0x00997F80, CAFile::ExpandPath @ 0x00998C90, ~CWideString @ 0x0099B510 (x3),
// CWideString() @ 0x0099AED0, CWideString::operator const wchar_t*() const @ 0x0099AF00, _wsplitpath @ 0x00BFED4C,
// CWideString::operator=(const wchar_t*) @ 0x0099B800.
// Shape: the drive letter is computed into a CWideString but the PC build never uses it: the result is the inline
// CAFile::GetHDSectorSize() constant (Ego_r.pdb lists GetHDSectorSize / GetDVDSectorSize as separate statics).
// The split buffers live in an inner block so the first drive buffer shares the current-path temporary's frame word.
// Leaf: ?GetDiskSectorSize@CAFile
// Flags: /GS
typedef unsigned short wchar16;          // wchar_t is not a built-in type under the retail flags

extern "C" void __cdecl _wsplitpath(const wchar16* path, wchar16* drive, wchar16* dir, wchar16* fname, wchar16* ext);   // 0x00BFED4C

struct CWideStringData;
class CWideString
{
public:
    CWideStringData* PStringData;                       // +0x00
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
    const CWideString& operator=(const wchar16* text);  // 0x0099B800
    operator const wchar16*() const;                    // 0x0099AF00
};

class CAFile
{
public:
    static CWideString __fastcall GetCurrentPath();                                             // 0x00997F80
    static CWideString __fastcall ExpandPath(const CWideString& path, const CWideString& base);  // 0x00998C90
    static long __fastcall GetHDSectorSize() { return 0x800; }
    static long __fastcall GetDiskSectorSize(const CWideString& path);
};

long __fastcall CAFile::GetDiskSectorSize(const CWideString& path)
{
    CWideString fullPath = ExpandPath(path, GetCurrentPath());
    CWideString drive;
    {
        wchar16 driveText[3];
        wchar16 dir[256];
        wchar16 name[256];
        wchar16 ext[256];
        _wsplitpath(fullPath, driveText, dir, name, ext);
        drive = driveText;
    }
    return GetHDSectorSize();
}