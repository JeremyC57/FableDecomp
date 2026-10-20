// behaviour test for CThingFilter_IsOpinionSourceWithFreshDeed::operator() (0x00960FC0): deed presence, staleness (id + 64-bit uid), awareness routes
#include <cstdio>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
struct CTCBase;
struct CInterfaceMap { struct SEntry { long type; CTCBase* component; }; SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) const { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; } };
class CThing; class CThingAICreature; class COpinionDeed;
struct CTCOpinionSource { bool UseNavLineOfSight; bool AlwaysAwareOf; bool ReactNonstopFlag; COpinionDeed* VirtualDeed; long VirtualDeedID; };
class CThing { public:
    u64 UniqueID; unsigned long UsingComponents[9]; CInterfaceMap Components;
    bool HasTC(long type) const { if (UsingComponents[type >> 5] & (1UL << (type & 31))) return true; return false; }
    template<class T> T* GetTC(long interfaceType) const
    { long type = interfaceType; const CInterfaceMap* map = &Components; CInterfaceMap::SEntry* it = map->LowerBound(&type);
      if (it == map->last || interfaceType < it->type) it = map->last; return (T*)it->component; } };
static int g_aware = 0, g_see = 0; static bool g_awareResult = false, g_seeResult = false; static bool g_lastFlag = false;
static bool IsAwareOfThingImplementation(CThingAICreature*, CThing*, bool flag) { ++g_aware; g_lastFlag = flag; return g_awareResult; }
static bool CanCreatureSeeCreature(const CThing*, const CThing*, bool flag) { ++g_see; g_lastFlag = flag; return g_seeResult; }
struct CIntelligentPointer { CThingAICreature* m_p; CThingAICreature* GetTypedItem() const { return m_p; } };
struct CThingFilter_IsOpinionSourceWithFreshDeed {
    CIntelligentPointer PCreature; long LastVDeedID; u64 LastSourceUID;
    bool IsAware(CThing* s, bool f) const { return IsAwareOfThingImplementation(PCreature.GetTypedItem(), s, f); }
    bool CanSee(CThing* s, bool f) const { return CanCreatureSeeCreature((CThing*)PCreature.GetTypedItem(), s, f); }
    bool operator()(CThing* source) const
    {
        if (source->HasTC(0x50)) {
            CTCOpinionSource* os = source->GetTC<CTCOpinionSource>(0x50);
            if (os->VirtualDeed) {
                if (!(LastVDeedID == os->VirtualDeedID && LastSourceUID == source->UniqueID)) {
                    if (os->AlwaysAwareOf) return true;
                    bool aware;
                    if (os->ReactNonstopFlag) aware = IsAware(source, os->UseNavLineOfSight); else aware = CanSee(source, os->UseNavLineOfSight);
                    if (aware) return true;
                }
            }
        }
        return false;
    } };
int main()
{
    CTCOpinionSource os; os.UseNavLineOfSight = true; os.AlwaysAwareOf = false; os.ReactNonstopFlag = false; os.VirtualDeed = 0; os.VirtualDeedID = 7;
    CInterfaceMap::SEntry e[1] = { { 0x50, (CTCBase*)&os } };
    CThing src; src.UniqueID = ((u64)3 << 32) | 9; for (int i = 0; i < 9; ++i) src.UsingComponents[i] = 0; src.Components.first = e; src.Components.last = e + 1;
    CThingFilter_IsOpinionSourceWithFreshDeed f; f.PCreature.m_p = 0; f.LastVDeedID = 7; f.LastSourceUID = src.UniqueID;
    int bad = 0;
    if (f(&src)) bad = 1;                                  // no TC
    src.UsingComponents[2] = 1UL << 16;
    if (f(&src)) bad = 1;                                  // no deed
    os.VirtualDeed = (COpinionDeed*)&os; os.AlwaysAwareOf = true;
    if (f(&src)) bad = 1;                                  // same deed id and uid: stale
    f.LastSourceUID = ((u64)4 << 32) | 9;                  // only the high word differs: fresh
    if (!f(&src) || g_aware + g_see != 0) bad = 1;         // always aware: no test
    os.AlwaysAwareOf = false; g_seeResult = true;
    if (!f(&src) || g_see != 1 || !g_lastFlag) bad = 1;    // sight test
    os.ReactNonstopFlag = true; os.UseNavLineOfSight = false;
    if (f(&src) || g_aware != 1 || g_lastFlag) bad = 1;    // nonstop awareness, false
    if (bad) { std::printf("00960fc0_TEST FAIL\n"); return 1; }
    std::printf("00960fc0_TEST PASS\n");
    return 0;
}