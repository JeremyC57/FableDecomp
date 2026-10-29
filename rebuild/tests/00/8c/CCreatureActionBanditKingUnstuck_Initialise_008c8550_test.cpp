#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __declspec(x)
#define __forceinline inline
#endif
// CCreatureAction_BanditKingUnstuck::Initialise() -- clear Knockdown, animation BK_UNSTUCK, drop creature mode 0x53, close the twin-blade "stuck" hit location and reopen the twin-blade one, create the UNSTUCK special effect at the creature and attach it through its particle emitter, then the base Initialise
// Leaf: ?Initialise@CCreatureAction_BanditKingUnstuck
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C8550, 586 bytes (592 with padding), virtual Initialise, plain `ret`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CCombatActionBase::SetCombatAnimation
//   0x008574D0, CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::LowerBound 0x0040F020 (inline interface lookups
//   0x31 CTCCreatureModeManager, 0x7F CTCHitLocations, 0x4B CTCDParticleEmitter), CTCCreatureModeManager::RemoveMode
//   0x0073ABA0, CTCHitLocations::GetPHitLocationFromName 0x00815820, CHitLocation::SetEnabled 0x00816390,
//   CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580, CCRC::CalcAndLookup 0x00669D30, CThing::GetPosition 0x004C73D0,
//   the special-effect Create(long, const C3DVector&, bool) 0x006E0880 (__fastcall),
//   CTCDParticleEmitter::AttachToThing 0x006E0BE0, CCombatActionBase::Initialise 0x00857520.
// Data: "BK_UNSTUCK" 0x01281AEC, "HIT_LOCATION_TWINBLADE_STUCK" 0x01281ACC, "HIT_LOCATION_TWINBLADE" 0x01281AB4,
//   "UNSTUCK" 0x01281AAC, "" 0x0122D70E.
// Layout (retail, local model): CCombatActionBase is 0x114 bytes (PCreature +0x08); Knockdown +0x114 (PDB +0x12c, the
//   engine header carries PDB offsets); CThing def +0x70, dead flag +0x91 bit 0.
// Shape: the landed BanditKingStuck::Initialise (0x008C8380) model. The mode-manager and hit-location components use the
//   out-parameter GetInterface with no null test, so the miss path reads the never-written out slot (retail reads the
//   spilled `this` word); the emitter is the `T* c = 0; if (HasTC) c = GetTC(ternary)` accessor (`xor esi,esi` before the
//   bit test); the created effect is used directly (no intelligent-pointer member); the "" bone name is the implicit
//   const CCharString& conversion, built before the other AttachToThing arguments.
class C3DVector { public: float X, Y, Z; };
class CTCBase;
class CThing;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};

class CSpecialEffectsDef { public:
    virtual void v00();
    virtual void Destroy();                           // slot 1
    long RefCount;                                    // +0x04
    bool CalcAndLookup(const CCharString& name, long* value);   // 0x00669D30
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
long Lookup_CSpecialEffectsDef_CalcAndLookup(const void* def, const CCharString& name);   // stand-in callee
// Stand-in body (out of line in retail @ 0x00669D30): it writes the out value only, so VC7.1 knows the local's address
// does not escape the call (the 0x008D9740 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::CalcAndLookup(const CCharString& name, long* value)
{
    long found = Lookup_CSpecialEffectsDef_CalcAndLookup(this, name);
    if (found < 0)
        return false;
    *value = found;
    return true;
}
#pragma auto_inline(on)
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CSpecialEffectsDef>* out);   // 0x00669580
};
void* FindDef_CDefClassBase_GetSubDef_0();             // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x008C4170 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CSpecialEffectsDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* m_pDef;                            // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;             // 0x004C73D0
    CDefClassBase* GetDef() const { return m_pDef; }
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline T* GetTCIfPresent(long interfaceType)
    {
        T* component = 0;
        if (HasTC(interfaceType))
            component = GetTC<T>(interfaceType);
        return component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
CThing* __fastcall CreateSpecialEffect(long effect, const C3DVector& position, bool attached);   // 0x006E0880

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                  // +4
    CThing* GetPItem() const;                         // 0x00A01B50
};
class CTCDParticleEmitter { public:
    void AttachToThing(CThing& thing, long boneIndex, const CCharString& boneName, long a, float b);   // 0x006E0BE0
};
class CTCCreatureModeManager { public:                       // interface type 0x31
    void RemoveMode(unsigned long mode);              // 0x0073ABA0
};
struct CHitLocation {
    void SetEnabled(bool enabled);                    // 0x00816390
};
class CTCHitLocations { public:                              // interface type 0x7F
    CHitLocation* GetPHitLocationFromName(const CCharString& name);   // 0x00815820
};
class CCombatActionBase { public:                            // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0x114 - 0x10];
    void SetCombatAnimation(const CCharString& name);   // 0x008574D0
    void Initialise();                                // 0x00857520
};
class CCreatureAction_BanditKingUnstuck : public CCombatActionBase { public:
    bool Knockdown;                                   // +0x114 (PDB +0x12c)
    bool NoTarget;                                    // +0x115
    void Initialise();
};

