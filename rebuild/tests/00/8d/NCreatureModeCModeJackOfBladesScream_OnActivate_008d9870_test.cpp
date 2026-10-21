// Behaviour test for 008d9870: src/008d9870.cpp copied verbatim; OnActivate clears creature flags 1|4 for a thing of type
// flag 2, takes the definition's special-effects sub-def, looks up "FLOAT_SCREAM", spawns the effect at the creature,
// stores it in PEmitter and (TC 0x4B) attaches it to the creature's TARGET_CURSOR_POINT; the sub-def is released.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NCreatureMode::CModeJackOfBladesScream::OnActivate() -- the Jack of Blades scream: clear two creature flags (for a thing of type flag 2), look up the "FLOAT_SCREAM" effect in the thing definition's special-effects sub-def, spawn it at the creature, keep it in PEmitter and attach its particle emitter to the creature's "TARGET_CURSOR_POINT" bone
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D9870, 312 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CDefClassBase::GetSubDef<CSpecialEffectsDef>(CDefPointer<CSpecialEffectsDef>&) 0x00669580, CCharString(const
//   char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CSpecialEffectsDef::GetEffectFromString(const CCharString&, long&)
//   0x00669D30, CThing::GetPosition 0x004C73D0, __fastcall CreateEffectThing(long, const C3DVector*, bool) 0x006E0880,
//   CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10,
//   CTCDParticleEmitter::AttachToThing(CThing*, long, const CCharString&, long, float) 0x006E0BE0.
// Layout: Ego_r.pdb CModeJackOfBladesScream (0xBC): PEmitter +0x9C; CModeStandBase ParentThing +0x04. Thing (retail
//   offsets, PDB - 0x10): UsingComponents bitset +0x20 (TC 0x4B = word 2 bit 11), PTCDriver +0x5C, ThingTypeFlags +0x6C,
//   PBaseDef +0x70, Dead bit 0 of +0x91; the creature flag byte +0xBC.
// Shape: GetSubDef and GetEffectFromString both have same-TU out-slot-only stand-in bodies, so neither the sub-def slot
//   nor the effect type escapes: the slot word is reused by the "FLOAT_SCREAM" string, the effect-type word by the
//   intelligent-pointer temporary and the bone-name string (frame 0xc); the creature-flag clear reads ParentThing into a
//   block-scoped local (keeps the scratch-register rotation); the thing's definition is copied (add-ref, GetSubDef,
//   release) in an inline helper; the effect sub-def is released by hand at the end (known non-null: no test).
struct C3DVector { float X, Y, Z; };

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
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
template <class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
};
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void* def, const CCharString& name);   // stand-in callee
// Stand-in body (out of line in retail): it writes the out type only, so VC7.1 knows the effect type's address does
// not escape the call.
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long& type) const
{
    long found = Lookup_CSpecialEffectsDef_GetEffectFromString(this, name);
    if (found < 0)
        return false;
    type = found;
    return true;
}
#pragma auto_inline(on)
struct CDefClassBase : CDefBase {
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00669580 (T = CSpecialEffectsDef)
};
void* Find_CDefClassBase_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.m_p = (T*)def;
    return true;
}
#pragma auto_inline(on)

struct CThing;
struct CTCDParticleEmitter {
    void AttachToThing(CThing* thing, long boneIndex, const CCharString& boneName, long flags, float scale);   // 0x006E0BE0
};

struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  UsingComponents[9];          // +0x20
    unsigned char  _pad44[0x5c - 0x44];
    void*          PTCDriver;                   // +0x5C
    unsigned char  _pad60[0x6c - 0x60];
    unsigned long  ThingTypeFlags;              // +0x6C
    CDefClassBase* PBaseDef;                    // +0x70
    unsigned char  _pad74[0x91 - 0x74];
    unsigned char  Dead : 1;                    // +0x91 bit 0
    unsigned char  _pad92[0xbc - 0x92];
    unsigned char  CreatureFlags;               // +0xBC (creature-derived member)
    const C3DVector& GetPosition() const;       // 0x004C73D0
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    void GetSpecialEffectsDef(CDefPointer<CSpecialEffectsDef>& out) const
    {
        CDefClassBase* def = PBaseDef;
        if (def)
            ++def->m_refCount;
        def->GetSubDef(out);
        if (def && --def->m_refCount == 0)
            def->Destroy();
    }
};

CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880

struct CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing() { }
    __forceinline CIntelligentPointerThing(CThing* item) { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CModeStandBase {                         // retail sizeof 0x68
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void OnActivate();                  // slot 5
    CThing*       ParentThing;                  // +0x04
    unsigned char _body[0x60];
};

namespace NCreatureMode {

struct CModeJackOfBladesScream : CModeStandBase {
    unsigned char            _pad68[0x9c - 0x68];
    CIntelligentPointerThing PEmitter;          // +0x9C
    virtual void OnActivate();
};

void CModeJackOfBladesScream::OnActivate()
{
    {
        CThing* thing = ParentThing;
        if (thing->ThingTypeFlags & 2)
            thing->CreatureFlags &= 0xfa;
    }
    CDefPointer<CSpecialEffectsDef> effectsSlot;
    long effectType = 0;
    ParentThing->GetSpecialEffectsDef(effectsSlot);
    CSpecialEffectsDef* effects = effectsSlot.m_p;
    if (effects) {
        if (effects->GetEffectFromString("FLOAT_SCREAM", effectType)) {
            CThing* effect = CreateEffectThing(effectType, &ParentThing->GetPosition(), false);
            if (effect && !effect->Dead) {
                PEmitter = CIntelligentPointerThing(effect);
                if (effect->HasTC(0x4b)) {
                    CTCDParticleEmitter* emitter = (CTCDParticleEmitter*)effect->PTCDriver;
                    emitter->AttachToThing(ParentThing, 0xc, "TARGET_CURSOR_POINT", 0, 0.0f);
                }
            }
        }
        if (DecRefToZero(effects->m_refCount))
            effects->Destroy();
    }
}

}

// ---- harness ----
static int g_strCtor = 0, g_strDtor = 0, g_destroyed = 0, g_ipDtor = 0, g_attach = 0, g_create = 0;
static char g_lastName[64]; static char g_attachName[64];
static void* g_subDef = 0; static const void* g_findOwner = 0;
static long g_createType = -1; static const C3DVector* g_createPos = 0; static bool g_createFlag = true;
static CThing* g_effect = 0; static CThing* g_attachThing = 0; static long g_attachType = -1, g_attachIndex = -1;
static float g_attachDelay = -1.0f; static const void* g_posThis = 0; static const void* g_attachThis = 0;
static C3DVector g_pos = { 1.0f, 2.0f, 3.0f };
CCharString::CCharString(const char* text, long length) { ++g_strCtor; m_pData = (void*)text; CHECK(length == -1); }
CCharString::~CCharString() { ++g_strDtor; }
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void* def, const CCharString& name)
{ strcpy(g_lastName, (const char*)name.m_pData); return strcmp(g_lastName, "FLOAT_SCREAM") == 0 ? 42 : -1; }
void* Find_CDefClassBase_GetSubDef(const void* owner) { g_findOwner = owner; if (g_subDef) ++((CDefBase*)g_subDef)->m_refCount; return g_subDef; }
void CDefVirtual::v00() { }
void CDefVirtual::Destroy() { ++g_destroyed; }
const C3DVector& CThing::GetPosition() const { g_posThis = this; return g_pos; }
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag)
{ ++g_create; g_createType = type; g_createPos = position; g_createFlag = flag; return g_effect; }
CBaseIntelligentPointer::CBaseIntelligentPointer() { PItem = 0; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_ipDtor; }
CThing* CBaseIntelligentPointer::GetPItem() const { return PItem; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { PItem = item; }
void CBaseIntelligentPointer::v00() { }
void CIntelligentPointerThing::v00() { }
void CTCDParticleEmitter::AttachToThing(CThing* thing, long a, const CCharString& name, long idx, float delay)
{ ++g_attach; g_attachThis = this; g_attachThing = thing; g_attachType = a; strcpy(g_attachName, (const char*)name.m_pData);
  g_attachIndex = idx; g_attachDelay = delay; }
static void reset() { g_strCtor = g_strDtor = g_destroyed = g_ipDtor = g_attach = g_create = 0; g_lastName[0] = 0;
  g_attachName[0] = 0; g_findOwner = 0; g_createType = -1; g_createPos = 0; g_createFlag = true; g_attachThing = 0;
  g_attachType = g_attachIndex = -1; g_attachDelay = -1.0f; g_posThis = 0; g_attachThis = 0; }
void CModeStandBase::v00() { } void CModeStandBase::v01() { } void CModeStandBase::v02() { }
void CModeStandBase::v03() { } void CModeStandBase::v04() { } void CModeStandBase::OnActivate() { }
int main()
{
    CDefClassBase defClass; defClass.m_refCount = 1;
    CSpecialEffectsDef fx; fx.m_refCount = 1;
    CTCDParticleEmitter emitter;
    CThing creature; memset(&creature, 0, sizeof creature); creature.PBaseDef = &defClass;
    creature.ThingTypeFlags = 2; creature.CreatureFlags = 0xff;
    CThing effect; memset(&effect, 0, sizeof effect); effect.PTCDriver = &emitter;
    effect.UsingComponents[2] = 1UL << 11;
    NCreatureMode::CModeJackOfBladesScream mode; mode.ParentThing = &creature; mode.PEmitter.PItem = 0;

    reset(); g_subDef = &fx; g_effect = &effect;
    mode.OnActivate();
    CHECK(creature.CreatureFlags == 0xfa);
    CHECK(g_findOwner == &defClass && defClass.m_refCount == 1 && fx.m_refCount == 1 && g_destroyed == 0);
    CHECK(strcmp(g_lastName, "FLOAT_SCREAM") == 0 && g_create == 1 && g_createType == 42 && g_createPos == &g_pos && !g_createFlag);
    CHECK(g_posThis == &creature && mode.PEmitter.PItem == &effect && g_ipDtor == 1);
    CHECK(g_attach == 1 && g_attachThis == &emitter && g_attachThing == &creature && g_attachType == 0xc);
    CHECK(strcmp(g_attachName, "TARGET_CURSOR_POINT") == 0 && g_attachIndex == 0 && g_attachDelay == 0.0f);
    CHECK(g_strCtor == 2 && g_strDtor == 2);

    // not a type-2 thing: flags kept; no TC: stored, not attached
    reset(); creature.ThingTypeFlags = 0; creature.CreatureFlags = 0xff; mode.PEmitter.PItem = 0; effect.UsingComponents[2] = 0;
    mode.OnActivate();
    CHECK(creature.CreatureFlags == 0xff && mode.PEmitter.PItem == &effect && g_attach == 0 && g_strCtor == 1);

    // dead effect: not stored
    reset(); mode.PEmitter.PItem = 0; effect.Dead = 1;
    mode.OnActivate();
    CHECK(g_create == 1 && mode.PEmitter.PItem == 0 && g_ipDtor == 0 && fx.m_refCount == 1);
    effect.Dead = 0;

    // no sub-def
    reset(); g_subDef = 0;
    mode.OnActivate();
    CHECK(g_strCtor == 0 && g_create == 0 && defClass.m_refCount == 1 && g_destroyed == 0);

    // last reference to the sub-def: destroyed at the end
    reset(); g_subDef = &fx; fx.m_refCount = 0;
    mode.OnActivate();
    CHECK(fx.m_refCount == 0 && g_destroyed == 1);

    if (fails == 0) printf("008d9870_TEST PASS\n");
    return fails;
}