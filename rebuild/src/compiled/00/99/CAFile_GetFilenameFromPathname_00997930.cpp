// CAFile::GetFilenameFromPathname(const CWideString& pathname) -- split the path and return file name + extension (drive and directory dropped)
// Leaf: ?GetFilenameFromPathname@CAFile@@SI?AVCWideString@@ABV2@@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00997930, length 172 bytes (+ int3 padding), /GS frame, static fastcall
// (hidden result ecx, pathname edx).
// Shape: `CWideString fname, ext; SplitPath(pathname, 0, 0, &fname, &ext); return fname + ext;` with the wide
// SplitPath (out of line @ 0x00997620) expanded here: _wsplitpath into stack buffers, the null drive/dir branches
// folded away, then the fname/ext assignments.
// Callees: CWideString::CWideString() @ 0x0099AED0, operator const wchar_t* @ 0x0099AF00, _wsplitpath @ 0x00BFED4C,
// CWideString::operator=(const wchar_t*) @ 0x0099B800, operator+(const CWideString&, const CWideString&) @ 0x0099BE70,
// CWideString::~CWideString @ 0x0099B510.
#include <stddef.h>
#include <stdlib.h>

extern "C" void __cdecl _wsplitpath(const wchar_t* path, wchar_t* drive, wchar_t* dir, wchar_t* fname,
                                    wchar_t* ext);   // 0x00BFED4C

class CWideString
{
public:
    CWideString();                                          // 0x0099AED0
    CWideString(const CWideString& other);
    ~CWideString();                                         // 0x0099B510
    operator const wchar_t*() const;                        // 0x0099AF00
    const CWideString& operator=(const wchar_t* text);      // 0x0099B800

private:
    void* Data;
};

CWideString __fastcall operator+(const CWideString& left, const CWideString& right);   // 0x0099BE70

class CAFile
{
public:
    static void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* dir,
                                     CWideString* fname, CWideString* ext)
    {
        wchar_t driveBuffer[_MAX_DRIVE];
        wchar_t dirBuffer[_MAX_DIR];
        wchar_t fnameBuffer[_MAX_FNAME];
        wchar_t extBuffer[_MAX_EXT];

        _wsplitpath(path, driveBuffer, dirBuffer, fnameBuffer, extBuffer);

        if (drive)
            *drive = driveBuffer;
        if (dir)
            *dir = dirBuffer;
        if (fname)
            *fname = fnameBuffer;
        if (ext)
            *ext = extBuffer;
    }

    static CWideString __fastcall GetFilenameFromPathname(const CWideString& pathname);
};

CWideString __fastcall CAFile::GetFilenameFromPathname(const CWideString& pathname)
{
    CWideString fname;
    CWideString ext;
    SplitPath(pathname, 0, 0, &fname, &ext);
    return fname + ext;
}