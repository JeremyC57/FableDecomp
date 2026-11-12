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
class CTCQuestCard;
namespace NUISystem { class CComponent; class CList; }
#ifdef FABLE_RETAIL_CTCINVENTORYQUESTS_QUESTLIST
// Retail opt-in (chunk 882 / H40f, row 0x0061E6D0 ConstructQuestListPC): the enumerator names are in neither PDB name set; these three
// are descriptive (0 the guild's available quests, OpenInventoryPC 0x006228FD; 1 the current quests 0x005FCAA7; 2 the past quests).
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_map.h"
enum QuestScreenType { QUEST_SCREEN_TYPE_AVAILABLE = 0, QUEST_SCREEN_TYPE_CURRENT = 1, QUEST_SCREEN_TYPE_OTHER = 2 };
#else
enum QuestScreenType { };
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYQUESTS_MEMBERS
#include "engine/CQuestCardItem.h"
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
    long QuestCardCount;                                                                           // +0x150
#endif
#if defined(FABLE_RETAIL_CTCINVENTORYQUESTS_QUESTLIST) && defined(FABLE_RETAIL_CTCINVENTORYQUESTS_MEMBERS)
    // (chunk 882 / H40f, row 0x0061E6D0) the members after QuestCardCount, Ego_r.pdb names and order at the retail offsets:
    // AvailableQuests +0x16C (`lea ecx,[ebx+16Ch]` insert_unique 0x0061F122), DisplayQuests +0x178 (`mov edi,[ebx+17Ch]` 0x0061F15F),
    // PreviousScreenType +0x168 (landed CloseInventoryPC 0x00621720); sizeof 0x188 (operator new(0x188) in Alloc 0x004D30A8).
    bool Teleport;                                                      // +0x154
    bool QuestCardActivatable;                                          // +0x155
    bool EnableQuestCardListInitialise;                                 // +0x156
    bool ActiveTestQuestMode;                                           // +0x157
    long TestQuestSelectedIndex;                                        // +0x158
    long TestQuestTopIndex;                                             // +0x15c
    bool TestQuestShowAll;                                              // +0x160
    long TestQuestListSize;                                             // +0x164
    NHeroInformationScreens::EScreenType PreviousScreenType;            // +0x168
    std::map<CThing*, NUISystem::CComponent*> AvailableQuests;          // +0x16c
    std::vector<CIntelligentPointer<CThing> > DisplayQuests;            // +0x178
    float AccumFadeTime;                                                // +0x184
#endif
    // Ego_r.pdb / FableWin.pdb ?ConstructQuestList@CTCInventoryQuests@@QAEXABV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@PAVCList@NUISystem@@W4QuestScreenType@@1@Z
    void ConstructQuestList(const std::vector<CThing*>& quests, NUISystem::CList* list, QuestScreenType type,
                            NUISystem::CList* parent);                                                     // 0x0061B610
    // Ego_r.pdb / FableWin.pdb ?ConstructQuestListPC@CTCInventoryQuests@@QAEXPAVCComponent@NUISystem@@ABV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@PAVCList@3@W4QuestScreenType@@2@Z
    void ConstructQuestListPC(NUISystem::CComponent* container, const std::vector<CThing*>& quests, NUISystem::CList* list,
                              QuestScreenType type, NUISystem::CList* parent);                             // 0x0061E6D0
#ifdef FABLE_RETAIL_CTCINVENTORYQUESTS_QUESTLIST
    // (chunk 4007, row 0x0061B610 ConstructQuestList) Ego_r.pdb / FableWin.pdb
    // ?IsQuestCardAvailable@CTCInventoryQuests@@QBE_NABVCTCQuestCard@@@Z: retail's out-of-line copy is 0x0061A260 (`this` unused,
    // the card by reference, `ret 4`; landed under the descriptive name IsQuestCardRegionFree). Inline in retail: both
    // ConstructQuestList rows expand it (0x0061D389..0x0061D3E2 here). Declaration only; a row that expands it defines the body.
    bool IsQuestCardAvailable(const CTCQuestCard& quest_card) const;                                       // 0x0061A260
#endif
};
#endif
