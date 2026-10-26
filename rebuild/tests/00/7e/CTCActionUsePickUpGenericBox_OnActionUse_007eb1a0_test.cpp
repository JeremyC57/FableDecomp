// Behaviour test for 007eb1a0: body copied from src/007eb1a0.cpp (CTCActionUsePickUpGenericBox::OnActionUse), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CThing;
class CDefClassBase;
static int g_destroyed = 0;
struct CPickUpDef {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long RefCount;
    void ReleaseTemporary() { if (--RefCount == 0) Destroy(); }
};
template <class T> class CDefPointer { public:
    T* Data;
    CDefPointer() : Data(0) {}
    ~CDefPointer()
    {
        if (Data && --Data->RefCount == 0)
            Data->Destroy();
    }
};
static CPickUpDef* g_def = 0; static int g_getSubDef = 0;
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CPickUpDef>* out) const;
};
bool CDefClassBase::GetSubDef(CDefPointer<CPickUpDef>* out) const
{
    ++g_getSubDef;
    if (!g_def) return false;
    ++g_def->RefCount; out->Data = g_def; return true;
}
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];
    unsigned char _pad44[0x70 - 0x44];
    CDefClassBase* PBaseDef;
    unsigned char _pad74[0x91 - 0x74];
    unsigned char Flags91;
    bool IsDead() const { return (Flags91 & 1) != 0; }
    unsigned long IsUsingComponent(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
};
class CCreatureActionBase;
static int g_setAction = 0; static const CCreatureActionBase* g_lastAction = 0;
class CThingCreatureBase : public CThing {
public:
    bool SetCurrentAction(const CCreatureActionBase& action) { ++g_setAction; g_lastAction = &action; return true; }
};
class CThingObject : public CThing {};
struct CBaseIntelligentPointer {
    virtual void v00() {}
    void* m_pItem;
    ~CBaseIntelligentPointer() {}
};
class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase() {}
    unsigned char _pad04[0xa8 - 4];
};
static int g_actionCtor = 0; static CThingCreatureBase* g_actionCreature = 0; static CThing* g_actionObject = 0;
class __declspec(novtable) CCreatureAction_PickUpGenericBox : public CCreatureActionBase {
public:
    CBaseIntelligentPointer PThingToPickUp;
    CCreatureAction_PickUpGenericBox(CThingCreatureBase* creature, CThing* thing) { ++g_actionCtor; g_actionCreature = creature; g_actionObject = thing; PThingToPickUp.m_pItem = thing; }
};
inline CThingCreatureBase* CastToCreature(CThing* thing) { return (CThingCreatureBase*)thing; }
class CTCBase {
public:
    virtual ~CTCBase() {}
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
};
static int g_baseUse = 0; static CThing* g_baseUser = 0;
class CTCActionUseBase : public CTCBase {
public:
    virtual void OnActionUse(CThing& user) { ++g_baseUse; g_baseUser = &user; }
};
class CTCActionUsePickUpGenericBox : public CTCActionUseBase {
public:
    virtual void OnActionUse(CThing& user);
};
void CTCActionUsePickUpGenericBox::OnActionUse(CThing& user)
{
    CTCActionUseBase::OnActionUse(user);
    CThingCreatureBase* creature = CastToCreature(&user);
    if (creature && !creature->IsDead()) {
        CDefPointer<CPickUpDef> def;
        ParentThing->PBaseDef->GetSubDef(&def);
        if (creature->IsUsingComponent(70)) {
            creature->SetCurrentAction(CCreatureAction_PickUpGenericBox(creature, (CThing*)ParentThing));
        }
    }
}
int main()
{
    static CThingCreatureBase user; static CThingObject object; static CDefClassBase classDef; static CPickUpDef def;
    memset(&user, 0, sizeof(user)); memset(&object, 0, sizeof(object)); object.PBaseDef = &classDef; def.RefCount = 1; g_def = &def;
    CTCActionUsePickUpGenericBox tc; tc.ParentThing = &object;
    // 1: live creature using component 70: the action is set
    user.UsingComponents[70 >> 5] |= 1UL << (70 & 31);
    tc.OnActionUse(user);
    CHECK(g_baseUse == 1 && g_baseUser == &user); CHECK(g_getSubDef == 1); CHECK(g_setAction == 1 && g_actionCtor == 1 && g_actionCreature == &user && g_actionObject == &object);
    CHECK(def.RefCount == 1 && g_destroyed == 0);
    // 2: not using component 70: def fetched, no action
    user.UsingComponents[70 >> 5] = 0; tc.OnActionUse(user); CHECK(g_getSubDef == 2 && g_setAction == 1);
    // 3: dead creature: nothing beyond the base call
    user.Flags91 = 1; tc.OnActionUse(user); CHECK(g_baseUse == 3 && g_getSubDef == 2);
    // 4: a def held only by the lookup is destroyed
    user.Flags91 = 0; def.RefCount = 0; tc.OnActionUse(user); CHECK(g_destroyed == 1);
    if (fails == 0) printf("007eb1a0_TEST PASS\n"); else printf("007eb1a0_TEST FAIL\n");
    return fails;
}