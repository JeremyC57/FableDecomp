// NCreatureMode::CModeScreamerSuperAttack::OnActivate() -- the stand-mode activation, suspend the creature's animation when it is in state 1, then (while the target is alive) look up the "SUPER_ATTACK_BEAM" effect in the thing definition's special-effects sub-def, spawn it at the creature and keep it in PLightningBeamParticleEmitter
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D8330, 285 bytes, plain `ret` (virtual).
// Callees: CModeStandBase::OnActivate 0x008621A0 (qualified base call, first), CThingAnimatable::SetSuspendAnimation(bool,
//   long) 0x00833A10, CBaseIntelligentPointer::GetPItem 0x00A01B50 (PTarget +0x74, twice), CDefClassBase::GetSubDef<
//   CSpecialEffectsDef>(CDefPointer<CSpecialEffectsDef>&) 0x00669580, CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0, CSpecialEffectsDef::GetEffectFromString(const CCharString&, long&) 0x00669D30 (result
//   ignored), CThing::GetPos 0x004C73D0, __fastcall CreateEffectThing(long, const C3DVector*, bool) 0x006E0880,
//   CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor 0x00A01C10.
// Layout: Ego_r.pdb CModeScreamerSuperAttack (0x84): PTarget +0x74, PLightningBeamParticleEmitter +0x7C; CModeStandBase
//   ParentThing +0x04. Thing (retail offsets): state byte +0x10, PBaseDef +0x70, Dead bit 0 of +0x91.
// Shape: the def copy is add-ref'd around GetSubDef and released; the effect sub-def is released by hand at the end. The
// effect-type long is declared before the sub-def slot, and GetEffectFromString gets a same-TU stand-in body (writes only the
// `long&`), so VC7.1 knows the word does not escape and stores its zero first, after the def's null test, as retail.
class C3DVector { public: float X, Y, Z; };

class CCharString { public:
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
class CSpecialEffectsDef : public CDefBase { public:
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
template <class T> class CDefPointer { public:
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer() { if (m_p && DecRefToZero(m_p->m_refCount)) m_p->Destroy(); }
};
class CDefClassBase : public CDefBase { public:
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
class CThing : public CThingAnimatable { public:
    unsigned char  _pad00[0x10];
    unsigned char  State;                       // +0x10
    unsigned char  _pad11[0x70 - 0x11];
    CDefClassBase* PBaseDef;                    // +0x70
    unsigned char  _pad74[0x91 - 0x74];
    unsigned char  Dead : 1;                    // +0x91 bit 0
    const C3DVector& GetPos() const;       // 0x004C73D0
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

class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing() { }
    __forceinline CIntelligentPointerThing(CThing* item) { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(((CThing*)other.GetPItem())); return *this; }
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
    if (((CThing*)target->GetPItem()) == 0 || ((CThing*)target->GetPItem())->Dead)
        return;
    long effectType = 0;
    CDefPointer<CSpecialEffectsDef> slot;
    ParentThing->GetSpecialEffectsDef(slot);
    slot.m_p->GetEffectFromString("SUPER_ATTACK_BEAM", effectType);
    CThing* beam = CreateEffectThing(effectType, &ParentThing->GetPos(), false);
    if (beam && !beam->Dead)
        PLightningBeamParticleEmitter = CIntelligentPointerThing(beam);
}

}