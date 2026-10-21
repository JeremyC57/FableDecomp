// Behaviour test for 008c4b80: src/008c4b80.cpp copied verbatim; Initialise runs the base Initialise keeping the body's
// facing angle, then (with the def's special-effects sub-def) spawns "EYE_GET_HIT" at the creature and, for a live
// effect with interface 0x4b, attaches its emitter to the creature's "EMITTER_EYE" helper; refs are released.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CCombatAction_StrikeResponseScorpionKing::Initialise() -- the scorpion king's hit response: run the generic strike-response Initialise while keeping the body's facing angle, then spawn the definition's "EYE_GET_HIT" special effect at the creature and attach its particle emitter to the creature's EMITTER_EYE helper
// Leaf: ?Initialise@CCombatAction_StrikeResponseScorpionKing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C4B80, length 302 bytes (plain `ret`, virtual).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature +8), the body's (+0x60) virtual slots 63 (+0xfc,
//          GetFacingAngleXY) / 66 (+0x108, SetFacingAngleXY), CCombatAction_GenericStrikeResponseBase::Initialise 0x00865F40,
//          CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580 (on a counted copy of the thing's def +0x70), CCharString
//          0x0099EBF0 / ~ 0x0099EAE0, CSpecialEffectsDef::GetEffectFromString 0x00669D30, CThing::GetPosition 0x004C73D0,
//          CreateEffectThing 0x006E0880 (__fastcall), CTCDParticleEmitter::AttachToThing 0x006E0BE0; def release slot 1.
// Model: the landed CCreatureAction_UndeadReturnToEarth::Initialise (0x008EC320) and CTCResurrectionItem::OnItemUsed
//          (0x007F04C0) models (the emitter is the effect thing's cached interface-0x4b component at +0x5c).
// Shape: GetSubDef and GetEffectFromString are same-TU noinline stand-in bodies that write only their out slots, so the
//          def slot and effectType do not escape: the frame packs into two words (the facing spill and effectType share one,
//          the "EYE_GET_HIT" temporary reuses the def slot word, "EMITTER_EYE" reuses effectType's).
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CDefVirtual {
    virtual ~CDefVirtual();                     // slot 0
    virtual void Destroy();                     // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                            // +0x04
};
struct CSpecialEffectsDef : CDefBase {
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
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
template <class T> struct CDefPointer {         // the out-slot GetSubDef fills (counted reference handed over)
    T* m_p;
    CDefPointer() : m_p(0) { }
};
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
struct CDefClassReference {                     // a counted CDefClassBase reference (AddRef / Release)
    CDefClassBase* m_p;
    CDefClassReference(CDefClassBase* p) : m_p(p) { if (m_p) ++m_p->m_refCount; }
    ~CDefClassReference() { if (m_p && --m_p->m_refCount == 0) m_p->Destroy(); }
    CDefClassBase* operator->() const { return m_p; }
};
struct CPhysicsBody {                           // the thing's physics body at +0x60
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062();
    virtual float GetFacingAngleXY();   // slot 63 (+0xfc)
    virtual void v064(); virtual void v065();
    virtual void SetFacingAngleXY(float angle);   // slot 66 (+0x108)
};
class CTCDParticleEmitter {
public:
    void AttachToThing(class CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];            // +0x20 std::bitset of interface types
    unsigned char  _pad44[0x5c - 0x44];
    CTCDParticleEmitter* PTCDriver;             // +0x5c (the cached interface-0x4b component)
    CPhysicsBody*  m_pBody;                     // +0x60
    unsigned char  _pad64[0x70 - 0x64];
    CDefClassBase* m_pDef;                      // +0x70
    unsigned char  _pad74[0x91 - 0x74];
    unsigned char  m_flags91;                   // +0x91 bit 0: dead
    CPhysicsBody* GetBody() const { return m_pBody; }
    const C3DVector& GetPosition() const;       // 0x004C73D0
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CCombatAction_GenericStrikeResponseBase {
    virtual void Initialise();                  // 0x00865F40
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
};
struct CCombatAction_StrikeResponseScorpionKing : CCombatAction_GenericStrikeResponseBase {
    virtual void Initialise();
};

void CCombatAction_StrikeResponseScorpionKing::Initialise()
{
    float facing = PCreature.GetPItem()->GetBody()->GetFacingAngleXY();
    CCombatAction_GenericStrikeResponseBase::Initialise();
    PCreature.GetPItem()->GetBody()->SetFacingAngleXY(facing);
    CDefPointer<CSpecialEffectsDef> effectsSlot;
    long effectType = 0;
    {
        CDefClassReference def(PCreature.GetPItem()->m_pDef);
        def->GetSubDef(effectsSlot);
    }
    CSpecialEffectsDef* effects = effectsSlot.m_p;
    if (effects) {
        if (effects->GetEffectFromString("EYE_GET_HIT", effectType)) {
            CThing* effect = CreateEffectThing(effectType, &PCreature.GetPItem()->GetPosition(), false);
            if (effect && !(effect->m_flags91 & 1) && effect->HasTC(0x4b)) {
                CTCDParticleEmitter* emitter = effect->PTCDriver;
                emitter->AttachToThing(PCreature.GetPItem(), 10, "EMITTER_EYE", 0, 0.0f);
            }
        }
        if (--effects->m_refCount == 0)
            effects->Destroy();
    }
}
// ---- harness ----
void CPhysicsBody::v000() { } void CPhysicsBody::v001() { } void CPhysicsBody::v002() { } void CPhysicsBody::v003() { } void CPhysicsBody::v004() { } void CPhysicsBody::v005() { } void CPhysicsBody::v006() { } void CPhysicsBody::v007() { } void CPhysicsBody::v008() { } void CPhysicsBody::v009() { } void CPhysicsBody::v010() { } void CPhysicsBody::v011() { } void CPhysicsBody::v012() { } void CPhysicsBody::v013() { } void CPhysicsBody::v014() { } void CPhysicsBody::v015() { } void CPhysicsBody::v016() { } void CPhysicsBody::v017() { } void CPhysicsBody::v018() { } void CPhysicsBody::v019() { } void CPhysicsBody::v020() { } void CPhysicsBody::v021() { } void CPhysicsBody::v022() { } void CPhysicsBody::v023() { } void CPhysicsBody::v024() { } void CPhysicsBody::v025() { } void CPhysicsBody::v026() { } void CPhysicsBody::v027() { } void CPhysicsBody::v028() { } void CPhysicsBody::v029() { } void CPhysicsBody::v030() { } void CPhysicsBody::v031() { } void CPhysicsBody::v032() { } void CPhysicsBody::v033() { } void CPhysicsBody::v034() { } void CPhysicsBody::v035() { } void CPhysicsBody::v036() { } void CPhysicsBody::v037() { } void CPhysicsBody::v038() { } void CPhysicsBody::v039() { } void CPhysicsBody::v040() { } void CPhysicsBody::v041() { } void CPhysicsBody::v042() { } void CPhysicsBody::v043() { } void CPhysicsBody::v044() { } void CPhysicsBody::v045() { } void CPhysicsBody::v046() { } void CPhysicsBody::v047() { } void CPhysicsBody::v048() { } void CPhysicsBody::v049() { } void CPhysicsBody::v050() { } void CPhysicsBody::v051() { } void CPhysicsBody::v052() { } void CPhysicsBody::v053() { } void CPhysicsBody::v054() { } void CPhysicsBody::v055() { } void CPhysicsBody::v056() { } void CPhysicsBody::v057() { } void CPhysicsBody::v058() { } void CPhysicsBody::v059() { } void CPhysicsBody::v060() { } void CPhysicsBody::v061() { } void CPhysicsBody::v062() { } void CPhysicsBody::v064() { } void CPhysicsBody::v065() { }
static int g_baseInit = 0, g_destroyed = 0, g_attach = 0, g_create = 0, g_strDtor = 0;
static float g_facing = 0.0f, g_facingAtBase = -1.0f, g_setFacing = -1.0f;
static char g_lookupName[64]; static char g_attachName[64];
static void* g_subDef = 0; static const void* g_findOwner = 0; static long g_findOwnerRef = -1;
static long g_createType = -1; static const C3DVector* g_createPos = 0; static bool g_createFlag = true;
static CThing* g_effect = 0; static const void* g_attachThis = 0; static CThing* g_attachThing = 0;
static long g_attachType = -1, g_attachIndex = -1; static float g_attachOffset = -1.0f;
static C3DVector g_pos = { 1.0f, 2.0f, 3.0f }; static const void* g_posThis = 0;
static CPhysicsBody* g_body = 0;
float CPhysicsBody::GetFacingAngleXY() { return g_facing; }
void CPhysicsBody::SetFacingAngleXY(float angle) { g_setFacing = angle; }
CCharString::CCharString(const char* text, long length) { PStringData = (void*)text; CHECK(length == -1); }
CCharString::~CCharString() { ++g_strDtor; }
CDefVirtual::~CDefVirtual() { }
void CDefVirtual::Destroy() { ++g_destroyed; }
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void* def, const CCharString& name)
{ strcpy(g_lookupName, (const char*)name.PStringData); return strcmp(g_lookupName, "EYE_GET_HIT") == 0 ? 33 : -1; }
void* Find_CDefClassBase_GetSubDef(const void* owner)
{ g_findOwner = owner; g_findOwnerRef = ((const CDefBase*)owner)->m_refCount; if (g_subDef) ++((CDefBase*)g_subDef)->m_refCount; return g_subDef; }
const C3DVector& CThing::GetPosition() const { g_posThis = this; return g_pos; }
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag)
{ ++g_create; g_createType = type; g_createPos = position; g_createFlag = flag; return g_effect; }
void CTCDParticleEmitter::AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset)
{ ++g_attach; g_attachThis = this; g_attachThing = thing; g_attachType = attachType;
  strcpy(g_attachName, (const char*)helperName.PStringData); g_attachIndex = helperIndex; g_attachOffset = offset; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CCombatAction_GenericStrikeResponseBase::Initialise() { ++g_baseInit; g_facingAtBase = g_facing; g_facing = 99.0f; }
static void reset() { g_baseInit = g_destroyed = g_attach = g_create = g_strDtor = 0; g_setFacing = -1.0f; g_facingAtBase = -1.0f;
  g_lookupName[0] = 0; g_attachName[0] = 0; g_findOwner = 0; g_findOwnerRef = -1; g_createType = -1; g_createPos = 0;
  g_createFlag = true; g_attachThis = 0; g_attachThing = 0; g_attachType = g_attachIndex = -1; g_attachOffset = -1.0f; g_posThis = 0; }
int main()
{
    CPhysicsBody body;
    CDefClassBase def; def.m_refCount = 1;
    CSpecialEffectsDef fx; fx.m_refCount = 1;
    CTCDParticleEmitter emitter;
    static unsigned char cbuf[sizeof(CThing)]; CThing& creature = *(CThing*)cbuf;
    creature.m_pBody = &body; creature.m_pDef = &def;
    static unsigned char ebuf[sizeof(CThing)]; CThing& effect = *(CThing*)ebuf;
    effect.PTCDriver = &emitter; effect.m_presentBits[2] = 1UL << 11;
    CCombatAction_StrikeResponseScorpionKing action; action.PCreature.m_pItem = &creature;

    reset(); g_facing = 1.5f; g_subDef = &fx; g_effect = &effect;
    action.Initialise();
    CHECK(g_baseInit == 1 && g_facingAtBase == 1.5f && g_setFacing == 1.5f);
    CHECK(g_findOwner == &def && g_findOwnerRef == 2 && def.m_refCount == 1);
    CHECK(strcmp(g_lookupName, "EYE_GET_HIT") == 0 && g_create == 1 && g_createType == 33 && g_createPos == &g_pos && !g_createFlag && g_posThis == &creature);
    CHECK(g_attach == 1 && g_attachThis == &emitter && g_attachThing == &creature && g_attachType == 10);
    CHECK(strcmp(g_attachName, "EMITTER_EYE") == 0 && g_attachIndex == 0 && g_attachOffset == 0.0f);
    CHECK(fx.m_refCount == 1 && g_destroyed == 0 && g_strDtor == 2);

    // no interface 0x4b: spawned, not attached
    reset(); g_facing = 2.0f; effect.m_presentBits[2] = 0;
    action.Initialise();
    CHECK(g_create == 1 && g_attach == 0 && g_setFacing == 2.0f);
    effect.m_presentBits[2] = 1UL << 11;

    // dead effect / no effect: not attached
    reset(); effect.m_flags91 = 1;
    action.Initialise();
    CHECK(g_create == 1 && g_attach == 0);
    effect.m_flags91 = 0;
    reset(); g_effect = 0;
    action.Initialise();
    CHECK(g_create == 1 && g_attach == 0);
    g_effect = &effect;

    // no sub-def: nothing spawned, def reference balanced
    reset(); g_subDef = 0;
    action.Initialise();
    CHECK(g_findOwner == &def && g_create == 0 && g_strDtor == 0 && def.m_refCount == 1 && g_destroyed == 0);

    // last reference to the sub-def: destroyed
    reset(); g_subDef = &fx; fx.m_refCount = 0;
    action.Initialise();
    CHECK(fx.m_refCount == 0 && g_destroyed == 1);

    // last reference to the thing def: destroyed by the counted reference
    reset(); g_subDef = 0; def.m_refCount = 0;
    action.Initialise();
    CHECK(def.m_refCount == 0 && g_destroyed == 1);

    if (fails == 0) printf("008c4b80_TEST PASS\n");
    return fails;
}