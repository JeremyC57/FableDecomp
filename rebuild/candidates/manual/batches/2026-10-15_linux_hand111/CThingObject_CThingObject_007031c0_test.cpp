#include <cstdio>
#include <new>
// Behaviour test for CThingObject::CThingObject (0x007031C0): type 5, owner pointer built, def/timer/flag cleared.
struct CIntelligentPointer { void* m_pItem; CIntelligentPointer() : m_pItem(0) { } };
struct CThing { long m_type; CThing(long type) : m_type(type) { } };
struct CThingGameObject : CThing { CIntelligentPointer m_owner; CThingGameObject(long type) : CThing(type), m_owner() { } };
struct CDefPointer { const void* m_pDef; CDefPointer() : m_pDef(0) { } };
struct CThingObject : CThingGameObject {
    CDefPointer PDef; long DieOffTimer; bool DiesOff;
    CThingObject() : CThingGameObject(5), PDef(), DieOffTimer(0), DiesOff(false) { }
};
int main()
{
    unsigned char raw[sizeof(CThingObject)];
    for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xAB;
    CThingObject* o = new (raw) CThingObject;
    int fails = 0;
    if (o->m_type != 5 || o->m_owner.m_pItem || o->PDef.m_pDef || o->DieOffTimer != 0 || o->DiesOff) ++fails;
    if (fails) { std::printf("007031c0_TEST FAIL\n"); return 1; }
    std::printf("007031c0_TEST PASS\n");
    return 0;
}
