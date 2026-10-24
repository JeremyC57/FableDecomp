// CAFile::SplitPath(const CWideString& path, CWideString* drive, CWideString* dir, CWideString* fname, CWideString* ext) -- _wsplitpath into stack buffers, copy each part to the outputs that are non-null
// Leaf: ?SplitPath@CAFile@@SIXABVCWideString@@PAV2@111@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00997620, length 168 bytes (+ int3 padding), /GS frame (drive[3] at +0,
// dir/fname/ext[256] at +8/+0x208/+0x408), static fastcall (path ecx, drive edx, three stack outputs, `ret 0xc`).
// Callees: CWideString::operator const wchar_t* @ 0x0099AF00, _wsplitpath @ 0x00BFED4C,
// CWideString::operator=(const wchar_t*) @ 0x0099B800.
// Label: the manifest's parameter list (five CCharString*) is the narrow twin's; this row is the CWideString overload
// (Ego_r.pdb / FableWin.pdb order: wide SplitPath, then the CCharString one @ 0x009976D0).
#include <stddef.h>
#include <stdlib.h>

extern "C" void __cdecl _wsplitpath(const wchar_t* path, wchar_t* drive, wchar_t* dir, wchar_t* fname,
                                    wchar_t* ext);   // 0x00BFED4C

class CWideString
{
public:
    operator const wchar_t*() const;                        // 0x0099AF00
    const CWideString& operator=(const wchar_t* text);      // 0x0099B800

private:
    void* Data;
};

class CAFile
{
public:
    static void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* dir,
                                     CWideString* fname, CWideString* ext);
};

void __fastcall CAFile::SplitPath(const CWideString& path, CWideString* drive, CWideString* dir,
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