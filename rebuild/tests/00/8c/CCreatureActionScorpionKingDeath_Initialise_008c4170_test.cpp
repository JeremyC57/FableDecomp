// Behaviour test for 008c4170: body copied from src/008c4170.cpp (CCreatureAction_ScorpionKingDeath::Initialise), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct C3DVector { float X, Y, Z; };
static int g_strCtors = 0, g_strDtors = 0;
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1) : m_pData((void*)text) { ++g_strCtors; CHECK(length == -1); }
    ~CCharString() { ++g_strDtors; }
};
struct CBaseObject;
struct CBaseIntelligentPointer {
    void*        m_vtable;
    CBaseObject* m_pItem;
    CBaseObject* GetPItem() const { return m_pItem; }
};
static int g_deleted = 0;
struct CDefPointeeBase {
    virtual void Destroy() {}
    virtual void Delete() { ++g_deleted; }
    long m_refCount;
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
template <class T> struct CDefHolder {
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Delete();
    }
    T* operator->() const { return Object; }
};
static bool g_found = false; static long g_effect = 0; static int g_lookups = 0; static const char* g_lookupName = 0;
struct CSpecialEffectsDef : CDefPointeeBase {
    bool CalcAndLookup(const CCharString& name, long* value) { ++g_lookups; g_lookupName = (const char*)name.m_pData; *value = g_effect; return g_found; }
};
static CSpecialEffectsDef* g_effects = 0; static int g_getSubDef = 0; static long g_countAtGet = -1;
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CSpecialEffectsDef>* out)
    {
        ++g_getSubDef; g_countAtGet = m_refCount;
        if (!g_effects) return false;
        ++g_effects->m_refCount; out->Object = g_effects; return true;
    }
};
template <class T> struct CDefCopy {
    T* Object;
    CDefCopy(T* object) : Object(object) { if (Object) ++Object->m_refCount; }
    ~CDefCopy()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Delete();
    }
    T* operator->() const { return Object; }
};
static C3DVector g_position = { 7, 8, 9 };
struct CThing {
    unsigned char  _pad00[0x70];
    CDefClassBase* PDef;
    const C3DVector& GetPosition() const { return g_position; }
};
static int g_creates = 0; static long g_createDef = -1; static const C3DVector* g_createPos = 0;
CThing* __fastcall Create(long defIndex, const C3DVector& position, long flags) { ++g_creates; g_createDef = defIndex; g_createPos = &position; CHECK(flags == 0); return 0; }
static int g_setAnim = 0; static const char* g_animName = 0; static int g_baseInit = 0;
struct CCombatActionBase {
    virtual void v00() {}
    long                    _pad04;
    CBaseIntelligentPointer POwner;
    unsigned char           _body[0x114 - 0x10];
    void SetAnimationName(const CCharString& name) { ++g_setAnim; g_animName = (const char*)name.m_pData; }
    void Initialise() { ++g_baseInit; CHECK(g_setAnim == 1 + g_baseInit - 1); }
    CThing* GetOwner() const { return (CThing*)POwner.GetPItem(); }
    CSpecialEffectsDef* GetSpecialEffectsDef() const
    {
        CDefPointer<CSpecialEffectsDef> out;
        CDefCopy<CDefClassBase> def(GetOwner()->PDef);
        def->GetSubDef(&out);
        return out.Object;
    }
};
struct CCreatureAction_ScorpionKingDeath : CCombatActionBase {
    void Initialise();
};
void CCreatureAction_ScorpionKingDeath::Initialise()
{
    SetAnimationName("FALL_TO_DEATH");
    CCombatActionBase::Initialise();
    CDefPointer<CSpecialEffectsDef> out;
    long effectIndex = 0;
    {
        CDefCopy<CDefClassBase> def(GetOwner()->PDef);
        def->GetSubDef(&out);
    }
    CDefHolder<CSpecialEffectsDef> effects(out.Object);
    if (effects.Object) {
        bool found;
        {
            CCharString name("DEATH", -1);
            found = effects->CalcAndLookup(name, &effectIndex);
        }
        if (found)
            Create(effectIndex, GetOwner()->GetPosition(), 0);
    }
}
int main()
{
    static CThing owner; static CDefClassBase classDef; static CSpecialEffectsDef effects; classDef.m_refCount = 1; effects.m_refCount = 1; owner.PDef = &classDef;
    CCreatureAction_ScorpionKingDeath a; memset(&a, 0, sizeof(a)); a.POwner.m_pItem = (CBaseObject*)&owner;
    // no special effects def: animation set, base initialised, nothing created
    g_effects = 0; a.Initialise();
    CHECK(g_setAnim == 1 && strcmp(g_animName, "FALL_TO_DEATH") == 0 && g_baseInit == 1); CHECK(g_getSubDef == 1 && g_countAtGet == 2 && classDef.m_refCount == 1); CHECK(g_lookups == 0 && g_creates == 0 && g_strCtors == 1 && g_strDtors == 1);
    // effects def without a DEATH entry: looked up, nothing created, def released
    g_effects = &effects; a.Initialise(); CHECK(g_lookups == 1 && strcmp(g_lookupName, "DEATH") == 0 && g_creates == 0 && effects.m_refCount == 1 && g_deleted == 0);
    // DEATH found: the effect created at the owner's position
    g_found = true; g_effect = 314; a.Initialise(); CHECK(g_creates == 1 && g_createDef == 314 && g_createPos == &g_position); CHECK(g_strCtors == 5 && g_strDtors == 5);
    // defs held only by the lookup are deleted
    classDef.m_refCount = 0; effects.m_refCount = 0; a.Initialise(); CHECK(g_deleted == 2 && g_creates == 2);
    if (fails == 0) printf("008c4170_TEST PASS\n"); else printf("008c4170_TEST FAIL\n");
    return fails;
}