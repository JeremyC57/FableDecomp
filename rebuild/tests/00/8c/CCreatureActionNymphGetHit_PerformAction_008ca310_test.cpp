// CCreatureAction_NymphGetHit::PerformAction() -- when the nymph's definition has a special-effects sub-def that maps "TELEPORT_POS" to an effect, spawn that effect at the nymph and attach its particle emitter to the nymph's "DUMMY_CENTRE" helper; then tell the CTCNymph component to fly around for the frames the action has left (TotalTime - FramesRunning)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CA310, length 387 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature +8), CDefClassBase::GetSubDef<CSpecialEffectsDef> @ 0x00669580 on the
// thing's definition (+0x70), CCharString ctor/dtor @ 0x0099EBF0 / 0x0099EAE0, CSpecialEffectsDef::GetEffectFromString(const CCharString&,
// long&) @ 0x00669D30, CThing::GetPosition @ 0x004C73D0, CreateEffectThing(long, const C3DVector*, bool) @ 0x006E0880 (`__fastcall`),
// CVectorMap::LowerBound @ 0x0040F020 (inlined lookups: 0x4B = CTCDParticleEmitter, bit 11 of +0x28, `p = 0; if (HasTC) p = GetTC`;
// 0xF5 = CTCNymph, bit 21 of +0x3c, tested), CTCDParticleEmitter::AttachToThing(CThing*, long, const CCharString&, long, float)
// @ 0x006E0BE0, CTCNymph::StartFlyingAround(long) @ 0x0080F5E0.
// Layout: CCreatureActionBase TotalTime +0x10, FramesRunning +0x1c (engine/CCreatureActionBase.h), PCreature +0x08.
// Shape: the landed CCreatureAction_TeleportIn 0x008B9F20 model -- the GetSubDef out-slot is a plain counted slot released by hand
// at the end (`dec [esi+4] ; call [edx+4]`); the lookup string is the implicit conversion inside a `found && ...` condition, so VC
// keeps a construction flag in bl and destroys the string before branching on the saved bool; the spawned effect is tested alive
// (null and bit 0 of +0x91) before the emitter lookup (`p = 0; if (HasTC) p = GetTC`). The nymph is found through the out-parameter
// GetInterface(T**, type) template (component left in eax, `mov ecx,eax` after the argument). GetSubDef and GetEffectFromString
// get same-TU stand-in bodies (write only the out slot / the `long&`), so neither the def slot nor the effect-type word escapes.
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
long Find_CSpecialEffectsDef_GetEffectFromString(const CSpecialEffectsDef* def, const CCharString& name);   // stand-in callee
// Stand-in body (the real one is out of line in retail): it writes the effect type only, so VC7.1 knows the
// effect-type word's address does not escape the call.
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
void* FindSubDef_CDefClassBase_GetSubDef(CDefClassBase* def);   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out)
{
    void* def = FindSubDef_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.m_p = (T*)def;
    return true;
}
#pragma auto_inline(on)
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* m_pDef;                      // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880
struct CTCDParticleEmitter {
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
struct CTCNymph {
    void StartFlyingAround(long frames);        // 0x0080F5E0
};
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CCreatureActionBase {                    // retail sizeof 0xa8
    virtual void v00();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    long                    TotalTime;          // +0x10
    long                    ActionTime;         // +0x14
    long                    AnimDelayTime;      // +0x18
    long                    FramesRunning;      // +0x1c
    unsigned char           _pad20[0xa8 - 0x20];
};
struct CCreatureAction_NymphGetHit : CCreatureActionBase {
    void PerformAction();
};

void CCreatureAction_NymphGetHit::PerformAction()
{
    CDefPointer<CSpecialEffectsDef> effectsSlot;
    long effectType = 0;
    CDefClassBase* def = PCreature.GetPItem()->m_pDef;
    bool found = def->GetSubDef(effectsSlot);
    if (found && effectsSlot.m_p->GetEffectFromString("TELEPORT_POS", effectType)) {
        CThing* effect = CreateEffectThing(effectType, &PCreature.GetPItem()->GetPosition(), false);
        if (effect && !(effect->m_flags91 & 1)) {
            CTCDParticleEmitter* emitter = 0;
            if (effect->HasTC(0x4b))
                emitter = effect->GetTC<CTCDParticleEmitter>(0x4b);
            emitter->AttachToThing(PCreature.GetPItem(), 10, "DUMMY_CENTRE", 0, 0.0f);
        }
    }
    CThing* creature = PCreature.GetPItem();
    CTCNymph* nymph;
    if (creature->GetInterface(&nymph, 0xf5))
        nymph->StartFlyingAround(TotalTime - FramesRunning);
    ReleaseDef(effectsSlot.m_p);
}

// ---- harness ----
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_strLive = 0, g_destroyed = 0, g_lookups = 0, g_creates = 0, g_attaches = 0, g_starts = 0;
static long g_effectType = 6, g_lastType = 0, g_lastAttachType = 0, g_lastFrames = 0;
static bool g_haveEffects = true;
static char g_helper[32];
static CThing* g_effectThing = 0;
static CThing* g_lastAttachThing = 0;
static const void* g_lastNymph = 0;
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
    return strcmp((const char*)name.m_pData, "TELEPORT_POS") == 0 ? g_effectType : -1;
}
void* FindSubDef_CDefClassBase_GetSubDef(CDefClassBase*)
{
    if (!g_haveEffects)
        return 0;
    ++g_effects.m_refCount;
    return &g_effects;
}
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type)
        ++it;
    return it;
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
void CTCNymph::StartFlyingAround(long frames) { ++g_starts; g_lastNymph = this; g_lastFrames = frames; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CCreatureActionBase::v00() { }
static CThing g_creature, g_effect;
static CTCDParticleEmitter g_particles;
static CTCNymph g_nymph;
static CInterfaceMap::SEntry g_creatureEntries[2], g_effectEntries[2];
int main()
{
    static CDefClassBase defClass;
    g_creature.m_pDef = &defClass;
    g_creatureEntries[0].type = 0x10; g_creatureEntries[0].component = 0;
    g_creatureEntries[1].type = 0xf5; g_creatureEntries[1].component = (CTCBase*)&g_nymph;
    g_creature.m_interfaces.first = g_creatureEntries; g_creature.m_interfaces.last = g_creatureEntries + 2;
    g_creature.m_presentBits[7] = 1UL << 21;
    g_effectEntries[0].type = 0x4b; g_effectEntries[0].component = (CTCBase*)&g_particles;
    g_effectEntries[1].type = 0x60; g_effectEntries[1].component = 0;
    g_effect.m_interfaces.first = g_effectEntries; g_effect.m_interfaces.last = g_effectEntries + 2;
    g_effect.m_presentBits[2] = 1UL << 11;
    g_effectThing = &g_effect;
    g_effects.m_refCount = 1;
    CCreatureAction_NymphGetHit action;
    memset(&action, 0, sizeof action);
    action.PCreature.m_pItem = &g_creature;
    action.TotalTime = 50; action.FramesRunning = 20;
    action.PerformAction();                            // spawn, attach, fly around for the frames left
    CHECK(g_lookups == 1 && g_creates == 1 && g_lastType == 6);
    CHECK(g_attaches == 1 && g_lastAttachThing == &g_creature && g_lastAttachType == 10 && strcmp(g_helper, "DUMMY_CENTRE") == 0);
    CHECK(g_starts == 1 && g_lastNymph == &g_nymph && g_lastFrames == 30);
    CHECK(g_strLive == 0 && g_effects.m_refCount == 1 && g_destroyed == 0);
    g_effect.m_flags91 = 1;                            // spawned dead: no attach
    action.FramesRunning = 45;
    action.PerformAction();
    CHECK(g_creates == 2 && g_attaches == 1 && g_starts == 2 && g_lastFrames == 5);
    g_effect.m_flags91 = 0; g_effectThing = 0;         // spawn failed
    action.PerformAction();
    CHECK(g_creates == 3 && g_attaches == 1 && g_starts == 3);
    g_effectType = -1;                                 // name not mapped: no spawn
    action.PerformAction();
    CHECK(g_lookups == 4 && g_creates == 3 && g_starts == 4 && g_effects.m_refCount == 1);
    g_haveEffects = false;                             // no sub-def: no lookup
    action.PerformAction();
    CHECK(g_lookups == 4 && g_starts == 5 && g_strLive == 0);
    g_creature.m_presentBits[7] = 0;                   // no nymph component: no fly-around
    action.PerformAction();
    CHECK(g_starts == 5);
    g_haveEffects = true; g_effects.m_refCount = 0;    // the slot owns the last reference
    action.PerformAction();
    CHECK(g_destroyed == 1 && g_effects.m_refCount == 0);
    if (fails == 0) printf("008ca310_TEST PASS\n");
    return fails;
}