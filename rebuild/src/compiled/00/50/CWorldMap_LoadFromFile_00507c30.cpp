// CWorldMap::LoadFromFile(CAFile& file, long length) -- load the world description (.wld): make the level loader, drop the old levels and regions, then either stream the compiled binary world file or parse the text (map blocks NewMap .. EndMap with LevelName / MapX / MapY / MapUID / IsSea / LoadedOnPlayerProximity, region blocks NewRegion .. EndRegion with names, the maps a region contains and sees and its mini-map / world-map layout), place every map, then set up the thing maps, the global things file, the loadable global things and the region graph
// Leaf: ?LoadFromFile@CWorldMap@@UAEXAAVCAFile@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00507C30, 7661 bytes (ends 0x00509A1D), thiscall, `ret 8`, frame 0x1BC, no EH frame.
// Identity: Ego_r.pdb 0x004DC750 / FableWin.pdb 0x01C79720 ?LoadFromFile@CWorldMap@@UAEXAAVCAFile@@J@Z; vtable 0x01244AF8.
// Inferred: 3 dead release asserts (FABLE_RETAIL_ASSERT, overlay fable_assert.h, device d) before the EndMap placement: they emit no
//   bytes; their dead arms' IL records re-key c2's live-range split numbering so the parse-loop exit reloads come out file / buffer
//   data / this (retail 0x00509395 ebp, 0x0050939C edi, 0x005093A3 ebx); without them the first two are swapped (the only residue,
//   measured with c2prio: split pieces are numbered by a hash-order walk). Counts 0..7 measured at this site: only 3 matches.
// FITTED: the three assert conditions (any condition grades the same; these are the map-placement preconditions the block relies on: index in range, MapX and MapY given).
// EVIDENCED: the buffer is a CArray<char> named `text` (Xbox Anniversary PDB S_REGREL32 `text`, std::vector<char>; retail calls the
//   out-of-line CArray<char>(unsigned) 0x00410F30); the unread CParsedItem (Xbox PDB `parsed_item`); ReadLineAsString 0x009BA390.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQUALSCI
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CAFILE_SPLITPATH_WIDE
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_LOADWORLDFROMBINARY
#define FABLE_RETAIL_CWORLDMAP_LOADFROMFILE
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CREGION_LOADBINARY
#define FABLE_RETAIL_CREGION_LOADFROMFILE
#define FABLE_RETAIL_CMAPINFO_LOADBINARY
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_SIZE_CTOR_VISIBLE_NOINLINE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_CSTRINGPARSER_READASUHUGE
#define FABLE_RETAIL_CSTRINGPARSER_READLINE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CPLAYERGUIDEF_CLASS
#define FABLE_RETAIL_CLEVELLOADER
#define FABLE_RETAIL_CTHINGMAPMANAGER
#define FABLE_RETAIL_CTHINGMAPMANAGER_INIT
#define FABLE_RETAIL_CLEVELLOADER_CTOR
#define FABLE_RETAIL_CENGINEMANAGER_CLEANUP
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CARRAY_DEALLOCATEUNUSEDMEMORY
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGMANAGER_WORLDLOAD
#define FABLE_RETAIL_CTHINGMANAGER_SETCURRENTTHINGUID
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include "fable_assert.h"
#include <string.h>
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CAFile.h"
#include "engine/CDiskFileWin32.h"
#include "engine/CArray.h"
#include "engine/CMapInfo.h"
#include "engine/CRegion.h"
#include "engine/CStringParser.h"
#include "engine/CLevelLoader.h"
#include "engine/CWorld.h"
#include "engine/CThingManager.h"
#include "engine/CWorldMap.h"
#include "engine/CEngineManager.h"
#include "engine/NProgressDisplay.h"
#include "engine/CThingMapManager.h"
#include "engine/CExceptionBase.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "fable_profile.h"

// Row-local template bodies (the landed 0x009BB110 / 0x00837E40 precedent): fable_string.h declares CBasicString<T>::Compare
// 0x00411570 and operator==(const T*) 0x004115A0; retail's TU holds their bodies. operator== is called out of line, and because
// its body is visible the token's PStringData stays in esi across the 24 keyword compares (0x00507EAE, 0x005081C6..).
template<class T> inline long __fastcall CBasicString<T>::Compare(const T* a,const T* b)
{
    while(*a || *b) {
        if(*a<*b)return -1;
        if(*a>*b)return 1;
        ++a; ++b;
    }
    return 0;
}
template<class T> inline bool CBasicString<T>::operator==(const T* text) const
{ return Compare(PData,text)==0; }

