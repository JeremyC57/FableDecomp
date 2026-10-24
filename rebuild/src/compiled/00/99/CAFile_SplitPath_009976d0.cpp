// CAFile::SplitPath(const CCharString& path, CCharString* drive, CCharString* dir, CCharString* fname, CCharString* ext) -- _splitpath into stack buffers, copy each part to the outputs that are non-null
// Leaf: ?SplitPath@CAFile@@SIXABVCCharString@@PAV2@111@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009976D0, length 168 bytes (+ int3 padding), /GS frame (drive[3] at +0,
// dir/fname/ext[256] at +4/+0x104/+0x204), static fastcall (path ecx, drive edx, three stack outputs, `ret 0xc`).
// Callees: CCharString::operator const char* @ 0x0099E4C0, _splitpath @ 0x00BFED52,
// CCharString::operator=(const char*) @ 0x0099EFE0.
// Twin: the CWideString overload @ 0x00997620 (same shape over _wsplitpath).
#include <stddef.h>
#include <stdlib.h>

extern "C" void __cdecl _splitpath(const char* path, char* drive, char* dir, char* fname, char* ext);   // 0x00BFED52

class CCharString
{
public:
    operator const char*() const;                         // 0x0099E4C0
    const CCharString& operator=(const char* text);       // 0x0099EFE0

private:
    void* Data;
};

class CAFile
{
public:
    static void __fastcall SplitPath(const CCharString& path, CCharString* drive, CCharString* dir,
                                     CCharString* fname, CCharString* ext);
};

void __fastcall CAFile::SplitPath(const CCharString& path, CCharString* drive, CCharString* dir,
                                  CCharString* fname, CCharString* ext)
{
    char driveBuffer[_MAX_DRIVE];
    char dirBuffer[_MAX_DIR];
    char fnameBuffer[_MAX_FNAME];
    char extBuffer[_MAX_EXT];

    _splitpath(path, driveBuffer, dirBuffer, fnameBuffer, extBuffer);

    if (drive)
        *drive = driveBuffer;
    if (dir)
        *dir = dirBuffer;
    if (fname)
        *fname = fnameBuffer;
    if (ext)
        *ext = extBuffer;
}