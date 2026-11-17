// CTCMultiArrow::Init() -- load SPECIAL_ABILITIES_MULTI_ARROW_DEF, take the spell level from the owner's ability inventory, derive the shot/volley counts and the ammo-effect delay from the def, make the cast noise, put the effect on the weapon, create the first ammo effect, spawn the def's CAST_EFFECT at the owner and play the def's cast sound
// Leaf: ?Init@CTCMultiArrow@@QAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Naming wave 18: the global stand-in g_FramesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CC4D0, 563 bytes, plain `ret`.
// Callees: CCharString @ 0x0099EBF0 / 0x0099EAE0, CGameDefinitionManager::Get @ 0x0044C6B0 (__fastcall),
//   CDefinitionManager::GetGlobalIndexFromClassIndex @ 0x009AD390, GetDef<CSpecialAbilitiesMultiArrowDef> @ 0x007CC8E0,
//   const LowerBound @ 0x004365B0 (interfaces 0x6F, 0x13) / LowerBound @ 0x0040F020 (0x10), CTCInventoryAbilities::
//   GetAbilityLevel @ 0x005EDE93 (ability 0x11), __ftol2 @ 0x00BFEA70, CThingCreatureBase::MakeNoise @ 0x00662110,
//   CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CTCMultiArrow::ApplyEffectOnWeapon @ 0x007CC060 / CreateAmmoEffect
//   @ 0x007CC2B0, GetSubDef<CSpecialEffectsDef> @ 0x00669580, CalcAndLookup @ 0x00669D30, CThing::GetPos @ 0x004C73D0,
//   CreateParticleEffect @ 0x0072AB70 (__fastcall), CDefStringTable::GetString @ 0x009D49B0 (table 0x013CA828),
//   CTCSoundPlayer::PlayAttachedCriteria @ 0x0066BD00; the def's slot 1.
// Data: "SPECIAL_ABILITIES_MULTI_ARROW_DEF" 0x01235AC0, "SIMPLE_NOISE" 0x01268A5C, "CAST_EFFECT" 0x0126EDF8, frames per
//   second (long) 0x01375550.
// Layout: CTCMultiArrow as Ego_r.pdb (landed siblings): PDef +0x0C, NumberOfShots +0x10, NumberInVolley +0x14, NumOfVolleys
//   +0x18, SpellLevel +0x1C, CreateAmmoEffectCount/Delay/Timer +0x24/+0x28/+0x2C. CSpecialAbilitiesMultiArrowDef is 4 bytes
//   below Ego_r in retail: NumberOfShots[4] +0x3C, NumberInVolley[4] +0x4C, AmmoEffectCreationDelaySecs +0x5C, CastSound +0x60.
#include "fable_compiler.h"
#include "engine/EHeroAbility.h"   // NF34: ?GetAbilityLevel@CTCInventoryAbilities@@QBEJW4EHeroAbility@@@Z (Ego_r + FableWin)
class CThing;
class CTCBase;
// NF49 (round 7): the def classes come from the shared headers (were local CCharString / CDefPointeeBase / CDefPointer / CSpecialEffectsDef / CDefClassBase / CDefinitionManager / CGameDefinitionManager models with same-TU stand-in GetSubDef / GetEffectFromString bodies over made-up FindDef_CDefClassBase_GetSubDef_0 /
// Lookup_CSpecialEffectsDef_CalcAndLookup callees): the call at
// 0x00669580 is the member template ??$GetSubDef@VCSpecialEffectsDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCSpecialEffectsDef@@@@@Z
// (Ego_r.pdb / FableWin.pdb), its body visible and kept out of line (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE, the 0x0066BC10
// idiom), and CSpecialEffectsDef::GetEffectFromString 0x00669D30 is the header's same-TU noinline body
// (FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY, the 0x007941D0 idiom), so the caller's handles stay uncaptured as with the stand-ins.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/C3DVector.h"
#include "engine/CGameDefinitionManager.h"
// The manager calls are CGameDefinitionManager::Get() 0x0044C6B0, CDefinitionManager::GetGlobalIndexFromClassIndex 0x009AD390 and
// the member template GetDef<CSpecialAbilitiesMultiArrowDef> 0x007CC8E0 (FableWin.pdb
// ??$GetDef@VCSpecialAbilitiesMultiArrowDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCSpecialAbilitiesMultiArrowDef@@@@@Z;
// FABLE_RETAIL_CGAMEDEFINITIONMANAGER drops the default-path GetDef that would hide it).
class CSpecialAbilitiesMultiArrowDef : public CDefClassBase { public:
    unsigned char _pad25[0x3c - 0x25];                  // (the shared CDefClassBase is 0x25 bytes, packed)
    long  NumberOfShots[4];                             // +0x3c
    long  NumberInVolley[4];                            // +0x4c
    float AmmoEffectCreationDelaySecs;                  // +0x5c
    long  CastSound;                                    // +0x60 (CDefString)
};

class CDefStringTable { public:
    CCharString GetString(long id) const;                    // 0x009D49B0
};
extern CDefStringTable GDefStringTable;   // 0x013CA828 ?GDefStringTable@@3VCDefStringTable@@A (ESTABLISHED; naming wave 21)               // 0x013CA828
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

// agentNF15: the stand-in CInterfaceMap struct is the explicit specialisation of CVectorMap<ETCInterfaceType, CTCBase*> (the landed
//   0x0074EE90 idiom); 0x0040F020 is its LowerBound (NF14 / established spelling).
enum ETCInterfaceType { };
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class C> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> value_type;
    value_type* _M_start;                                   // +0x00 (the STLport vector)
    value_type* _M_finish;                                  // +0x04
    const value_type* LowerBound(const ETCInterfaceType& key) const;   // 0x004365B0
    value_type* LowerBound(const ETCInterfaceType& key);    // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

