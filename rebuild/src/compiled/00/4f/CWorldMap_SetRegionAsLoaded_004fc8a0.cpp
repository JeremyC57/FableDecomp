// CWorldMap::SetRegionAsLoaded(long, const CCharString&) -- record the loaded region and initialise the mini-map for it, bracketed by two loading-progress ticks
// Retail: Fable.exe @ 0x004FC8A0, 134 bytes, `ret 8`. PDB ?SetRegionAsLoaded@CWorldMap@@QAEXJABVCCharString@@@Z
// Callees: CCharString(const char*, long) 0x0099EBF0 ("SetRegionAsLoaded: Initialise MiniMap" 0x0124444C,
// "... End" 0x01244420), __fastcall DisplayProgress(const CCharString&, float, bool, bool) 0x009E9F40, ~CCharString
// 0x0099EAE0 (manifest label "_Cons_val<...>" is a byte twin), CPlayerGui::GetPMiniMapDisplay() 0x00437CE0 (label
// "CVertexBufferWin32::DoSizeof" is a byte twin) on g_pPlayerGui 0x013B8790, CMiniMapDisplay::InitialiseOnRegionLoad
// (const CRegion&) 0x0082BA00. The name argument is unused.
// Retail CWorldMap (the engine header engine/CWorldMap.h is the PDB layout with 16-byte containers; retail STLport
// vectors / CArrays are 12 bytes and the map member is 12 bytes, so everything after +0x14 sits lower):
//   +0x20 Maps (CArray<CMapInfo>: start +0x20, finish +0x24; PDB +0x24), +0x2C Regions (vector<CRegion>, 0x58-byte
//   elements; PDB +0x34), +0x38 RegionTime, +0x44 PRegionGraph, +0x4C SoundThemesDisabled, +0x50 Granularity,
//   +0x54 GranularityShift (PDB +0x64), +0x58 MapPlacementPitch (PDB +0x68), +0x5C Area {TLX, TLY, BRX, BRY}
//   (PDB +0x6C), +0x6C MapPlacement (vector<unsigned short>, PDB +0x7C), +0x9C LoadedRegion (PDB +0xBC).
// CMapInfo (0x48 bytes, PDB = retail): +0x00 Area, +0x10 PMap, +0x18 LevelName, +0x1C LevelScriptName,
// +0x20 MapUID, +0x24 Used ... +0x40 IsSea.
// CRegion (retail 0x58 bytes; PDB 100 with 16-byte vectors): +0x00 Maps (vector<long>), +0x0C VisMaps, +0x18 Name
// (PDB +0x20), +0x1C DisplayName, +0x20 RegionDefName, ...
template <class T> struct CRetailVector {
    T* Start;                                   // +0x00
    T* Finish;                                  // +0x04
    T* EndOfStorage;                            // +0x08
    unsigned long size() const { return Finish - Start; }
    const T* begin() const { return Start; }
    const T& operator[](unsigned long i) const { return *(begin() + i); }
};
// Shape: as the landed CThingManager::ActivateAfterLoading 0x0051E5A0: each progress text is a named CCharString in its
// own block; the region reference is formed from the re-read LoadedRegion before the GUI getter.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40

struct CRegion { unsigned char _body[0x58]; };
struct CMiniMapDisplay { void InitialiseOnRegionLoad(const CRegion& region); };   // 0x0082BA00
struct CPlayerGui { CMiniMapDisplay* GetPMiniMapDisplay(); };                     // 0x00437CE0
extern CPlayerGui* g_pPlayerGui;                // 0x013B8790

struct CWorldMap {
    unsigned char          _pad[0x2c];
    CRetailVector<CRegion> Regions;             // +0x2C
    unsigned char          _pad38[0x64];
    long                   LoadedRegion;        // +0x9C (PDB +0xBC)
    void SetRegionAsLoaded(long region, const CCharString& name);
};

void CWorldMap::SetRegionAsLoaded(long region, const CCharString& name)
{
    LoadedRegion = region;
    {
        CCharString text("SetRegionAsLoaded: Initialise MiniMap");
        DisplayProgress(text, -1.0f, false, false);
    }
    g_pPlayerGui->GetPMiniMapDisplay()->InitialiseOnRegionLoad(Regions[LoadedRegion]);
    {
        CCharString text("SetRegionAsLoaded: Initialise MiniMap End");
        DisplayProgress(text, -1.0f, false, false);
    }
}