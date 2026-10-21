// CThingManager::AllocateNewThingUID(long mapNumber, bool global, bool levelLess) -- hand out the next 64-bit thing UID (map-specific or global counter)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051E100, length 266 bytes (+ int3 padding), `ret 0Ch`, result in edx:eax.
// Callees: CWorldMap map-UID prefix getter @ 0x004FCE00 (catalog "CEditWorldMap::GetEditMap" is a propagated byte twin),
// CWorldMap::AllocateMapThingUID(long) @ 0x004FB330, __allmul @ 0x00BFEB50 (the prefix * 2^40 product).
// Caller: CThing::CreateUID 0x004C7FF0 (landed; passes the class-info "global" flag and the def's LevelLess bit).
// Layout (retail, below Ego_r.pdb: 12-byte vectors, 16-byte map): WorldMap +0x28, CurrentUniqueID +0x68,
// CurrentLocalThingUniqueID +0x70, UseMapSpecificUIDs +0x98 (PDB +0x34/+0x80/+0x88/+0xb4). Modelled locally because
// no engine header exists for CThingManager and the PDB offsets are not the retail ones.
// Shape: the zero result is preloaded in edi:ebx; a global id is the post-incremented counter plus the 0xFFFFFD00 high
// tag, a plain one the other counter plus 0xFFFFFF00; with map-specific UIDs a positive map number builds the id from
// the map's prefix (shifted 40 bits through __allmul) plus the map's own counter. Matching details: the counter step is
// `uid = counter++; uid += tag;` as two statements (one expression schedules the tag add before the stores); the
// prefix is an `unsigned __int64` local (the 8-byte frame, only its low word stored, `xor eax,eax` for the high);
// the world map is re-read into a named local after the first call (`mov esi,[esi+28h] ; mov ecx,esi`).
#pragma pack(push, 4)
class CWorldMap {
public:
    unsigned long    GetMapUIDPrefix(long mapNumber);          // 0x004FCE00
    unsigned __int64 AllocateMapThingUID(long mapNumber);      // 0x004FB330
};

class CThingManager {
public:
    void*            __vftable;                                // +0x00
    unsigned char    _pad04[0x28 - 0x04];
    CWorldMap*       WorldMap;                                 // +0x28
    unsigned char    _pad2c[0x68 - 0x2c];
    unsigned __int64 CurrentUniqueID;                          // +0x68
    unsigned __int64 CurrentLocalThingUniqueID;                // +0x70
    unsigned char    _pad78[0x98 - 0x78];
    bool             UseMapSpecificUIDs;                       // +0x98
    unsigned __int64 AllocateNewThingUID(long mapNumber, bool global, bool levelLess);
};
#pragma pack(pop)

unsigned __int64 CThingManager::AllocateNewThingUID(long mapNumber, bool global, bool levelLess)
{
    unsigned __int64 uid = 0;
    if (UseMapSpecificUIDs) {
        if (global)
            { uid = CurrentLocalThingUniqueID++; uid += 0xFFFFFD0000000000ui64; }
        if (levelLess)
            return 0;
        if (mapNumber > 0) {
            unsigned __int64 prefix = WorldMap->GetMapUIDPrefix(mapNumber);
            CWorldMap* worldMap = WorldMap;
            uid = worldMap->AllocateMapThingUID(mapNumber) + prefix * 0x10000000000ui64;
        }
    } else {
        if (levelLess)
            return 0;
        if (global)
            { uid = CurrentLocalThingUniqueID++; uid += 0xFFFFFD0000000000ui64; }
        else
            { uid = CurrentUniqueID++; uid += 0xFFFFFF0000000000ui64; }
    }
    return uid;
}