// NCreatureMode::CModeScreamerSuperAttack::OnActivate() -- the stand-mode activation, suspend the creature's animation when it is in state 1, then (while the target is alive) look up the "SUPER_ATTACK_BEAM" effect in the thing definition's special-effects sub-def, spawn it at the creature and keep it in PLightningBeamParticleEmitter
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D8330, 285 bytes, plain `ret` (virtual).
// Callees: CModeStandBase::OnActivate 0x008621A0 (qualified base call, first), CThingAnimatable::SetSuspendAnimation(bool,
//   long) 0x00833A10, CBaseIntelligentPointer::GetPItem 0x00A01B50 (PTarget +0x74, twice), CDefClassBase::GetSubDef<
//   CSpecialEffectsDef>(CDefPointer<CSpecialEffectsDef>&) 0x00669580, CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0, CSpecialEffectsDef::GetEffectFromString(const CCharString&, long&) 0x00669D30 (result
//   ignored), CThing::GetPosition 0x004C73D0, __fastcall CreateEffectThing(long, const C3DVector*, bool) 0x006E0880,
//   CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor 0x00A01C10.
// Layout: Ego_r.pdb CModeScreamerSuperAttack (0x84): PTarget +0x74, PLightningBeamParticleEmitter +0x7C; CModeStandBase
//   ParentThing +0x04. Thing (retail offsets): state byte +0x10, PBaseDef +0x70, Dead bit 0 of +0x91.
// Shape: the def copy is add-ref'd around GetSubDef and released; the effect sub-def is released by hand at the end. The
// effect-type long is declared before the sub-def slot, and GetEffectFromString gets a same-TU stand-in body (writes only the
// `long&`), so VC7.1 knows the word does not escape and stores its zero first, after the def's null test, as retail.
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
template <class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer() { if (m_p && DecRefToZero(m_p->m_refCount)) m_p->Destroy(); }
};
struct CDefClassBase : CDefBase {
    bool GetSubDef(CDefPointer<CSpecialEffectsDef>& out);   // 0x00669580 GetSubDef<CSpecialEffectsDef>
};

// Same-TU stand-in for the out-of-line GetSubDef<CSpecialEffectsDef> (0x00669580, landed): it only fills the out
// pointer (retail's LTCG knew the slot does not escape, so the def stays in ebp for the final release).
CSpecialEffectsDef* LookupSpecialEffectsDef(const CDefClassBase* owner);   // the manager lookup inside 0x00669580
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CDefPointer<CSpecialEffectsDef>& out)
{
    CSpecialEffectsDef* def = LookupSpecialEffectsDef(this);
    if (def == 0)
        return false;
    ++def->m_refCount;
    out.m_p = def;
    return true;
}
#pragma auto_inline(on)

struct CThingAnimatable {
    void SetSuspendAnimation(bool suspend, long owner);   // 0x00833A10
};
struct CThing : CThingAnimatable {
    unsigned char  _pad00[0x10];
    unsigned char  State;                       // +0x10
    unsigned char  _pad11[0x70 - 0x11];
    CDefClassBase* PBaseDef;                    // +0x70
    unsigned char  _pad74[0x91 - 0x74];
    unsigned char  Dead : 1;                    // +0x91 bit 0
    const C3DVector& GetPosition() const;       // 0x004C73D0
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
    virtual void OnActivate();                  // slot 5, 0x008621A0
    CThing*       ParentThing;                  // +0x04
    unsigned char _body[0x60];
};

namespace NCreatureMode {

struct CModeScreamerSuperAttack : CModeStandBase {
    unsigned char            _pad68[0x74 - 0x68];
    CBaseIntelligentPointer  PTarget;           // +0x74 CIntelligentPointer<const CThing>
    CIntelligentPointerThing PLightningBeamParticleEmitter;   // +0x7C
    virtual void OnActivate();
};

void CModeScreamerSuperAttack::OnActivate()
{
    CModeStandBase::OnActivate();
    if (ParentThing->State == 1)
        ParentThing->SetSuspendAnimation(true, (long)this);
    CBaseIntelligentPointer* target = &PTarget;
    if (target->GetPItem() == 0 || target->GetPItem()->Dead)
        return;
    long effectType = 0;
    CDefPointer<CSpecialEffectsDef> slot;
    ParentThing->GetSpecialEffectsDef(slot);
    slot.m_p->GetEffectFromString("SUPER_ATTACK_BEAM", effectType);
    CThing* beam = CreateEffectThing(effectType, &ParentThing->GetPosition(), false);
    if (beam && !beam->Dead)
        PLightningBeamParticleEmitter = CIntelligentPointerThing(beam);
}

}

