// CTCInventoryWeapons::OnSerialise(CPersistContext& context, bool loading) -- persists the active weapons: on save, refreshes the active def indices, records whether each active weapon is carried in the primary slot and writes the confiscated / previously wielded weapon def names; on load, reads the carried flags and maps the def names back to global indices; both ways transfer the active def indices
// Leaf: OnSerialise@CTCInventoryWeapons
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad21[0x44 - 0x21] removed.
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C3A95, 1020 bytes, `ret 8`. Frame-pointer size TU: optimize("s", on).
// Callees: RefreshActiveWeaponData 0x005C30D3, CTCInventoryBase::OnSerialise 0x005BFD5E, CPersistContext saving test 0x004AEF50
//   (labelled IsBinary) / Transfer<bool>(name, value, default) 0x004045C0 / Transfer(name, bool*) 0x004107C0 / Transfer<CCharString>
//   0x00411010 / Transfer<CDefIndex>(name, CDefIndex&) 0x005CC300, IS_THING_ALIVE 0x004CC340, CBaseIntelligentPointer::GetPItem 0x00A01B50, bitset test
//   0x00410DE0, const LowerBound 0x004365B0 (carrying 0x46), CTCCarrying::IsCarryingThing 0x006A1060 / IsCarriedThingInPrimarySlot
//   0x006A1490, CGameDefinitionManager::Get 0x0044C6B0 / GetDefNameFromGlobalIndex 0x009ACCC0 / GetDefGlobalIndexFromName 0x009AD410,
//   CDefString::operator CCharString 0x00415DD0, CCharString 0x0099E4B0 / = 0x0099EFB0 / != 0x0099E960 / ~ 0x0099EAE0.
// Shape: VC7.1 homes the saved melee-carried bool in the dead `loading` slot and the load branch's two name strings in the dead
//   argument slots on its own; CDefString has a declared copy constructor so it comes back through a hidden pointer as retail.
// Layout: packed weapons block -- melee pointer +0x134 / def index +0x13C / carried +0x140, ranged pointer +0x141 / def index
//   +0x14C / carried +0x150; confiscated melee/ranged +0x160/+0x164, previously wielded +0x168/+0x16C; thing def index word +0x8C.
// NF188 (2026-11-20, q4510): the def-index members are CDefIndex (Ego_r.pdb CTCInventoryWeapons ActiveMeleeWeaponDefIndex /
//   ActiveRangedWeaponDefIndex; respelled in place in this row's local model) and the two transfers call the two-argument member template
//   ??$Transfer@VCDefIndex@@@CPersistContext@@QAEXPBDAAVCDefIndex@@@Z at 0x005CC300 (FableWin.pdb 0x0199627C; was the stand-in
//   ?Transfer@CPersistContext@@QAEXPBDPAJ@Z).
// NF196 (2026-11-20, q4492): the row-local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared
//   engine/CIntelligentPointer.h (members CIntelligentPointer<CThing>, the Ego_r.pdb member type) and the IS_THING_ALIVE template
//   declaration, as NF191 did for the sibling 0x005C30D3: the liveness tests bind the FableWin.pdb
//   ??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThing@@@@@@YI_NABV?$CIntelligentPointer@VCThing@@@@@Z (was ?IS_THING_ALIVE@@YI_NABUCIntelligentPointer@@@Z).
#include "fable_compiler.h"
#include "rebuild_abi.h"
#include "engine/CDefIndex.h"
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: the leaf is byte- and relocation-identical without it (size mode keeps
//   its frame); it only framed end-of-TU template and compiler-generated copies (each copy checked in the T3 nf416 audit).
#pragma optimize("s", on)
class CTCBase;
class CCharString { public:
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    bool operator!=(const char* text) const;            // 0x0099E960
};
class CDefString { public:
    long Index;
    CDefString(const CDefString& other);
    operator CCharString() const;                       // 0x00415DD0, called out of line for the conversion (retail reloads the temporary
                                                        //   after the call, the out-of-line conversion pattern; see NOTES on the 0x00415D70 / 0x00415DD0 twins)
};
class CDefinitionManager { public:   // the PDB base the accessors belong to (empty here: offset 0)
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410 (Ego_r.pdb CDefinitionManager, const)
    CDefString GetDefNameFromGlobalIndex(long index) const;          // 0x009ACCC0 (Ego_r.pdb CDefinitionManager, const)
};
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};
class CThing { public:
    unsigned short _pad[0x46];
    unsigned short DefIndex;                            // +0x8c
};
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
template <class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing);   // 0x004CC340
class CPersistContext { public:
    bool IsSaving() const;                              // 0x004AEF50
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // 0x004045C0 <bool>
    template <class T> void Transfer(const char* name, T& value);   // 0x004107C0 <bool> / 0x00411010 <CCharString>
};
class CTCCarrying { public:
    bool IsCarryingThing(const CThing& thing) const;   // 0x006A1060 (Ego_r/FableWin ?IsCarryingThing@CTCCarrying@@QBE_NABVCThing@@@Z)
    bool IsCarriedThingInPrimarySlot(const CThing& thing) const;   // 0x006A1490 (Ego_r/FableWin ?IsCarriedThingInPrimarySlot@CTCCarrying@@QBE_NABVCThing@@@Z)
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
struct CThingBase {
    unsigned char  _pad00[0x20];
    std::bitset<274>  m_present;                           // +0x20
    CInterfaceMap  m_interfaces;                        // +0x44
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** out, long kind) const
    {
        if (m_present.test(kind)) {
            ETCInterfaceType key = (ETCInterfaceType)(kind);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(key);
            *out = (T*)((it == map->end() || kind < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
};
namespace NInventory {
class CTCInventoryBase { public:                        // the base component (Ego_r.pdb NInventory::CTCInventoryBase); vptr +0
protected:
    virtual void OnSerialise(CPersistContext& context, bool loading);   // 0x005BFD5E ?OnSerialise@CTCInventoryBase@NInventory@@MAEXAAVCPersistContext@@_N@Z
};
}
#pragma pack(push, 1)
class CTCInventoryWeapons : public NInventory::CTCInventoryBase { public:
    const CThingBase* ParentThing;                      // +0x04
    unsigned char _pad08[0x134 - 0x08];
    CIntelligentPointer<CThing> MeleeWeapon;                    // +0x134
    CDefIndex   MeleeDefIndex;                          // +0x13c
    bool        MeleeCarried;                           // +0x140
    CIntelligentPointer<CThing> RangedWeapon;                   // +0x141
    unsigned char _pad149[3];
    CDefIndex   RangedDefIndex;                         // +0x14c
    bool        RangedCarried;                          // +0x150
    unsigned char _pad151[0x160 - 0x151];
    long        ConfiscatedMelee;                       // +0x160
    long        ConfiscatedRanged;                      // +0x164
    long        PreviousMelee;                          // +0x168
    long        PreviousRanged;                         // +0x16c
    void RefreshActiveWeaponData() const;               // 0x005C30D3
protected:
    virtual void OnSerialise(CPersistContext& context, bool loading);   // Ego_r.pdb ?OnSerialise@CTCInventoryWeapons@@MAEXAAVCPersistContext@@_N@Z
};
#pragma pack(pop)
void CTCInventoryWeapons::OnSerialise(CPersistContext& context, bool loading)
{
    RefreshActiveWeaponData();
    NInventory::CTCInventoryBase::OnSerialise(context, loading);
    if (context.IsSaving()) {
        if (IS_THING_ALIVE(MeleeWeapon))
            MeleeDefIndex = ((CThing*)((CBaseIntelligentPointer*)&MeleeWeapon)->GetPItem())->DefIndex;
        else
            MeleeDefIndex = 0;
        if (IS_THING_ALIVE(RangedWeapon))
            RangedDefIndex = ((CThing*)((CBaseIntelligentPointer*)&RangedWeapon)->GetPItem())->DefIndex;
        else
            RangedDefIndex = 0;
        bool meleeCarried = false;
        bool rangedCarried = false;
        const CTCCarrying* carrying;
        if (ParentThing->GetInterface(&carrying, 0x46)) {
            if (((CThing*)((CBaseIntelligentPointer*)&MeleeWeapon)->GetPItem()) && carrying->IsCarryingThing(*((CThing*)((CBaseIntelligentPointer*)&MeleeWeapon)->GetPItem())))
                meleeCarried = carrying->IsCarriedThingInPrimarySlot(*((CThing*)((CBaseIntelligentPointer*)&MeleeWeapon)->GetPItem()));
            if (((CThing*)((CBaseIntelligentPointer*)&RangedWeapon)->GetPItem()) && carrying->IsCarryingThing(*((CThing*)((CBaseIntelligentPointer*)&RangedWeapon)->GetPItem())))
                rangedCarried = carrying->IsCarriedThingInPrimarySlot(*((CThing*)((CBaseIntelligentPointer*)&RangedWeapon)->GetPItem()));
        }
        context.Transfer("MeleeWeaponCarried", meleeCarried, false);
        context.Transfer("RangedWeaponCarried", rangedCarried, false);
        CCharString meleeName;
        if (ConfiscatedMelee > 0)
            meleeName = CGameDefinitionManager::Get().GetDefNameFromGlobalIndex(ConfiscatedMelee);
        CCharString rangedName;
        if (ConfiscatedRanged > 0)
            rangedName = CGameDefinitionManager::Get().GetDefNameFromGlobalIndex(ConfiscatedRanged);
        context.Transfer("ConfiscatedMeleeWeaponDefName", meleeName);
        context.Transfer("ConfiscatedRangedWeaponDefName", rangedName);
        if (PreviousMelee > 0)
            meleeName = CGameDefinitionManager::Get().GetDefNameFromGlobalIndex(PreviousMelee);
        if (PreviousRanged > 0)
            rangedName = CGameDefinitionManager::Get().GetDefNameFromGlobalIndex(PreviousRanged);
        context.Transfer("PreviouslyWieldedMeleeWeaponDefName", meleeName);
        context.Transfer("PreviouslyWieldedRangedWeaponDefName", rangedName);
    } else {
        context.Transfer("MeleeWeaponCarried", MeleeCarried);
        context.Transfer("RangedWeaponCarried", RangedCarried);
        CCharString meleeName;
        context.Transfer("ConfiscatedMeleeWeaponDefName", meleeName);
        if (meleeName != "")
            ConfiscatedMelee = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(meleeName);
        CCharString rangedName;
        context.Transfer("ConfiscatedRangedWeaponDefName", rangedName);
        if (rangedName != "")
            ConfiscatedRanged = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(rangedName);
        context.Transfer("PreviouslyWieldedMeleeWeaponDefName", meleeName);
        if (meleeName != "")
            PreviousMelee = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(meleeName);
        context.Transfer("PreviouslyWieldedRangedWeaponDefName", rangedName);
        if (rangedName != "")
            PreviousRanged = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(rangedName);
    }
    context.Transfer("ActiveMeleeWeaponDefIndex2", MeleeDefIndex);
    context.Transfer("ActiveRangedWeaponDefIndex2", RangedDefIndex);
}