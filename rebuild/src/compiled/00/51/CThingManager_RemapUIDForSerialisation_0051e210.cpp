// CThingManager::RemapUIDForSerialisation(bool saving, bool levelLess, long level, unsigned __int64 uid) -- translate a thing UID's 24-bit region field between the level's region and the "current level" marker 0xfffffe for saving/loading (0xffffff global UIDs pass, 0xfffffd and level-less UIDs become 0)
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
struct CRegionLookup_004fce00 {
    long GetRegionForLevel(long level);        // 0x004FCE00
};

unsigned __int64 __stdcall MakeUID_0051d990(unsigned __int64 region, unsigned __int64 id);   // 0x0051D990

class CThingManager {
public:
    unsigned char _pad00[0x28];
    CRegionLookup_004fce00* m_pRegionLookup;    // +0x28
    unsigned char _pad2c[0x9a - 0x2c];
    bool m_uidRemapDisabled;                    // +0x9a
    unsigned __int64 RemapUIDForSerialisation(bool saving, bool levelLess, long level, unsigned __int64 uid);
};

unsigned __int64 CThingManager::RemapUIDForSerialisation(bool saving, bool levelLess, long level, unsigned __int64 uid)
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
        if (region != (unsigned long)m_pRegionLookup->GetRegionForLevel(level))
            return uid;
        return MakeUID_0051d990(0xfffffe, uid & 0xFFFFFFFFFFui64);
    }
    long levelRegion = m_pRegionLookup->GetRegionForLevel(level);
    if (region == 0xfffffe)
        return MakeUID_0051d990((unsigned long)levelRegion, uid & 0xFFFFFFFFFFui64);
    return uid;
}