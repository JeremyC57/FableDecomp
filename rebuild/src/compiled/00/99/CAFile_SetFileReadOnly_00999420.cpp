// CAFile::SetFileReadOnly(const CWideString& path, bool readOnly) -- set or clear FILE_ATTRIBUTE_READONLY on the expanded path
// Retail: Fable.exe (TLC, VC7.1) @ 0x00999420, 126 bytes. FableWin.pdb ?SetFileReadOnly@CAFile@@SIXABVCWideString@@_N@Z:
// a static `__fastcall` (path in ecx, readOnly in dl), plain `ret`.
// Callees: CWideString() @ 0x0099AED0, CAFile::GetCurrentPath() @ 0x00997F80, CAFile::ExpandPath @ 0x00998C90,
// CWideString::operator=(const CWideString&) @ 0x0099B7D0, ~CWideString @ 0x0099B510, CWideString::operator const
// wchar_t*() const @ 0x0099AF00 (x2), GetFileAttributesW (IAT 0x0143FDBC), SetFileAttributesW (IAT 0x0143FDB4).
// Shape: the landed PathExists (0x00999230) prologue; the attributes are or-ed / and-ed in place by the if/else and
// passed straight to SetFileAttributesW (the second conversion call is evaluated after the attribute push).
// Leaf: ?SetFileReadOnly@CAFile
typedef unsigned short wchar16;          // wchar_t is not a built-in type under the retail flags

extern "C" __declspec(dllimport) unsigned long __stdcall GetFileAttributesW(const wchar16* fileName);   // IAT 0x0143FDBC
extern "C" __declspec(dllimport) int __stdcall SetFileAttributesW(const wchar16* fileName, unsigned long attributes);   // IAT 0x0143FDB4

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
    static void __fastcall SetFileReadOnly(const CWideString& path, bool readOnly);
};

void __fastcall CAFile::SetFileReadOnly(const CWideString& path, bool readOnly)
{
    CWideString fullPath;
    fullPath = ExpandPath(path, GetCurrentPath());
    unsigned long attributes = GetFileAttributesW(fullPath);
    if (readOnly)
        attributes |= 1;                                // FILE_ATTRIBUTE_READONLY
    else
        attributes &= ~1UL;
    SetFileAttributesW(fullPath, attributes);
}