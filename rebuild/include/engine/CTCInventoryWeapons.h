#ifndef FABLE_ENGINE_CTCINVENTORYWEAPONS_H
#define FABLE_ENGINE_CTCINVENTORYWEAPONS_H
// CTCInventoryWeapons (new shared header, chunk 900): the hero's weapons inventory screen TC. Retail polymorphic class: RTTI
// .?AVCTCInventoryWeapons@@ = NInventory::CTCInventoryBase @0 (0x130 bytes in retail) + NHeroInformationScreens::CBase @0x130
// (ghidra_out/rtti_bases.tsv), vtable 0x01253FF4 (slot 77 ConstructScreen 0x005C8459, slot 87 ConstructScreenPC 0x005C9A95).
// Members: the 19 Ego_r.pdb members in order and spelling (struct_layouts_egor.tsv, 456 bytes there) at the retail offsets: Ego_r
// puts them 0x18 higher (Dinkumware base vectors) and its Dinkumware vector / map are 4 bytes larger than STLport 4.6's, so
//   PActiveMeleeWeapon +0x134 / PActiveRangedWeapon +0x141 (CIntelligentPointer is byte-packed: Ego_r +0x14C / +0x159),
//   ActiveWeaponPrimitiveHandles +0x154 (landed CloseInventory 0x005CAC8D), WeaponMenuItemMap +0x170 (map clear 0x004DB806 in
//   0x005C8459 / 0x005C9A95), WeaponsList +0x17C, PAssignedRangedIcon .. PMeleeList +0x180..+0x194 (zeroed by both screens),
//   AvailableWeaponsEntries +0x198 (list clear 0x0042A18F); members end at 0x19C, sizeof 0x1A0 (8-byte class alignment).
// Methods: Ego_r.pdb / FableWin.pdb spellings of the members the screens call (addresses: retail call targets).
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/NHeroInformationScreens_CBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CDefPointer.h"
class CEnginePrimitiveHandle;                                            // engine/CEnginePrimitiveHandle.h (only the 12-byte vector is laid out here)
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
class CThing;
namespace NUISystem { class CComponent; class CList; class CManager; class CViewport; class CUIMiscThingsDef; }
class CDefIndex {                                                        // Ego_r.pdb CDefIndex (4 bytes: long Val)
public:
    long Val;
};
class CTCInventoryWeapons : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase {
public:
    virtual NUISystem::CComponent* ConstructScreen(NUISystem::CComponent* parent);       // slot 77 0x005C8459
    virtual NUISystem::CComponent* ConstructScreenPC(NUISystem::CComponent* parent);     // slot 87 0x005C9A95
    CCountedPointer<NUISystem::CComponent> ConstructAugEntry(bool, std::vector<std::pair<long, long> >&, NUISystem::CViewport*);   // 0x005C574E
    CCountedPointer<NUISystem::CComponent> ConstructAugEntryPC(bool, std::vector<std::pair<long, long> >&, NUISystem::CViewport*,
                                                               NUISystem::CComponent*);                                         // 0x005C5F11
    float GetActiveMeleeWeaponDamage() const;                                                                                   // inline in retail
    float GetActiveProjectileWeaponDamage() const;                                                                              // inline in retail
private:
    void ConstructWeaponItemEntries(std::vector<std::pair<long, long> >&, NUISystem::CList*, CCountedPointer<NUISystem::CComponent>&,
                                    NUISystem::CComponent*, NUISystem::CManager*, NUISystem::CViewport*,
                                    CDefPointer<const NUISystem::CUIMiscThingsDef>&, float,
                                    CCountedPointer<NUISystem::CComponent>&);                                                  // 0x005C66B2
    void ConstructWeaponItemEntriesPC(std::vector<std::pair<long, long> >&, NUISystem::CList*, CCountedPointer<NUISystem::CComponent>&,
                                      NUISystem::CComponent*, NUISystem::CManager*, NUISystem::CViewport*,
                                      CDefPointer<const NUISystem::CUIMiscThingsDef>&, float,
                                      CCountedPointer<NUISystem::CComponent>&);                                                // 0x005C8176
#ifdef FABLE_RETAIL_CTCINVENTORYWEAPONS_METHODS
    // Retail opt-in (chunk 1002): the rest of the class's Ego_r.pdb members in their exact spellings (access, const-ness).
public:
    virtual void OnInitialActivate();                                                                    // CTCBase slot 6 0x005CB974
    virtual void RemoveItemFromInventory(long itemDefIndex, long count);                                 // slot 79 0x005CB128
    void RefreshActiveWeaponData() const;                                                                // 0x005C30D3
    CCountedPointer<NUISystem::CComponent> ConstructWieldEntry(bool melee, NUISystem::CViewport* viewport);   // 0x005C323A
    CCountedPointer<NUISystem::CComponent> ConstructWieldEntryPC(bool melee, NUISystem::CViewport* viewport,
                                                                 NUISystem::CComponent* parent, long index);   // 0x005C3444
    void AugmentWeapon(long weaponDefIndex, long augmentationDefIndex, bool& augmented);                 // 0x005C44FA
    long GetFreeAugmentationSlotForWeapon(long weaponDefIndex);                                          // 0x005C4671
    void ConstructAugmentationsLeftDescription(CWideString& text, std::vector<std::pair<long, long> >& augmentations, bool pc); // 0x005C4F42
    void RemoveWeaponsFromSheathes();                                                                    // 0x005C552C
    void RefreshAugmentationsLeftDescriptions();                                                         // 0x005C69AA
    void ConstructWeaponDescription(CWideString& text, long itemDefIndex, float damage, bool pc) const;  // 0x005C6B26
    void ConstructWeaponDescriptionNoDamageNorWeight(CWideString& text, long itemDefIndex, float damage, bool pc) const; // 0x005C71AA
    void ConstructWeaponDescriptionDamage(CWideString& text, long itemDefIndex, float damage) const;     // 0x005C7452
    void ConstructWeaponDescriptionWeight(CWideString& text, long itemDefIndex, float damage) const;     // 0x005C770C
    void PutWeaponsInSheathes();                                                                         // 0x005C8101
    void ConstructWeaponDescriptionContainer(NUISystem::CComponent* container, long itemDefIndex, float damage) const; // 0x005C90FC
    void RefreshSheathedWeapons();                                                                       // 0x005C9962
    long GetTimeWieldedWeapon(long itemDefIndex) const;                                                  // 0x005C99BE
    void RefreshWeaponDescriptions();                                                                    // 0x005CACB9
    void SetThingAsActiveWeapon(long itemDefIndex, NUISystem::CComponent* entry);                        // 0x005CB2B0
    void CheckForSoldWieldedWeapons();                                                                   // 0x005CB780
    void AddComponentToAvailableWeaponsEntries(NUISystem::CComponent* entry);
    void SetWeaponMappingToMenuEntry(NUISystem::CComponent* entry, long itemDefIndex);
private:
    void FinalConstructWeaponItemEntries(std::vector<std::pair<long, long> >&, NUISystem::CList*, CCountedPointer<NUISystem::CComponent>&,
                                         NUISystem::CComponent*, NUISystem::CManager*, NUISystem::CViewport*,
                                         CDefPointer<const NUISystem::CUIMiscThingsDef>&, float,
                                         CCountedPointer<NUISystem::CComponent>&, CCountedPointer<NUISystem::CComponent>&,
                                         unsigned long);                                                   // 0x005C3E91
    void FinalConstructWeaponItemEntriesPC(std::vector<std::pair<long, long> >&, NUISystem::CList*, CCountedPointer<NUISystem::CComponent>&,
                                           NUISystem::CComponent*, NUISystem::CManager*, NUISystem::CViewport*,
                                           CDefPointer<const NUISystem::CUIMiscThingsDef>&, float,
                                           CCountedPointer<NUISystem::CComponent>&, CCountedPointer<NUISystem::CComponent>&,
                                           unsigned long);                                                 // 0x005C47D5
    void SetupActiveWeaponsOnCarrier();                                                                  // 0x005CA68E
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYWEAPONS_ITEMDESCRIPTION
    // Retail opt-in (agentNF10, rows 0x005C7913 / 0x005C7B10 / 0x005C7D0D / 0x005C7F07): Ego_r.pdb
    // ?GetItemDescription@CTCInventoryWeapons@@UBEXAAVCWideString@@J_N@Z (the override of NInventory::CTCInventoryBase slot 78, vtable
    // 0x01253FF4), ?GetItemDescriptionNoDamageNorWeight@CTCInventoryWeapons@@QBEXAAVCWideString@@J_N@Z,
    // ?GetItemDescriptionWeight@CTCInventoryWeapons@@QBEXAAVCWideString@@J@Z and ?GetItemDescriptionDamage@CTCInventoryWeapons@@QBEXAAVCWideString@@J@Z
    // (the four byte-twins; the last three `ret 0Ch` / `ret 8`). Declarations only: no layout change.
public:
    virtual void GetItemDescription(CWideString& text, long itemDefIndex, bool pc) const;               // slot 78 0x005C7913
    void GetItemDescriptionNoDamageNorWeight(CWideString& text, long itemDefIndex, bool pc) const;       // 0x005C7B10
    void GetItemDescriptionWeight(CWideString& text, long itemDefIndex) const;                           // 0x005C7D0D
    void GetItemDescriptionDamage(CWideString& text, long itemDefIndex) const;                           // 0x005C7F07
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYWEAPONS_ACTIVEMELEE
    // Retail opt-in (chunk 1024, row 0x007CD390 CTCMultiStrike::CTCMultiStrike): FableWin.pdb
    // ?GetActiveMeleeWeapon@CTCInventoryWeapons@@QBEPAVCThing@@XZ (inline in retail: lea ecx,[eax+134h] then GetPItem 0x00A01B50, 0x007CD469) and
    // the TCI_INVENTORY_WEAPONS (19) key of CThing::PeekTC<CTCInventoryWeapons>. No layout change.
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_INVENTORY_WEAPONS; }
    CThing* GetActiveMeleeWeapon() const { return PActiveMeleeWeapon.Get(); }
