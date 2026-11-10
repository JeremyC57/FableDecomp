// CWorldMap::LoadThingsWithSectionNames(const std::set<CCharString>&, ELoadType) -- queue every loaded map's thing file (from the level WAD bank or from disk) as one level-loader object restricted to the given sections, and for a synchronous load run the loader until it is idle
// Leaf: ?LoadThingsWithSectionNames@CWorldMap@@QAEXABV?$set@VCCharString@@U?$less@VCCharString@@@std@@V?$allocator@VCCharString@@@3@@std@@W4ELoadType@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00500EB0, 668 bytes, thiscall, `ret 8`, frame 0x44.
// Identity: Ego_r.pdb / FableWin.pdb (above), Ego_r 0x004D5... beside LoadLevelFromFiles (landed 0x00501150, the same loader shape).
// Callees: CCharString::ToWideString 0x0099E480, CWorldMap::GetThingFilenameFromLevelFilename 0x004FAFF0 (__fastcall), ~CWideString
//   0x0099B510, operator new 0x00BFEA1A, CWADFile::CWADFile(CWADBank&, const CWideString&, FileAccessType, unsigned long) 0x00A39D80,
//   CDiskFileWin32::CDiskFileWin32(const CWideString&, FileAccessType, unsigned long) 0x0099AD80, CCountedPointer<CAFile>::Reset
//   0x0050F550, vector<CLevelLoader::CObject::CLevelFiles>::push_back 0x0051AE60, CLevelFiles::~CLevelFiles 0x0050F980, malloc
//   0x00BFEA0E (the CObject's set header), CCountedPointer<CLevelLoader::CObject>::DeleteData 0x0051AF00 (address stored),
//   CLevelLoader::CObject::SetAsLoading 0x006C27A0, CLevelLoader::Add 0x006C2120, CLevelLoader::IsBusy 0x006C20A0,
//   CSuspendableProcessMicrothread::RunSuspendableProcess (slot 1), operator delete 0x00BFE9BC, free 0x00BFEA14.
// Layout: engine/CWorldMap.h (Maps +0x20, LoadedMaps +0x90, LevelWADBank +0xA0, UseLevelWADBank +0xA8, PLevelLoader +0xBC);
//   CLevelLoader / CObject / CLevelFiles from the FABLE_RETAIL_CLEVELLOADER branch.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_LOADTHINGSWITHSECTIONNAMES
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

void CWorldMap::LoadThingsWithSectionNames(const std::set<CCharString>& sections, ELoadType loadType)
{
    std::vector<CLevelLoader::CObject::CLevelFiles> levelFiles;
    for (CArray<long>::iterator it = LoadedMaps.begin(); it != LoadedMaps.end(); ++it)
    {
        long map = *it;
        CWideString thingFilename = GetThingFilenameFromLevelFilename(Maps[map].LevelName.ToWideString());
        CCountedPointer<CAFile> file;
        if (UseLevelWADBank)
            file.Reset(new CWADFile(*LevelWADBank, thingFilename, FILE_ACCESS_READ, 2));
        else
            file.Reset(new CDiskFileWin32(thingFilename, FILE_ACCESS_READ, 2));
        levelFiles.push_back(CLevelLoader::CObject::CLevelFiles());
        levelFiles.back().LevelID = map;
        levelFiles.back().PThingFile = file;
    }
    CCountedPointer<CLevelLoader::CObject> object(new CLevelLoader::CObject);
    object->SetAsLoading(levelFiles, sections, 0);
    PLevelLoader->Add(object);
    while (loadType == WAIT_FOR_LOAD && PLevelLoader->IsBusy())
        PLevelLoader->RunSuspendableProcess();
}