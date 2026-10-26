// Behaviour test for 006c5f80: body copied from src/006c5f80.cpp (CTCHeroOpinionDeedLog::RemoveCrimeFromVillage), the callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
};
static int g_getPItem = 0;
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer() {}
    void* PData;
    CThing* GetPItem() const { ++g_getPItem; return (CThing*)PData; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {};
static int g_removeCrime = 0; static long g_removedHandle = 0;
class CTCVillage {
public:
    unsigned char _pad00[0x7c];
    bool CrimeReportedFlag;
    void RemoveCrime(long handle) { ++g_removeCrime; g_removedHandle = handle; }
};
class COpinionDeed { public: unsigned char _pad00[0x22]; bool ReportedToVillage; };
struct SDeedNode { long _M_color; SDeedNode* _M_parent; SDeedNode* _M_left; SDeedNode* _M_right; long first; COpinionDeed* second; };
struct CDeedMapIterator {
    SDeedNode* _M_node;
    CDeedMapIterator(SDeedNode* n) : _M_node(n) {}
    SDeedNode* operator->() const { return _M_node; }
};
static int g_finds = 0; static long g_findKey = 0; static SDeedNode* g_findResult = 0;
struct COpinionDeedMap {
    SDeedNode*   _M_header;
    unsigned int _M_node_count;
    long         _M_key_compare;
    CDeedMapIterator find(const long& key) { ++g_finds; g_findKey = key; return CDeedMapIterator(g_findResult); }
};
class CTCBaseRoot {
public:
    virtual ~CTCBaseRoot() {}
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
};
class CTCHeroOpinionDeedLog : public CTCBaseRoot {
public:
    COpinionDeedMap OpinionDeedMap;
    void*           OpinionDeedsSearchingForWitnesses;
    long            NextHandle;
    CIntelligentPointer<CThing> VillageForCrimes;
    void RemoveCrimeFromVillage(long handle);
};
void CTCHeroOpinionDeedLog::RemoveCrimeFromVillage(long handle)
{
    if (VillageForCrimes.GetPItem()) {
        CThing* villageThing = VillageForCrimes.GetPItem();
        if (villageThing->HasTC(0x22)) {
            CTCVillage* village = villageThing->GetTC<CTCVillage>(0x22);
            if (!village->CrimeReportedFlag) {
                village->RemoveCrime(handle);
                CDeedMapIterator it = OpinionDeedMap.find(handle);
                if (it._M_node != OpinionDeedMap._M_header)
                    it->second->ReportedToVillage = false;
            }
        }
    }
}
int main()
{
    CTCHeroOpinionDeedLog log; SDeedNode header; memset(&header, 0, sizeof(header)); log.OpinionDeedMap._M_header = &header;
    CThing thing; memset(&thing, 0, sizeof(thing)); CTCVillage village; memset(&village, 0, sizeof(village));
    CInterfaceMap::SEntry entries[2] = { { 0x22, (CTCBase*)&village }, { 0x30, (CTCBase*)0x99 } };
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 2;
    COpinionDeed deed; deed.ReportedToVillage = true; SDeedNode node; memset(&node, 0, sizeof(node)); node.first = 9; node.second = &deed;
    // 1. no village thing: nothing happens (one GetPItem)
    log.VillageForCrimes.PData = 0; log.RemoveCrimeFromVillage(9);
    CHECK(g_getPItem == 1 && g_removeCrime == 0 && g_finds == 0);
    // 2. village thing without the village component
    log.VillageForCrimes.PData = &thing; log.RemoveCrimeFromVillage(9);
    CHECK(g_getPItem == 3 && g_removeCrime == 0 && g_finds == 0);
    // 3. component present but the crime was reported: nothing
    thing.m_presentBits[0x22 >> 5] = 1UL << (0x22 & 31); village.CrimeReportedFlag = true; log.RemoveCrimeFromVillage(9);
    CHECK(g_getPItem == 5 && g_removeCrime == 0 && g_finds == 0);
    // 4. not reported, deed found: crime removed and the deed's flag cleared
    village.CrimeReportedFlag = false; g_findResult = &node; log.RemoveCrimeFromVillage(9);
    CHECK(g_removeCrime == 1 && g_removedHandle == 9 && g_finds == 1 && g_findKey == 9 && deed.ReportedToVillage == false);
    // 5. deed not in the map (end iterator): crime removed, nothing dereferenced
    deed.ReportedToVillage = true; g_findResult = &header; log.RemoveCrimeFromVillage(12);
    CHECK(g_removeCrime == 2 && g_removedHandle == 12 && g_finds == 2 && deed.ReportedToVillage == true);
    if (fails == 0) printf("006c5f80_TEST PASS\n"); else printf("006c5f80_TEST FAIL\n");
    return fails;
}