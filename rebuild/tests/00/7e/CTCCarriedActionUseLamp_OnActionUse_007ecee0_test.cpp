// Behaviour test for CTCCarriedActionUseLamp::vfunc_28 (0x007ECEE0): the user's light (interface 0x27) gets the lamp
// def and is toggled; the lamp's interface-0x1a component is notified when present; the def reference is released.
#include <cstdio>
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* p = first;
        while (p != last && p->type < *type) ++p;
        return p;
    }
};
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
static int g_destroyed = 0;
struct CDefVirtual {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
};
class CLightDef : public CDefVirtual, public CRefCounted { public:
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object)
            Object->ReleaseTemporary();
    }
};
static CLightDef* g_subDef = 0;
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CLightDef>* out)
    {
        out->Object = g_subDef;
        if (g_subDef) { ++g_subDef->m_count; return true; }
        return false;
    }
};
class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];
    CInterfaceMap  m_interfaces;
    unsigned char  _pad4c[0x70 - 0x4c];
    CDefClassBase* PDef;
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
class CTCLight { public:
    unsigned char _pad00[0x1c];
    bool Active;
    CLightDef* SetDef;
    int SetActiveCalls;
    void SetLightDef(const CDefPointer<CLightDef>& def) { SetDef = def.Object; }
    void SetActive(bool active) { Active = active; ++SetActiveCalls; }
};
struct CTCShowableItem {
    int Calls;
    void OnShowEnded() { ++Calls; }
};
class CTCCarriedActionUseBase { public:
    virtual void v00() {}
    CThing* ParentThing;
    int BaseCalls;
    void OnUseBase(CThing* user) { ++BaseCalls; }
};
struct CTCCarriedActionUseLamp : CTCCarriedActionUseBase {
    void vfunc_28(CThing* user);
};

void CTCCarriedActionUseLamp::vfunc_28(CThing* user)
{
    CTCCarriedActionUseBase::OnUseBase(user);
    CTCLight* light = 0;
    if (user->HasTC(0x27))
        light = (CTCLight*)user->GetTC(0x27);
    CDefPointer<CLightDef> def;
    ParentThing->PDef->GetSubDef(&def);
    light->SetLightDef(def);
    light->SetActive(!light->Active);
    if (ParentThing->HasTC(0x1a))
        ((CTCShowableItem*)ParentThing->GetTC(0x1a))->OnShowEnded();
}

static void Clear(CThing& t)
{
    for (int i = 0; i < 9; ++i) t.m_presentBits[i] = 0;
    t.PDef = 0;
}
int main()
{
    CTCLight light; light.Active = false; light.SetDef = 0; light.SetActiveCalls = 0;
    CTCShowableItem notify; notify.Calls = 0;
    CThing user; Clear(user);
    CInterfaceMap::SEntry userEntries[3];
    userEntries[0].type = 0x10; userEntries[0].component = 0;
    userEntries[1].type = 0x27; userEntries[1].component = (CTCBase*)&light;
    userEntries[2].type = 0x30; userEntries[2].component = 0;
    user.m_interfaces.first = userEntries; user.m_interfaces.last = userEntries + 3;
    user.m_presentBits[1] = 0x80;                    // type 0x27
    CDefClassBase defs;
    CThing lamp; Clear(lamp); lamp.PDef = &defs;
    CInterfaceMap::SEntry lampEntries[1];
    lampEntries[0].type = 0x1a; lampEntries[0].component = (CTCBase*)&notify;
    lamp.m_interfaces.first = lampEntries; lamp.m_interfaces.last = lampEntries + 1;
    lamp.m_presentBits[0] = 0x4000000;               // type 0x1a
    CLightDef def; def.m_count = 1; g_subDef = &def;
    CTCCarriedActionUseLamp action; action.ParentThing = &lamp; action.BaseCalls = 0;
    int bad = 0;
    action.vfunc_28(&user);
    if (action.BaseCalls != 1 || light.SetDef != &def || !light.Active || notify.Calls != 1) bad |= 1;
    if (def.m_count != 1 || g_destroyed != 0) bad |= 2;
    lamp.m_presentBits[0] = 0;                       // no 0x1a component: no notify; toggles back off
    action.vfunc_28(&user);
    if (light.Active || light.SetActiveCalls != 2 || notify.Calls != 1 || def.m_count != 1) bad |= 4;
    def.m_count = 0;                                 // the GetSubDef add-ref is the only reference: released and destroyed
    action.vfunc_28(&user);
    if (def.m_count != 0 || g_destroyed != 1 || !light.Active) bad |= 8;
    g_subDef = 0;                                    // no sub def: the light gets a null def, nothing released
    action.vfunc_28(&user);
    if (light.SetDef != 0 || g_destroyed != 1) bad |= 16;
    if (bad) { std::printf("007ecee0_TEST FAIL\n"); return 1; }
    std::printf("007ecee0_TEST PASS\n");
    return 0;
}