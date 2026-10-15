#include <cstdio>

// Model of CTCHero::CTCHero(CThing*) @ 0x006C9FE0: base keeps the owner; flags, title, defs, podium flag and the
// 64-bit weapon id are zeroed; both intelligent pointers start empty.
struct CThing { int id; };
struct CTCBase { CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
    CTCBase(CThing* t) : ParentThing(t), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { } virtual ~CTCBase() { } };
struct CDefPointer { void* Object; CDefPointer() : Object(0) { } };
struct CIntelligentPointer { void* m_pItem; CIntelligentPointer() : m_pItem(0) { } };
struct CTCHero : CTCBase {
    bool WeaponsUsable, WillUsable, WeaponUseOverriddenByScripts, WillUseOverriddenByScripts;
    long Title; long HeroTitleObjectDef; CDefPointer PHeroTitleSubDef; bool OnBoastingPodium;
    CIntelligentPointer LastWeaponEquipped; unsigned long long LastWeaponEquippedID; CIntelligentPointer PTeleportParticle;
    CTCHero(CThing* thing)
        : CTCBase(thing), WeaponsUsable(false), WillUsable(false), WeaponUseOverriddenByScripts(false),
          WillUseOverriddenByScripts(false), Title(0), HeroTitleObjectDef(0), OnBoastingPodium(false), LastWeaponEquippedID(0) { }
};

int main()
{
    CThing t; t.id = 1;
    CTCHero* h = new CTCHero(&t);
    bool ok = h->ParentThing == &t && !h->WeaponsUsable && !h->WillUsable && !h->WeaponUseOverriddenByScripts &&
              !h->WillUseOverriddenByScripts && h->Title == 0 && h->HeroTitleObjectDef == 0 && h->PHeroTitleSubDef.Object == 0 &&
              !h->OnBoastingPodium && h->LastWeaponEquipped.m_pItem == 0 && h->LastWeaponEquippedID == 0 &&
              h->PTeleportParticle.m_pItem == 0;
    delete h;
    if (!ok) { std::printf("006c9fe0_TEST FAIL\n"); return 1; }
    std::printf("006c9fe0_TEST PASS\n");
    return 0;
}
