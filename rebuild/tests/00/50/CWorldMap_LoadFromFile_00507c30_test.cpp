// Behaviour test for 0x00507C30 CWorldMap::LoadFromFile: the row's own function text (copied unchanged from src/00507c30.cpp, the
// dead release asserts compile to nothing here) over a mock engine that logs every engine call. Run 1 (text world): MapUIDCount,
// ThingManagerUIDCount, LevelScriptName, two maps (the first placed, the second refused: placement not free, Used cleared, its
// fields kept), map 1 with MapUID 0 gets AllocateMapUID, the level box moved to MapX / MapY, IsSea / LoadedOnPlayerProximity
// parsed case-insensitively, regions (RegionDef, DisplayName / RegionName, NewDisplayName, ContainsMap / SeesMap lookups by level
// path, mini-map / world-map floats and the exit text offsets), a statement without ';' skips its line (ReadLineAsString), the
// binary save when g_WADBuildEnabled, the .gtng things file when it exists, then global things and the region graph. Run 2: the
// compiled-binary world path (.bwd, single-file global things). Run 3: a missing quote throws CGenericException.
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
static std::string gLog;
static void L(const std::string& s) { gLog += s; gLog += ";"; }
static std::string W2S(const unsigned short* w) { std::string s; while (*w) s += (char)*w++; return s; }
static std::string I2S(long v) { char b[32]; sprintf(b, "%ld", v); return b; }
static std::string F2S(float v) { char b[32]; sprintf(b, "%g", v); return b; }
struct CWideString;
struct CCharString {
    std::string S;
    CCharString() { }
    CCharString(const char* s) : S(s) { }
    CCharString(const std::string& s) : S(s) { }
    bool operator==(const char* t) const { return S == t; }
    bool operator!=(const char* t) const { return S != t; }
    bool operator==(const CCharString& t) const { return S == t.S; }
    bool EqualsCaseInsensitive(const char* t) const {
        if (S.size() != strlen(t)) return false;
        for (size_t i = 0; i < S.size(); ++i) { char a = S[i], b = t[i]; if (a >= 'a' && a <= 'z') a -= 32; if (b >= 'a' && b <= 'z') b -= 32; if (a != b) return false; }
        return true;
    }
    CCharString operator+(const CCharString& t) const { return CCharString(S + t.S); }
    CWideString ToWideString() const;
};
struct CWideString {
    std::string S;
    CWideString() { }
    CWideString(const std::string& s) : S(s) { }
    CWideString operator+(const CWideString& t) const { return CWideString(S + t.S); }
    CWideString operator+(const unsigned short* t) const { return CWideString(S + W2S(t)); }
    CCharString ToCharString() const { return CCharString(S); }
};
CWideString CCharString::ToWideString() const { return CWideString(S); }
class CExceptionBase { };
class CGenericException : public CExceptionBase { public: CGenericException() { } };
#define FABLE_RETAIL_ASSERT(c) ((void)0)
namespace NProfileTimer {
    void StartProfile(const CCharString& name, long flags) { L("profile " + name.S); }
    void EndProfile(long flags) { L("endprofile"); }
}
namespace NProgressDisplay { void DisplayProgress(const char* s, float, bool, bool) { L(std::string("progress ") + s); } }
namespace NResourceDirectoryNames { CWideString GetLevelDir() { return CWideString("Data\\Levels\\"); } }
static bool g_bUseCompiledWorldFiles, g_WADBuildEnabled, g_bUseCompiledGlobalThings, gGtngExists;
// --- file ---
struct CAFile {
    std::string Path, Body; long ReadLen;
    virtual ~CAFile() { }
    CWideString GetPathname() const { return CWideString(Path); }
    long GetLength() { return (long)Body.size(); }
    void Read(char* to, long n, long) { ReadLen = n; memcpy(to, Body.data(), n); }
    static void SplitPath(const CWideString& p, CWideString* drive, CWideString* dir, CWideString* name, CWideString*) {
        std::string s = p.S; size_t c = s.find(':'), sl = s.rfind('\\'), dot = s.rfind('.');
        if (drive) drive->S = s.substr(0, c + 1);
        if (dir) dir->S = s.substr(c + 1, sl - c);
        if (name) name->S = s.substr(sl + 1, dot - sl - 1);
    }
    static void SplitPath(const CCharString& p, CCharString*, CCharString*, CCharString* name, CCharString*) {
        std::string s = p.S; size_t sl = s.rfind('\\'), dot = s.rfind('.'); name->S = s.substr(sl + 1, dot - sl - 1);
    }
    static bool PathExists(const CWideString& p) { L("exists " + p.S); return gGtngExists; }
};
enum { FILE_ACCESS_READ = 1 };
struct CDiskFileWin32 : CAFile { CDiskFileWin32(const CWideString& p, int, int) { Path = p.S; L("open " + p.S); } };
template<class T> struct CCountedPointer { T* P; CCountedPointer() : P(0) { } CCountedPointer(T* p) : P(p) { } ~CCountedPointer() { delete P; }
    bool IsNull() const { return P == 0; } void Reset(T* p) { delete P; P = p; } T* operator->() const { return P; } };
