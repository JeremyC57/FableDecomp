// CTCInventoryWeapons::OnInitialActivate() -- re-wields the melee and ranged weapons saved as active and, for weapons carried at load, puts them back in their carry slot; then the base activation
// Grade: DIFFER(693v706) 26 of 230 lines (was 37): non-const GetInterface select as `last = map->last; if (it == last || t < it->type) it = last;` (retail jle/mov eax,edi), const overload keeps the ternary (retail jg/mov edi,eax at the carryable site) -- all three select sites now retail; residue: frame 8 v 12 (retail shares the key slot [ebp-4] with carrying, ours keeps carrying in ebx)
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace NUISystem { struct CComponent; }
struct CPresenceBits { bool test(long kind) const; };      // 0x00410DE0
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long& type);                   // 0x0040F020
    const SEntry* LowerBound(const long& type) const;       // 0x004365B0
};
struct CThing {
    unsigned char _pad00[0x20];
    CPresenceBits present;                                  // +0x20
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap m_interfaces;                             // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (present.test(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            CInterfaceMap::SEntry* last = map->last;
            if (it == last || interfaceType < it->type)
                it = last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (present.test(interfaceType)) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CThing* GetPItem() const;                               // 0x00A01B50
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer { };
#pragma pack(pop)
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x004CC340
struct CTCHeroWeaponState {                                 // interface 0x29
    unsigned char _pad00[0x0c];
    bool WeaponsOut;                                        // +0x0C
};
struct CTCCarryable {                                       // interface 0x47
    unsigned char _pad00[0x10];
    void* Carrier;                                          // +0x10
    CThing* GetCarrier() const;                             // 0x00705170
    long GetCarrySlot() const;                              // 0x00705CC0
};
struct CTCCarrying {                                        // interface 0x46
    void RemoveThing(CThing* thing);                        // 0x006A19B0
    void PutThingInCarrySlot(CThing* thing, long slot, bool immediately);   // 0x006A1F80
};
struct CTCInventoryBase {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    unsigned char _pad08[0x134 - 0x08];
    void OnInitialActivate();                               // 0x005BD1EB
};
#pragma pack(push, 1)
struct CTCInventoryWeapons : CTCInventoryBase {
    CIntelligentPointer<CThing> PActiveMeleeWeapon;         // +0x134
    long          ActiveMeleeWeaponDefIndex;                // +0x13C
    bool          MeleeWeaponCarriedOnLoad;                 // +0x140
    CIntelligentPointer<CThing> PActiveRangedWeapon;        // +0x141
    unsigned char _pad149[3];
    long          ActiveRangedWeaponDefIndex;               // +0x14C
    bool          RangedWeaponCarriedOnLoad;                // +0x150
    void SetThingAsActiveWeapon(long defIndex, NUISystem::CComponent* menuEntry);   // 0x005CB2B0
    void OnInitialActivate();
};
#pragma pack(pop)
void CTCInventoryWeapons::OnInitialActivate()
{
    if (ActiveMeleeWeaponDefIndex > 0) {
        SetThingAsActiveWeapon(ActiveMeleeWeaponDefIndex, 0);
        ActiveMeleeWeaponDefIndex = 0;
        if (MeleeWeaponCarriedOnLoad && IS_THING_ALIVE(PActiveMeleeWeapon)) {
            CTCHeroWeaponState* state;
            if (((const CThing*)ParentThing)->GetInterface(&state, 0x29) && state && state->WeaponsOut) {
                CTCCarryable* carryable;
                if (((const CThing*)PActiveMeleeWeapon.GetPItem())->GetInterface(&carryable, 0x47)) {
                    CTCCarrying* carrying;
                    if (ParentThing->GetInterface(&carrying, 0x46)) {
                        if (carryable->Carrier && ParentThing == carryable->GetCarrier())
                            carrying->RemoveThing(PActiveMeleeWeapon.GetPItem());
                        long slot = carryable->GetCarrySlot();
                        if (slot > 0)
                            carrying->PutThingInCarrySlot(PActiveMeleeWeapon.GetPItem(), slot, true);
                    }
                }
            }
            MeleeWeaponCarriedOnLoad = false;
        }
    }
    if (ActiveRangedWeaponDefIndex > 0) {
        SetThingAsActiveWeapon(ActiveRangedWeaponDefIndex, 0);
        ActiveRangedWeaponDefIndex = 0;
        if (RangedWeaponCarriedOnLoad && IS_THING_ALIVE(PActiveRangedWeapon)) {
            CTCHeroWeaponState* state;
            if (((const CThing*)ParentThing)->GetInterface(&state, 0x29) && state && state->WeaponsOut) {
                CTCCarryable* carryable;
                if (((const CThing*)PActiveRangedWeapon.GetPItem())->GetInterface(&carryable, 0x47)) {
                    CTCCarrying* carrying;
                    if (ParentThing->GetInterface(&carrying, 0x46)) {
                        if (carryable->Carrier && ParentThing == carryable->GetCarrier())
                            carrying->RemoveThing(PActiveRangedWeapon.GetPItem());
                        long slot = carryable->GetCarrySlot();
                        if (slot > 0)
                            carrying->PutThingInCarrySlot(PActiveRangedWeapon.GetPItem(), slot, true);
                    }
                }
            }
            RangedWeaponCarriedOnLoad = false;
        }
    }
    CTCInventoryBase::OnInitialActivate();
}