// ---- harness ----
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_strLive = 0, g_destroyed = 0, g_lookups = 0, g_creates = 0, g_baseActivate = 0, g_suspends = 0;
static long g_effectType = 4, g_lastType = -5, g_suspendOwner = 0;
static bool g_suspendFlag = false;
static CThing* g_beam = 0;
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
    return strcmp((const char*)name.m_pData, "SUPER_ATTACK_BEAM") == 0 ? g_effectType : -1;
}
CSpecialEffectsDef* LookupSpecialEffectsDef(const CDefClassBase*) { return &g_effects; }
void CThingAnimatable::SetSuspendAnimation(bool suspend, long owner) { ++g_suspends; g_suspendFlag = suspend; g_suspendOwner = owner; }
const C3DVector& CThing::GetPosition() const { return g_pos; }
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag)
{
    ++g_creates; g_lastType = type;
    CHECK(position == &g_pos && !flag);
    return g_beam;
}
void CBaseIntelligentPointer::v00() { }
CBaseIntelligentPointer::CBaseIntelligentPointer() : PItem(0) { }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { }
CThing* CBaseIntelligentPointer::GetPItem() const { return PItem; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { PItem = item; }
void CIntelligentPointerThing::v00() { }
void CModeStandBase::v00() { }
void CModeStandBase::v01() { }
void CModeStandBase::v02() { }
void CModeStandBase::v03() { }
void CModeStandBase::v04() { }
void CModeStandBase::OnActivate() { ++g_baseActivate; }
static CThing g_self, g_target, g_beamThing;
int main()
{
    static CDefClassBase defClass;
    defClass.m_refCount = 1;
    g_effects.m_refCount = 1;
    g_self.PBaseDef = &defClass;
    g_self.State = 1;
    g_beam = &g_beamThing;
    {
        NCreatureMode::CModeScreamerSuperAttack mode;
        mode.ParentThing = &g_self;
        mode.PTarget.SetPItem(&g_target);
        mode.OnActivate();                             // suspend, spawn the beam, keep it
        CHECK(g_baseActivate == 1 && g_suspends == 1 && g_suspendFlag && g_suspendOwner == (long)&mode);
        CHECK(g_lookups == 1 && g_creates == 1 && g_lastType == 4);
        CHECK(mode.PLightningBeamParticleEmitter.GetPItem() == &g_beamThing);
        CHECK(g_strLive == 0 && defClass.m_refCount == 1 && g_effects.m_refCount == 1 && g_destroyed == 0);
    }
    g_self.State = 0;
    {
        NCreatureMode::CModeScreamerSuperAttack mode;
        mode.ParentThing = &g_self;
        mode.OnActivate();                             // no target: only the base activation
        CHECK(g_baseActivate == 2 && g_suspends == 1 && g_creates == 1);
        mode.PTarget.SetPItem(&g_target);
        g_target.Dead = 1;                             // dead target
        mode.OnActivate();
        CHECK(g_baseActivate == 3 && g_creates == 1);
        g_target.Dead = 0; g_effectType = -1;          // name not mapped: type stays 0, spawn still tried
        g_beam = 0;
        mode.OnActivate();
        CHECK(g_creates == 2 && g_lastType == 0 && mode.PLightningBeamParticleEmitter.GetPItem() == 0);
        g_effectType = 9; g_beam = &g_beamThing; g_beamThing.Dead = 1;   // spawned dead: not kept
        mode.OnActivate();
        CHECK(g_creates == 3 && g_lastType == 9 && mode.PLightningBeamParticleEmitter.GetPItem() == 0);
        g_effects.m_refCount = 0;                      // the slot owns the last reference
        mode.OnActivate();
        CHECK(g_destroyed == 1 && g_effects.m_refCount == 0 && defClass.m_refCount == 1 && g_strLive == 0);
    }
    if (fails == 0) printf("008d8330_TEST PASS\n");
    return fails;
}