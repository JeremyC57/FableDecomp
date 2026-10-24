// CAFile::MakePath(const CWideString& drive, const CWideString& dir, const CWideString& fname, const CWideString& ext) -- _wmakepath the four parts into a MAX_PATH buffer and return it
// Leaf: ?MakePath@CAFile@@SI?AVCWideString@@ABV2@000@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00997780, length 121 bytes (+ int3 padding), /GS frame (path[260]),
// static fastcall (hidden result ecx, drive edx, dir/fname/ext on the stack, `ret 0xc`).
// Callees: CWideString::operator const wchar_t* @ 0x0099AF00 (ext, fname, dir, drive -- right to left),
// _wmakepath @ 0x00BFED58, CWideString::CWideString(const wchar_t*) @ 0x0099B6B0.
#include <stddef.h>

extern "C" void __cdecl _wmakepath(wchar_t* path, const wchar_t* drive, const wchar_t* dir, const wchar_t* fname,
                                   const wchar_t* ext);   // 0x00BFED58

class CWideString
{
public:
    CWideString(const wchar_t* text);           // 0x0099B6B0
    ~CWideString();
    operator const wchar_t*() const;            // 0x0099AF00

private:
    void* Data;
};

class CAFile
{
public:
    static CWideString __fastcall MakePath(const CWideString& drive, const CWideString& dir,
                                           const CWideString& fname, const CWideString& ext);
};

CWideString __fastcall CAFile::MakePath(const CWideString& drive, const CWideString& dir,
                                        const CWideString& fname, const CWideString& ext)
{
    wchar_t path[260];
    _wmakepath(path, drive, dir, fname, ext);
    return CWideString(path);
}