#pragma auto_inline(off)
// Same-TU body of the non-const CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound (0x0040F020): the key's address then
// does not escape, so the interface-0x10 key shares the effect index's frame word as in retail.
__declspec(noinline) CInterfaceMap::value_type* CInterfaceMap::LowerBound(const ETCInterfaceType& key)
{
    value_type* it = _M_start;
    long count = _M_finish - _M_start;
    while (count > 0) {
        long half = count / 2;
        if (it[half].first < key) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                    // +0x20
    CInterfaceMap m_interfaces;                        // +0x44
    FABLE_FORCEINLINE bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    FABLE_FORCEINLINE CTCBase* GetTC(long type)
    {
        ETCInterfaceType key = (ETCInterfaceType)type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(key);
        CInterfaceMap::value_type* end = map->_M_finish;
        return (it == end || type < it->first ? end : it)->second;
    }
    FABLE_FORCEINLINE CTCBase* GetTC(long type) const
    {
        ETCInterfaceType key = (ETCInterfaceType)type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::value_type* it = map->LowerBound(key);
        CInterfaceMap::value_type* end = map->_M_finish;
        return (it == end || type < it->first ? end : it)->second;
    }
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->_M_finish || interfaceType < it->first)
                it = map->_M_finish;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->_M_finish || interfaceType < it->first)
                it = map->_M_finish;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
    const C3DVector& GetPos() const;              // 0x004C73D0
    void MakeNoise(const CCharString& noise);          // 0x00662110 (CThingCreatureBase)
};
// NF49 (round 7): CreateParticleEffect 0x0072AB70 is the public static __fastcall
// ?CreateParticleEffect@CTCSpecialAbilities@@SIPAVCThing@@JABVC3DVector@@@Z (Ego_r.pdb / FableWin.pdb), declared by
// engine/CTCSpecialAbilities.h; was a free stand-in.
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#include "engine/CTCSpecialAbilities.h"

class CTCInventoryAbilities {
public:
    long GetAbilityLevel(EHeroAbility ability) const;                // 0x005EDE93 (EHeroAbility)
};
class CTCSoundPlayer {
public:
    unsigned long PlayAttachedCriteria(CCharString const&a0, bool a1, bool a2, bool a3, long a4);   // 0x0066BD00 (Ego_r.pdb ?PlayAttachedCriteria@CTCSoundPlayer@@QAEKABVCCharString@@_N11J@Z)
private:
};
#pragma pack(push, 1)
struct CTCWeaponHolder {                               // interface 0x13 (descriptive)
    unsigned char _pad00[0x141];
    CIntelligentPointer<CThing> PWeapon;                   // +0x141
};
#pragma pack(pop)

class CTCMultiArrow {
public:
    void*         __vftable;                           // +0x00
    CThing*       ParentThing;                         // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    CDefPointer<const CSpecialAbilitiesMultiArrowDef> PDef;   // +0x0c
    long          NumberOfShots;                       // +0x10
    long          NumberInVolley;                      // +0x14
    long          NumOfVolleys;                        // +0x18
    long          SpellLevel;                          // +0x1c
    bool          RemovedEffect;                       // +0x20
    long          CreateAmmoEffectCount;               // +0x24
    long          CreateAmmoEffectDelay;               // +0x28
    long          CreateAmmoEffectTimer;               // +0x2c
    void ApplyEffectOnWeapon(CThing* weapon);          // 0x007CC060
protected:
    void CreateAmmoEffect();                           // 0x007CC2B0 ?CreateAmmoEffect@CTCMultiArrow@@IAEXXZ (Ego_r/FableWin: protected)
public:
    void Init();
};

void CTCMultiArrow::Init()
{
    CThing* parent = ParentThing;
    long index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("SPECIAL_ABILITIES_MULTI_ARROW_DEF", 1);
    CGameDefinitionManager::Get().GetDef(index, &PDef);
    const CThing* parentThing = parent;
    if (parentThing->HasTC(0x6f))
        SpellLevel = ((CTCInventoryAbilities*)parentThing->GetTC(0x6f))->GetAbilityLevel(HERO_ABILITY_MULTI_ARROW);
    NumberOfShots = PDef->NumberOfShots[SpellLevel];
    NumberInVolley = PDef->NumberInVolley[SpellLevel];
    NumOfVolleys = NumberOfShots / NumberInVolley;
    CreateAmmoEffectCount = NumOfVolleys;
    CreateAmmoEffectDelay = CreateAmmoEffectTimer = (long)(CMainGameComponent::GetConstantFPS() * PDef->AmmoEffectCreationDelaySecs);
    parent->MakeNoise("SIMPLE_NOISE");
    CTCWeaponHolder* holder;
    ((const CThing*)ParentThing)->GetInterface(&holder, 0x13);
    ApplyEffectOnWeapon(holder->PWeapon.GetPItem());
    CreateAmmoEffect();
    CDefPointer<const CSpecialEffectsDef> effects;
    {
        long effect = 0;
        bool found = PDef->GetSubDef(&effects) && effects->GetEffectFromString("CAST_EFFECT", effect);
        if (found)
            CTCSpecialAbilities::CreateParticleEffect(effect, parent->GetPos());
    }
    CTCSoundPlayer* player;
    if (parent->GetInterface(&player, 0x10)) {
        CCharString sound = GDefStringTable.GetString(PDef->CastSound);
        ((CTCSoundPlayer*)player)->PlayAttachedCriteria(sound, 1, 0, 0, -1);
    }
}