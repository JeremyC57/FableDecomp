// Behaviour test for 00662c10: body copied from src/00662c10.cpp (CThingCreatureBase::GetAnimsWithNameStartingWith), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CCharString { public: const char* text; };
class CAnimationEntry;
static int g_destroyed = 0;
class CDefPointeeBase { public:
    virtual void v000() {}
    virtual void DestroyDef() { ++g_destroyed; }
    long m_refCount;
    void ReleaseDef()
    {
        if (--m_refCount == 0)
            DestroyDef();
    }
};
template<class T> class CDefPointer { public:
    T* m_pObject;
    CDefPointer() : m_pObject(0) { }
};
struct CAnimationEntryVector { int count; };
class CAnimFilterBase { public:
    virtual void v000() {}
    virtual void v001() {}
    virtual void v002() {}
    virtual int Kind() const { return 0; }
};
struct CAnimFilterNoGroup : CAnimFilterBase {
    CAnimFilterNoGroup() { }
    virtual void v000() {}
    virtual void v001() {}
    virtual void v002() {}
    virtual int Kind() const { return 7; }
};
static int g_queries = 0; static const char* g_prefix = 0; static CAnimationEntryVector* g_anims = 0; static int g_filterKind = 0; static const void* g_set = 0;
class CAnimationSet { public:
    void GetAnimsWithNameStartingWith(const CCharString& prefix, CAnimationEntryVector& anims, const CAnimFilterBase& filter) const
    { ++g_queries; g_prefix = prefix.text; g_anims = &anims; g_filterKind = filter.Kind(); g_set = this; anims.count += 3; }
};
class CAppearanceDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x2c];
    CAnimationSet AnimationSet;
};
static CAppearanceDef* g_appearance = 0; static int g_getSubDef = 0;
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CAppearanceDef>& out)
    {
        ++g_getSubDef;
        if (!g_appearance) return false;
        ++g_appearance->m_refCount; out.m_pObject = g_appearance; return true;
    }
};
class CThing { public:
    unsigned char  _pad00[0x70];
    CDefClassBase* PBaseDef;
};
class CThingCreatureBase : public CThing { public:
    void GetAnimsWithNameStartingWith(const CCharString& prefix, CAnimationEntryVector& anims) const;
};

void CThingCreatureBase::GetAnimsWithNameStartingWith(const CCharString& prefix, CAnimationEntryVector& anims) const
{
    CDefPointer<CAppearanceDef> def;
    PBaseDef->GetSubDef(def);
    CAppearanceDef* appearance = def.m_pObject;
    appearance->AnimationSet.GetAnimsWithNameStartingWith(prefix, anims, CAnimFilterNoGroup());
    if (appearance)
        appearance->ReleaseDef();
}
int main()
{
    static CDefClassBase classDef; static CAppearanceDef app; app.m_refCount = 1; g_appearance = &app;
    CThingCreatureBase c; memset(&c, 0, sizeof(c)); c.PBaseDef = &classDef;
    CCharString prefix = { "WALK" }; CAnimationEntryVector anims = { 0 };
    c.GetAnimsWithNameStartingWith(prefix, anims);
    CHECK(g_getSubDef == 1 && g_queries == 1); CHECK(g_prefix == prefix.text && g_anims == &anims && anims.count == 3); CHECK(g_filterKind == 7 && g_set == &app.AnimationSet);
    CHECK(app.m_refCount == 1 && g_destroyed == 0);
    // a def held only by the lookup is destroyed after the query
    app.m_refCount = 0; c.GetAnimsWithNameStartingWith(prefix, anims); CHECK(g_queries == 2 && g_destroyed == 1 && anims.count == 6);
    if (fails == 0) printf("00662c10_TEST PASS\n"); else printf("00662c10_TEST FAIL\n");
    return fails;
}