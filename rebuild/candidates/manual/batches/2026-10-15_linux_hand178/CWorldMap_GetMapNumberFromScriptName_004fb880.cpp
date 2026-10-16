// CWorldMap::GetMapNumberFromScriptName(const CCharString&) const -- index of the map whose level script name matches (case-insensitive), 0 when none
// Retail: Fable.exe @ 0x004FB880, 150 bytes. PDB ?GetMapNumberFromScriptName@CWorldMap@@QBEJABVCCharString@@@Z
// Callee: _stricmp 0x00BFEBA8 (CRT thunk). Globals: the CCharString empty text "" 0x0129AAF4.
// Retail CWorldMap (the engine header engine/CWorldMap.h is the PDB layout with 16-byte containers; retail STLport
// vectors / CArrays are 12 bytes and the map member is 12 bytes, so everything after +0x14 sits lower):
//   +0x20 Maps (CArray<CMapInfo>: start +0x20, finish +0x24; PDB +0x24), +0x2C Regions (vector<CRegion>, 0x58-byte
//   elements; PDB +0x34), +0x38 RegionTime, +0x44 PRegionGraph, +0x4C SoundThemesDisabled, +0x50 Granularity,
//   +0x54 GranularityShift (PDB +0x64), +0x58 MapPlacementPitch (PDB +0x68), +0x5C Area {TLX, TLY, BRX, BRY}
//   (PDB +0x6C), +0x6C MapPlacement (vector<unsigned short>, PDB +0x7C), +0x9C LoadedRegion (PDB +0xBC).
// CMapInfo (0x48 bytes, PDB = retail): +0x00 Area, +0x10 PMap, +0x18 LevelName, +0x1C LevelScriptName,
// +0x20 MapUID, +0x24 Used ... +0x40 IsSea.
// Shape: map 0 is never matched; size() is the CArray's signed (finish - start) / 0x48 re-evaluated every pass; both
// strings go through the null-first inline text accessor (name first, it is the second argument); the element's
// script name is bound to a named reference (an inline expression turns the name's null test into a select).
extern "C" int __cdecl _stricmp(const char* a, const char* b);   // 0x00BFEBA8
extern char g_CCharStringEmptyText[];      // 0x0129AAF4 ("")

struct CCharStringData { char* Text; };
struct CCharString {
    CCharStringData* PStringData;
    const char* GetText() const { if (PStringData == 0) return g_CCharStringEmptyText; return PStringData->Text; }
};

struct CMapInfo {
    long          Area[4];                      // +0x00 C2DBoxI
    void*         PMap[2];                      // +0x10 CCountedPointer<CMap>
    CCharString   LevelName;                    // +0x18
    CCharString   LevelScriptName;              // +0x1C
    unsigned long MapUID;                       // +0x20
    unsigned char _rest[0x24];                  // +0x24 .. +0x48
};

template <class T> struct CArray {
    T* Start;                                   // +0x00
    T* Finish;                                  // +0x04
    T* EndOfStorage;                            // +0x08
    unsigned long size() const { return Finish - Start; }
    const T& operator[](unsigned long i) const { return Start[i]; }
};

struct CWorldMap {
    unsigned char    _pad[0x20];
    CArray<CMapInfo> Maps;                      // +0x20
    long GetMapNumberFromScriptName(const CCharString& name) const;
};

long CWorldMap::GetMapNumberFromScriptName(const CCharString& name) const
{
    for (unsigned long i = 1; i < Maps.size(); ++i)
    {
        const CCharString& scriptName = Maps[i].LevelScriptName;
        if (_stricmp(scriptName.GetText(), name.GetText()) == 0)
            return i;
    }
    return 0;
}
