// CWorldMap::LoadLevel(long, ELoadType) -- open a map's level file and its things file (from the level WAD bank or from disk), load them through LoadLevelFromFiles (the level file serves as topology and navigation) and record both files' modification stamps in the map info
// Leaf: ?LoadLevel@CWorldMap@@IAEXJW4ELoadType@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00502620, 559 bytes, thiscall, `ret 8`, frame 0x1c.
// Identity: Ego_r.pdb / FableWin.pdb ?LoadLevel@CWorldMap@@IAEXJW4ELoadType@1@@Z (protected; the only caller of LoadLevelFromFiles).
// Callees: CCharString::ToWideString 0x0099E480, CWorldMap::GetThingFilenameFromLevelFilename 0x004FAFF0 (__fastcall), ~CWideString
//   0x0099B510, operator new 0x00BFEA1A, CWADFile::CWADFile(CWADBank&, const CWideString&, FileAccessType, unsigned long) 0x00A39D80,
//   CDiskFileWin32::CDiskFileWin32(const CWideString&, FileAccessType, unsigned long) 0x0099AD80, CCountedPointer<CAFile>::Reset
//   0x0050F550, CWorldMap::LoadLevelFromFiles 0x00501150, CAFile::GetFileModifiedTimeAsSomeWackyWin32BigNumber 0x00999320 (__fastcall,
//   unsigned __int64 in edx:eax), operator delete 0x00BFE9BC.
// Layout: engine/CWorldMap.h (Maps +0x20, LevelWADBank +0xA0, UseLevelWADBank +0xA8); engine/CMapInfo.h (LevelName +0x18, LevStamp
//   +0x30, TngStamp +0x38).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_LOADLEVEL
#define FABLE_RETAIL_CAFILE_MODIFIEDTIME_WACKY
#define FABLE_RETAIL_CWADFILE
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CARRAY
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CMapInfo.h"
#include "engine/CWorldMap.h"
#include "engine/CWADFile.h"
#include "engine/CDiskFileWin32.h"

void CWorldMap::LoadLevel(long map, ELoadType loadType)
{
    CWideString thingFilename = GetThingFilenameFromLevelFilename(Maps[map].LevelName.ToWideString());
    CCountedPointer<CAFile> levelFile;
    CCountedPointer<CAFile> thingFile;
    if (UseLevelWADBank)
    {
        levelFile.Reset(new CWADFile(*LevelWADBank, Maps[map].LevelName.ToWideString(), FILE_ACCESS_READ, 2));
        thingFile.Reset(new CWADFile(*LevelWADBank, thingFilename, FILE_ACCESS_READ, 2));
    }
    else
    {
        levelFile.Reset(new CDiskFileWin32(Maps[map].LevelName.ToWideString(), FILE_ACCESS_READ, 2));
        thingFile.Reset(new CDiskFileWin32(thingFilename, FILE_ACCESS_READ, 2));
    }
    LoadLevelFromFiles(map, levelFile, levelFile, thingFile, loadType);
    Maps[map].LevStamp = CAFile::GetFileModifiedTimeAsSomeWackyWin32BigNumber(Maps[map].LevelName.ToWideString());
    Maps[map].TngStamp = CAFile::GetFileModifiedTimeAsSomeWackyWin32BigNumber(thingFilename);
}