// CModeDragonFireBeam::StartBeamEffect() -- once: find the dragon's mouth dummy, look up the BEAM_ATTACK effect in the owner's special-effects def, create it at the mouth into PBeamWeaponEffect and attach its particle emitter to the mouth dummy; mark the beam active
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail 0x008DB620, 481 bytes, plain `ret`.
// Shape: same-TU out-only stand-in bodies for GetSubDef (0x00669580) and GetEffectFromString (0x00669D30) keep the sub-def
//   and the effect type from escaping (one packed slot for effectType / the CIntelligentPointer temp / the key / the second
//   name, and the sub-def release keeps the pointer in ecx); operator= reads the item into a named local before SetPItem.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CTCGraphicAppearance::GetDummyObject
//   0x004C08C0, CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580, CSpecialEffectsDef::GetEffectFromString
//   0x00669D30 (label "CCRC::CalcAndLookup" is wrong), __fastcall effect-thing Create 0x006E0880, CBaseIntelligentPointer
//   ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10 (CIntelligentPointer vtable
//   0x01238C6C), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020, CTCDParticleEmitter::AttachToThing
//   0x006E0BE0.
// Strings: "DRAGON_MOUTH_DUMMY" 0x01285B4C, "BEAM_ATTACK" 0x0127203C.
// Layout: CModeDragonFireBeam (PDB): owner creature +0x04, PBeamWeaponEffect +0x84, BeamActive +0x90; CThing graphic
//   appearance +0x64, def +0x70 (counted: count +4, slot 1 Destroy), interface bitset +0x20, map +0x44, flags +0x91.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
#include "engine/C3DVector.h"
class CRightHandedSet { public: C3DVector Up, Forward; };
struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* position,
        CRightHandedSet* axes, float scale);          // 0x004C08C0
};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CDefBase {
    virtual ~CDefBase();
    virtual void Destroy();                           // slot 1
    long m_refCount;                                  // +0x04
};
class CSpecialEffectsDef : public CDefBase { public:
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
template <class T> class CDefPointer { public:
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer() { if (m_p && --m_p->m_refCount == 0) m_p->Destroy(); }
    T* operator->() const { return m_p; }
};
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void* def, const CCharString& name);   // stand-in callee
// Stand-in body (out of line in retail @ 0x00669D30): it writes the out value only, so VC7.1 knows the local's address
// does not escape the call (the 0x008D9740 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long& value) const
{
    long found = Lookup_CSpecialEffectsDef_GetEffectFromString(this, name);
    if (found < 0)
        return false;
    value = found;
    return true;
}
#pragma auto_inline(on)
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00669580
};
void* FindDef_CDefClassBase_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only, so VC7.1 knows the sub-def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out)
{
    void* def = FindDef_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.m_p = (T*)def;
    return true;
}
#pragma auto_inline(on)
struct CDefClassReference {
    CDefClassBase* m_p;
    CDefClassReference(CDefClassBase* p) : m_p(p) { if (m_p) ++m_p->m_refCount; }
    ~CDefClassReference() { if (m_p && --m_p->m_refCount == 0) m_p->Destroy(); }
    CDefClassBase* operator->() const { return m_p; }
};
class CThing;
class CTCDParticleEmitter {
public:
    static CThing* __fastcall Create(long a0, C3DVector const&a1, bool a2);   // 0x006E0880 (Ego_r.pdb ?Create@CTCDParticleEmitter@@SIPAVCThing@@JABVC3DVector@@_N@Z)
private:
public:
    void AttachToThing(CThing& thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                  // +0x20
    CInterfaceMap  m_interfaces;                      // +0x44
    unsigned char  _pad54[0x10];
    CTCGraphicAppearance* PGraphicAppearance;         // +0x64
    unsigned char  _pad68[0x70 - 0x68];
    CDefClassBase* m_pDef;                            // +0x70
    unsigned char  _pad74[0x91 - 0x74];
    unsigned char  m_flags91;                         // +0x91 bit 0: dead
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            *ppOut = (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }

    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

struct CModeDragonFireBeam {
    void*                       __vftable;
    CThing*                     Owner;                // +0x04
    unsigned char               _pad08[0x84 - 0x08];
    CIntelligentPointer<CThing> PBeamWeaponEffect;    // +0x84
    long                        LastExplodeFrame;     // +0x8C
    bool                        BeamActive;           // +0x90
    void StartBeamEffect();
};

void CModeDragonFireBeam::StartBeamEffect()
{
    if (BeamActive)
        return;
    CDefPointer<CSpecialEffectsDef> effects;
    long effectType = 0;
    C3DVector position;
    CRightHandedSet axes;
    {
        CCharString name("DRAGON_MOUTH_DUMMY");
        Owner->PGraphicAppearance->GetDummyObject(name, 0, 0, &position, &axes, 1.0f);
    }
    if (CDefClassReference(Owner->m_pDef)->GetSubDef(effects) && effects.m_p->GetEffectFromString("BEAM_ATTACK", effectType)) {
        PBeamWeaponEffect = CIntelligentPointer<CThing>(CTCDParticleEmitter::Create(effectType, *(&position), false));
        CTCDParticleEmitter* emitter;
        if (PBeamWeaponEffect.GetPItem() && !(PBeamWeaponEffect.GetPItem()->m_flags91 & 1)
            && PBeamWeaponEffect.GetPItem()->GetInterface(&emitter, 0x4b)) {
            CCharString name("DRAGON_MOUTH_DUMMY");
            emitter->AttachToThing(*Owner, 0x2a, name, 0, 0.0f);
        }
        BeamActive = true;
    }
}