template<class T> struct CArray : std::vector<T> { CArray() { } explicit CArray(unsigned n) : std::vector<T>(n) { }
    void DeallocateUnusedMemory() { L("dealloc"); } };
struct CParsedItem { };
// --- parser over a token list: S string, Q the quote symbol, Y other symbol, I integer, F float, U uhuge, R rest-of-line ---
struct Tok { char k; std::string s; };
static std::vector<Tok> gToks; static size_t gPos; static std::string gParsedText;
static std::string Next(char k) { if (gPos >= gToks.size()) { L("parser overrun"); return ""; } Tok t = gToks[gPos++];
    if (t.k != k && !(k == 'Y' && t.k == 'Q')) L(std::string("type ") + k + " got " + t.k + " " + t.s); return t.s; }
struct CStringParser {
    CStringParser(const char* text) { gParsedText = text; gPos = 0; }
    bool NextItemExists() { return gPos < gToks.size(); }
    CCharString ReadAsString() { return CCharString(Next('S')); }
    long ReadAsInteger() { return atol(Next('I').c_str()); }
    unsigned __int64 ReadAsUHUGE() { return (unsigned __int64)atol(Next('U').c_str()); }
    float ReadAsFloat() { return (float)atof(Next('F').c_str()); }
    CCharString ReadAsSymbol() { return CCharString(Next('Y')); }
    CCharString ReadAsStringUntilString(const char*) { return CCharString(Next('S')); }
    CCharString ReadLineAsString() { L("skipline " + Next('R')); return CCharString(); }
};
// --- world ---
struct C2DBoxI { long TLX, TLY, BRX, BRY; };
struct CMapInfo { CCharString LevelName, LevelScriptName; bool LoadedOnPlayerProximity, Used, IsSea, Placed; C2DBoxI Area; unsigned long MapUID;
    CMapInfo() : LoadedOnPlayerProximity(false), Used(false), IsSea(false), Placed(false), MapUID(0) { Area.TLX = Area.TLY = Area.BRX = Area.BRY = 0; } };
struct C2DVectorF { float X, Y; };
struct CRegion { CCharString DefName, Name, DisplayName, MiniMapGraphic; std::vector<long> Maps, VisMaps; bool AppearOnWorldMap;
    float MiniMapScale; C2DVectorF MiniMapOffset, WorldMapOffset, NameGraphicOffset; std::string Exits;
    CRegion() : AppearOnWorldMap(false), MiniMapScale(0) { MiniMapOffset.X = MiniMapOffset.Y = WorldMapOffset.X = WorldMapOffset.Y = NameGraphicOffset.X = NameGraphicOffset.Y = 0; }
    void SetRegionDefName(const CCharString& n) { DefName = n; }
    void AddMap(long m) { Maps.push_back(m); }
    void SetMiniMapRegionExitTextOffsetX(const CCharString& e, float x) { Exits += e.S + " x " + F2S(x) + ","; }
    void SetMiniMapRegionExitTextOffsetY(const CCharString& e, float y) { Exits += e.S + " y " + F2S(y) + ","; }
};
class CThing;
struct CThingManager { void SetCurrentThingUID(unsigned __int64 u) { L("uid " + I2S((long)u)); }
    void LoadFromFile(CCountedPointer<CAFile>& f, int, int sections, std::vector<CThing*>&, int, int) { L("things " + f->Path + " " + I2S(sections)); }
    void ActivateThings(const std::vector<CThing*>&) { L("activate"); } };
