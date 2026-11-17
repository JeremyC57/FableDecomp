// CWorldMap::CreateNavMap(long map, const set<CCharString>& sections, CAFile& file, CASuspendableProcess& process) -- give the navigator manager the navigation map of a level file: from the matching named section when the file has a section table, else from the single navigation block
// Leaf: ?CreateNavMap@CWorldMap@@QAEXJABV?$set@VCCharString@@U?$less@VCCharString@@@std@@V?$allocator@VCCharString@@@3@@std@@AAVCAFile@@AAVCASuspendableProcess@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00500230, 425 bytes, `ret 10h`.
// Callees: CAFile slot 5 SetPosition (+0x14) / slot 7 GetPosition (+0x1C), CReplaceSerialise<CFileFormatLevel,COldFileFormatLevel3>::ReadFromFile
//   0x0051A240, CAFile::ReadVar<int> 0x00516180 (out of line), CMemoryFile (CBaseClassNonCopyable ctor 0x0099A2F0 / dtor 0x0099A300, vtable
//   0x0122E0FC, CWideString ctor 0x0099AED0 / dtor 0x0099B510, OpenFromFile 0x009E9420, Close 0x009E9390, free 0x00BFEA14),
//   CWorldMap::GetNavigationSectionOffsetInFile 0x004FE630 (pair<CCharString,long> by value), CNavigatorManager
//   IsNavigationMapLoadedWithSectionName 0x00A14EE0 / IsNavigationMapLoaded 0x00A14E80 / RemoveNavigationMap 0x00A155D0 / AddNavigationMap
//   0x00A157C0, CCharString(const char*, long) 0x0099EBF0 ("" 0x0122D70E) / ~CCharString 0x0099EAE0.
// NF62 (2026-11-17): ported onto engine/CWorldMap.h / CWorld.h / CNavigatorManager.h / CMemoryFile.h / CAFile.h / CFileFormatBrush.h /
//   CMapInfo.h / fable_string.h / stlport set (every local model deleted). Overlay opt-ins: FABLE_RETAIL_CAFILE_READVAR_DECL,
//   FABLE_RETAIL_CMEMORYFILE_OPENFROMFILE, FABLE_RETAIL_CNAVIGATORMANAGER_LEVELMAPS, FABLE_RETAIL_CWORLDMAP_CREATENAVMAP and
//   FABLE_RETAIL_CWORLDMAP_NAVIGATIONSECTION (the section finder 0x004FE630, its Ego_r.pdb spelling).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CAFILE_READVAR_DECL
#define FABLE_RETAIL_CMEMORYFILE
#define FABLE_RETAIL_CMEMORYFILE_OPENFROMFILE
#define FABLE_RETAIL_CNAVIGATORMANAGER_LEVELMAPS
#define FABLE_RETAIL_CFILEFORMATBRUSH
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_NAVIGATORMANAGER
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_GETMAPBOX
#define FABLE_RETAIL_CWORLDMAP_CREATENAVMAP
#define FABLE_RETAIL_CWORLDMAP_NAVIGATIONSECTION
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/C2DBoxI.h"
#include "engine/CAFile.h"
#include "engine/CMemoryFile.h"
#include "engine/CFileFormatBrush.h"
#include "engine/CNavigatorManager.h"
#include "engine/CMapInfo.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"

void CWorldMap::CreateNavMap(long map, const std::set<CCharString>& sections, CAFile& file, CASuspendableProcess& process)
{
    file.SetPosition(0);
    CFileFormatLevel level;
    level.ReadFromFile(file, &process);
    CNavigatorManager& navigator = World.GetNavigatorManager();
    if (level.NavigationSectionsOffset != 0)
    {
        file.SetPosition(level.NavigationSectionsOffset);
        int sections_end;
        file.ReadVar(sections_end, &process);
        CMemoryFile section_table;
        section_table.OpenFromFile(file, sections_end - file.GetPosition(), &process);
        std::pair<CCharString,long> section = GetNavigationSectionOffsetInFile(section_table, sections);
        if (section.second > 0 && !navigator.IsNavigationMapLoadedWithSectionName(map, section.first))
        {
            if (navigator.IsNavigationMapLoaded(map))
                navigator.RemoveNavigationMap(map);
            file.SetPosition(section.second);
            navigator.AddNavigationMap(map, GetMapBox(map), section.first, file, &process);
        }
    }
    else if (level.NavigationDataOffset != 0 && !navigator.IsNavigationMapLoaded(map))
    {
        file.SetPosition(level.NavigationDataOffset);
        navigator.AddNavigationMap(map, GetMapBox(map), "", file, &process);
    }
}
