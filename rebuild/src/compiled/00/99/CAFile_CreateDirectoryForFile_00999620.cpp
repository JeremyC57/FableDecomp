// 00999620 CAFile::CreateDirectoryForFile(const CWideString& path) -- when the expanded path does not exist yet, rebuild its drive and
//   directory part through MakePath and create that directory unless it already exists
// Flags: /O2 /Oy /W3 /Gr /GS
// Leaf: ?CreateDirectoryForFile@CAFile@@SIXABVCWideString@@@Z
// Identity: Ego_r.pdb 0x00B01240 / FableWin.pdb 0x02FF3F20 ?CreateDirectoryForFile@CAFile@@SIXABVCWideString@@@Z (static __fastcall).
//   NF93 (q11296): ported from the ExternalRetry_00999620 stand-in namespace (local CWideString model with in-TU member bodies and an
//   in-TU MakePath body) onto fable_string.h CWideString and engine/CAFile.h.
// Callees: CAFile::GetCurrentPath 0x00997F80 / ExpandPath 0x00998C90 / PathExists 0x00999230 / MakePath 0x00997780 (Ego_r.pdb /
//   FableWin.pdb ?MakePath@CAFile@@SI?AVCWideString@@ABV2@000@Z) / CreateDirectoryW 0x009994A0; CWideString() 0x0099AED0,
//   CWideString(const wchar_t*) 0x0099B6B0 (the two L"" name / extension temporaries, extension first), operator const wchar_t*
//   0x0099AF00, operator=(const wchar_t*) 0x0099B800, ~CWideString 0x0099B510; MSVCR71 _wsplitpath import thunk 0x00BFED4C.
// Layout: retail frame 0x620: drive[3], dir / name / ext[256].
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CAFILE_EXPANDPATH
#define FABLE_RETAIL_CAFILE_CREATEDIRECTORY
#include "engine/CAFile.h"                  // fable_string.h CWideString; CAFile path statics

extern "C" void __cdecl _wsplitpath(const unsigned short* path, unsigned short* drive, unsigned short* dir, unsigned short* name,
                                    unsigned short* ext);   // 0x00BFED4C MSVCR71 import thunk

// The parent directory: drive and directory with an empty name and extension (two named locals, extension first, as retail builds them).
FABLE_FORCEINLINE CWideString MakeParentDirectory(const CWideString& drive, const CWideString& dir)
{
    CWideString extension(L""), name(L"");
    return CAFile::MakePath(drive, dir, name, extension);
}

void __fastcall CAFile::CreateDirectoryForFile(const CWideString& input)
{
    CWideString path = ExpandPath(input, GetCurrentPath());
    if (!PathExists(path)) {
        CWideString drive, dir;
        {
            unsigned short d[3], folder[256], name[256], ext[256];
            _wsplitpath(path, d, folder, name, ext);
            drive = d;
            dir = folder;
        }
        CWideString parent = MakeParentDirectory(drive, dir);
        if (!PathExists(parent))
            CreateDirectoryW(parent);
    }
}
