// Behaviour test for 008d9740: src/008d9740.cpp copied verbatim; OnActivate takes the creature definition's special-effects
// sub-def, looks up "FLOAT_POWER", spawns that effect at the creature, stores it in PEmitter2 and (when it has the particle
// emitter TC 0x4B) attaches it to the creature's TARGET_CURSOR_POINT; the definition and the sub-def are released.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CModeJackOfBladesFloat::OnActivate() -- spawn the FLOAT_POWER effect at the creature and attach it to the creature's target cursor point
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D9740, 293 bytes, plain `ret` (virtual).
// Callees: CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580, CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0, CSpecialEffectsDef::GetEffectFromString(const CCharString&, long&) 0x00669D30,
//   CThing::GetPosition 0x004C73D0, __fastcall CreateEffectThing 0x006E0880, CBaseIntelligentPointer ctor 0x00A01B10 /
//   SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10, CTCDParticleEmitter::AttachToThing 0x006E0BE0.
// Layout: PEmitter2 +0x7C (CIntelligentPointer<CThing>); the creature (+0x04) definition +0x70, particle emitter +0x5C.
// Shape: GetSubDef and GetEffectFromString both have same-TU out-slot-only stand-in bodies, so neither the sub-def slot
//   nor the effect type escapes: the slot word is reused by the "FLOAT_POWER" string, the effect-type word by the
//   intelligent-pointer temporary and the bone-name string (frame 0xc); the definition is a counted reference in its own
//   block (add-ref, GetSubDef, release), the effect sub-def is released by hand at the end (known non-null: no test).
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
template <class T> struct CDefSlot {            // the out-slot GetSubDef fills (counted reference handed over)
    T* m_p;
    CDefSlot() : m_p(0) { }
};
template <class T> class CDefPointer { public:         // a counted def reference
    T* Object;
    CDefPointer(T* object) : Object(object) { if (Object) ++Object->m_refCount; }
    ~CDefPointer() { if (Object) { if (--Object->m_refCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
template <class T> __forceinline void ReleaseDef(T* def)
{
    if (--def->m_refCount == 0)
        def->Destroy();
}
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
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CDefSlot<T>& out);   // 0x00669580 (T = CSpecialEffectsDef)
};
void* Find_CDefClassBase_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefSlot<T>& out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.m_p = (T*)def;
    return true;
}
#pragma auto_inline(on)
class CThing;
class CTCDParticleEmitter { public:
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float delay);   // 0x006E0BE0
};
class CThing { public:
    unsigned char        _pad00[0x20];
    unsigned long        m_presentBits[9];      // +0x20 std::bitset of interface types
    unsigned char        _pad44[0x5c - 0x44];
    CTCDParticleEmitter* m_pParticleEmitter;    // +0x5c
    unsigned char        _pad60[0x70 - 0x60];
    CDefClassBase*       m_pDef;                // +0x70
    unsigned char        _pad74[0x91 - 0x74];
    unsigned char        m_flags91;             // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CModeJackOfBladesFloat {
    void*                    __vftable;         // +0x00
    CThing*                  PCreature;         // +0x04
    unsigned char            _pad08[0x74 - 0x08];
    CIntelligentPointerThing PEmitter;          // +0x74
    CIntelligentPointerThing PEmitter2;         // +0x7c
    void OnActivate();
};

