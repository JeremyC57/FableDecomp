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
// Naming (NF147, 2026-11-20; q6408): moved onto the mode's shared header engine/NCreatureMode_CModeBriarRoseConcentrate.h (overlay opt-in
//   FABLE_RETAIL_NCREATUREMODE_CMODEBRIARROSECONCENTRATE_CTOR: the PDB constructor) with engine/CTCCreatureModeManager.h (ADDMODEPOINTER) and
//   engine/CCountedPointer.h (CALLED_CTOR: retail calls 0x00718A00); the local CCharString / CDefPointer / CDefClassBase / CSpecialEffectsDef
//   models and their same-TU GetSubDef / GetEffectFromString stand-ins replaced by fable_string.h, engine/CDefPointer.h, engine/CDefClassBase.h,
//   engine/CSpecialEffectsDef.h (GETEFFECT_BODY) and engine/CDefClassBase_GetSubDef.h (NOINLINE), the landed 0x008C4CB0 set; the local
//   CThing's def member respelled in place as that row's CDefPointer<const CDefClassBase> PDef / GetDef().
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_ADDMODEPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_NCREATUREMODE_CMODEBRIARROSECONCENTRATE_CTOR
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
class CTCBase;
#include "engine/C3DVector.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                  // +0x20 interface-present bitset
    CInterfaceMap  m_interfaces;                      // +0x44
    unsigned char  _pad54[0x1c];
    CDefPointer<const CDefClassBase> PDef;            // +0x70
    unsigned char  _pad74[0x91 - 0x74];
    unsigned char  m_flags91;                         // +0x91 bit 0: dead
    const C3DVector& GetPos() const;             // 0x004C73D0
    void UnsheatheMeleeWeapon_00666820();                      // 0x00666820 (CThingCreatureBase)
    CDefPointer<const CDefClassBase> GetDef() const { return PDef; }
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
class CBaseObject;
#include "engine/NCreatureMode_CModeBriarRoseConcentrate.h"
struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>
    CThingCreatureBase* GetCreature() const { return ((CThingCreatureBase*)GetPItem()); }
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
        modes->AddMode(CCountedPointer<CTCCreatureModeManager::CModeBase>(
            new NCreatureMode::CModeBriarRoseConcentrate(*GetCreature(), *PEnemy.GetCreature(), Imposter)));
    CDefPointer<const CSpecialEffectsDef> effects;
    long effectType = 0;
    bool found = GetCreature()->GetDef()->GetSubDef(&effects) &&
                 effects.Get()->GetEffectFromString("TELEPORT_INTO", effectType);
    if (found)
        CTCDParticleEmitter::Create(effectType, *(&GetCreature()->GetPos()), false);
}