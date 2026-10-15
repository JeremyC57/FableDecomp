#include <cstdio>
#include <cstring>

struct CThing { unsigned char m_type; unsigned char Dead : 1; };
struct CThingObject : CThing { };
struct CThingCreatureBase;
static int g_baseUses = 0, g_ipDtors = 0, g_actionDtors = 0, g_actions = 0;
static CThingObject* g_object = 0;
struct CIntelligentPointer { void* m_pItem; CIntelligentPointer() : m_pItem(0) { } ~CIntelligentPointer() { ++g_ipDtors; } };
struct CCreatureActionBase { int kind; ~CCreatureActionBase() { ++g_actionDtors; } };
struct CCreatureAction_AddRealObjectToInventory : CCreatureActionBase {
    CIntelligentPointer PObject;
    CCreatureAction_AddRealObjectToInventory(CThingCreatureBase*, CThingObject* object) { kind = 5; g_object = object; }
};
struct CThingCreatureBase : CThing { bool SetCurrentAction(CCreatureActionBase* a) { if (a->kind == 5) ++g_actions; return true; } };
struct CTCActionUseBase { CThing* ParentThing; void OnActionUse(CThing&) { ++g_baseUses; } };
struct CTCActionUsePutInInventory : CTCActionUseBase {
    void OnActionUse(CThing& user)
    {
        CTCActionUseBase::OnActionUse(user);
        CThingCreatureBase* creature = static_cast<CThingCreatureBase*>(&user);
        if (creature && !creature->Dead && ParentThing->m_type == 5) {
            CCreatureAction_AddRealObjectToInventory action(creature, static_cast<CThingObject*>(ParentThing));
            creature->SetCurrentAction(&action);
        }
    }
};
int main()
{
    CThingObject apple; apple.m_type = 5; apple.Dead = 0;
    CThingCreatureBase hero; hero.m_type = 1; hero.Dead = 0;
    CTCActionUsePutInInventory use; use.ParentThing = &apple;
    use.OnActionUse(hero);
    if (g_baseUses != 1 || g_actions != 1 || g_object != &apple || g_ipDtors != 1 || g_actionDtors != 1) { std::printf("007eb390_TEST FAIL\n"); return 1; }
    hero.Dead = 1; use.OnActionUse(hero);
    if (g_baseUses != 2 || g_actions != 1) { std::printf("007eb390_TEST FAIL\n"); return 1; }
    hero.Dead = 0; apple.m_type = 3; use.OnActionUse(hero);
    if (g_baseUses != 3 || g_actions != 1 || g_actionDtors != 1) { std::printf("007eb390_TEST FAIL\n"); return 1; }
    std::printf("007eb390_TEST PASS\n");
    return 0;
}