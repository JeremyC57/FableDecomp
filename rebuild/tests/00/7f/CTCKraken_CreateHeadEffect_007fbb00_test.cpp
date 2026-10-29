// Behaviour test for 007fbb00: src/007fbb00.cpp copied verbatim; CreateHeadEffect drops the old head effect, looks the
// named effect up in the parent's special-effects sub-def (type 0 when absent), spawns it at the parent, stores it in
// PHeadEffect and, when alive, attaches TC 0x4B's emitter to the parent's EFFECT_WATER helper; the sub-def is released.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCKraken::CreateHeadEffect(const CCharString& name) -- replace the head effect: look the named effect up in the parent's special-effects sub-def, spawn it at the parent's position, keep it as the head effect and attach its particle emitter to the parent's "EFFECT_WATER" helper
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FBB00, length 297 bytes (+ int3 padding), `ret 4`. FableWin.pdb
// ?CreateHeadEffect@CTCKraken@@AAEXABVCCharString@@@Z (private).
// Callees: CTCKraken @ 0x007FB150 (no arguments, called first: drops the previous head effect; name descriptive),
// CDefClassBase::GetSubDef<CSpecialEffectsDef> @ 0x00669580, CSpecialEffectsDef::GetEffectFromString @ 0x00669D30 (result
// unused), CThing::GetPosition @ 0x004C73D0, __fastcall CreateEffectThing(long, const C3DVector*, bool) @ 0x006E0880,
// CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01B50/0x00A01C10 (CIntelligentPointer<CThing>
// vtable 0x01238C6C), the component-map LowerBound @ 0x0040F020, CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0 ("EFFECT_WATER"
// @ 0x01271CF0), CTCDParticleEmitter::AttachToThing(CThing*, long, const CCharString&, long, float) @ 0x006E0BE0.
// Shape: the landed UndeadReturnToEarth::Initialise (0x008EC320) def lookup: the out-slot and the effect type zeroed from the
// ebx zero register, the def copied into a named pointer (ebp) and released by hand at the end; the new effect goes through
// an intelligent-pointer temporary and CIntelligentPointer::operator= (SetPItem(other.GetPItem())); IS_THING_ALIVE is the
// double-GetPItem helper; the emitter pointer is preloaded with 0 (it reuses ebx) and the 0x4b key and the helper-name
// temporary are homed in the dead `name` argument slot. GetSubDef and GetEffectFromString are same-TU noinline stand-in
// bodies that write only their out slots (so the out-slot and effect type do not escape and their words are reused), the
// def class is split into a local, and the "EFFECT_WATER" name is a named local in its own block (its word and the 0x4b key
// temporary then share the dead argument slot).
// Layout: Ego_r.pdb CTCKraken: ParentThing +0x04, PHeadEffect +0x28 (CIntelligentPointer<CThing>); CThing PBaseDef +0x70,
// UsingComponents +0x20 (0x4b = word 2 bit 11), components map +0x44, dead bit +0x91. No engine header.
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
template <class T> class CDefPointer { public:
    T* m_p;
    CDefPointer() : m_p(0) { }
};
template <class T> __forceinline void ReleaseDef(T* def)
{
    if (def && --def->m_refCount == 0)
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
struct CTCBase_Retail;
struct STCEntry_Retail { long m_kind; CTCBase_Retail* m_pTC; };
struct CTCMap_Retail {
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind);   // 0x0040F020
    __forceinline CTCBase_Retail* Find(long kind)
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->m_kind) ? m_pLast : it)->m_pTC;
    }
};
class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                 // +0x20 UsingComponents
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                       // +0x44
    unsigned char  _pad50[0x70 - 0x50];
    CDefClassBase* m_pDef;                      // +0x70
    unsigned char  _pad74[0x91 - 0x74];
    unsigned char  m_flags91;                   // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880

class CBaseObject;
struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointed-to item
    CBaseObject* m_pItem;                       // +4 (after the vptr)
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
class CTCDParticleEmitter { public:
    void AttachToThing(CThing& thing, long boneIndex, const CCharString& boneName, long flags, float scale);   // 0x006E0BE0
};