#endif
public:
    CIntelligentPointer<CThing> PActiveMeleeWeapon;                      // +0x134
    CDefIndex ActiveMeleeWeaponDefIndex;                                 // +0x13C
    bool MeleeWeaponCarriedOnLoad;                                       // +0x140
    CIntelligentPointer<CThing> PActiveRangedWeapon;                     // +0x141 (packed pointer class)
    CDefIndex ActiveRangedWeaponDefIndex;                                // +0x14C
    bool RangedWeaponCarriedOnLoad;                                      // +0x150
    std::vector<CEnginePrimitiveHandle> ActiveWeaponPrimitiveHandles;    // +0x154
    long ConfiscatedActiveMeleeWeaponDefIndex;                           // +0x160
    long ConfiscatedActiveRangedWeaponDefIndex;                          // +0x164
    long PreviouslyWieldedMeleeWeaponDefIndex;                           // +0x168
    long PreviouslyWieldedRangedWeaponDefIndex;                          // +0x16C
    std::map<NUISystem::CComponent*, long> WeaponMenuItemMap;            // +0x170
    NUISystem::CComponent* WeaponsList;                                  // +0x17C
    NUISystem::CComponent* PAssignedRangedIcon;                          // +0x180
    NUISystem::CComponent* PAssignedMeleeIcon;                           // +0x184
    NUISystem::CComponent* PAssignedRangedMenuEntry;                     // +0x188
    NUISystem::CComponent* PAssignedMeleeMenuEntry;                      // +0x18C
    NUISystem::CComponent* PRangedList;                                  // +0x190
    NUISystem::CComponent* PMeleeList;                                   // +0x194
    std::list<NUISystem::CComponent*> AvailableWeaponsEntries;           // +0x198
#ifdef FABLE_RETAIL_CTCINVENTORYWEAPONS_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual void OnChangeCategory();  // slot 37, 0x005CC4F8
public:
#endif
};
FABLE_STATIC_ASSERT(sizeof(CTCInventoryWeapons) == 0x1A0);   // 0x19C of members, rounded to the base's 8-byte alignment (double LastRenderTime)
FABLE_STATIC_ASSERT(offsetof(CTCInventoryWeapons, PActiveMeleeWeapon) == 0x134);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryWeapons, ActiveWeaponPrimitiveHandles) == 0x154);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryWeapons, WeaponMenuItemMap) == 0x170);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryWeapons, WeaponsList) == 0x17C);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryWeapons, AvailableWeaponsEntries) == 0x198);
#endif // FABLE_ENGINE_CTCINVENTORYWEAPONS_H
