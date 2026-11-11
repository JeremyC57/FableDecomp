// CWorldMap::LoadFromFile(CAFile& file, long length) -- load the world description (.wld): make the level loader, drop the old levels and regions, then either stream the compiled binary world file or parse the text (map blocks NewMap .. EndMap with LevelName / MapX / MapY / MapUID / IsSea / LoadedOnPlayerProximity, region blocks NewRegion .. EndRegion with names, the maps a region contains and sees and its mini-map / world-map layout), place every map, then set up the thing maps, the global things file, the loadable global things and the region graph
// Grade: DIFFER(6489v7661) -- first hand-authored draft (chunk H40g, 2 grades; 2257 masked lines): every statement of the retail body is transcribed and the call sequence follows retail from 0x00507C30 to the keyword loop, but (a) retail tests the token's PStringData ONCE (esi, 0x00507EAE) and lays the 24-keyword chain out twice -- a null-token chain of `repe cmpsb` against "" (0x00507EBA..0x005081C1) and a chain of out-of-line CBasicString<char>::operator==(const char*) 0x004115A0 calls on esi -- where ours reloads [token] and re-tests it after every failed compare (one chain, ~1.2 KB short); (b) frame 0x1B0 v 0x1BC and this in ebp v ebx follow from (a)
// Next: find the spelling that lets VC7.1 keep the token pointer in a register across the 0x004115A0 calls. Dead ends (no-grade compiles, w7/507c..f): `#pragma optimize("a", on)` (same 6489 bytes), `/Oa` (6487), `CCharString token; token = ...` (6530), the visible CBasicString operator==(const char*) body FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE (6520, no second chain). Untested: a keyword helper taking the token by value or a `const CCharString&` bound to the ReadAsString temporary; the keyword tests as an early-`continue` ladder instead of else-if; CStringParser 0x009BA390 is ReadLineAsString (coverage.tsv name), declared nowhere yet (the draft calls ReadAsStringUntilWhitespace in its place); the IsSea / LoadedOnPlayerProximity compare destroys the ReadAsString result BEFORE the "TRUE" string (0x005085F1..0x0050860D), ours the other way round; the thrown CGenericException temporary shares the loadedOnPlayerProximity byte slot [esp+13h] in retail (an uninitialised empty object: 0x00508328)
// Leaf: ?LoadFromFile@CWorldMap@@UAEXAAVCAFile@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00507C30, 7661 bytes (ends 0x00509A1D), thiscall, `ret 8`, frame 0x1BC, no EH frame.
// Identity: Ego_r.pdb 0x004DC750 / FableWin.pdb 0x01C79720 ?LoadFromFile@CWorldMap@@UAEXAAVCAFile@@J@Z; vtable 0x01244AF8.
// HEADER DEBT (differ draft only): CGenericException, NProfileTimer, the three mode flags, the levels-directory helper
// 0x0041A060 and CStringParser 0x009BA390 are declared in the row; the CWorldMap / CRegion / CThingManager opt-ins are in hdr/.
#define FABLE_RETAIL_CCHARSTRING
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
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CSTRINGPARSER_READASUHUGE
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

class CGenericException { };                            // throw info 0x013692F8 (.?AVCGenericException@@, 1 byte)
namespace NProfileTimer {
void FABLE_FASTCALL StartProfile(const CCharString& name, long flags);   // 0x009D8240
void FABLE_FASTCALL EndProfile(long flags);                              // 0x009D8250
}
CWideString FABLE_FASTCALL GetLevelsDirectory();        // 0x0041A060 (descriptive: returns L"Data\\Levels\\")
extern bool GLoadBinaryWorld;                           // 0x013B8618 (descriptive)
extern bool GSaveBinaryWorld;                           // 0x01375459 (descriptive)
extern bool GLoadGlobalThingsFromSingleFile;            // 0x013B8609 (descriptive)
class CPlayerGUIDef;
const CPlayerGUIDef& FABLE_FASTCALL GetPlayerGUIDef();  // 0x00438570 (the cached definition; the region graph name is its CCharString at +0xA94)

// profile / progress markers: retail builds a named string and passes its address (0x00507C9F..0x00507CB7)
inline void StartProfile(const char* name)
{
    CCharString text(name);
    NProfileTimer::StartProfile(text, 0);
}
inline void DisplayProgress(const char* name)
{
    CCharString text(name);
    NProgressDisplay::DisplayProgress(text, -1.0f, false, false);
}

// the parser must sit on a quote: anything else is a malformed world file
#define EXPECT_QUOTE() if (!(parser.ReadAsSymbol() == "\"")) throw CGenericException()

