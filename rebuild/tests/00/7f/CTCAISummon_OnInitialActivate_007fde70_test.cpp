// Behaviour test for CTCAISummon::OnInitialActivate @ 0x007FDE70: after the base hook, a counted copy of
// the thing's def fetches the summon sub-def; the copy's reference is returned afterwards.
#include <cstdio>

struct CSummonDef;
template <class T> struct CDefPointer;
static int g_baseCalls = 0;
static int g_destroyed = 0;

struct CDefClassBase {
    long m_refCount;
    CSummonDef* m_sub;
    virtual ~CDefClassBase() {}
    virtual void Destroy() { ++g_destroyed; }
    bool GetSubDef(CDefPointer<CSummonDef>* out) const;
};

template <class T>
struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->m_refCount; }
    ~CDefPointer()
    {
        T* object = Object;
        if (object != 0) {
            if (--object->m_refCount == 0)
                object->Destroy();
        }
    }
    T* operator->() const { return Object; }
};

struct CThingDef : CDefClassBase {};
struct CSummonDef : CDefClassBase {};

bool CDefClassBase::GetSubDef(CDefPointer<CSummonDef>* out) const
{
    if (m_refCount < 2) return false;           // the caller's copy must hold a reference
    out->Object = m_sub;
    return true;
}

struct CThing { CDefPointer<CThingDef> PDef; };

struct CTCBase {
    CThing* ParentThing;
    void OnInitialActivate() { ++g_baseCalls; }
};

struct CTCAISummon : CTCBase {
    CDefPointer<CSummonDef> PSummonDef;
    void OnInitialActivate()
    {
        CTCBase::OnInitialActivate();
        CDefPointer<CThingDef> thingDef(ParentThing->PDef);
        thingDef->GetSubDef(&PSummonDef);
    }
};

int main()
{
    CSummonDef summon; summon.m_refCount = 1; summon.m_sub = 0;
    CThingDef def; def.m_refCount = 1; def.m_sub = &summon;
    CThing thing; thing.PDef.Object = &def;
    CTCAISummon s; s.ParentThing = &thing;
    s.OnInitialActivate();
    bool ok = g_baseCalls == 1 && s.PSummonDef.Object == &summon && def.m_refCount == 1 && g_destroyed == 0;
    thing.PDef.Object = 0;
    s.PSummonDef.Object = 0;
    if (!ok) { std::printf("007fde70_TEST FAIL\n"); return 1; }
    std::printf("007fde70_TEST PASS\n");
    return 0;
}