void CModeJackOfBladesFloat::OnActivate()
{
    CDefSlot<CSpecialEffectsDef> effectsSlot;
    long effectType = 0;
    {
        CDefPointer<CDefClassBase> def(PCreature->m_pDef);
        def->GetSubDef(effectsSlot);
    }
    CSpecialEffectsDef* effects = effectsSlot.m_p;
    if (effects) {
        if (effects->GetEffectFromString("FLOAT_POWER", effectType)) {
            CThing* effect = CreateEffectThing(effectType, &PCreature->GetPosition(), false);
            if (effect && !(effect->m_flags91 & 1)) {
                PEmitter2 = effect;
                if (effect->HasTC(0x4b)) {
                    CTCDParticleEmitter* emitter = effect->m_pParticleEmitter;
                    emitter->AttachToThing(PCreature, 0xc, "TARGET_CURSOR_POINT", 0, 0.0f);
                }
            }
        }
        ReleaseDef(effects);
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
{ strcpy(g_lastName, (const char*)name.m_pData); return strcmp(g_lastName, "FLOAT_POWER") == 0 ? 42 : -1; }
void* Find_CDefClassBase_GetSubDef(const void* owner) { g_findOwner = owner; if (g_subDef) ++((CDefBase*)g_subDef)->m_refCount; return g_subDef; }
void CDefVirtual::v00() { }
void CDefVirtual::Destroy() { ++g_destroyed; }
const C3DVector& CThing::GetPosition() const { g_posThis = this; return g_pos; }
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag)
{ ++g_create; g_createType = type; g_createPos = position; g_createFlag = flag; return g_effect; }
CBaseIntelligentPointer::CBaseIntelligentPointer() { m_pItem = 0; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_ipDtor; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { m_pItem = item; }
void CBaseIntelligentPointer::v00() { }
void CIntelligentPointerThing::v00() { }
void CTCDParticleEmitter::AttachToThing(CThing* thing, long a, const CCharString& name, long idx, float delay)
{ ++g_attach; g_attachThis = this; g_attachThing = thing; g_attachType = a; strcpy(g_attachName, (const char*)name.m_pData);
  g_attachIndex = idx; g_attachDelay = delay; }
static void reset() { g_strCtor = g_strDtor = g_destroyed = g_ipDtor = g_attach = g_create = 0; g_lastName[0] = 0;
  g_attachName[0] = 0; g_findOwner = 0; g_createType = -1; g_createPos = 0; g_createFlag = true; g_attachThing = 0;
  g_attachType = g_attachIndex = -1; g_attachDelay = -1.0f; g_posThis = 0; g_attachThis = 0; }
int main()
{
    CDefClassBase defClass; defClass.m_refCount = 1;
    CSpecialEffectsDef fx; fx.m_refCount = 1;
    CTCDParticleEmitter emitter;
    CThing creature; memset(&creature, 0, sizeof creature); creature.m_pDef = &defClass;
    CThing effect; memset(&effect, 0, sizeof effect); effect.m_pParticleEmitter = &emitter;
    effect.m_presentBits[2] = 1UL << 11;
    static unsigned char modeBuf[sizeof(CModeJackOfBladesFloat)];   // no default ctor: zeroed storage
    CModeJackOfBladesFloat& mode = *(CModeJackOfBladesFloat*)modeBuf; mode.PCreature = &creature;

    // full path
    reset(); g_subDef = &fx; g_effect = &effect;
    mode.OnActivate();
    CHECK(g_findOwner == &defClass && defClass.m_refCount == 1 && fx.m_refCount == 1 && g_destroyed == 0);
    CHECK(strcmp(g_lastName, "FLOAT_POWER") == 0 && g_create == 1 && g_createType == 42 && g_createPos == &g_pos && !g_createFlag);
    CHECK(g_posThis == &creature && mode.PEmitter2.m_pItem == &effect && g_ipDtor == 1);
    CHECK(g_attach == 1 && g_attachThis == &emitter && g_attachThing == &creature && g_attachType == 0xc);
    CHECK(strcmp(g_attachName, "TARGET_CURSOR_POINT") == 0 && g_attachIndex == 0 && g_attachDelay == 0.0f);
    CHECK(g_strCtor == 2 && g_strDtor == 2);

    // no particle-emitter TC: stored, not attached
    reset(); mode.PEmitter2.m_pItem = 0; effect.m_presentBits[2] = 0;
    mode.OnActivate();
    CHECK(mode.PEmitter2.m_pItem == &effect && g_attach == 0 && g_strCtor == 1 && g_strDtor == 1);

    // dead effect: not stored
    reset(); mode.PEmitter2.m_pItem = 0; effect.m_flags91 = 1;
    mode.OnActivate();
    CHECK(g_create == 1 && mode.PEmitter2.m_pItem == 0 && g_ipDtor == 0 && fx.m_refCount == 1);
    effect.m_flags91 = 0;

    // no effect spawned
    reset(); g_effect = 0;
    mode.OnActivate();
    CHECK(g_create == 1 && mode.PEmitter2.m_pItem == 0 && g_attach == 0 && fx.m_refCount == 1);
    g_effect = &effect;

    // no sub-def: nothing looked up, the definition still released
    reset(); g_subDef = 0;
    mode.OnActivate();
    CHECK(g_strCtor == 0 && g_create == 0 && defClass.m_refCount == 1 && g_destroyed == 0);

    // last reference to the sub-def: destroyed at the end
    reset(); g_subDef = &fx; fx.m_refCount = 0;
    mode.OnActivate();
    CHECK(fx.m_refCount == 0 && g_destroyed == 1);

    if (fails == 0) printf("008d9740_TEST PASS\n");
    return fails;
}