void CWorldMap::LoadFromFile(CAFile& file, long length)
{
    if (PLevelLoader.IsNull())
        PLevelLoader.Reset(new CLevelLoader(World.GetThingManager(), *this, World));
    DeleteAllLevels();
    DeleteAllRegions();
    CEngineManager::Get().Cleanup();
    StartProfile("Load .wld file");

    if (GLoadBinaryWorld) {
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
        CArray<char> buffer(file.GetLength() + 1);
        char* text = buffer.GetAsCArray();
        file.Read(text, length, 0);
        text[file.GetLength()] = 0;

        CCharString error;
        CStringParser parser(text);
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
        World.GetThingManager().CurrentUniqueID = 1;

        while (parser.NextItemExists()) {
            CCharString token = parser.ReadAsString();
            if (token == "MapUIDCount") {
                CurrentMapUIDCount = parser.ReadAsInteger();
            } else if (token == "ThingManagerUIDCount") {
                World.GetThingManager().CurrentUniqueID = parser.ReadAsUHUGE();
            } else if (token == "LevelScriptName") {
                EXPECT_QUOTE();
                levelScriptName = parser.ReadAsStringUntilString(CCharString("\""));
                EXPECT_QUOTE();
            } else if (token == "NewMap") {
                DisplayProgress("Loading maps");
                newMap = parser.ReadAsInteger();
            } else if (token == "EndMap") {
                if (newMap) {
                    if (Maps.size() < (unsigned int)(newMap + 1))
                        Maps.resize(newMap + 1);
                    CMapInfo& info = Maps[newMap];
                    info.LevelName = levelName;
                    info.LevelScriptName = (levelScriptName == "") ? levelBaseName : levelScriptName;
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
                isSea = strcmp(CCharString("TRUE").GetAsCharArray(), parser.ReadAsString().GetAsCharArray()) == 0;
            } else if (token == "LoadedOnPlayerProximity") {
                loadedOnPlayerProximity = strcmp(CCharString("TRUE").GetAsCharArray(), parser.ReadAsString().GetAsCharArray()) == 0;
            } else if (token == "LevelName") {
                EXPECT_QUOTE();
                levelName = GetLevelsDirectory().ToCharString() + parser.ReadAsStringUntilString(CCharString("\""));
                CAFile::SplitPath(levelName, 0, 0, &levelBaseName, 0);
                EXPECT_QUOTE();
            } else if (token == "NewRegion") {
                Regions.push_back(CRegion());
                parser.ReadAsInteger();
            } else if (token == "EndRegion") {
            } else if (token == "RegionDef") {
                EXPECT_QUOTE();
                CCharString regionDef = parser.ReadAsStringUntilString(CCharString("\""));
                EXPECT_QUOTE();
                Regions.back().SetRegionDefName(regionDef);
            } else if (token == "EnvironmentDef") {
                EXPECT_QUOTE();
                parser.ReadAsStringUntilString(CCharString("\""));
                EXPECT_QUOTE();
            } else if (token == "DisplayName" || token == "RegionName") {
                EXPECT_QUOTE();
                CCharString regionName = parser.ReadAsStringUntilString(CCharString("\""));
                EXPECT_QUOTE();
                Regions.back().Name = regionName;
            } else if (token == "NewDisplayName") {
                EXPECT_QUOTE();
                CCharString displayName = parser.ReadAsStringUntilString(CCharString("\""));
                EXPECT_QUOTE();
                Regions.back().DisplayName = displayName;
            } else if (token == "ContainsMap") {
                EXPECT_QUOTE();
                CCharString mapName = GetLevelsDirectory().ToCharString() + parser.ReadAsStringUntilString(CCharString("\""));
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
                CCharString mapName = GetLevelsDirectory().ToCharString() + parser.ReadAsStringUntilString(CCharString("\""));
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
                Regions.back().MiniMapScale = parser.ReadAsFloat();
            } else if (token == "MiniMapOffsetX") {
                Regions.back().MiniMapOffset.X = parser.ReadAsFloat();
            } else if (token == "MiniMapOffsetY") {
                Regions.back().MiniMapOffset.Y = parser.ReadAsFloat();
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
                Regions.back().WorldMapOffset.X = parser.ReadAsFloat();
            } else if (token == "WorldMapOffsetY") {
                Regions.back().WorldMapOffset.Y = parser.ReadAsFloat();
            } else if (token == "NameGraphicOffsetX") {
                Regions.back().NameGraphicOffset.X = parser.ReadAsFloat();
            } else if (token == "NameGraphicOffsetY") {
                Regions.back().NameGraphicOffset.Y = parser.ReadAsFloat();
            }
            // every statement ends with ';' -- anything else is skipped to the end of its line
            if (parser.ReadAsSymbol() != ";")
                parser.ReadAsStringUntilWhitespace();   // UNVERIFIED: retail calls 0x009BA390 (a CCharString-returning read)
        }

        if (GSaveBinaryWorld) {
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

    StartProfile("Init thing maps");
    PThingMapManager->Init(&World, Maps.size());
    StartProfile("Load GTNG");
    CWideString worldName;
    CAFile::SplitPath(file.GetPathname(), 0, 0, &worldName, 0);
    CWideString globalThingsFile = GetLevelsDirectory() + worldName + L".gtng";
    if (CAFile::PathExists(globalThingsFile)) {
        CCountedPointer<CAFile> thingFile(new CDiskFileWin32(globalThingsFile, FILE_ACCESS_READ, 2));
        std::vector<CThing*> things;
        World.GetThingManager().LoadFromFile(thingFile, 0, GetLoadableSections(), things, 0, 0);
        World.GetThingManager().InitialiseLoadedThings(things);
    }
    StartProfile("Load global things");
    if (GLoadGlobalThingsFromSingleFile) {
        CWideString directory;
        CWideString drive;
        CWideString name;
        CAFile::SplitPath(file.GetPathname(), &drive, &directory, &name, 0);
        CWideString thingsFile = drive + directory + name + L".gtg";
        LoadAllLoadableGlobalThingsFromSingleFile(thingsFile);
    } else {
        LoadLoadableGlobalThings();
    }
    DisplayProgress("Load global things end");
    Maps.DeallocateUnusedMemory();                      // 0x0051B560
    StartProfile("Load region graph");
    DisplayProgress("Load region graph");
    LoadRegionGraph(((const CCharString*)((const char*)&GetPlayerGUIDef() + 0xa94))->ToWideString());
    NProfileTimer::EndProfile(0);
}
