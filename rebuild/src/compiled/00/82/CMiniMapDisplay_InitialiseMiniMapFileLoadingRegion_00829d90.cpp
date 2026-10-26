// CMiniMapDisplay::InitialiseMiniMapFileLoading_Region(const CWorldRegion* region, const CCharString& levelName, bool regenerate)
//   -- reset the region minimap, make sure both minimap directories exist, size it for the region on the world map, then
//   build "<region name>.tga" in the cache and data directories and the level's "Data\\Levels\\<level>.wld" time stamp:
//   a data-directory TGA (else a cache-directory TGA) is loaded unless `regenerate` is set and the .wld is newer, in which
//   case the minimap is generated into the cache path; with neither file it is generated only when `regenerate` is set;
//   a loaded minimap builds its texture, which is kept at +0x50, and frees its data
// Leaf: InitialiseMiniMapFileLoading_Region
// Retail: Fable.exe (TLC, VC7.1) @ 0x00829D90, 648 bytes (`ret 0Ch`).
// Callees: CRegionMinimap 0x0066DBB0 Initialise / 0x0066CC40 InitialiseRegionMinimapSize / 0x0066CA80 LoadRegionMinimapTGAFile /
//   0x0066DC50 generate-and-save (manifest "CParticleSimulation::Clear") / 0x0066C610 is-loaded (manifest
//   "C3DMeshInfo::HasPhysicsMesh") / 0x0066CB00 InitialiseTexture / 0x0066CAF0 texture getter (manifest "CModeBlock::GetAnimFilter")
//   / 0x0066C7A0 free data (manifest "CRenderTarget::DetachTarget"); PathExists 0x00999230 and CreateDirectory 0x009994A0
//   (__fastcall, const CWideString&); the world's slot 13 GetWorldMap via the main game component 0x013B86A0 (+0x24);
//   CCharString copy 0x0099EC30, operator+(const CCharString&, const char*) 0x0099F600 (__fastcall, manifest "AppendCString"),
//   ToWideString 0x0099E480, ~CCharString 0x0099EAE0, operator+(CWideString, CWideString) 0x0099BE70 and (CWideString,
//   const wchar_t*) 0x0099BF30 (__fastcall), CWideString(const wchar_t*) 0x0099B6B0, ~CWideString 0x0099B510,
//   CDateAndTime(7 longs) 0x009E9A70, GetFileModifiedTime 0x009992A0 (__fastcall, CDateAndTime by hidden pointer),
//   CDateAndTime compare 0x009E9BE0 (manifest "operator<", but it returns this > other: `setg` on the first differing field).
// Data: the minimap directories 0x013BAF38 (data, tried first) / 0x013BAF3C (cache, generation target) (CWideString globals),
//   ".tga" 0x01272588, L"Data\\Levels\\" 0x0122F3B4, L".wld" 0x01238508.
// Shape: the TGA name goes through an inline helper taking the CCharString by value (the GetName copy and the appended
//   string die right after ToWideString, whose result pointer is pushed after the append); the two time locals are
//   declared cache-first and assigned from GetFileModifiedTime temporaries (rep movsd), the .wld time is initialised directly.
class CWideString { public:
    void* Data;
    CWideString(const unsigned short* text);            // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
};
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);      // 0x0099BE70
CWideString __fastcall operator+(const CWideString& a, const unsigned short* b);   // 0x0099BF30

class CCharString { public:
    void* Data;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CWideString ToWideString() const;                   // 0x0099E480
};
CCharString __fastcall operator+(const CCharString& a, const char* b);   // 0x0099F600

bool __fastcall PathExists(const CWideString& path);                     // 0x00999230
void __fastcall CreateDirectory(const CWideString& path);                // 0x009994A0