// CGenericException : CExceptionBase -- the shared header (ThrowInfo 0x013692F8 = __TI2?AVCGenericException@@, 1 byte).
#include "engine/CGenericException.h"




namespace NResourceDirectoryNames { CWideString __fastcall GetLevelDir(); }   // 0x0041A060 (Ego_r.pdb; landed CWorldMap::SaveMapsToString 0x004FB990 declares it so)
extern bool g_bUseCompiledWorldFiles;                   // 0x013B8618 (landed CProgressDisplay::DrawRetailDisplay 0x00498490)
extern bool GAllowDataGeneration;                          // 0x01375459 (landed CNewFrontendGameComponent::InitialiseEngine 0x0042E204) (Ego_r / FableWin / Ego_d publics; NF236: was g_WADBuildEnabled)
extern bool g_bUseCompiledGlobalThings;                 // 0x013B8609 (landed CProgressDisplay::DrawRetailDisplay 0x00498490)


// the parser must sit on a quote: anything else is a malformed world file
#define EXPECT_QUOTE() if (!(parser.ReadAsSymbol() == "\"")) throw CGenericException()

void CWorldMap::LoadFromFile(CAFile& file, long length)
{
    if (PLevelLoader.IsNull())
        PLevelLoader.Reset(new CLevelLoader(World.GetThingManager(), *this, World));
    DeleteAllLevels();
    DeleteAllRegions();
    CEngineManager::Get().Cleanup();
    NProfileTimer::StartProfile("Load .wld file", 0);

    if (g_bUseCompiledWorldFiles) {
        CWideString binaryFile;
        CWideString directory;
        CWideString drive;
        CWideString name;
        CAFile::SplitPath(file.GetPathname(), &drive, &directory, &name, 0);
        binaryFile = drive + directory + name + L".bwd";
        LoadWorldFromBinaryFile(binaryFile);
    } else {
        if (length == -1)
            length = file.GetLength();
        CArray<char> text(file.GetLength() + 1);
        file.Read(&text[0], length, 0);
        text[file.GetLength()] = 0;

        CParsedItem item;      // unused: retail constructs and destroys it (String +8 at [esp+160h], 0x00507DFE / 0x0050953A)
        CStringParser parser(&text[0]);
        long newMap = 0;
        long mapX = -1;
        long mapY = -1;
        unsigned long mapUID = 0;
        CCharString levelBaseName;
        CCharString levelName;
        CCharString levelScriptName;
        CurrentMapUIDCount = 1;
        bool loadedOnPlayerProximity = true;
        bool isSea = false;
        World.GetThingManager().SetCurrentThingUID(1);

        while (parser.NextItemExists()) {
            CCharString token = parser.ReadAsString();
            if (token == "MapUIDCount") {
                CurrentMapUIDCount = parser.ReadAsInteger();
            } else if (token == "ThingManagerUIDCount") {
                World.GetThingManager().SetCurrentThingUID(parser.ReadAsUHUGE());
            } else if (token == "LevelScriptName") {
                EXPECT_QUOTE();
                levelScriptName = parser.ReadAsStringUntilString("\"");
                EXPECT_QUOTE();
            } else if (token == "NewMap") {
                NProgressDisplay::DisplayProgress("Loading maps", -1.0f, false, false);
                newMap = parser.ReadAsInteger();
            } else if (token == "EndMap") {
                if (newMap) {
                    if (Maps.size() < (unsigned int)(newMap + 1))
                        Maps.resize(newMap + 1);
                    FABLE_RETAIL_ASSERT((unsigned int)newMap < Maps.size());
                    FABLE_RETAIL_ASSERT(mapX != -1);
                    FABLE_RETAIL_ASSERT(mapY != -1);
                    CMapInfo& info = Maps[newMap];
                    info.LevelName = levelName;
                    if (levelScriptName == "")
                        info.LevelScriptName = levelBaseName;
                    else
                        info.LevelScriptName = levelScriptName;
                    info.LoadedOnPlayerProximity = loadedOnPlayerProximity;
                    info.Used = true;
                    info.IsSea = isSea;
                    info.Area = GetLevelDimensions(levelName);
                    // move the level's box to its place on the world grid
                    info.Area.BRX += mapX - info.Area.TLX;
                    info.Area.BRY += mapY - info.Area.TLY;
                    info.Area.TLX = mapX;
                    info.Area.TLY = mapY;
                    info.Placed = true;
                    info.MapUID = mapUID;
                    if (IsMapPlacementPositionFree(info.Area)) {
                        if (mapUID == 0)
                            info.MapUID = AllocateMapUID(newMap);
                        SetMapPlacement(info.Area, newMap);
                        CEngineManager::Get().AddMap(newMap);
                        levelName = "";
                        mapY = -1;
                        mapX = -1;
                        newMap = 0;
                    } else {
                        info.Used = false;
                    }
                }
            } else if (token == "MapUID") {
                mapUID = parser.ReadAsInteger();
            } else if (token == "MapX") {
                mapX = parser.ReadAsInteger();
            } else if (token == "MapY") {
                mapY = parser.ReadAsInteger();
            } else if (token == "IsSea") {
                CCharString value = parser.ReadAsString();
                if (value.EqualsCaseInsensitive("TRUE"))
                    isSea = true;
                else
                    isSea = false;
            } else if (token == "LoadedOnPlayerProximity") {
                CCharString value = parser.ReadAsString();
                if (value.EqualsCaseInsensitive("TRUE"))
                    loadedOnPlayerProximity = true;
                else
                    loadedOnPlayerProximity = false;
            } else if (token == "LevelName") {
                EXPECT_QUOTE();
                levelName = NResourceDirectoryNames::GetLevelDir().ToCharString() + parser.ReadAsStringUntilString("\"");
                CAFile::SplitPath(levelName, 0, 0, &levelBaseName, 0);
                EXPECT_QUOTE();
            } else if (token == "NewRegion") {
                Regions.push_back(CRegion());
                parser.ReadAsInteger();
            } else if (token == "EndRegion") {
            } else if (token == "RegionDef") {
                EXPECT_QUOTE();
                CCharString regionDef = parser.ReadAsStringUntilString("\"");
                EXPECT_QUOTE();
                Regions.back().SetRegionDefName(regionDef);
            } else if (token == "EnvironmentDef") {
                EXPECT_QUOTE();
                parser.ReadAsStringUntilString("\"");
                EXPECT_QUOTE();
            } else if (token == "DisplayName" || token == "RegionName") {
                EXPECT_QUOTE();
                CCharString regionName = parser.ReadAsStringUntilString("\"");
                EXPECT_QUOTE();
                Regions.back().Name = regionName;
            } else if (token == "NewDisplayName") {
                EXPECT_QUOTE();
                CCharString displayName = parser.ReadAsStringUntilString("\"");
                EXPECT_QUOTE();
                Regions.back().DisplayName = displayName;
            } else if (token == "ContainsMap") {
                EXPECT_QUOTE();
                CCharString mapName = NResourceDirectoryNames::GetLevelDir().ToCharString() + parser.ReadAsStringUntilString("\"");
                CAFile::SplitPath(mapName, 0, 0, &levelBaseName, 0);
                EXPECT_QUOTE();
                long map = 1;
                for (; (unsigned int)map < Maps.size(); ++map) {
                    if (Maps[map].LevelName == mapName)
                        break;
                }
                Regions.back().AddMap(map);
            } else if (token == "SeesMap") {
                EXPECT_QUOTE();
                CCharString mapName = NResourceDirectoryNames::GetLevelDir().ToCharString() + parser.ReadAsStringUntilString("\"");
                EXPECT_QUOTE();
                for (long map = 1; (unsigned int)map < Maps.size(); ++map) {
                    if (Maps[map].LevelName == mapName) {
                        Regions.back().VisMaps.push_back(map);
                        break;
                    }
                }
            } else if (token == "AppearOnWorldMap") {
                Regions.back().AppearOnWorldMap = true;
            } else if (token == "MiniMapGraphic") {
                CCharString graphic = parser.ReadAsString();
                Regions.back().MiniMapGraphic = graphic;
            } else if (token == "MiniMapScale") {
                float value = parser.ReadAsFloat();
                Regions.back().MiniMapScale = value;
            } else if (token == "MiniMapOffsetX") {
                float value = parser.ReadAsFloat();
                Regions.back().MiniMapOffset.X = value;
            } else if (token == "MiniMapOffsetY") {
                float value = parser.ReadAsFloat();
                Regions.back().MiniMapOffset.Y = value;
            } else if (token == "MiniMapRegionExitTextOffsetX") {
                parser.ReadAsSymbol();
                CCharString exitName = parser.ReadAsString();
                parser.ReadAsSymbol();
                float x = parser.ReadAsFloat();
                Regions.back().SetMiniMapRegionExitTextOffsetX(exitName, x);
            } else if (token == "MiniMapRegionExitTextOffsetY") {
                parser.ReadAsSymbol();
                CCharString exitName = parser.ReadAsString();
                parser.ReadAsSymbol();
                float y = parser.ReadAsFloat();
                Regions.back().SetMiniMapRegionExitTextOffsetY(exitName, y);
            } else if (token == "WorldMapOffsetX") {
                float value = parser.ReadAsFloat();
                Regions.back().WorldMapOffset.X = value;
            } else if (token == "WorldMapOffsetY") {
                float value = parser.ReadAsFloat();
                Regions.back().WorldMapOffset.Y = value;
            } else if (token == "NameGraphicOffsetX") {
                float value = parser.ReadAsFloat();
                Regions.back().NameGraphicOffset.X = value;
            } else if (token == "NameGraphicOffsetY") {
                float value = parser.ReadAsFloat();
                Regions.back().NameGraphicOffset.Y = value;
            }
            // every statement ends with ';' -- anything else is skipped to the end of its line
            if (parser.ReadAsSymbol() != ";")
                parser.ReadLineAsString();             // 0x009BA390
        }

        if (GAllowDataGeneration) {
            CWideString directory;
            CWideString drive;
            CWideString name;
            CAFile::SplitPath(file.GetPathname(), &drive, &directory, &name, 0);
            CWideString binaryFile = drive + directory + name + L".bwd";
            SaveWorldToBinaryFile(binaryFile);
            CWideString thingsFile = drive + directory + name + L".gtg";
            CompileLoadableGlobalThingsToSingleFile(thingsFile);
        }
    }

    NProfileTimer::StartProfile("Init thing maps", 0);
    PThingMapManager->Init(&World, Maps.size());
    NProfileTimer::StartProfile("Load GTNG", 0);
    CWideString worldName;
    CAFile::SplitPath(file.GetPathname(), 0, 0, &worldName, 0);
    CWideString globalThingsFile = NResourceDirectoryNames::GetLevelDir() + worldName + L".gtng";
    if (CAFile::PathExists(globalThingsFile)) {
        CCountedPointer<CAFile> thingFile(new CDiskFileWin32(globalThingsFile, FILE_ACCESS_READ, 2));
        std::vector<CThing*> things;
        World.GetThingManager().LoadFromFile(thingFile, 0, GetLoadableSections(), things, 0, 0);
        World.GetThingManager().ActivateThings(things);
    }
    NProfileTimer::StartProfile("Load global things", 0);
    if (g_bUseCompiledGlobalThings) {
        CWideString directory;
        CWideString drive;
        CWideString name;
        CAFile::SplitPath(file.GetPathname(), &drive, &directory, &name, 0);
        CWideString thingsFile = drive + directory + name + L".gtg";
        LoadAllLoadableGlobalThingsFromSingleFile(thingsFile);
    } else {
        LoadLoadableGlobalThings();
    }
    NProgressDisplay::DisplayProgress("Load global things end", -1.0f, false, false);
    Maps.DeallocateUnusedMemory();                      // 0x0051B560
    NProfileTimer::StartProfile("Load region graph", 0);
    NProgressDisplay::DisplayProgress("Load region graph", -1.0f, false, false);
    LoadRegionGraph(CPlayerGui::PeekGuiDef().RegionGraphFileName.ToWideString());
    NProfileTimer::EndProfile(0);
}