struct CWorld { CThingManager TM; CThingManager& GetThingManager() { return TM; } };
struct CLevelLoader { CLevelLoader(CThingManager&, struct CWorldMap&, CWorld&) { L("levelloader"); } };
struct CEngineManager { static CEngineManager& Get() { static CEngineManager e; return e; } void Cleanup() { L("cleanup"); } void AddMap(long m) { L("addmap " + I2S(m)); } };
struct CThingMapManager { void Init(CWorld*, unsigned n) { L("thingmaps " + I2S((long)n)); } };
struct CPlayerGuiDef { CCharString RegionGraphFileName; };
struct CPlayerGui { static const CPlayerGuiDef& PeekGuiDef() { static CPlayerGuiDef d; d.RegionGraphFileName = "graph.rg"; return d; } };
static std::vector<bool> gFree; static size_t gFreePos;
struct CWorldMap {
    CCountedPointer<CLevelLoader> PLevelLoader; CWorld World; CArray<CMapInfo> Maps; std::vector<CRegion> Regions; long CurrentMapUIDCount;
    CThingMapManager TMM; CThingMapManager* PThingMapManager;
    CWorldMap() : CurrentMapUIDCount(0), PThingMapManager(&TMM) { }
    void DeleteAllLevels() { L("deletelevels"); }
    void DeleteAllRegions() { L("deleteregions"); }
    void LoadWorldFromBinaryFile(const CWideString& f) { L("loadbinary " + f.S); }
    void SaveWorldToBinaryFile(const CWideString& f) { L("savebinary " + f.S); }
    void CompileLoadableGlobalThingsToSingleFile(const CWideString& f) { L("compilegtg " + f.S); }
    void LoadAllLoadableGlobalThingsFromSingleFile(const CWideString& f) { L("loadgtg " + f.S); }
    void LoadLoadableGlobalThings() { L("loadglobals"); }
    void LoadRegionGraph(const CWideString& f) { L("regiongraph " + f.S); }
    int GetLoadableSections() { return 7; }
    C2DBoxI GetLevelDimensions(const CCharString& n) { C2DBoxI b; b.TLX = 10; b.TLY = 20; b.BRX = 10 + (long)n.S.size(); b.BRY = 25; return b; }
    bool IsMapPlacementPositionFree(const C2DBoxI& b) { L("free? " + I2S(b.TLX) + "," + I2S(b.TLY) + "," + I2S(b.BRX) + "," + I2S(b.BRY)); return gFreePos < gFree.size() ? (bool)gFree[gFreePos++] : true; }
    unsigned long AllocateMapUID(long m) { L("allocuid " + I2S(m)); return 900 + m; }
    void SetMapPlacement(const C2DBoxI&, long m) { L("place " + I2S(m)); }
    void LoadFromFile(CAFile& file, long length);
};

