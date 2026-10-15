// CThingFilter_InRegion::operator()(CThing* thing) -- true when the region of the thing's current map (CWorldMap::GetRegionNumberMapIsIn) is RegionID
// Leaf: ??RCThingFilter_InRegion
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A7960, length 34 bytes (+ int3 padding).
// Callees: CWorldMap::GetRegionNumberMapIsIn @ 0x004FC190 (`__fastcall(long map)`).
// Layout: PDB CThingFilter_InRegion { CWorldMap* WorldMap; long RegionID; }; CThing::CurrentMapNumber is a short at
// retail +0x8E (PDB +0x9E less the retail CThing's 0x10 narrower head; `movsx ecx,word ptr [eax+8Eh]`).
// Shape: `return call == RegionID;` gives `sub eax,[esi+4] ; neg ; sbb ; inc`.
struct CThing {
    unsigned char _pad00[0x8e];
    short         CurrentMapNumber;                 // +0x8e
};

struct CWorldMap {
    long GetRegionNumberMapIsIn(long map);          // 0x004FC190
};

struct CThingFilter_InRegion {
    CWorldMap* WorldMap;                            // +0x00
    long       RegionID;                            // +0x04
    bool operator()(CThing* thing) const;
};

bool CThingFilter_InRegion::operator()(CThing* thing) const
{
    return WorldMap->GetRegionNumberMapIsIn(thing->CurrentMapNumber) == RegionID;
}