// CThingManager::RemapUIDForSerialisation(bool saving, bool levelLess, unsigned long level, unsigned __int64 uid) const -- translate a thing UID's 24-bit region field between the level's region and the "current level" marker 0xfffffe for saving/loading (0xffffff global UIDs pass, 0xfffffd and level-less UIDs become 0)
// Leaf: ?RemapUIDForSerialisation@CThingManager@@QBE_K_N0K_K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051E210, length 174 bytes, `ret 14h`, unsigned __int64 in edx:eax.
// Callees: the level-to-region lookup @ 0x004FCE00 on the manager's +0x28 member (manifest label
//   "CEditWorldMap::GetEditMap" is propagated; name unproven), the UID builder @ 0x0051D990 (`__stdcall`, `ret 10h`:
//   region * 2^40 + id through __allmul; manifest "Multiply64Bit").
// Identity: engine_api.tsv gives CThingManager::RemapUIDForSerialisation(bool, bool, ulong, __uint64) returning __uint64;
//   the landed callers' declaration (saving, levelLess, level, uid) is kept.
// Layout: retail CThingManager +0x28 the region lookup object, +0x9a the "UIDs are not remapped" flag (names unproven).
// Shape: the region is `(uid >> 40) & 0xffffff` (VC shifts the high dword by 8 and keeps the mask); each arm
//   calls the lookup itself (the loading arm into a local before its marker test); the builder takes the region and the
//   low 40 bits of the UID.
// NF108 (2026-11-19, q4250): the +0x28 member is CThingManager::WorldMap (the landed AllocateNewThingUID 0x0051E100 model) and the
//   lookup 0x004FCE00 is ?GetMapUID@CWorldMap@@QBEKJ@Z (Ego_r.pdb 0x004D03C0 + FableWin.pdb 0x01C94F90): the local
//   CRegionLookup_004fce00::GetRegionForLevel stand-in is replaced by the shared engine/CWorldMap.h.
// NF316 (2026-11-23): the PDB signature (Ego_r + FableWin ?RemapUIDForSerialisation@CThingManager@@QBE_K_N0K_K@Z: const, `unsigned long level`, as
//   engine/CThingManager.h declares it); the body only forwards `level` to CWorldMap::GetMapUID(long) (no code); every landed binder of the old
//   QAE `long` spelling moves in the same landing (NF304 Q6).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CWORLDMAP
#include "fable_string.h"
#include "engine/CMapInfo.h"
#include "engine/CWorldMap.h"

unsigned __int64 __fastcall GFCalcThingUID(unsigned __int64 region, unsigned __int64 id);   // 0x0051D990

class CThingManager {
public:
    unsigned char _pad00[0x28];
    CWorldMap* WorldMap;                        // +0x28
    unsigned char _pad2c[0x9a - 0x2c];
    bool m_uidRemapDisabled;                    // +0x9a
    unsigned __int64 RemapUIDForSerialisation(bool saving, bool levelLess, unsigned long level, unsigned __int64 uid) const;
};

unsigned __int64 CThingManager::RemapUIDForSerialisation(bool saving, bool levelLess, unsigned long level, unsigned __int64 uid) const
{
    if (m_uidRemapDisabled)
        return uid;
    unsigned long region = (unsigned long)((uid >> 40) & 0xffffff);
    if (region == 0xfffffd)
        return 0;
    if (region == 0xffffff)
        return uid;
    if (levelLess)
        return 0;
    if (saving) {
        if (region != (unsigned long)WorldMap->GetMapUID(level))
            return uid;
        return GFCalcThingUID(0xfffffe, uid & 0xFFFFFFFFFFui64);
    }
    long levelRegion = WorldMap->GetMapUID(level);
    if (region == 0xfffffe)
        return GFCalcThingUID((unsigned long)levelRegion, uid & 0xFFFFFFFFFFui64);
    return uid;
}