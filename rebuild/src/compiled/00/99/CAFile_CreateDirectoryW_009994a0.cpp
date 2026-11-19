// 009994a0 CAFile::CreateDirectoryW(const CWideString& path) -- expand the path against the current directory, switch to its
//   drive and walk its components, changing into each one and creating it when the change fails; restore the drive after
// Flags: /O2 /Oy /W3 /Gr /GS
// Leaf: ?CreateDirectoryW@CAFile@@SIXABVCWideString@@@Z
// Identity: Ego_r.pdb 0x00B010E0 / FableWin.pdb 0x02FF3630 ?CreateDirectoryW@CAFile@@SIXABVCWideString@@@Z (static __fastcall; the source
//   wrote CreateDirectory and <windows.h> made it CreateDirectoryW). NF93 (q11296): ported from the AFileSupport stand-in namespace
//   (local CWideString model with in-TU member bodies) onto fable_string.h CWideString and engine/CAFile.h.
// Callees: CAFile::GetCurrentPath 0x00997F80 / CAFile::ExpandPath 0x00998C90 (engine/CAFile.h EXPANDPATH), ~CWideString 0x0099B510,
//   CWideString() 0x0099AED0, operator const wchar_t* 0x0099AF00 (the path), operator=(const wchar_t*) 0x0099B800,
//   GetAsCharArray 0x0099AF10 (the drive's first character), NFile::GetItemFromPath 0x00997880 (Ego_r.pdb / FableWin.pdb
//   ?GetItemFromPath@NFile@@YI_NPB_WKPA_W@Z; wide text is unsigned short here), MSVCR71 _wsplitpath / _getdrive / _chdrive /
//   _wchdir / _wmkdir import thunks. NFile::ConvertDriveLetterToNumber (Ego_r.pdb / FableWin.pdb ?ConvertDriveLetterToNumber@NFile@@YIJ_W@Z)
//   is expanded in place (no out-of-line copy in Fable.exe).
// Layout: retail frame 0x820: path +0, drive +4, temp/d +8, component +0x10, ext +0x21c, dir +0x41c, name +0x61c.
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CAFILE_EXPANDPATH
#define FABLE_RETAIL_CAFILE_CREATEDIRECTORY
#include "engine/CAFile.h"                  // fable_string.h CWideString; CAFile::GetCurrentPath / ExpandPath

extern "C" void __cdecl _wsplitpath(const unsigned short* path, unsigned short* drive, unsigned short* dir, unsigned short* name,
                                    unsigned short* ext);   // 0x00BFED4C MSVCR71 import thunk
extern "C" int __cdecl _getdrive();                         // 0x00BFED70
extern "C" int __cdecl _chdrive(int drive);                 // 0x00BFED6A
extern "C" int __cdecl _wchdir(const unsigned short* dir);  // 0x00BFEAFE
extern "C" int __cdecl _wmkdir(const unsigned short* dir);  // 0x00BFED64

namespace NFile {
bool __fastcall GetItemFromPath(const wchar_t* pathname, unsigned long directory_number, wchar_t* directory_name);   // 0x00997880
inline long __fastcall ConvertDriveLetterToNumber(wchar_t c)
{
    if (c >= 'a' && c <= 'z')
        return c - 96;
    if (c >= 'A' && c <= 'Z')
        return c - 64;
    return 0;
}
}

void __fastcall CAFile::CreateDirectoryW(const CWideString& input)
{
    CWideString path = ExpandPath(input, GetCurrentPath());
    CWideString drive;
    {
        unsigned short d[3], dir[256], ext[256], name[256], part[262];
        _wsplitpath(path, d, dir, name, ext);
        drive = d;
        const unsigned short& c = drive.GetAsCharArray()[0];
        if (c) {
            unsigned short n = (unsigned short)NFile::ConvertDriveLetterToNumber(c);
            int previous = _getdrive();
            _chdrive(n);
            if (n) {
                int index = 0;
                while (NFile::GetItemFromPath(path, index, part) == 1) {
                    if (_wchdir(part)) {
                        if (_wmkdir(part) == -1)
                            break;
                        if (_wchdir(part))
                            break;
                    }
                    ++index;
                }
            }
            if (previous != -1)
                _chdrive(previous);
        }
    }
}
