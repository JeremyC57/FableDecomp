// Behaviour test for CThingDeadCreature::CThingDeadCreature (0x00835C20)
#include <cstdio>
static int g_pointerCtors = 0; static long g_physicalType = -1;
struct CBaseIntelligentPointer {
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_pointerCtors; }
    virtual ~CBaseIntelligentPointer() { }
};
struct CThingPointer : CBaseIntelligentPointer { CThingPointer() { } };
struct CCreaturePointer : CBaseIntelligentPointer { CCreaturePointer() { } };
struct CAnimFilterBase; struct CCPPointerInfo;
struct CAnimFilterPointer { CAnimFilterBase* Data; CCPPointerInfo* Info; CAnimFilterPointer() : Data(0), Info(0) { } };
struct CThingBodyReorienter;
struct CReorienterPointer { CThingBodyReorienter* ptr; explicit CReorienterPointer(CThingBodyReorienter* p = 0) : ptr(p) { } };
struct CThingPhysical { long m_type; CThingPhysical(long type) : m_type(type) { g_physicalType = type; } virtual ~CThingPhysical() { } };
struct CThingGameObject : CThingPhysical {
    CThingPointer PThingStandingOn; unsigned char m_gameObjectFlags;
    CThingGameObject(long type) : CThingPhysical(type) { }
};
struct CThingDeadCreature : CThingGameObject {
    CAnimFilterPointer PAnimFilter; CCreaturePointer PKiller; long BodyDieTimer; CReorienterPointer BodyReorienter; bool StartedDecay;
    CThingDeadCreature() : CThingGameObject(13), StartedDecay(false) { }
};
#define CHECK(c) if (!(c)) { std::printf("00835c20_TEST FAIL\n"); return 1; }
int main()
{
    CThingDeadCreature d;
    CHECK(g_physicalType == 13 && d.m_type == 13 && g_pointerCtors == 2);
    CHECK(d.PAnimFilter.Data == 0 && d.PAnimFilter.Info == 0 && d.PKiller.m_pItem == 0 && d.PThingStandingOn.m_pItem == 0);
    CHECK(d.BodyReorienter.ptr == 0 && d.StartedDecay == false);
    std::printf("00835c20_TEST PASS\n");
    return 0;
}