void CCreatureAction_BanditKingUnstuck::Initialise()
{
    Knockdown = false;
    {
        CCharString name("BK_UNSTUCK");
        SetCombatAnimation(name);
    }
    CTCCreatureModeManager* modes;
    PCreature.GetPItem()->GetInterface(&modes, 0x31);
    modes->RemoveMode(0x53);
    CTCHitLocations* hitLocations;
    PCreature.GetPItem()->GetInterface(&hitLocations, 0x7f);
    CHitLocation* stuck = hitLocations->GetPHitLocationFromName("HIT_LOCATION_TWINBLADE_STUCK");
    CHitLocation* blade = hitLocations->GetPHitLocationFromName("HIT_LOCATION_TWINBLADE");
    if (stuck)
        stuck->SetEnabled(false);
    if (blade)
        blade->SetEnabled(true);
    CDefPointer<CSpecialEffectsDef> effects;
    long effect = 0;
    if (PCreature.GetPItem()->GetDef()->GetSubDef(&effects) && effects->CalcAndLookup("UNSTUCK", &effect)) {
        CThing* created = CreateSpecialEffect(effect, PCreature.GetPItem()->GetPosition(), false);
        if (created && !(created->m_flags91 & 1)) {
            CTCDParticleEmitter* emitter = created->GetTCIfPresent<CTCDParticleEmitter>(0x4b);
            emitter->AttachToThing(*PCreature.GetPItem(), 0x2a, "", 0, 0);
        }
    }
    CCombatActionBase::Initialise();
}

