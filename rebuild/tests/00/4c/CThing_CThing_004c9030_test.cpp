// Behaviour test for CThing::CThing (0x004C9030): the class base gets the thing manager, class type and false; every
// member starts blank (ids, presence bits, component map, self-linked update list header, pointers, list links,
// map number) with the default flag set; the global thing count goes up.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
static void* g_factory = 0; static unsigned long g_classType = 0; static bool g_registerNow = true; static int g_classCtors = 0, g_strings = 0;
class CClassFactory;
class CClass { public: virtual void v00() {} CClass(CClassFactory& factory, unsigned long type, bool registerNow) { ++g_classCtors; g_factory = &factory; g_classType = type; g_registerNow = registerNow; } };
class CThingManager { public: int x; };
static CThingManager g_manager; static CThingManager* g_pThingManager = &g_manager; static long g_thingCount = 5;
class CCharString { public: void* PStringData; CCharString() : PStringData((void*)0x1234) { ++g_strings; } };
struct CPresenceBits { unsigned long words[9]; CPresenceBits() { memset(words, 0, sizeof(words)); } };
class CTCBase; struct SComponentPair { long first; CTCBase* second; }; struct CKeyPairCompareLess { };
struct CComponentMap { SComponentPair* m_first; SComponentPair* m_last; SComponentPair* m_end; CKeyPairCompareLess m_compare; bool m_sorted; CComponentMap() : m_first(0), m_last(0), m_end(0), m_sorted(false) { } };
struct SListNode { SListNode* _M_next; SListNode* _M_prev; CTCBase* _M_data; };
struct CTCList { SListNode* _M_node; CTCList() : _M_node(0) { SListNode* node = (SListNode*)malloc(sizeof(SListNode)); node->_M_next = node; node->_M_prev = node; _M_node = node; } };
class CThingBaseDef; struct CDefPointer { const CThingBaseDef* Object; CDefPointer() : Object(0) { } };
class CTCDriverBase; class CTCPhysicsBase; struct CTCGraphicAppearance;
class CThing : public CClass {
public:
    unsigned char _pad04[0x18 - 0x04]; unsigned long long UniqueID; CPresenceBits UsingComponents; CComponentMap Components; CTCList UpdateComponentsList;
    CCharString SerialisationSectionName; CTCDriverBase* PTCDriver; CTCPhysicsBase* PhysicsTC; CTCGraphicAppearance* PTCGraphicAppearance; long WorldFrameCreatedAt; unsigned long ThingTypeFlags;
    CDefPointer PBaseDef; CCharString ScriptName; CCharString ScriptData; CThing* RenderListPrev; CThing* RenderListNext; CThing* UpdateListPrev; CThing* UpdateListNext;
    unsigned short DefGlobalIndex; short CurrentMapNumber; signed char PlayerNumber;
    unsigned char Dead : 1; unsigned char CreatedByOwnerThingCreation : 1; unsigned char IncludeInThingScans : 1; unsigned char Activated : 1; unsigned char GloballyLoaded : 1; unsigned char InLimbo : 1; unsigned char KillLocked : 1; unsigned char CreatedBySerialisation : 1;
    unsigned char DeletedOnOwnerLevelOutOfScope : 1; unsigned char Drawable : 1; unsigned char Dying : 1; unsigned char ForceFrameUpdate : 1; unsigned char LevelPersistent : 1; unsigned char GameStatePersisted : 1; unsigned char PersistOnUnloadWhenInLimbo : 1; unsigned char PausedDuringCutscenes : 1;
    unsigned char NotPausedDuringCutscenes : 1; unsigned char DeathPersistent : 1; unsigned char KillOnLevelUnload : 1; unsigned char ActivatedScript : 1; unsigned char LevelIsUnloading : 1; unsigned char IsAffectedByBulletTimeOverride : 1; unsigned char AddingTCsPermitted : 1;
    unsigned char InitialisationStage : 3;
    CThing(unsigned long classType);
};
CThing::CThing(unsigned long classType)
    : CClass(*(CClassFactory*)g_pThingManager, classType, false),
      UniqueID(0), UsingComponents(), Components(), UpdateComponentsList(), SerialisationSectionName(),
      PTCDriver(0), PhysicsTC(0), PTCGraphicAppearance(0), PBaseDef(), ScriptName(), ScriptData(),
      RenderListPrev(0), RenderListNext(0), UpdateListPrev(0), UpdateListNext(0), CurrentMapNumber(0),
      Dead(false), CreatedByOwnerThingCreation(false), IncludeInThingScans(true), Activated(false), GloballyLoaded(false),
      InLimbo(false), KillLocked(false), CreatedBySerialisation(false),
      DeletedOnOwnerLevelOutOfScope(true), Drawable(true), Dying(false), ForceFrameUpdate(false), GameStatePersisted(false),
      PersistOnUnloadWhenInLimbo(false), PausedDuringCutscenes(false),
      NotPausedDuringCutscenes(false), DeathPersistent(false), KillOnLevelUnload(true), ActivatedScript(false),
      LevelIsUnloading(false), IsAffectedByBulletTimeOverride(true), AddingTCsPermitted(true), InitialisationStage(0)
{
    ++g_thingCount;
}
static int fail(const char* why) { std::printf("004c9030_TEST FAIL: %s\n", why); return 1; }
int main()
{
    static unsigned char raw[sizeof(CThing)]; memset(raw, 0xff, sizeof(raw));
    CThing* t = new (raw) CThing(0x77);
    if (g_classCtors != 1 || g_factory != (void*)&g_manager || g_classType != 0x77 || g_registerNow) return fail("class base");
    if (t->UniqueID != 0 || g_thingCount != 6 || g_strings != 3) return fail("id / count / strings");
    for (int i = 0; i < 9; ++i) if (t->UsingComponents.words[i] != 0) return fail("presence bits");
    if (t->Components.m_first || t->Components.m_last || t->Components.m_end || t->Components.m_sorted) return fail("component map");
    SListNode* n = t->UpdateComponentsList._M_node; if (!n || n->_M_next != n || n->_M_prev != n) return fail("update list header");
    if (t->PTCDriver || t->PhysicsTC || t->PTCGraphicAppearance || t->PBaseDef.Object || t->RenderListPrev || t->RenderListNext || t->UpdateListPrev || t->UpdateListNext || t->CurrentMapNumber != 0) return fail("pointers / map number");
    if (t->Dead || t->CreatedByOwnerThingCreation || !t->IncludeInThingScans || t->Activated || t->GloballyLoaded || t->InLimbo || t->KillLocked || t->CreatedBySerialisation) return fail("+0x91 flags");
    if (!t->DeletedOnOwnerLevelOutOfScope || !t->Drawable || t->Dying || t->ForceFrameUpdate || !t->LevelPersistent || t->GameStatePersisted || t->PersistOnUnloadWhenInLimbo || t->PausedDuringCutscenes) return fail("+0x92 flags (LevelPersistent left alone)");
    if (t->NotPausedDuringCutscenes || t->DeathPersistent || !t->KillOnLevelUnload || t->ActivatedScript || t->LevelIsUnloading || !t->IsAffectedByBulletTimeOverride || !t->AddingTCsPermitted || t->InitialisationStage != 0) return fail("+0x93/+0x94 flags");
    if (t->ThingTypeFlags != (unsigned long)-1 || t->DefGlobalIndex != 0xffff || t->PlayerNumber != -1 || t->WorldFrameCreatedAt != -1) return fail("fields the constructor leaves alone were written");
    free(n);
    std::printf("004c9030_TEST PASS\n");
    return 0;
}