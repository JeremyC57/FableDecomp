#include <cstdio>

class CBaseObject { public: int x; };
static int g_ctor = 0;
struct CBaseIntelligentPointer { virtual void v00() { } CBaseObject* PItem; CBaseIntelligentPointer() : PItem((CBaseObject*)1) { ++g_ctor; }
    void SetPItem(CBaseObject* item) { PItem = item; } };

struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() { } CIntelligentPointer(CBaseObject* item) : CBaseIntelligentPointer() { SetPItem(item); } };
class CThingFilter_IsOpinionSourceWithFreshDeed { public: CIntelligentPointer PCreature; long LastVDeedID; unsigned __int64 LastSourceUID;
    CThingFilter_IsOpinionSourceWithFreshDeed(CBaseObject* creature, long lastVDeedID, unsigned __int64 lastSourceUID)
        : PCreature(creature), LastVDeedID(lastVDeedID), LastSourceUID(lastSourceUID) { } };
int main()
{
    CBaseObject o; unsigned __int64 uid = 0x1122334455667788ui64;
    CThingFilter_IsOpinionSourceWithFreshDeed f(&o, -5, uid);
    if (g_ctor != 1 || f.PCreature.PItem != &o || f.LastVDeedID != -5 || f.LastSourceUID != uid || sizeof(f) != 0x18) { std::printf("00960f80_TEST FAIL\n"); return 1; }
    std::printf("00960f80_TEST PASS\n"); return 0;
}