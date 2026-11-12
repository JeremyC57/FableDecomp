// CTCWillLightning::StartHandEffects() -- unless the spell is finished, create the primary and secondary hand effects on the caster (swapped for a left-handed cast) and attach them to the caster's right and left hand bones
// Leaf: ?StartHandEffects@CTCWillLightning
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A00E0, 542 bytes (+ int3 padding), plain `ret`, 8-aligned frame.
// Callees: CBaseIntelligentPointer() 0x00A01B10 / ~ 0x00A01C10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90;
//   CThing::GetPos() const 0x004C73D0; CreateParticleEffect(long, const C3DVector*) 0x0072AB70 (`__fastcall`);
//   LowerBound 0x0040F020 (interface 0x4B particle emitter); CTCDParticleEmitter::AttachToThing 0x006E0BE0 with
//   PRIMARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME 0x013BAC30 / SECONDARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME 0x013BAC34. CIntelligentPointer<CThing> vtable 0x01238C6C.
// Layout (retail CTCWillLightning, the PDB list/pointer members 0xc larger): WeaponState +0x0c, LeftHandAnim +0x10,
//   PrimaryHandEffect +0x20, SecondaryHandEffect +0x24, m_lsState +0x6c, m_cbpPlayer +0x70, m_tpLeftHandEffect +0x78,
//   m_tpRightHandEffect +0x80.
// Name fix (agentNF6 2026-11-10): 0x013BAC30 is ?PRIMARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME@@3VCCharString@@B (const CCharString "WEAPON_FOCUS_01"; Ego_r.pdb / FableWin.pdb public records and their dynamic initializer names).
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): the object binders below now use the PDB object names (Ego_r.pdb + FableWin.pdb
//   `dynamic initializer for '...'`, defining rows named by agentFAM09):
//   0x013BAC34 SECONDARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME (global, const; initializer 0x0121E620), was g_LeftHandAttachmentName.
#include "fable_compiler.h"
#include "engine/C3DVector.h"
class CTCBase;
class CBaseObject;
class CCharString { public:
    void* PStringData;
};
extern const CCharString PRIMARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME;   // 0x013BAC30
extern const CCharString SECONDARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME;    // 0x013BAC34

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad_0x54[0x3d];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template<class T> FABLE_FORCEINLINE T* GetTC(long wanted)
    {
        ETCInterfaceType type = (ETCInterfaceType)(wanted);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        return (T*)((it == map->end() || wanted < it->first) ? map->end() : it)->second;
    }
    template<class T> FABLE_FORCEINLINE T* GetInterface(long wanted)
    {
        if (HasTC(wanted))
            return GetTC<T>(wanted);
        return 0;
    }
    template<class T> FABLE_FORCEINLINE void GetInterface(T** ppOut, long wanted)
    {
        if (HasTC(wanted))
            *ppOut = GetTC<T>(wanted);
    }
    const C3DVector& GetPos() const;       // 0x004C73D0
};
CThing* __fastcall CreateParticleEffect(long effect, const C3DVector* pos);   // 0x0072AB70

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = ((CThing*)other.GetPItem());
        SetPItem((CBaseObject*)item);
        return *this;
    }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!(((CThing*)p.GetPItem())->m_flags91 & 1))
            return true;
    }
    return false;
}
class CTCDParticleEmitter { public:                    // interface 0x4B
    void AttachToThing(CThing& thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};

class CTCWillLightning { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[0x0c - 0x08];
    long WeaponState;                           // +0x0c
    bool LeftHandAnim;                          // +0x10
    unsigned char _pad11[0x20 - 0x11];
    long PrimaryHandEffect;                     // +0x20
    long SecondaryHandEffect;                   // +0x24
    unsigned char _pad28[0x6c - 0x28];
    long m_lsState;                             // +0x6c
    CIntelligentPointer<CThing> m_cbpPlayer;    // +0x70
    CIntelligentPointer<CThing> m_tpLeftHandEffect;    // +0x78
    CIntelligentPointer<CThing> m_tpRightHandEffect;   // +0x80
    bool IsRightHanded() const
    {
        switch (WeaponState) {
        case 0:
            return !LeftHandAnim;
        case 1:
            return false;
        }
        return true;
    }
    void StartHandEffects();
};

void CTCWillLightning::StartHandEffects()
{
    if (m_lsState == 6)
        return;
    if (IsRightHanded()) {
        m_tpRightHandEffect = CIntelligentPointer<CThing>(CreateParticleEffect(PrimaryHandEffect, &((CThing*)m_cbpPlayer.GetPItem())->GetPos()));
        m_tpLeftHandEffect = CIntelligentPointer<CThing>(CreateParticleEffect(SecondaryHandEffect, &((CThing*)m_cbpPlayer.GetPItem())->GetPos()));
    } else {
        m_tpRightHandEffect = CIntelligentPointer<CThing>(CreateParticleEffect(SecondaryHandEffect, &((CThing*)m_cbpPlayer.GetPItem())->GetPos()));
        m_tpLeftHandEffect = CIntelligentPointer<CThing>(CreateParticleEffect(PrimaryHandEffect, &((CThing*)m_cbpPlayer.GetPItem())->GetPos()));
    }
    if (IS_THING_ALIVE(m_tpRightHandEffect)) {
        CTCDParticleEmitter* emitter = ((CThing*)m_tpRightHandEffect.GetPItem())->GetInterface<CTCDParticleEmitter>(0x4b);
        if (emitter)
            emitter->AttachToThing(*(((CThing*)m_cbpPlayer.GetPItem())), 10, PRIMARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME, 0, 0.0f);
    }
    if (IS_THING_ALIVE(m_tpLeftHandEffect)) {
        CTCDParticleEmitter* emitter = 0;
        ((CThing*)m_tpLeftHandEffect.GetPItem())->GetInterface(&emitter, 0x4b);
        emitter->AttachToThing(*(((CThing*)m_cbpPlayer.GetPItem())), 10, SECONDARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME, 0, 0.0f);
    }
}