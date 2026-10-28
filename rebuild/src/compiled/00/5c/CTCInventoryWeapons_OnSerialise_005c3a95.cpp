// CTCInventoryWeapons::OnSerialise(CPersistContext& context, long version) -- persists the active weapons: on save, refreshes the active def indices, records whether each active weapon is carried in the primary slot and writes the confiscated / previously wielded weapon def names; on load, reads the carried flags and maps the def names back to global indices; both ways transfer the active def indices
// Leaf: OnSerialise@CTCInventoryWeapons
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C3A95, 1020 bytes, `ret 8`. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: RefreshActiveWeaponData 0x005C30D3, CTCInventoryBase::OnSerialise 0x005BFD5E, CPersistContext saving test 0x004AEF50
//   (labelled IsBinary) / Transfer<bool>(name, value, default) 0x004045C0 / Transfer(name, bool*) 0x004107C0 / Transfer<CCharString>
//   0x00411010 / Transfer<long> 0x005CC300, IS_THING_ALIVE 0x004CC340, CBaseIntelligentPointer::GetPItem 0x00A01B50, bitset test
//   0x00410DE0, const LowerBound 0x004365B0 (carrying 0x46), CTCCarrying::IsCarryingThing 0x006A1060 / IsCarriedThingInPrimarySlot
//   0x006A1490, CGameDefinitionManager::Get 0x0044C6B0 / GetDefNameFromGlobalIndex 0x009ACCC0 / GetDefGlobalIndexFromName 0x009AD410,
//   CDefString::operator CCharString 0x00415DD0, CCharString 0x0099E4B0 / = 0x0099EFB0 / != 0x0099E960 / ~ 0x0099EAE0.
// Shape: VC7.1 homes the saved melee-carried bool in the dead `version` slot and the load branch's two name strings in the dead
//   argument slots on its own; CDefString has a declared copy constructor so it comes back through a hidden pointer as retail.
// Layout: packed weapons block -- melee pointer +0x134 / def index +0x13C / carried +0x140, ranged pointer +0x141 / def index
//   +0x14C / carried +0x150; confiscated melee/ranged +0x160/+0x164, previously wielded +0x168/+0x16C; thing def index word +0x8C.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CTCBase;
class CCharString { public:
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    bool operator!=(const char* text) const;            // 0x0099E960
};
namespace N_00415dd0 { struct CDefString {
    long Index;
    CDefString(const CDefString& other);
    operator CCharString() const;                       // 0x00415DD0
}; } using N_00415dd0::CDefString;
class CGameDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
    CDefString GetDefNameFromGlobalIndex(long index);   // 0x009ACCC0
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
class CThing { public:
    unsigned short _pad[0x46];
    unsigned short DefIndex;                            // +0x8c
};
#pragma pack(push, 1)
struct CIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CThing* GetPItem() const;                           // 0x00A01B50
};
#pragma pack(pop)
bool __fastcall IS_THING_ALIVE(const CIntelligentPointer& p);   // 0x004CC340
class CPersistContext { public:
    bool IsSaving() const;                              // 0x004AEF50
    void Transfer(const char* name, bool& value, const bool& defaultValue);   // 0x004045C0
    void Transfer(const char* name, bool* value);       // 0x004107C0
    void Transfer(const char* name, CCharString* value);   // 0x00411010
    void Transfer(const char* name, long* value);       // 0x005CC300
};
class CTCCarrying { public:
    bool IsCarryingThing(CThing* thing) const;          // 0x006A1060
    bool IsCarriedThingInPrimarySlot(CThing* thing) const;   // 0x006A1490
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CPresenceBits { bool test(long kind) const; };   // 0x00410DE0
struct CThingBase {
    unsigned char  _pad00[0x20];
    CPresenceBits  m_present;                           // +0x20
    unsigned char  _pad21[0x44 - 0x21];
    CInterfaceMap  m_interfaces;                        // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long kind) const
    {
        if (m_present.test(kind)) {
            long key = kind;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *out = (T*)((it == map->last || kind < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
#pragma pack(push, 1)
class CTCInventoryWeapons { public:
    void*       __vftable;
    const CThingBase* ParentThing;                      // +0x04
    unsigned char _pad08[0x134 - 0x08];
    CIntelligentPointer MeleeWeapon;                    // +0x134
    long        MeleeDefIndex;                          // +0x13c
    bool        MeleeCarried;                           // +0x140
    CIntelligentPointer RangedWeapon;                   // +0x141
    unsigned char _pad149[3];
    long        RangedDefIndex;                         // +0x14c
    bool        RangedCarried;                          // +0x150
    unsigned char _pad151[0x160 - 0x151];
    long        ConfiscatedMelee;                       // +0x160
    long        ConfiscatedRanged;                      // +0x164
    long        PreviousMelee;                          // +0x168
    long        PreviousRanged;                         // +0x16c
    void RefreshActiveWeaponData() const;               // 0x005C30D3
    void BaseOnSerialise(CPersistContext& context, long version);   // CTCInventoryBase::OnSerialise 0x005BFD5E
    void OnSerialise(CPersistContext& context, long version);
};
#pragma pack(pop)
void CTCInventoryWeapons::OnSerialise(CPersistContext& context, long version)
{
    RefreshActiveWeaponData();
    BaseOnSerialise(context, version);
    if (context.IsSaving()) {
        if (IS_THING_ALIVE(MeleeWeapon))
            MeleeDefIndex = MeleeWeapon.GetPItem()->DefIndex;
        else
            MeleeDefIndex = 0;
        if (IS_THING_ALIVE(RangedWeapon))
            RangedDefIndex = RangedWeapon.GetPItem()->DefIndex;
        else
            RangedDefIndex = 0;
        bool meleeCarried = false;
        bool rangedCarried = false;
        const CTCCarrying* carrying;
        if (ParentThing->GetInterface(&carrying, 0x46)) {
            if (MeleeWeapon.GetPItem() && carrying->IsCarryingThing(MeleeWeapon.GetPItem()))
                meleeCarried = carrying->IsCarriedThingInPrimarySlot(MeleeWeapon.GetPItem());
            if (RangedWeapon.GetPItem() && carrying->IsCarryingThing(RangedWeapon.GetPItem()))
                rangedCarried = carrying->IsCarriedThingInPrimarySlot(RangedWeapon.GetPItem());
        }
        context.Transfer("MeleeWeaponCarried", meleeCarried, false);
        context.Transfer("RangedWeaponCarried", rangedCarried, false);
        CCharString meleeName;
        if (ConfiscatedMelee > 0)
            meleeName = CGameDefinitionManager::Get().GetDefNameFromGlobalIndex(ConfiscatedMelee);
        CCharString rangedName;
        if (ConfiscatedRanged > 0)
            rangedName = CGameDefinitionManager::Get().GetDefNameFromGlobalIndex(ConfiscatedRanged);
        context.Transfer("ConfiscatedMeleeWeaponDefName", &meleeName);
        context.Transfer("ConfiscatedRangedWeaponDefName", &rangedName);
        if (PreviousMelee > 0)
            meleeName = CGameDefinitionManager::Get().GetDefNameFromGlobalIndex(PreviousMelee);
        if (PreviousRanged > 0)
            rangedName = CGameDefinitionManager::Get().GetDefNameFromGlobalIndex(PreviousRanged);
        context.Transfer("PreviouslyWieldedMeleeWeaponDefName", &meleeName);
        context.Transfer("PreviouslyWieldedRangedWeaponDefName", &rangedName);
    } else {
        context.Transfer("MeleeWeaponCarried", &MeleeCarried);
        context.Transfer("RangedWeaponCarried", &RangedCarried);
        CCharString meleeName;
        context.Transfer("ConfiscatedMeleeWeaponDefName", &meleeName);
        if (meleeName != "")
            ConfiscatedMelee = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(meleeName);
        CCharString rangedName;
        context.Transfer("ConfiscatedRangedWeaponDefName", &rangedName);
        if (rangedName != "")
            ConfiscatedRanged = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(rangedName);
        context.Transfer("PreviouslyWieldedMeleeWeaponDefName", &meleeName);
        if (meleeName != "")
            PreviousMelee = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(meleeName);
        context.Transfer("PreviouslyWieldedRangedWeaponDefName", &rangedName);
        if (rangedName != "")
            PreviousRanged = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(rangedName);
    }
    context.Transfer("ActiveMeleeWeaponDefIndex2", &MeleeDefIndex);
    context.Transfer("ActiveRangedWeaponDefIndex2", &RangedDefIndex);
}