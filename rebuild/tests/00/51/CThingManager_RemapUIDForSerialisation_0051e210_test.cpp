// behaviour test for CThingManager::RemapUIDForSerialisation (0x0051E210): region field remapping between the level region and the 0xfffffe marker
#include <cstdio>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#define ID_MASK 0xFFFFFFFFFFui64
#else
typedef unsigned long long u64;
#define ID_MASK 0xFFFFFFFFFFull
#endif
static u64 UID(unsigned long region, unsigned long id) { return ((u64)region << 40) | id; }
struct CRegionLookup_004fce00 { long m_region; long GetRegionForLevel(long) { return m_region; } };
static u64 MakeUID_0051d990(u64 region, u64 id) { return region * ((u64)1 << 40) + id; }
class CThingManager { public:
    CRegionLookup_004fce00* m_pRegionLookup; bool m_uidRemapDisabled;
    u64 RemapUIDForSerialisation(bool saving, bool levelLess, long level, u64 uid)
    {
        if (m_uidRemapDisabled) return uid;
        unsigned long region = (unsigned long)((uid >> 40) & 0xffffff);
        if (region == 0xfffffd) return 0;
        if (region == 0xffffff) return uid;
        if (levelLess) return 0;
        if (saving) {
            if (region != (unsigned long)m_pRegionLookup->GetRegionForLevel(level)) return uid;
            return MakeUID_0051d990(0xfffffe, uid & ID_MASK);
        }
        long levelRegion = m_pRegionLookup->GetRegionForLevel(level);
        if (region == 0xfffffe) return MakeUID_0051d990((unsigned long)levelRegion, uid & ID_MASK);
        return uid;
    } };
int main()
{
    CRegionLookup_004fce00 lookup; lookup.m_region = 42;
    CThingManager m; m.m_pRegionLookup = &lookup; m.m_uidRemapDisabled = false;
    int bad = 0;
    u64 inLevel = UID(42, 0x1234) | ((u64)0x7f << 32);              // id uses the low 40 bits
    if (m.RemapUIDForSerialisation(true, false, 3, inLevel) != (UID(0xfffffe, 0x1234) | ((u64)0x7f << 32))) bad = 1;
    if (m.RemapUIDForSerialisation(true, false, 3, UID(41, 5)) != UID(41, 5)) bad = 1;          // other region: unchanged
    if (m.RemapUIDForSerialisation(false, false, 3, UID(0xfffffe, 9)) != UID(42, 9)) bad = 1;  // load: marker -> region
    if (m.RemapUIDForSerialisation(false, false, 3, UID(42, 9)) != UID(42, 9)) bad = 1;
    if (m.RemapUIDForSerialisation(true, false, 3, UID(0xffffff, 1)) != UID(0xffffff, 1)) bad = 1;   // global
    if (m.RemapUIDForSerialisation(true, false, 3, UID(0xfffffd, 1)) != 0) bad = 1;
    if (m.RemapUIDForSerialisation(true, true, 3, inLevel) != 0) bad = 1;                       // level-less
    m.m_uidRemapDisabled = true; if (m.RemapUIDForSerialisation(true, true, 3, inLevel) != inLevel) bad = 1;
    if (bad) { std::printf("0051e210_TEST FAIL\n"); return 1; }
    std::printf("0051e210_TEST PASS\n");
    return 0;
}