// ---- the row (src/00507c30.cpp), unchanged ----
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

        if (g_WADBuildEnabled) {
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

static void T(char k, const char* s) { Tok t; t.k = k; t.s = s; gToks.push_back(t); }
static void Stmt() { T('Y', ";"); }
static int gFail;
static void Check(bool c, const char* what) { if (!c) { printf("FAIL: %s\n", what); ++gFail; } }
static void Reset() { gLog = ""; gToks.clear(); gPos = 0; gFree.clear(); gFreePos = 0; }
int main()
{
    // ---- run 1: text world ----
    Reset();
    g_bUseCompiledWorldFiles = false; g_WADBuildEnabled = true; g_bUseCompiledGlobalThings = false; gGtngExists = true;
    T('S', "MapUIDCount"); T('I', "42"); Stmt();
    T('S', "ThingManagerUIDCount"); T('U', "77"); Stmt();
    T('S', "LevelScriptName"); T('Q', "\""); T('S', "Script1"); T('Q', "\""); Stmt();
    T('S', "NewMap"); T('I', "1"); Stmt();
    T('S', "LevelName"); T('Q', "\""); T('S', "Oak\\Oakvale.lev"); T('Q', "\""); Stmt();
    T('S', "MapX"); T('I', "100"); Stmt();
    T('S', "MapY"); T('I', "200"); Stmt();
    T('S', "IsSea"); T('S', "true"); Stmt();
    T('S', "LoadedOnPlayerProximity"); T('S', "FALSE"); Stmt();
    T('S', "EndMap"); Stmt();
    T('S', "NewMap"); T('I', "3"); Stmt();
    T('S', "LevelName"); T('Q', "\""); T('S', "Bow.lev"); T('Q', "\""); Stmt();
    T('S', "MapUID"); T('I', "55"); Stmt();
    T('S', "MapX"); T('I', "7"); Stmt();
    T('S', "IsSea"); T('S', "no"); Stmt();
    T('S', "EndMap"); Stmt();
    T('S', "Junk"); T('Y', "="); T('R', "rest of line");
    T('S', "NewRegion"); T('I', "0"); Stmt();
    T('S', "RegionDef"); T('Q', "\""); T('S', "REGION_OAK"); T('Q', "\""); Stmt();
    T('S', "RegionName"); T('Q', "\""); T('S', "Oakvale"); T('Q', "\""); Stmt();
    T('S', "NewDisplayName"); T('Q', "\""); T('S', "OakDisp"); T('Q', "\""); Stmt();
    T('S', "ContainsMap"); T('Q', "\""); T('S', "Oak\\Oakvale.lev"); T('Q', "\""); Stmt();
    T('S', "ContainsMap"); T('Q', "\""); T('S', "Nowhere.lev"); T('Q', "\""); Stmt();
    T('S', "SeesMap"); T('Q', "\""); T('S', "Bow.lev"); T('Q', "\""); Stmt();
    T('S', "SeesMap"); T('Q', "\""); T('S', "Oak\\Oakvale.lev"); T('Q', "\""); Stmt();
    T('S', "AppearOnWorldMap"); Stmt();
    T('S', "MiniMapGraphic"); T('S', "gfx"); Stmt();
    T('S', "MiniMapScale"); T('F', "1.5"); Stmt();
    T('S', "MiniMapOffsetX"); T('F', "2.5"); Stmt();
    T('S', "MiniMapOffsetY"); T('F', "3.5"); Stmt();
    T('S', "MiniMapRegionExitTextOffsetX"); T('Y', "("); T('S', "ExitA"); T('Y', ","); T('F', "4"); Stmt();
    T('S', "MiniMapRegionExitTextOffsetY"); T('Y', "("); T('S', "ExitB"); T('Y', ","); T('F', "5"); Stmt();
    T('S', "WorldMapOffsetX"); T('F', "6"); Stmt();
    T('S', "WorldMapOffsetY"); T('F', "7"); Stmt();
    T('S', "NameGraphicOffsetX"); T('F', "8"); Stmt();
    T('S', "NameGraphicOffsetY"); T('F', "9"); Stmt();
    T('S', "NewRegion"); T('I', "1"); Stmt();
    T('S', "DisplayName"); T('Q', "\""); T('S', "Second"); T('Q', "\""); Stmt();
    T('S', "EndRegion"); Stmt();
    gFree.push_back(true); gFree.push_back(false);
    {
        CWorldMap map; CAFile file; file.Path = "C:\\Data\\Levels\\FinalAlbion.wld"; file.Body = "abcdef"; file.ReadLen = 0;
        map.LoadFromFile(file, -1);
        Check(gPos == gToks.size(), "all tokens read");
        Check(file.ReadLen == 6 && gParsedText == "abcdef", "length -1 reads the whole file, text terminated");
        Check(map.CurrentMapUIDCount == 42, "MapUIDCount");
        Check(map.Maps.size() == 4, "maps resized to newMap + 1");
        const CMapInfo& m1 = map.Maps[1];
        Check(m1.LevelName.S == "Data\\Levels\\Oak\\Oakvale.lev", "level name = levels dir + name");
        Check(m1.LevelScriptName.S == "Script1", "level script name");
        Check(m1.Used && m1.Placed && m1.IsSea && !m1.LoadedOnPlayerProximity, "map 1 flags");
        Check(m1.Area.TLX == 100 && m1.Area.TLY == 200 && m1.Area.BRX == 100 + 27 && m1.Area.BRY == 205, "map 1 box moved");
        Check(m1.MapUID == 901, "map 1 uid allocated");
        const CMapInfo& m3 = map.Maps[3];
        Check(!m3.Used && m3.Placed && !m3.IsSea && !m3.LoadedOnPlayerProximity, "map 3 refused: Used cleared");
        Check(m3.LevelScriptName.S == "Script1", "map 3 script name");
        Check(m3.MapUID == 55 && m3.Area.TLX == 7 && m3.Area.TLY == -1, "map 3 uid and position (MapY reset to -1)");
        Check(map.Regions.size() == 2, "two regions");
        const CRegion& r = map.Regions[0];
        Check(r.DefName.S == "REGION_OAK" && r.Name.S == "Oakvale" && r.DisplayName.S == "OakDisp", "region names");
        Check(r.Maps.size() == 2 && r.Maps[0] == 1 && r.Maps[1] == 4, "ContainsMap: index or size when not found");
        Check(r.VisMaps.size() == 2 && r.VisMaps[0] == 3 && r.VisMaps[1] == 1, "SeesMap");
        Check(r.AppearOnWorldMap && r.MiniMapGraphic.S == "gfx" && r.MiniMapScale == 1.5f, "mini map graphic / scale");
        Check(r.MiniMapOffset.X == 2.5f && r.MiniMapOffset.Y == 3.5f && r.WorldMapOffset.X == 6 && r.WorldMapOffset.Y == 7, "offsets");
        Check(r.NameGraphicOffset.X == 8 && r.NameGraphicOffset.Y == 9 && r.Exits == "ExitA x 4,ExitB y 5,", "name graphic / exits");
        Check(map.Regions[1].Name.S == "Second", "DisplayName sets Name");
    }
    const char* expect1 = "levelloader;deletelevels;deleteregions;cleanup;profile Load .wld file;uid 1;uid 77;progress Loading maps;"
        "free? 100,200,127,205;allocuid 1;place 1;addmap 1;progress Loading maps;free? 7,-1,26,4;skipline rest of line;"
        "savebinary C:\\Data\\Levels\\FinalAlbion.bwd;compilegtg C:\\Data\\Levels\\FinalAlbion.gtg;profile Init thing maps;thingmaps 4;"
        "profile Load GTNG;exists Data\\Levels\\FinalAlbion.gtng;open Data\\Levels\\FinalAlbion.gtng;things Data\\Levels\\FinalAlbion.gtng 7;"
        "activate;profile Load global things;loadglobals;progress Load global things end;dealloc;profile Load region graph;"
        "progress Load region graph;regiongraph graph.rg;endprofile;";
    Check(gLog == expect1, "run 1 call log");
    if (gLog != expect1) printf("%s\n", gLog.c_str());
    // ---- run 2: compiled binary world ----
    Reset();
    g_bUseCompiledWorldFiles = true; g_WADBuildEnabled = false; g_bUseCompiledGlobalThings = true; gGtngExists = false;
    {
        CWorldMap map; map.PLevelLoader.Reset(new CLevelLoader(map.World.GetThingManager(), map, map.World)); gLog = "";
        CAFile file; file.Path = "D:\\W\\Albion.wld"; file.Body = "x";
        map.LoadFromFile(file, 1);
        Check(map.CurrentMapUIDCount == 0, "binary path does not parse");
    }
    const char* expect2 = "deletelevels;deleteregions;cleanup;profile Load .wld file;loadbinary D:\\W\\Albion.bwd;profile Init thing maps;"
        "thingmaps 0;profile Load GTNG;exists Data\\Levels\\Albion.gtng;profile Load global things;loadgtg D:\\W\\Albion.gtg;"
        "progress Load global things end;dealloc;profile Load region graph;progress Load region graph;regiongraph graph.rg;endprofile;";
    Check(gLog == expect2, "run 2 call log");
    if (gLog != expect2) printf("%s\n", gLog.c_str());
    // ---- run 3: a missing quote throws ----
    Reset();
    g_bUseCompiledWorldFiles = false; g_WADBuildEnabled = false;
    T('S', "LevelName"); T('Y', "x"); T('S', "a.lev"); T('Q', "\""); Stmt();
    bool thrown = false;
    try { CWorldMap map; CAFile file; file.Path = "C:\\a\\b.wld"; file.Body = "q"; map.LoadFromFile(file, -1); }
    catch (CGenericException&) { thrown = true; }
    Check(thrown && gPos == 2, "missing quote throws CGenericException");
    if (!gFail) printf("00507c30_TEST PASS\n");
    return gFail;
}