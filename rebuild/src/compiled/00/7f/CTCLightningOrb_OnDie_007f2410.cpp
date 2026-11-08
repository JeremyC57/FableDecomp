// CTCLightningOrb::OnDie() -- when the orb dies, spawns its definition's LIGHTNING_ORB_HIT special effect at the orb's position (when the def's special-effects sub-def maps that name)
// Leaf: OnDie
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F2410, length 208 bytes (+ int3 padding); speed TU (no frame pointer).
// Callees: CDefClassBase::GetSubDef<CSpecialEffectsDef>(CDefPointer<CSpecialEffectsDef>&) @ 0x00669580, CCharString(const char*, long)
// @ 0x0099EBF0 ("LIGHTNING_ORB_HIT" 0x01271D48) and ~CCharString @ 0x0099EAE0, CSpecialEffectsDef::GetEffectFromString(const CCharString&,
// long&) const @ 0x00669D30 (catalog "CCRC::CalcAndLookup"), CThing::GetPos @ 0x004C73D0, CreateEffectThing(long, const C3DVector*,
// bool) @ 0x006E0880 (`__fastcall`), the defs' virtual slot 1 on release. CThing::PDef +0x70; ParentThing +0x04.
// Shape: the thing's definition taken by value (CThing::GetPDef: the copy's addref and its release at the end of the condition) and
// the `&&` string temporary; the effects sub-def is a zeroed out slot released by hand (null-tested) after the spawn. GetSubDef and
// GetEffectFromString get same-TU stand-in bodies that write only the out slot / the `long&`, so VC7.1 knows neither address
// escapes; that orders the zero stores of the effect-type word and the out slot as in retail.
#include "fable_compiler.h"
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
#include "engine/C3DVector.h"
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
};
class CSpecialEffectsDef : public CDefBase { public:
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
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
template <class T> struct CDefPointerSlot {      // the out-slot GetSubDef fills (counted reference handed over)
    T* m_p;
    CDefPointerSlot() : m_p(0) { }
};
template <class T> FABLE_FORCEINLINE void ReleaseDef(T* def)
{
    if (def && --def->m_refCount == 0)
        def->Destroy();
}
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CDefPointerSlot<T>& out);   // 0x00669580 (T = CSpecialEffectsDef)
};
void* Find_CDefClassBase_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointerSlot<T>& out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.m_p = (T*)def;
    return true;
}
#pragma auto_inline(on)
template <class T> class CDefPointer { public:          // a counted def reference held by value
    T* m_p;
    CDefPointer(const CDefPointer& other) : m_p(other.m_p)
    {
        if (m_p)
            ++m_p->m_refCount;
    }
    ~CDefPointer()
    {
        if (m_p && --m_p->m_refCount == 0)
            m_p->Destroy();
    }
    T* operator->() const { return m_p; }
};
class CThing { public:
    unsigned char _pad00[0x70];
    CDefPointer<CDefClassBase> PDef;             // +0x70
    CDefPointer<CDefClassBase> GetPDef() const { return PDef; }
    const C3DVector& GetPos() const;        // 0x004C73D0
};
class CTCDParticleEmitter { public:
    static CThing* __fastcall Create(long a0, C3DVector const&a1, bool a2);   // 0x006E0880 (Ego_r.pdb ?Create@CTCDParticleEmitter@@SIPAVCThing@@JABVC3DVector@@_N@Z)
};

class CTCLightningOrb { public:
    unsigned char _pad00[0x04];
    CThing*       ParentThing;                   // +0x04
    void OnDie();
};
void CTCLightningOrb::OnDie()
{
    CDefPointerSlot<CSpecialEffectsDef> effectsSlot;
    long effectType = 0;
    bool found = ParentThing->GetPDef()->GetSubDef(effectsSlot) && effectsSlot.m_p->GetEffectFromString("LIGHTNING_ORB_HIT", effectType);
    if (found)
        CTCDParticleEmitter::Create(effectType, *(&ParentThing->GetPos()), false);
    ReleaseDef(effectsSlot.m_p);
}