// ---- test stubs ----
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("CHECK failed line %d: %s\n", __LINE__, #c); fails++; } } while (0)
static int g_strings, g_strDtors;
CCharString::CCharString(const char* text, long) { PStringData = (void*)text; g_strings++; }
CCharString::~CCharString() { g_strDtors++; }
static int g_destroys;
void CSpecialEffectsDef::v00() {}
void CSpecialEffectsDef::Destroy() { g_destroys++; }
static long g_lookup; static const char* g_lookupName;
long Lookup_CSpecialEffectsDef_CalcAndLookup(const void* def, const CCharString& name) { (void)def; g_lookupName = (const char*)name.PStringData; return g_lookup; }
static void* g_subDef;
void* FindDef_CDefClassBase_GetSubDef_0() { return g_subDef; }
static C3DVector g_pos = { 1.0f, 2.0f, 3.0f };
const C3DVector& CThing::GetPosition() const { return g_pos; }
static CThing* g_created; static long g_createdEffect; static const C3DVector* g_createdAt; static int g_createdAttached, g_creates;
CThing* __fastcall CreateSpecialEffect(long effect, const C3DVector& position, bool attached)
{ g_createdEffect = effect; g_createdAt = &position; g_createdAttached = attached; g_creates++; return g_created; }
void CBaseIntelligentPointer::v00() {}
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
static void* g_attachThis; static CThing* g_attachThing; static long g_attachBone, g_attachA, g_attachB; static const char* g_attachName; static int g_attaches;
void CTCDParticleEmitter::AttachToThing(CThing& thing_ref, long boneIndex, const CCharString& boneName, long a, float b)
{ CThing* thing = &thing_ref; g_attachThis = this; g_attachThing = thing; g_attachBone = boneIndex; g_attachName = (const char*)boneName.PStringData; g_attachA = a; g_attachB = b; g_attaches++; }
static void* g_modeThis; static unsigned long g_removed; static int g_removes;
void CTCCreatureModeManager::RemoveMode(unsigned long mode) { g_modeThis = this; g_removed = mode; g_removes++; }
static CHitLocation g_stuckLoc, g_bladeLoc; static int g_stuckOn = -1, g_bladeOn = -1; static void* g_hitThis;
static bool g_haveStuck, g_haveBlade;
CHitLocation* CTCHitLocations::GetPHitLocationFromName(const CCharString& name)
{
    g_hitThis = this;
    const char* n = (const char*)name.PStringData;
    if (strcmp(n, "HIT_LOCATION_TWINBLADE_STUCK") == 0) return g_haveStuck ? &g_stuckLoc : 0;
    if (strcmp(n, "HIT_LOCATION_TWINBLADE") == 0) return g_haveBlade ? &g_bladeLoc : 0;
    return 0;
}
void CHitLocation::SetEnabled(bool enabled) { if (this == &g_stuckLoc) g_stuckOn = enabled; else if (this == &g_bladeLoc) g_bladeOn = enabled; }
static const char* g_anim; static int g_baseInits;
void CCombatActionBase::v00() {}
void CCombatActionBase::SetCombatAnimation(const CCharString& name) { g_anim = (const char*)name.PStringData; }
void CCombatActionBase::Initialise() { g_baseInits++; }

static CThing g_creature, g_effect;
static CSpecialEffectsDef g_fx; static CDefClassBase g_classDef; static CTCDParticleEmitter g_particles;
static CTCCreatureModeManager g_modes; static CTCHitLocations g_hits;
static void Reset() { g_strings = g_strDtors = g_destroys = g_creates = g_attaches = g_baseInits = g_removes = 0; g_anim = 0; g_lookupName = 0; g_attachThis = 0; g_stuckOn = g_bladeOn = -1; }
int main()
{
    CThing* creature = &g_creature; CThing* effect = &g_effect;
    creature->m_pDef = &g_classDef;
    CInterfaceMap::SEntry ce[] = { { 0x10, 0 }, { 0x31, (CTCBase*)&g_modes }, { 0x7f, (CTCBase*)&g_hits } };
    creature->m_interfaces.first = ce; creature->m_interfaces.last = ce + 3;
    creature->m_presentBits[0x31 >> 5] |= 1UL << (0x31 & 31);
    creature->m_presentBits[0x7f >> 5] |= 1UL << (0x7f & 31);
    CInterfaceMap::SEntry e[] = { { 0x10, 0 }, { 0x4b, (CTCBase*)&g_particles }, { 0x60, 0 } };
    effect->m_interfaces.first = e; effect->m_interfaces.last = e + 3;
    effect->m_presentBits[0x4b >> 5] = 1UL << (0x4b & 31);
    static CCreatureAction_BanditKingUnstuck action; CCreatureAction_BanditKingUnstuck* a = &action;
    a->PCreature.m_pItem = creature;
    // no special-effects sub-def: mode dropped, hit locations swapped, nothing created
    a->Knockdown = true; g_subDef = 0; g_haveStuck = g_haveBlade = true; Reset();
    a->Initialise();
    CHECK(a->Knockdown == false && strcmp(g_anim, "BK_UNSTUCK") == 0 && g_baseInits == 1);
    CHECK(g_removes == 1 && g_modeThis == &g_modes && g_removed == 0x53 && g_hitThis == &g_hits);
    CHECK(g_stuckOn == 0 && g_bladeOn == 1 && g_creates == 0 && g_lookupName == 0 && g_strings == g_strDtors);
    // hit locations missing: no SetEnabled; sub-def found but UNSTUCK unknown: def released, nothing created
    g_haveStuck = g_haveBlade = false; g_subDef = &g_fx; g_fx.RefCount = 1; g_lookup = -1; Reset();
    a->Initialise();
    CHECK(g_stuckOn == -1 && g_bladeOn == -1 && strcmp(g_lookupName, "UNSTUCK") == 0 && g_creates == 0 && g_destroys == 1 && g_fx.RefCount == 0);
    CHECK(g_baseInits == 1 && g_strings == g_strDtors);
    // only the twin-blade location: reopened; effect creation fails: nothing attached, def kept (2 -> 1)
    g_haveBlade = true; g_fx.RefCount = 2; g_lookup = 33; g_created = 0; Reset();
    a->Initialise();
    CHECK(g_stuckOn == -1 && g_bladeOn == 1 && g_creates == 1 && g_createdEffect == 33 && g_createdAt == &g_pos && g_createdAttached == 0);
    CHECK(g_attaches == 0 && g_destroys == 0 && g_fx.RefCount == 1 && g_baseInits == 1);
    // created effect is dead: not attached
    g_fx.RefCount = 2; g_created = effect; effect->m_flags91 = 1; Reset();
    a->Initialise();
    CHECK(g_creates == 1 && g_attaches == 0 && g_baseInits == 1);
    // live effect with its particle emitter: attached to the creature at bone 0x2a with "" name
    effect->m_flags91 = 0; g_fx.RefCount = 2; Reset();
    a->Initialise();
    CHECK(g_attaches == 1 && g_attachThis == &g_particles && g_attachThing == creature);
    CHECK(g_attachBone == 0x2a && strcmp(g_attachName, "") == 0 && g_attachA == 0 && g_attachB == 0 && g_baseInits == 1);
    CHECK(g_strings == g_strDtors && g_fx.RefCount == 1);
    if (fails == 0) printf("008c8550_TEST PASS\n"); else printf("008c8550_TEST FAIL\n");
    return fails;
}