class CDateAndTime { public:
    long Fields[7];
    CDateAndTime(long year = 0, long month = 0, long day = 0, long hour = 0, long minute = 0, long second = 0,
                 long millisecond = 0);                 // 0x009E9A70
    bool operator>(const CDateAndTime& other) const;    // 0x009E9BE0
};
CDateAndTime __fastcall GetFileModifiedTime(const CWideString& path);    // 0x009992A0

extern CWideString g_MiniMapDataDirectory;              // 0x013BAF38
extern CWideString g_MiniMapCacheDirectory;             // 0x013BAF3C

class CWorldMap;
class CWorld { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12();
    virtual CWorldMap* GetWorldMap();                   // slot 13
};
class CMainGameComponent { public:
    unsigned char _pad[0x24];
    CWorld* World;                                      // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0

static inline CWideString ToWide(CCharString text)
{
    return text.ToWideString();
}

struct CWorldRegion {
    unsigned char _pad[0x18];
    CCharString Name;                                   // +0x18
    CCharString GetName() const { return Name; }
};

class CTexture;
struct CRegionMinimap {
    void Initialise();                                                      // 0x0066DBB0
    void InitialiseRegionMinimapSize(CWorldMap* map, const CWorldRegion* region);   // 0x0066CC40
    void LoadRegionMinimapTGAFile(const CWideString& path);                 // 0x0066CA80
    void GenerateAndSave(CWorldMap* map, const CWideString& path);          // 0x0066DC50
    bool IsLoaded() const;                                                  // 0x0066C610
    void InitialiseTexture();                                               // 0x0066CB00
    CTexture* GetTexture() const;                                           // 0x0066CAF0
    void FreeData();                                                        // 0x0066C7A0
};

class CMiniMapDisplay { public:
    unsigned char _pad[0x50];
    CTexture* RegionTexture;                            // +0x50
    CRegionMinimap RegionMinimap;                       // +0x54
    void InitialiseMiniMapFileLoading_Region(const CWorldRegion* region, const CCharString& levelName, bool regenerate);
};

void CMiniMapDisplay::InitialiseMiniMapFileLoading_Region(const CWorldRegion* region, const CCharString& levelName,
                                                          bool regenerate)
{
    RegionMinimap.Initialise();
    if (!PathExists(g_MiniMapDataDirectory))
        CreateDirectory(g_MiniMapDataDirectory);
    if (!PathExists(g_MiniMapCacheDirectory))
        CreateDirectory(g_MiniMapCacheDirectory);
    CWorldMap* worldMap = g_pMainGameComponent->World->GetWorldMap();
    RegionMinimap.InitialiseRegionMinimapSize(worldMap, region);
    CWideString tgaName = ToWide(region->GetName() + ".tga");
    CWideString cachePath = g_MiniMapCacheDirectory + tgaName;
    CWideString dataPath = g_MiniMapDataDirectory + tgaName;
    CDateAndTime cacheTime;
    CDateAndTime dataTime;
    CWideString levelsDir(L"Data\\Levels\\");
    CDateAndTime wldTime = GetFileModifiedTime(levelsDir + levelName.ToWideString() + L".wld");
    if (PathExists(dataPath)) {
        dataTime = GetFileModifiedTime(dataPath);
        if (regenerate && wldTime > dataTime)
            RegionMinimap.GenerateAndSave(worldMap, cachePath);
        else
            RegionMinimap.LoadRegionMinimapTGAFile(dataPath);
    } else if (PathExists(cachePath)) {
        cacheTime = GetFileModifiedTime(cachePath);
        if (regenerate && wldTime > cacheTime)
            RegionMinimap.GenerateAndSave(worldMap, cachePath);
        else
            RegionMinimap.LoadRegionMinimapTGAFile(cachePath);
    } else if (regenerate)
        RegionMinimap.GenerateAndSave(worldMap, cachePath);
    if (RegionMinimap.IsLoaded()) {
        RegionMinimap.InitialiseTexture();
        RegionTexture = RegionMinimap.GetTexture();
        RegionMinimap.FreeData();
    }
}