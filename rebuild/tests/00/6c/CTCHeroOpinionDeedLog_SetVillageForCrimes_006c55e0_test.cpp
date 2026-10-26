// Test for 0x006C55E0 CTCHeroOpinionDeedLog::SetVillageForCrimes: the member intelligent pointer ends up on the village.
#include <stdio.h>
class CBaseObject { public: virtual ~CBaseObject() { } };
class CThing : public CBaseObject { };
static int g_sets = 0, g_dtors = 0;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer() : PItem(0) { }
    virtual ~CBaseIntelligentPointer() { ++g_dtors; }
    CBaseObject* GetPItem() const { return PItem; }
    void SetPItem(CBaseObject* item) { ++g_sets; PItem = item; }
    CBaseObject* PItem;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
class CTCHeroOpinionDeedLog {
public:
    virtual void v00() { }
    unsigned char _pad04[0x20 - 0x04];
    CIntelligentPointer<CThing> VillageForCrimes;
    void SetVillageForCrimes(CThing& village);
};
void CTCHeroOpinionDeedLog::SetVillageForCrimes(CThing& village)
{
    VillageForCrimes = &village;
}
int main()
{
    CThing a, b; CTCHeroOpinionDeedLog log;
    bool ok = (char*)&log.VillageForCrimes - (char*)&log == 0x20;
    log.SetVillageForCrimes(a);
    ok = ok && log.VillageForCrimes.GetPItem() == &a && g_sets == 2 && g_dtors == 1;
    log.SetVillageForCrimes(b);
    ok = ok && log.VillageForCrimes.GetPItem() == &b && g_dtors == 2;
    if (!ok) { printf("006c55e0_TEST FAIL\n"); return 1; }
    printf("006c55e0_TEST PASS\n");
    return 0;
}
