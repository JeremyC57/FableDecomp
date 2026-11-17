// CWorldMap::GetLevelDimensions(const CCharString&) -- open the level file (from the WAD bank when WAD loading is on, else from disk) and return its world box through the file overload
// Leaf: ?GetLevelDimensions@CWorldMap@@QBE?AVC2DBoxI@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00506E30, 247 bytes (`ret 8`, C2DBoxI returned through the hidden pointer).
// Callees: CCharString::ToWideString 0x0099E480, ~CWideString 0x0099B510, operator new 0x00BFEA1A, CWADFile(CWADBank&, name, 1, 2) 0x00A39D80,
//   CDiskFileWin32(name, 1, 2) 0x0099AD80, CCountedPointer<CAFile>::Reset 0x0050F550 (out of line in this TU), the protected const
//   GetLevelDimensions(CAFile&) 0x00506BB0, operator delete 0x00BFE9BC. LevelWADBank +0xA0, UseLevelWADBank +0xA8.
// NF62 (2026-11-17): ported onto engine/CWorldMap.h (LOADFROMFILE: the public const self; LEVELHEADER: the protected const CAFile& overload,
//   queue entry q10464) / CCountedPointer.h (overlay opt-in RESET_NOINLINE: the default Reset body kept out of line, as the landed row's
//   same-TU stand-in did) / CWADFile.h / CDiskFileWin32.h / fable_string.h; every local model deleted.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CWADFILE
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_NOINLINE
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_LOADFROMFILE
#define FABLE_RETAIL_CWORLDMAP_LEVELHEADER
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_ORIGINAL_FOUR_COORD_CTOR
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_SCOPED_PTR
#include <string.h>
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C2DBoxI.h"
#include "engine/CAFile.h"
#include "engine/CDiskFileWin32.h"
#include "engine/CWADBank.h"
#include "engine/CWADFile.h"
#include "engine/CCountedPointer.h"
#include "engine/CWorldMap.h"

C2DBoxI CWorldMap::GetLevelDimensions(const CCharString& levelName) const
{
    CCountedPointer<CAFile> file;
    if (UseLevelWADBank)
        file.Reset(new CWADFile(*LevelWADBank, levelName.ToWideString(), FILE_ACCESS_READ, 2));
    else
        file.Reset(new CDiskFileWin32(levelName.ToWideString(), FILE_ACCESS_READ, 2));
    return GetLevelDimensions(*file);
}
