#ifndef FABLE_ENGINE_CTCINVENTORYQUESTS_H
#define FABLE_ENGINE_CTCINVENTORYQUESTS_H
// CTCInventoryQuests (chunk 1011, rows 0x005FCBE0 / 0x005FDDD0 CTCInventoryMap::ConstructPastQuestsList(PC)): declarations only.
// RTTI .?AVCTCInventoryQuests@@ : NInventory::CTCInventoryBase @0, NHeroInformationScreens::CBase @0x130 (ghidra_out/rtti_bases.tsv),
// the same bases as CTCInventoryMap. Ego_r.pdb sizeof = 0x1B0; the members past the bases are NOT modelled here yet (header debt:
// do not take sizeof or member offsets from this header). Interface type: CTCInventoryQuests::GetInterfaceType 0x00624D60 is
// `mov eax,19h` = TCI_INVENTORY_QUESTS 25; FableWin.pdb ?StatGetInterfaceType@CTCInventoryQuests@@SI?AW4ETCInterfaceType@@XZ.
// QuestScreenType: Ego_r.pdb/FableWin.pdb W4QuestScreenType@@ (global enum, 4 bytes); the enumerator names are not in the
// extracts on disk (values seen: 1 = current quests 0x005FC2D0, 2 = past quests 0x005FCBE0), so none are declared.
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/NHeroInformationScreens_CBase.h"
#include "engine/ETCInterfaceType.h"
#include "stlport/fable_containers.h"
class CThing;
namespace NUISystem { class CComponent; class CList; }
enum QuestScreenType { };
#ifdef FABLE_RETAIL_CTCINVENTORYQUESTS_MEMBERS
#include "engine/CQuestCardItem.h"
#ifdef FABLE_RETAIL_CTCINVENTORYQUESTS_OPENINVENTORY
#include "stlport/fable_map.h"
#include "engine/CIntelligentPointer.h"
#endif
class CEnginePrimitiveHandle;
#endif
class CTCInventoryQuests : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_INVENTORY_QUESTS; }
#ifdef FABLE_RETAIL_CTCINVENTORYQUESTS_MEMBERS
    // Retail opt-in (chunk 1023, row 0x0061A6A0): the slot-4 override ?OnCreate@CTCInventoryQuests@@UAEXXZ (vtable 0x012585B4) and the
    // first members after the NHeroInformationScreens::CBase subobject (+0x130), Ego_r.pdb names and order at the retail offsets the
    // landed ~CTCInventoryQuests 0x0061A500 and OnCreate read (12-byte STLport vectors): the members past QuestCardCount are not
    // modelled yet (no sizeof; retail operator new(0x188) in Alloc 0x004D30A8).
    virtual void OnCreate();                                                                       //  4 0x0061A6A0
    std::vector<CEnginePrimitiveHandle> InventoryQuestCardSelectPrimitiveHandles;                  // +0x134
    std::vector<CQuestCardItem> QuestCardItemList;                                                 // +0x140
    bool IsGuildValid;                                                                             // +0x14c
    bool InventoryScreen;                                                                          // +0x14d
#ifdef FABLE_RETAIL_CTCINVENTORYQUESTS_OPENINVENTORY
    // Nested opt-in (chunk Q24, row 0x00622A00 OpenInventory; needs FABLE_RETAIL_CTCINVENTORYQUESTS_MEMBERS and
    // FABLE_RETAIL_CTCINVENTORYBASE_VTABLE): the rest of the Ego_r.pdb member list at the retail offsets (PDB offset - 0x20 past
    // QuestCardCount, as the landed ctor 0x0061A420 / ActivateSelectedTestQuest 0x0061AB30 read them; sizeof 0x188 = Alloc's
    // operator new(0x188)), the slot-30 override ?OpenInventory@CTCInventoryQuests@@UAEXXZ (vtable 0x012585B4 slot 30) and
    // ?InitialiseQuestCardList@CTCInventoryQuests@@QAEXXZ (out of line at 0x00621A20). Ego_r.pdb / FableWin.pdb spellings.
    virtual void OpenInventory();                                                                  // 30 0x00622A00
    void InitialiseQuestCardList();                                                                // 0x00621A20
#endif
    long QuestCardCount;                                                                           // +0x150
#ifdef FABLE_RETAIL_CTCINVENTORYQUESTS_OPENINVENTORY
    bool Teleport;                                                                                 // +0x154
    bool QuestCardActivatable;                                                                     // +0x155
    bool EnableQuestCardListInitialise;                                                            // +0x156
    bool ActiveTestQuestMode;                                                                      // +0x157
    long TestQuestSelectedIndex;                                                                   // +0x158
    long TestQuestTopIndex;                                                                        // +0x15c
    bool TestQuestShowAll;                                                                         // +0x160
    long TestQuestListSize;                                                                        // +0x164
    NHeroInformationScreens::EScreenType PreviousScreenType;                                       // +0x168
    std::map<CThing*, NUISystem::CComponent*> AvailableQuests;                                     // +0x16c
    std::vector<CIntelligentPointer<CThing> > DisplayQuests;                                       // +0x178
    float AccumFadeTime;                                                                           // +0x184
#endif
#endif
    // Ego_r.pdb / FableWin.pdb ?ConstructQuestList@CTCInventoryQuests@@QAEXABV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@PAVCList@NUISystem@@W4QuestScreenType@@1@Z
    void ConstructQuestList(const std::vector<CThing*>& quests, NUISystem::CList* list, QuestScreenType type,
                            NUISystem::CList* parent);                                                     // 0x0061B610
    // Ego_r.pdb / FableWin.pdb ?ConstructQuestListPC@CTCInventoryQuests@@QAEXPAVCComponent@NUISystem@@ABV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@PAVCList@3@W4QuestScreenType@@2@Z
    void ConstructQuestListPC(NUISystem::CComponent* container, const std::vector<CThing*>& quests, NUISystem::CList* list,
                              QuestScreenType type, NUISystem::CList* parent);                             // 0x0061E6D0
#ifdef FABLE_RETAIL_CTCINVENTORYQUESTS_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool IsComponentSerialisable() const;  // slot 23, 0x00624DB0
public:
    virtual void OnChangeCategory();  // slot 37, 0x00624DA0
public:
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYQUESTS_HISCREEN
// agentFAM13 (2026-11-12): the NHeroInformationScreens::CBase overrides this declaration lacks (the secondary vtable at +0x130,
// slots 0 GetScreenName / 1 GetPlayerMode / 2 GetScreenType / 3 IsInventoryScreen, engine/NHeroInformationScreens_CBase.h);
// spellings from the Ego_r / FableWin name sets. Declaration only (no layout change; the vtable exists).
public:
    virtual CWideString GetScreenName() const;
    virtual EPlayerMode GetPlayerMode() const;
    virtual NHeroInformationScreens::EScreenType GetScreenType() const;
    virtual bool IsInventoryScreen() const;
#endif // FABLE_RETAIL_CTCINVENTORYQUESTS_HISCREEN
};
#endif
