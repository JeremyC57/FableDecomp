// NCreatureMode::CModeJackOfBladesFloat::OnActivate() -- spawn the FLOAT_POWER effect at the creature and attach it to the creature's target cursor point
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D9740, 293 bytes, plain `ret` (virtual).
// Callees: CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580, CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0, CSpecialEffectsDef::GetEffectFromString(const CCharString&, long&) 0x00669D30,
//   CThing::GetPos 0x004C73D0, __fastcall CreateEffectThing 0x006E0880, CBaseIntelligentPointer ctor 0x00A01B10 /
//   SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10, CTCDParticleEmitter::AttachToThing 0x006E0BE0.
// Layout: PEmitter2 +0x7C (CIntelligentPointer<CThing>); the creature (+0x04) definition +0x70, particle emitter +0x5C.
// Shape: GetSubDef and GetEffectFromString both have same-TU out-slot-only stand-in bodies, so neither the sub-def slot
//   nor the effect type escapes: the slot word is reused by the "FLOAT_POWER" string, the effect-type word by the
//   intelligent-pointer temporary and the bone-name string (frame 0xc); the definition is a counted reference in its own
//   block (add-ref, GetSubDef, release), the effect sub-def is released by hand at the end (known non-null: no test).
#include "engine/C3DVector.h"
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
class CTCDParticleEmitter {
public:
    static CThing* __fastcall Create(long a0, C3DVector const&a1, bool a2);   // 0x006E0880 (Ego_r.pdb ?Create@CTCDParticleEmitter@@SIPAVCThing@@JABVC3DVector@@_N@Z)
private: public:
    void AttachToThing(CThing& thing, long attachType, const CCharString& helperName, long helperIndex, float delay);   // 0x006E0BE0
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
    const C3DVector& GetPos() const;       // 0x004C73D0
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    __forceinline CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
struct CModeJackOfBladesFloat {
    void*                    __vftable;         // +0x00
    CThing*                  PCreature;         // +0x04
    unsigned char            _pad08[0x74 - 0x08];
    CIntelligentPointer<CThing> PEmitter;          // +0x74
    CIntelligentPointer<CThing> PEmitter2;         // +0x7c
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
            CThing* effect = CTCDParticleEmitter::Create(effectType, *(&PCreature->GetPos()), false);
            if (effect && !(effect->m_flags91 & 1)) {
                PEmitter2 = effect;
                if (effect->HasTC(0x4b)) {
                    CTCDParticleEmitter* emitter = effect->m_pParticleEmitter;
                    emitter->AttachToThing(*PCreature, 0xc, "TARGET_CURSOR_POINT", 0, 0.0f);
                }
            }
        }
        ReleaseDef(effects);
    }
}