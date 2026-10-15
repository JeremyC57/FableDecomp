#include <cstdio>

// Model of CTCHeroCentreDoorMarker::CTCHeroCentreDoorMarker(CThing*) @ 0x0077F570: radius 1.0, door type 0,
// hero-inside flag true, both intelligent pointers empty.
struct CThing { int id; };
struct CTCBase { CThing* ParentThing; CTCBase(CThing* t) : ParentThing(t) { } virtual ~CTCBase() { } };
struct CIntelligentPointer { void* m_pItem; CIntelligentPointer() : m_pItem(0) { } };
struct CTCHeroCentreDoorMarker : CTCBase {
    float Radius; long DoorType; bool HeroInsideRadius; CIntelligentPointer PParticle; CIntelligentPointer PHeroCentre;
    CTCHeroCentreDoorMarker(CThing* thing) : CTCBase(thing), Radius(1.0f), DoorType(0), HeroInsideRadius(true) { }
};

int main()
{
    CThing t; t.id = 4;
    CTCHeroCentreDoorMarker* m = new CTCHeroCentreDoorMarker(&t);
    bool ok = m->ParentThing == &t && m->Radius == 1.0f && m->DoorType == 0 && m->HeroInsideRadius &&
              m->PParticle.m_pItem == 0 && m->PHeroCentre.m_pItem == 0;
    delete m;
    if (!ok) { std::printf("0077f570_TEST FAIL\n"); return 1; }
    std::printf("0077f570_TEST PASS\n");
    return 0;
}
