// Behaviour test for NThingShot::CTCShotDefinitionBase::~CTCShotDefinitionBase @ 0x007BF2B0: an empty user body, so the
// members go in reverse declaration order (collision thing, the two def pointers released and nulled, weapon, shooter,
// target, hit list, effect) before the CTCBase part.
#include <stdio.h>
#include <string.h>
static char g_log[64];
static void Log(const char* s) { strcat(g_log, s); }
struct CIntelligentPointer { const char* Tag; CIntelligentPointer(const char* t) : Tag(t) { } ~CIntelligentPointer() { Log(Tag); } };
struct CHitList { ~CHitList() { Log("H"); } };
struct CPointee { int RefCount; int Deleted; };
struct CDefPointer { CPointee* Object; const char* Tag;
    CDefPointer(CPointee* p, const char* t) : Object(p), Tag(t) { }
    ~CDefPointer() { CPointee* object = Object; if (object) { if (--object->RefCount == 0) object->Deleted++; Object = 0; } Log(Tag); } };
class CTCBase { public: virtual ~CTCBase() { Log("B"); } };
namespace NThingShot {
class CTCShotDefinitionBase : public CTCBase { public:
    CIntelligentPointer PEffect; CHitList HitThingsList; CIntelligentPointer PTargetThing, PShooter, PWeapon;
    CDefPointer PShotDef, PShotAbilityDef; float AccuracyPercentage; CIntelligentPointer PBestCollisionThing;
    CTCShotDefinitionBase(CPointee* a, CPointee* b) : PEffect("e"), PTargetThing("t"), PShooter("s"), PWeapon("w"),
        PShotDef(a, "D"), PShotAbilityDef(b, "A"), AccuracyPercentage(1.0f), PBestCollisionThing("c") { }
    ~CTCShotDefinitionBase() { }
};
}
int main()
{
    CPointee shotDef = { 1, 0 }, ability = { 2, 0 };
    NThingShot::CTCShotDefinitionBase* shot = new NThingShot::CTCShotDefinitionBase(&shotDef, &ability);
    delete shot;
    if (strcmp(g_log, "cADwstHeB") != 0 || shotDef.RefCount != 0 || shotDef.Deleted != 1 || ability.RefCount != 1 || ability.Deleted != 0) {
        printf("007bf2b0_TEST FAIL\n"); return 1;
    }
    g_log[0] = 0;
    shot = new NThingShot::CTCShotDefinitionBase(0, 0); delete shot;
    if (strcmp(g_log, "cADwstHeB") != 0) { printf("007bf2b0_TEST FAIL\n"); return 1; }
    printf("007bf2b0_TEST PASS\n");
    return 0;
}