// CCreatureAction_WaspQueenDeath::Initialise() -- names the "FALL_TO_DEATH" animation, runs the base Initialise, then when the creature definition's special-effects sub-def maps "DEATH" to an effect, spawns it at the creature, keeps it as PDeathEffect and attaches its particle emitter to the creature's "TARGET_CURSOR_POINT" helper
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C4CB0, length 364 bytes (+ int3 padding).
// Callees: CCharString ctor/dtor @ 0x0099EBF0 / 0x0099EAE0, CCombatActionBase::SetCombatAnimation @ 0x008574D0 and Initialise @ 0x00857520,
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature +8), CDefClassBase::GetSubDef<CSpecialEffectsDef> @ 0x00669580 on a by-value
// copy of the thing's def (+0x70, add-ref / release around the call), CSpecialEffectsDef::GetEffectFromString @ 0x00669D30, CThing::
// GetPosition @ 0x004C73D0, CreateEffectThing @ 0x006E0880 (`__fastcall`), CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem
// @ 0x00A01B90 / dtor @ 0x00A01C10 (the `PDeathEffect = CIntelligentPointer<CThing>(effect)` temporary, vtable 0x01238C6C),
// CTCDParticleEmitter::AttachToThing @ 0x006E0BE0.
// Layout: retail CCombatActionBase is 0x114 bytes (PCreature +8): PDeathEffect is this class's +0x114. CThing +0x70 the def pointer,
// +0x91 dead bit, +0x28 present bits (type 0x4B = bit 11) with the particle-emitter component cached at +0x5c.
// Shape: the GetSubDef out-slot is a plain counted slot (the TeleportIn 0x008B9F20 model) copied into a named pointer and released by
// hand inside the `if (effects)` block (null test folded); the effect name is a named string in its own block with the result in bl.
// GetSubDef and GetEffectFromString get same-TU stand-in bodies (write the out slot / `long&` only), so VC7.1 knows neither the def
// slot nor the effect-type word escapes: the `PDeathEffect = CIntelligentPointer<CThing>(effect)` temporary (forceinline ctor: base
// ctor, vtable store, SetPItem) then overlays the dead effect-type word as in retail.
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
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
class CSpecialEffectsDef : public CDefBase { public:
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
template <class T> struct CDefSlot {         // the out-slot GetSubDef fills (counted reference handed over)
    T* m_p;
    CDefSlot() : m_p(0) { }
};
template <class T> __forceinline void ReleaseDef(T* def)
{
    if (def && --def->m_refCount == 0)
        def->Destroy();
}
long Find_CSpecialEffectsDef_GetEffectFromString(const CSpecialEffectsDef* def, const CCharString& name);   // stand-in callee
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long& type) const
{
    long found = Find_CSpecialEffectsDef_GetEffectFromString(this, name);
    if (found < 0)
        return false;
    type = found;
    return true;
}
#pragma auto_inline(on)
class CDefClassBase : public CDefBase { public:
    bool GetSubDef(CDefSlot<CSpecialEffectsDef>& out);   // 0x00669580
};
void* Find_CDefClassBase_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefSlot<CSpecialEffectsDef>& out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.m_p = (CSpecialEffectsDef*)def;
    return true;
}
#pragma auto_inline(on)
struct CDefPointer {                            // CDefPointer<const CDefClassBase> by value
    CDefClassBase* Object;
    CDefPointer(const CDefPointer& o) : Object(o.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
    ~CDefPointer()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Destroy();
    }
    CDefClassBase* operator->() const { return Object; }
};
class CThing;
class CTCDParticleEmitter { public:
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
class CThing { public:
    unsigned char _pad0[0x28];
    unsigned long m_presentBits2;               // +0x28 (interface types 0x40..0x5f)
    unsigned char _pad2c[0x5c - 0x2c];
    CTCDParticleEmitter* m_pParticleEmitter;    // +0x5c cached component
    unsigned char _pad60[0x70 - 0x60];
    CDefPointer PDef;                           // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    CDefPointer GetDef() const { return PDef; }
    bool GetTCParticleEmitter(CTCDParticleEmitter** out)
    {
        if (m_presentBits2 & (1UL << 11)) {
            *out = m_pParticleEmitter;
            return true;
        }
        return false;
    }
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                              // +4
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentThingPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentThingPointer() : CBaseIntelligentPointer() { }
    __forceinline CIntelligentThingPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentThingPointer& operator=(const CIntelligentThingPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();                  // 0x00857520
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0x114 - 0x10];
    void SetCombatAnimation(const CCharString& name);   // 0x008574D0
};
class CCreatureAction_WaspQueenDeath : public CCombatActionBase { public:
    CIntelligentThingPointer PDeathEffect;      // +0x114
    virtual void Initialise();
};

void CCreatureAction_WaspQueenDeath::Initialise()
{
    SetCombatAnimation("FALL_TO_DEATH");
    CCombatActionBase::Initialise();
    CDefSlot<CSpecialEffectsDef> effectsSlot;
    long effectType = 0;
    PCreature.GetPItem()->GetDef()->GetSubDef(effectsSlot);
    CSpecialEffectsDef* effects = effectsSlot.m_p;
    if (effects) {
        bool found;
        {
            CCharString name("DEATH");
            found = effects->GetEffectFromString(name, effectType);
        }
        if (found) {
            CThing* effect = CreateEffectThing(effectType, &PCreature.GetPItem()->GetPosition(), false);
            if (effect && !(effect->m_flags91 & 1)) {
                PDeathEffect = CIntelligentThingPointer(effect);
                CTCDParticleEmitter* emitter;
                if (effect->GetTCParticleEmitter(&emitter)) {
                    CCharString helper("TARGET_CURSOR_POINT");
                    emitter->AttachToThing(PCreature.GetPItem(), 0xc, helper, 0, 0.0f);
                }
            }
        }
        ReleaseDef(effectsSlot.m_p);
    }
}

// ---- harness ----
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_strLive = 0, g_destroyed = 0, g_lookups = 0, g_creates = 0, g_baseInit = 0, g_attaches = 0;
static long g_effectType = 5, g_lastType = 0, g_lastAttachType = 0;
static bool g_haveEffects = true;
static char g_anim[32], g_helper[32];
static CThing* g_effectThing = 0;
static CThing* g_lastAttachThing = 0;
static C3DVector g_pos = { 1.0f, 2.0f, 3.0f };
CCharString::CCharString(const char* text, long) : m_pData((void*)text) { ++g_strLive; }
CCharString::~CCharString() { --g_strLive; }
void CDefVirtual::v00() { }
void CDefVirtual::Destroy() { ++g_destroyed; }
static CSpecialEffectsDef g_effects;
long Find_CSpecialEffectsDef_GetEffectFromString(const CSpecialEffectsDef* def, const CCharString& name)
{
    ++g_lookups;
    CHECK(def == &g_effects);
    return strcmp((const char*)name.m_pData, "DEATH") == 0 ? g_effectType : -1;
}
void* Find_CDefClassBase_GetSubDef(const void*)
{
    if (!g_haveEffects)
        return 0;
    ++g_effects.m_refCount;
    return &g_effects;
}
const C3DVector& CThing::GetPosition() const { return g_pos; }
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag)
{
    ++g_creates; g_lastType = type;
    CHECK(position == &g_pos && !flag);
    return g_effectThing;
}
void CTCDParticleEmitter::AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset)
{
    ++g_attaches; g_lastAttachThing = thing; g_lastAttachType = attachType;
    strcpy(g_helper, (const char*)helperName.m_pData);
    CHECK(helperIndex == 0 && offset == 0.0f);
}
void CBaseIntelligentPointer::v00() { }
CBaseIntelligentPointer::CBaseIntelligentPointer() : PItem(0) { }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { }
CThing* CBaseIntelligentPointer::GetPItem() const { return PItem; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { PItem = item; }
void CIntelligentThingPointer::v00() { }
void CCombatActionBase::v00() { }
void CCombatActionBase::Initialise() { ++g_baseInit; }
void CCombatActionBase::SetCombatAnimation(const CCharString& name) { strcpy(g_anim, (const char*)name.m_pData); }
static unsigned char creatureMem[0x100], effectMem[0x100];
static CTCDParticleEmitter g_particles;
int main()
{
    static CDefClassBase defClass;
    defClass.m_refCount = 1;
    CThing* creature = (CThing*)creatureMem;
    *(CDefClassBase**)&creature->PDef = &defClass;
    CThing* effect = (CThing*)effectMem;
    effect->m_presentBits2 = 1UL << 11;
    effect->m_pParticleEmitter = &g_particles;
    g_effectThing = effect;
    g_effects.m_refCount = 1;
    {
        CCreatureAction_WaspQueenDeath action;
        action.PCreature.SetPItem(creature);
        action.Initialise();                           // effect spawned, kept and attached
        CHECK(strcmp(g_anim, "FALL_TO_DEATH") == 0 && g_baseInit == 1);
        CHECK(g_lookups == 1 && g_creates == 1 && g_lastType == 5);
        CHECK(action.PDeathEffect.GetPItem() == effect);
        CHECK(g_attaches == 1 && g_lastAttachThing == creature && g_lastAttachType == 0xc && strcmp(g_helper, "TARGET_CURSOR_POINT") == 0);
        CHECK(g_strLive == 0 && defClass.m_refCount == 1 && g_effects.m_refCount == 1 && g_destroyed == 0);
    }
    {
        CCreatureAction_WaspQueenDeath action;
        action.PCreature.SetPItem(creature);
        effect->m_flags91 = 1;                         // spawned dead: not kept
        action.Initialise();
        CHECK(g_creates == 2 && action.PDeathEffect.GetPItem() == 0 && g_attaches == 1 && g_effects.m_refCount == 1);
        effect->m_flags91 = 0; effect->m_presentBits2 = 0;   // no particle emitter: kept, not attached
        action.Initialise();
        CHECK(g_creates == 3 && action.PDeathEffect.GetPItem() == effect && g_attaches == 1);
        g_effectThing = 0; action.PDeathEffect.SetPItem(0);  // spawn failed
        action.Initialise();
        CHECK(g_creates == 4 && action.PDeathEffect.GetPItem() == 0 && g_attaches == 1);
        g_effectType = -1;                             // name not mapped: no spawn
        action.Initialise();
        CHECK(g_lookups == 5 && g_creates == 4 && g_effects.m_refCount == 1);
        g_haveEffects = false;                         // no sub-def: no lookup
        action.Initialise();
        CHECK(g_lookups == 5 && g_baseInit == 6 && defClass.m_refCount == 1 && g_strLive == 0);
        g_haveEffects = true; g_effects.m_refCount = 0;    // the slot owns the last reference
        action.Initialise();
        CHECK(g_destroyed == 1 && g_effects.m_refCount == 0);
    }
    if (fails == 0) printf("008c4cb0_TEST PASS\n");
    return fails;
}