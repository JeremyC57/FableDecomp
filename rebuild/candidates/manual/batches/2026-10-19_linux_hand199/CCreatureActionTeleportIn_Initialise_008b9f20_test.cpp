#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#endif
struct C3DVector { float X, Y, Z; };

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                            // +0x04
};
struct CSpecialEffectsDef : CDefBase {
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
template <class T> struct CDefPointer {      // the out-slot GetSubDef fills (counted reference handed over)
    T* m_p;
    CDefPointer() : m_p(0) { }
};
template <class T> __forceinline void ReleaseDef(T* def)
{
    if (def && --def->m_refCount == 0)
        def->Destroy();
}
struct CDefClassBase : CDefBase {
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00669580 (T = CSpecialEffectsDef)
};

struct CThing {
    unsigned char  _pad0[0x70];
    CDefClassBase* m_pDef;                      // +0x70
    const C3DVector& GetPosition() const;       // 0x004C73D0
};

CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

struct CCombatActionBase {
    virtual void v00();
    virtual void Initialise();                  // 0x00857520
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0x114 - 0x10];
};

struct CCreatureAction_TeleportIn : CCombatActionBase {
    bool CreateEffects;                         // +0x114
    virtual void Initialise();
};

void CCreatureAction_TeleportIn::Initialise()
{
    CCombatActionBase::Initialise();
    if (CreateEffects) {
        CDefPointer<CSpecialEffectsDef> effectsSlot;
        long effectType = 0;
        CThing* creature = PCreature.GetPItem();
        CDefClassBase* def = creature->m_pDef;
        bool found = def->GetSubDef(effectsSlot);
        CSpecialEffectsDef* effects = effectsSlot.m_p;
        if (found && effects->GetEffectFromString("TELEPORT_IN", effectType))
            CreateEffectThing(effectType, &PCreature.GetPItem()->GetPosition(), false);
        ReleaseDef(effects);
    }
}

// ---- stubs ----
static int g_base = 0, g_suspend = 0, g_created = 0, g_destroyed = 0, g_strings = 0;
static long g_createdType = 0;
static const C3DVector* g_createdPos = 0;
static bool g_haveSubDef = true, g_haveName = true;
static char g_lastName[32];
CCharString::CCharString(const char* text, long) { m_pData = (void*)text; ++g_strings; }
CCharString::~CCharString() { --g_strings; }
void CDefVirtual::v00() { }
void CDefVirtual::Destroy() { ++g_destroyed; }
static CSpecialEffectsDef g_effects;
bool CSpecialEffectsDef::GetEffectFromString(const CCharString& n, long& type) const
{
    std::strcpy(g_lastName, (const char*)n.m_pData);
    if (!g_haveName) return false;
    type = 77; return true;
}
template <class T> bool CDefClassBase::GetSubDef(CDefPointer<T>& out)
{
    if (!g_haveSubDef) return false;
    out.m_p = &g_effects; ++g_effects.m_refCount; return true;
}
static C3DVector g_pos = { 1.0f, 2.0f, 3.0f };
const C3DVector& CThing::GetPosition() const { return g_pos; }
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag)
{ ++g_created; g_createdType = type; g_createdPos = position; if (flag) return 0; return 0; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CCombatActionBase::v00() { }
void CCombatActionBase::Initialise() { ++g_base; }
int main()
{
    static CDefClassBase thingDef;
    static CThing creature; creature.m_pDef = &thingDef;
    CCreatureAction_TeleportIn a;
    a.PCreature.m_pItem = &creature;
    a.CreateEffects = true;
    bool ok = true;
    int expectSuspend = 0;
    g_effects.m_refCount = 1;
    a.Initialise();
    expectSuspend = 0;
    if (g_base != 1 || g_created != 1 || g_createdType != 77 || g_createdPos != &g_pos || std::strcmp(g_lastName, "TELEPORT_IN") != 0
        || g_effects.m_refCount != 1 || g_destroyed != 0 || g_strings != 0 ) ok = false;
    g_haveName = false;               // the def has no such effect: nothing created, the def still released
    a.Initialise();
    expectSuspend *= 2;
    if (g_base != 2 || g_created != 1 || g_effects.m_refCount != 1 || g_strings != 0 ) ok = false;
    g_haveName = true; g_haveSubDef = false;  // no sub-def at all
    a.Initialise();
    if (g_base != 3 || g_created != 1) ok = false;
    g_haveSubDef = true; g_effects.m_refCount = 0;   // last reference: released through slot 1
    a.Initialise();
    if (g_created != 2 || g_destroyed != 1 || g_effects.m_refCount != 0) ok = false;
    a.CreateEffects = false;
    a.Initialise();
    if (g_created != 2 || g_base != 5) ok = false;
    if (!ok) { std::printf("008b9f20_TEST FAIL\n"); return 1; }
    std::printf("008b9f20_TEST PASS\n");
    return 0;
}
