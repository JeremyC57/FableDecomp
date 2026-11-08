// CCreatureAction_BriarRoseConcentrateInto::Initialise() -- unsheathe Briar Rose's melee weapon, animation POWER_UP_INTO,
// base Initialise; with a creature-mode manager and a live enemy add a CModeBriarRoseConcentrate(creature, enemy,
// Imposter); then spawn the def's TELEPORT_INTO special effect at her position
// Leaf: ?Initialise@CCreatureAction_BriarRoseConcentrateInto@@UAEXXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Layout: retail CCombatActionBase is 0x114 (Ego_r.pdb +0x18): Imposter +0x114 and PEnemy +0x115 (the PDB has the same
//   unaligned pair at +0x12c/+0x12d, so the class is packed to 1); CThing interface bitset +0x20, map +0x44, def +0x70.
// Shape: same-TU stand-ins for LowerBound 0x0040F020, GetSubDef 0x00669580 and GetEffectFromString 0x00669D30 (the landed
//   0x008C4B80 idiom); the counted def reference and the effect name are conditional temporaries of one && expression;
//   both mode-ctor pointers come through inline accessors (GetCreature, PEnemy.GetCreature) so the two GetPItem calls run
//   before any push (enemy first) and the Imposter byte is loaded last.
#include "fable_compiler.h"
class CTCBase;
#include "engine/C3DVector.h"
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
struct CDefVirtual {
    virtual ~CDefVirtual();                           // slot 0
    virtual void Destroy();                           // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                                  // +0x04
};
class CSpecialEffectsDef : public CDefBase { public:
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void* def, const CCharString& name);
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
template <class T> class CDefPointer { public:
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer()
    {
        if (m_p && --m_p->m_refCount == 0)
            m_p->Destroy();
    }
};
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00669580 (T = CSpecialEffectsDef)
};
void* Find_CDefClassBase_GetSubDef(const void* owner);
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
struct CDefClassReference {                           // a counted CDefClassBase reference (AddRef / Release)
    CDefClassBase* m_p;
    CDefClassReference(CDefClassBase* p) : m_p(p) { if (m_p) ++m_p->m_refCount; }
    ~CDefClassReference() { if (m_p && --m_p->m_refCount == 0) m_p->Destroy(); }
    CDefClassBase* operator->() const { return m_p; }
};
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                  // +0x20 interface-present bitset
    CInterfaceMap  m_interfaces;                      // +0x44
    unsigned char  _pad54[0x1c];
    CDefClassBase* m_pDef;                            // +0x70
    unsigned char  _pad74[0x91 - 0x74];
    unsigned char  m_flags91;                         // +0x91 bit 0: dead
    const C3DVector& GetPos() const;             // 0x004C73D0
    void UnsheatheMeleeWeapon_00666820();                      // 0x00666820 (CThingCreatureBase)
    template <class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CTCDParticleEmitter { public:
    static CThing* __fastcall Create(long a0, C3DVector const&a1, bool a2);   // 0x006E0880 (Ego_r.pdb ?Create@CTCDParticleEmitter@@SIPAVCThing@@JABVC3DVector@@_N@Z)
};

struct SCountBlock;
struct CModeBase { virtual void v00(); };
struct CCountedModePointer {
    CModeBase* m_pObject;
    SCountBlock* m_pCount;
    CCountedModePointer(CModeBase* object);           // 0x00718A00
    CCountedModePointer(const CCountedModePointer& other);
    ~CCountedModePointer();
};
struct CModeBriarRoseConcentrate : CModeBase {
    unsigned char _body[0xa0];                        // sizeof = 0xa4
    CModeBriarRoseConcentrate(CThing* creature, CThing* enemy, bool imposter);   // 0x008E78C0
};
class CTCCreatureModeManager { public:                       // interface 0x31
    bool AddMode(CCountedModePointer mode);           // 0x0073ACC0
};
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>
    CThing* GetCreature() const { return ((CThing*)GetPItem()); }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(((CThing*)p.GetPItem())->m_flags91 & 1))
            return true;
    }
    return false;
}
class CCombatActionBase { public:                            // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0x114 - 0x10];
    void SetCombatAnimation(const CCharString& name);   // 0x008574D0
    virtual void Initialise();                                // 0x00857520
    CThing* GetCreature() const { return ((CThing*)PCreature.GetPItem()); }
};
#pragma pack(push, 1)
class CCreatureAction_BriarRoseConcentrateInto : public CCombatActionBase { public:
    bool                    Imposter;                 // +0x114
    CIntelligentPointerCreature PEnemy;               // +0x115
    void Initialise();
};
#pragma pack(pop)

void CCreatureAction_BriarRoseConcentrateInto::Initialise()
{
    CTCCreatureModeManager* modes = 0;
    GetCreature()->UnsheatheMeleeWeapon_00666820();
    SetCombatAnimation("POWER_UP_INTO");
    CCombatActionBase::Initialise();
    if (GetCreature()->GetInterface(&modes, 0x31) && IS_THING_ALIVE(PEnemy))
        modes->AddMode(CCountedModePointer(new CModeBriarRoseConcentrate(GetCreature(), PEnemy.GetCreature(), Imposter)));
    CDefPointer<CSpecialEffectsDef> effects;
    long effectType = 0;
    bool found = CDefClassReference(GetCreature()->m_pDef)->GetSubDef(effects) &&
                 effects.m_p->GetEffectFromString("TELEPORT_INTO", effectType);
    if (found)
        CTCDParticleEmitter::Create(effectType, *(&GetCreature()->GetPos()), false);
}