class CTCKraken { public:
    void*   __vftable;                          // +0x00
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x28 - 0x08];
    CIntelligentPointer PHeadEffect;            // +0x28
    void RemoveHeadEffect();                    // 0x007FB150
    void CreateHeadEffect(const CCharString& name);
};

void CTCKraken::CreateHeadEffect(const CCharString& name)
{
    RemoveHeadEffect();
    CDefPointer<CSpecialEffectsDef> effectsSlot;
    long effectType = 0;
    CDefClassBase* defClass = ParentThing->m_pDef;
    bool found = defClass->GetSubDef(effectsSlot);
    CSpecialEffectsDef* effects = effectsSlot.m_p;
    if (found)
        effects->GetEffectFromString(name, effectType);
    PHeadEffect = CIntelligentPointer(CreateEffectThing(effectType, &ParentThing->GetPosition(), false));
    if (IS_THING_ALIVE(PHeadEffect)) {
        CTCDParticleEmitter* emitter = 0;
        CThing* effect = PHeadEffect.GetPItem();
        if (effect->HasTC(0x4b))
            emitter = (CTCDParticleEmitter*)effect->m_tcs.Find(0x4b);
        {
        CCharString boneName("EFFECT_WATER");
        emitter->AttachToThing(*ParentThing, 0x4a, boneName, 0, 0.0f);
        }
    }
    ReleaseDef(effects);
}
// ---- harness ----
static int g_strCtor = 0, g_strDtor = 0, g_destroyed = 0, g_ipDtor = 0, g_attach = 0, g_create = 0, g_remove = 0;
static char g_lastName[64]; static char g_attachName[64];
static void* g_subDef = 0; static const void* g_findOwner = 0;
static long g_createType = -1; static const C3DVector* g_createPos = 0; static bool g_createFlag = true;
static CThing* g_effect = 0; static CThing* g_attachThing = 0; static long g_attachType = -1, g_attachFlags = -1;
static float g_attachScale = -1.0f; static const void* g_posThis = 0; static const void* g_attachThis = 0;
static C3DVector g_pos = { 1.0f, 2.0f, 3.0f };
CCharString::CCharString(const char* text, long length) { ++g_strCtor; m_pData = (void*)text; CHECK(length == -1); }
CCharString::~CCharString() { ++g_strDtor; }
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void* def, const CCharString& name)
{ strcpy(g_lastName, (const char*)name.m_pData); return strcmp(g_lastName, "KRAKEN_HEAD") == 0 ? 42 : -1; }
void* Find_CDefClassBase_GetSubDef(const void* owner) { g_findOwner = owner; if (g_subDef) ++((CDefBase*)g_subDef)->m_refCount; return g_subDef; }
void CDefVirtual::v00() { }
void CDefVirtual::Destroy() { ++g_destroyed; }
const C3DVector& CThing::GetPosition() const { g_posThis = this; return g_pos; }
STCEntry_Retail* CTCMap_Retail::LowerBound(const unsigned long& kind)
{ STCEntry_Retail* it = m_pFirst; while (it != m_pLast && (unsigned long)it->m_kind < kind) ++it; return it; }
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag)
{ ++g_create; g_createType = type; g_createPos = position; g_createFlag = flag; return g_effect; }
CBaseIntelligentPointer::CBaseIntelligentPointer() { m_pItem = 0; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_ipDtor; }
CThing* CBaseIntelligentPointer::GetPItem() const { return (CThing*)m_pItem; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { m_pItem = (CBaseObject*)item; }
void CTCDParticleEmitter::AttachToThing(CThing& thing_ref, long boneIndex, const CCharString& boneName, long flags, float scale)
{ CThing* thing = &thing_ref; ++g_attach; g_attachThis = this; g_attachThing = thing; g_attachType = boneIndex; strcpy(g_attachName, (const char*)boneName.m_pData);
  g_attachFlags = flags; g_attachScale = scale; }
void CTCKraken::RemoveHeadEffect() { ++g_remove; }
static void reset() { g_strCtor = g_strDtor = g_destroyed = g_ipDtor = g_attach = g_create = g_remove = 0; g_lastName[0] = 0;
  g_attachName[0] = 0; g_findOwner = 0; g_createType = -1; g_createPos = 0; g_createFlag = true; g_attachThing = 0;
  g_attachType = g_attachFlags = -1; g_attachScale = -1.0f; g_posThis = 0; g_attachThis = 0; }
int main()
{
    CDefClassBase defClass; defClass.m_refCount = 1;
    CSpecialEffectsDef fx; fx.m_refCount = 1;
    CTCDParticleEmitter emitter;
    STCEntry_Retail entries[3] = { { 0x10, 0 }, { 0x4b, (CTCBase_Retail*)&emitter }, { 0x60, 0 } };
    static unsigned char parentBuf[sizeof(CThing)]; CThing& parent = *(CThing*)parentBuf; parent.m_pDef = &defClass;
    static unsigned char effectBuf[sizeof(CThing)]; CThing& effect = *(CThing*)effectBuf;
    effect.m_tcMask[2] = 1UL << 11; effect.m_tcs.m_pFirst = entries; effect.m_tcs.m_pLast = entries + 3; effect.m_tcs.m_pEnd = entries + 3;
    static unsigned char krakenBuf[sizeof(CTCKraken)]; CTCKraken& kraken = *(CTCKraken*)krakenBuf;
    kraken.ParentThing = &parent; kraken.PHeadEffect.m_pItem = 0;
    CCharString name("KRAKEN_HEAD");

    reset(); g_subDef = &fx; g_effect = &effect;
    kraken.CreateHeadEffect(name);
    CHECK(g_remove == 1 && g_findOwner == &defClass && fx.m_refCount == 1 && g_destroyed == 0);
    CHECK(strcmp(g_lastName, "KRAKEN_HEAD") == 0 && g_create == 1 && g_createType == 42 && g_createPos == &g_pos && !g_createFlag);
    CHECK(g_posThis == &parent && kraken.PHeadEffect.m_pItem == (CBaseObject*)&effect && g_ipDtor == 1);
    CHECK(g_attach == 1 && g_attachThis == &emitter && g_attachThing == &parent && g_attachType == 0x4a);
    CHECK(strcmp(g_attachName, "EFFECT_WATER") == 0 && g_attachFlags == 0 && g_attachScale == 0.0f);
    CHECK(g_strCtor == 1 && g_strDtor == 1);

    // unknown name: effect type stays 0
    reset(); CCharString other("NOPE");
    kraken.CreateHeadEffect(other);
    CHECK(g_create == 1 && g_createType == 0 && g_attach == 1);

    // dead effect: stored but not attached
    reset(); kraken.PHeadEffect.m_pItem = 0; effect.m_flags91 = 1;
    kraken.CreateHeadEffect(name);
    CHECK(g_create == 1 && kraken.PHeadEffect.m_pItem == (CBaseObject*)&effect && g_attach == 0 && g_strCtor == 0);
    effect.m_flags91 = 0;

    // no effect created: nothing attached
    reset(); g_effect = 0; kraken.PHeadEffect.m_pItem = 0;
    kraken.CreateHeadEffect(name);
    CHECK(g_create == 1 && kraken.PHeadEffect.m_pItem == 0 && g_attach == 0);
    g_effect = &effect;

    // no sub-def: type 0, still spawned and attached
    reset(); g_subDef = 0;
    kraken.CreateHeadEffect(name);
    CHECK(g_lastName[0] == 0 && g_create == 1 && g_createType == 0 && g_attach == 1 && g_destroyed == 0);

    // last reference to the sub-def: destroyed at the end
    reset(); g_subDef = &fx; fx.m_refCount = 0;
    kraken.CreateHeadEffect(name);
    CHECK(fx.m_refCount == 0 && g_destroyed == 1);

    if (fails == 0) printf("007fbb00_TEST PASS\n");
    return fails;
}