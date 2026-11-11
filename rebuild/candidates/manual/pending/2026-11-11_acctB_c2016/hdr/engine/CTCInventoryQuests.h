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
class CEnginePrimitiveHandle;
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYQUESTS_OPENPC
#include "stlport/fable_map.h"
#include "engine/CIntelligentPointer.h"
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYQUESTS_TESTQUESTS
#include "engine/CTestQuest.h"
class CEnginePrimitivePrepareInfo;
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
    long QuestCardCount;                                                                           // +0x150
#ifdef FABLE_RETAIL_CTCINVENTORYQUESTS_OPENPC
    // Nested opt-in (w22 agent2002, row 0x006224C0 OpenInventoryPC): the rest of the Ego_r.pdb members in order at the retail offsets
    // (STLport map / vector 12 bytes): ActiveTestQuestMode +0x157, PreviousScreenType +0x168, AvailableQuests +0x16C (clear() inline,
    // _M_erase 0x00623560), DisplayQuests +0x178, AccumFadeTime +0x184 (the stores of 0x006224C0; ~CTCInventoryQuests 0x0061A500 and
    // CloseInventoryPC 0x00621720 touch +0x168 / +0x134 / +0x140). Needs stlport/fable_map.h and the retail CIntelligentPointer.
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
    // Ego_r.pdb / FableWin.pdb overrides and members the row calls (vtable 0x012585B4: slot 49 GetInventoryGuildValidity 0x0061A0B0,
    // slot 86 OpenInventoryPC 0x006224C0; the NHeroInformationScreens::CBase slot 3 IsInventoryScreen override is called through the
    // +0x130 base); ?InitialiseQuestCardList@CTCInventoryQuests@@QAEXXZ out of line at 0x00621A20 (Ego_r donor order: CloseInventoryPC,
    // InitialiseQuestCardList, CloseInventory = retail 0x00621720, 0x00621A20, 0x00621C80).
    virtual bool GetInventoryGuildValidity();                                                      // 49 0x0061A0B0
    virtual bool IsInventoryScreen() const;                                                        // CBase slot 3
    virtual void OpenInventoryPC();                                                                // 86 0x006224C0
    void InitialiseQuestCardList();                                                                // 0x00621A20
#ifdef FABLE_RETAIL_CTCINVENTORYQUESTS_TESTQUESTS
    // Nested opt-in (chunk 2016, row 0x0061AD60): Ego_r.pdb 0x005982E0 / FableWin.pdb 0x01F7D530
    // ?GetTestQuestsToDisplay@CTCInventoryQuests@@QAEXAAV?$vector@VCTestQuest@@V?$allocator@VCTestQuest@@@std@@@std@@@Z (retail 0x0061A8A0, out
    // of line, the bound spelling) and Ego_r.pdb 0x005987B0 ?DrawActiveTestQuests@CTCInventoryQuests@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
    // (retail 0x0061AD60, the next function in donor order; DrawPreparePrimitivesForRendering calls it at 0x0061B518). Declarations only.
    void GetTestQuestsToDisplay(std::vector<CTestQuest>& quests);                                  // 0x0061A8A0
    void DrawActiveTestQuests(const CEnginePrimitivePrepareInfo& info);                            // 0x0061AD60
#endif
#endif
#endif
    // Ego_r.pdb / FableWin.pdb ?ConstructQuestList@CTCInventoryQuests@@QAEXABV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@PAVCList@NUISystem@@W4QuestScreenType@@1@Z
    void ConstructQuestList(const std::vector<CThing*>& quests, NUISystem::CList* list, QuestScreenType type,
                            NUISystem::CList* parent);                                                     // 0x0061B610
    // Ego_r.pdb / FableWin.pdb ?ConstructQuestListPC@CTCInventoryQuests@@QAEXPAVCComponent@NUISystem@@ABV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@PAVCList@3@W4QuestScreenType@@2@Z
    void ConstructQuestListPC(NUISystem::CComponent* container, const std::vector<CThing*>& quests, NUISystem::CList* list,
                              QuestScreenType type, NUISystem::CList* parent);                             // 0x0061E6D0
};
#endif
