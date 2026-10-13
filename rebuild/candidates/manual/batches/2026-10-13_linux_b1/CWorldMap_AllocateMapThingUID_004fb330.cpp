// CWorldMap::AllocateMapThingUID(long)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004FB330, oracle length 20 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov edx,[esp+4] ; mov eax,[ecx] ; push edx ; call [eax+48h] ; mov ecx,eax ; call CMap::AllocateMapThingUID ; ret 4
// Virtual slot 18 (+0x48) maps a map/region index to its CMap*; the UID is then allocated by
// CMap::AllocateMapThingUID @ 0x0081E780 (__thiscall, returns unsigned __int64 in edx:eax, which
// passes straight through). engine/CWorldMap.h supplies the typed `this`; the virtual slot is
// reached through a vtable-shaped view because the PDB export does not name vtable slots.
#include "engine/CWorldMap.h"

struct CMap {
    unsigned __int64 AllocateMapThingUID();     // 0x0081E780
};

struct CWorldMapVirtuals {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44();
    virtual CMap* GetMap(long mapIndex);   // slot 18 (+0x48)
};

struct CWorldMap_Methods : CWorldMap {
    unsigned __int64 AllocateMapThingUID(long mapIndex);
};

unsigned __int64 CWorldMap_Methods::AllocateMapThingUID(long mapIndex)
{
    CMap* map = reinterpret_cast<CWorldMapVirtuals*>(this)->GetMap(mapIndex);
    return map->AllocateMapThingUID();
}
