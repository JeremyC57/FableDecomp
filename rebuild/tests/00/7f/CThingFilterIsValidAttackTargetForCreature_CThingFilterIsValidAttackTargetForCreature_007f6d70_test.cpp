// Behaviour test for 007f6d70: body copied from src/007f6d70.cpp (CThingFilter_IsValidAttackTargetForCreature ctor), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CThing;
class CThingAICreature;
static int g_destroyed = 0;
struct CDefPointeeBase {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long m_refCount;
};
template <class T> struct CDefPointer {
    T* Object;
};
template <class T> struct CSubDefPointer {
    T* Object;
    CSubDefPointer() : Object(0) {}
    ~CSubDefPointer() { if (--Object->m_refCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
struct CCreatureSubDef : CDefPointeeBase {
    unsigned char _pad08[0xe4 - 0x08];
    bool IsGhost;
};
static CCreatureSubDef* g_subDef = 0; static int g_getSubDef = 0; static long g_countAtGet = -1;
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CSubDefPointer<CCreatureSubDef>* out);
};
bool CDefClassBase::GetSubDef(CSubDefPointer<CCreatureSubDef>* out)
{
    ++g_getSubDef; g_countAtGet = m_refCount;
    if (!g_subDef) return false;
    ++g_subDef->m_refCount; out->Object = g_subDef; return true;
}
struct CThingBase {
    unsigned char _pad00[0x70];
    CDefPointer<CDefClassBase> PDef;
};
template <class T> struct vector;
struct CThingFilter_IsAICreatureWillingToAttack {
    CThingAICreature* SourceCreature;
    CThingFilter_IsAICreatureWillingToAttack(CThingAICreature* c) : SourceCreature(c) {}
};
struct CThingFilter_IsAICreatureAwareOfMoreGenerous {
    CThingAICreature* PCreature;
    CThingFilter_IsAICreatureAwareOfMoreGenerous(CThingAICreature* c) : PCreature(c) {}
};
struct CThingFilter_IsNot {
    CThing* Thing;
    CThingFilter_IsNot(CThing* t) : Thing(t) {}
};
struct CThingFilter_IsThingAbleToBeEngagedInCombat {
};
struct CThingFilter_EnemyCreature {
    CThing* SourceThing;
    CThingFilter_EnemyCreature(CThing* t) : SourceThing(t) {}
};
struct CThingFilter_IsNotList {
    vector<const CThing*>* PNotList;
    void*                  PNotList2;
    CThingFilter_IsNotList(vector<const CThing*>* l) : PNotList(l), PNotList2(0) {}
};
struct CThingFilter_IsValidAttackTargetForCreature {
    CThingFilter_IsAICreatureWillingToAttack     FilterWilling;
    CThingFilter_IsAICreatureAwareOfMoreGenerous FilterAware;
    CThingFilter_IsNot                           FilterNot;
    CThingFilter_IsThingAbleToBeEngagedInCombat  FilterAbleToBeEngaged;
    CThingFilter_EnemyCreature                   FilterEnemy;
    CThingFilter_IsNotList                       FilterNotInList;
    bool                                         CheckAwareness;
    bool                                         IAmAGhostExclaim;
    CThingFilter_IsValidAttackTargetForCreature(CThingAICreature* creature, vector<const CThing*>* notList, bool checkAwareness);
};
CThingFilter_IsValidAttackTargetForCreature::CThingFilter_IsValidAttackTargetForCreature(CThingAICreature* creature, vector<const CThing*>* notList, bool checkAwareness)
    : FilterWilling(creature), FilterAware(creature), FilterNot((CThing*)creature), FilterEnemy((CThing*)creature),
      FilterNotInList(notList), CheckAwareness(checkAwareness), IAmAGhostExclaim(false)
{
    CDefClassBase* thingDef = ((CThingBase*)creature)->PDef.Object;
    CSubDefPointer<CCreatureSubDef> def;
    if (thingDef)
        ++thingDef->m_refCount;
    thingDef->GetSubDef(&def);
    if (thingDef && --thingDef->m_refCount == 0)
        thingDef->Destroy();
    if (def->IsGhost)
        IAmAGhostExclaim = true;
}
int main()
{
    static CThingBase creature; static CDefClassBase classDef; static CCreatureSubDef subDef;
    memset(&creature, 0, sizeof(creature)); creature.PDef.Object = &classDef; classDef.m_refCount = 1; subDef.m_refCount = 1; g_subDef = &subDef;
    vector<const CThing*>* notList = (vector<const CThing*>*)0x1234;
    subDef.IsGhost = true;
    {
        CThingFilter_IsValidAttackTargetForCreature f((CThingAICreature*)&creature, notList, true);
        CHECK(f.FilterWilling.SourceCreature == (CThingAICreature*)&creature && f.FilterAware.PCreature == (CThingAICreature*)&creature);
        CHECK(f.FilterNot.Thing == (CThing*)&creature && f.FilterEnemy.SourceThing == (CThing*)&creature);
        CHECK(f.FilterNotInList.PNotList == notList && f.FilterNotInList.PNotList2 == 0); CHECK(f.CheckAwareness && f.IAmAGhostExclaim);
        CHECK(g_getSubDef == 1 && g_countAtGet == 2); CHECK(classDef.m_refCount == 1 && subDef.m_refCount == 1 && g_destroyed == 0);
    }
    subDef.IsGhost = false;
    {
        CThingFilter_IsValidAttackTargetForCreature f((CThingAICreature*)&creature, 0, false);
        CHECK(!f.CheckAwareness && !f.IAmAGhostExclaim && f.FilterNotInList.PNotList == 0);
    }
    // a sub-def held only by the lookup is destroyed at scope end
    subDef.m_refCount = 0;
    { CThingFilter_IsValidAttackTargetForCreature f((CThingAICreature*)&creature, 0, false); }
    CHECK(g_destroyed == 1);
    if (fails == 0) printf("007f6d70_TEST PASS\n"); else printf("007f6d70_TEST FAIL\n");
    return fails;
}