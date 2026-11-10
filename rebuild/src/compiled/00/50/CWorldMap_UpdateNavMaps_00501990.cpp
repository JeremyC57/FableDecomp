// CWorldMap::UpdateNavMaps(const std::set<CCharString>&, ELoadType) -- queue every loaded map's level file (from the level WAD bank or from disk) as the navigation file of one level-loader object restricted to the given sections, with loading-screen progress ticks, and for a synchronous load run the loader until it is idle
// Leaf: ?UpdateNavMaps@CWorldMap@@QAEXABV?$set@VCCharString@@U?$less@VCCharString@@@std@@V?$allocator@VCCharString@@@3@@std@@W4ELoadType@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00501990, 919 bytes, thiscall, `ret 8`, frame 0x50.
// Identity: Ego_r.pdb / FableWin.pdb (above); the twin of LoadThingsWithSectionNames 0x00500EB0 (navigation file instead of the thing
//   file, the level filename itself, and the progress ticks).
// Callees: as 0x00500EB0, plus CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 and
//   NProgressDisplay::DisplayProgress(const CCharString&, float, bool, bool) 0x009E9F40 (__fastcall: text in ecx, the first bool in dl).
// Shape: the ToWideString temporaries are built inside the two new-expressions (destroyed under VC7.1's conditional-destroy flag
//   bits, 0x00501A2D / 0x00501A88); the progress texts are implicit CCharString temporaries.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_UPDATENAVMAPS
#define FABLE_RETAIL_CLEVELLOADER
#define FABLE_RETAIL_CWADFILE
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CMapInfo.h"
#include "engine/CLevelLoader.h"
#include "engine/CWorldMap.h"
#include "engine/CWADFile.h"
#include "engine/CDiskFileWin32.h"

namespace NProgressDisplay {
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40
}

void CWorldMap::UpdateNavMaps(const std::set<CCharString>& sections, ELoadType loadType)
{
    std::vector<CLevelLoader::CObject::CLevelFiles> levelFiles;
    for (CArray<long>::iterator it = LoadedMaps.begin(); it != LoadedMaps.end(); ++it)
    {
        NProgressDisplay::DisplayProgress("CWorldMap::UpdateNavMaps", -1.0f, false, false);
        long map = *it;
        CCountedPointer<CAFile> file;
        if (UseLevelWADBank)
            file.Reset(new CWADFile(*LevelWADBank, Maps[map].LevelName.ToWideString(), FILE_ACCESS_READ, 2));
        else
            file.Reset(new CDiskFileWin32(Maps[map].LevelName.ToWideString(), FILE_ACCESS_READ, 2));
        levelFiles.push_back(CLevelLoader::CObject::CLevelFiles());
        levelFiles.back().LevelID = map;
        levelFiles.back().PNavigationFile = file;
    }
    CCountedPointer<CLevelLoader::CObject> object(new CLevelLoader::CObject);
    object->SetAsLoading(levelFiles, sections, 0);
    NProgressDisplay::DisplayProgress("CWorldMap::UpdateNavMaps - SetAsLoading", -1.0f, false, false);
    PLevelLoader->Add(object);
    NProgressDisplay::DisplayProgress("CWorldMap::UpdateNavMaps - Add", -1.0f, false, false);
    while (loadType == WAIT_FOR_LOAD && PLevelLoader->IsBusy())
    {
        NProgressDisplay::DisplayProgress("CWorldMap::UpdateNavMaps - WAIT_FOR_LOAD", -1.0f, false, false);
        PLevelLoader->RunSuspendableProcess();
    }
}