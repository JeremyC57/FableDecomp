#ifndef FABLE_ENGINE_CTCINVENTORYQUESTS_H
#define FABLE_ENGINE_CTCINVENTORYQUESTS_H
// CTCInventoryQuests -- the hero's quest-card inventory screen (available guild quests, current quests, the PC quest lists).
// New shared header (chunk 882, row 0x0061E6D0 CTCInventoryQuests::ConstructQuestListPC). Retail-only model.
// Identity: RTTI .?AVCTCInventoryQuests@@ : NInventory::CTCInventoryBase (+0) and NHeroInformationScreens::CBase (+0x130), the
// same two bases as CTCInventoryMap (engine/CTCInventoryMap.h); Ego_r.pdb / FableWin.pdb name set (??0CTCInventoryQuests@@QAE@AAVCThing@@@Z,
// ?ConstructQuestListPC@CTCInventoryQuests@@QAEXPAVCComponent@NUISystem@@ABV?$vector@PAVCThing@@...@std@@PAVCList@3@W4QuestScreenType@@2@Z ...).
// Layout: Ego_r.pdb (struct_layouts_egor.tsv, 432 bytes) minus 0x18 for the retail CTCInventoryBase (0x130 against 0x148) and 4 per
// STLport container (12 against 16): InventoryQuestCardSelectPrimitiveHandles +0x134, QuestCardItemList +0x140, IsGuildValid +0x14C,
// InventoryScreen +0x14D, PreviousScreenType +0x168 (landed CloseInventoryPC 0x00621720 reads +0x134 / +0x14D / +0x168),
// AvailableQuests +0x16C and DisplayQuests +0x178 (ConstructQuestListPC 0x0061F122 `lea ecx,[ebx+16Ch]` insert_unique,
// 0x0061F15F `mov edi,[ebx+17Ch]` push_back), AccumFadeTime +0x184; sizeof 0x188.
// Members only the rows need are typed; the two vectors of classes without a shared header stay opaque (12 bytes each).
// QuestScreenType: Ego_r.pdb `W4QuestScreenType@@` (a global enum); its enumerator names are in neither PDB name set, so the
// three values the callers pass are named descriptively: 0 the guild's available quests (OpenInventoryPC 0x006228FD), 1 the
// current quests (0x005FCAA7), 2 the third list (0x005FE010) -- the row only distinguishes 0, 1 and "not 0".
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/NHeroInformationScreens_CBase.h"
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
class CThing;
namespace NUISystem { class CComponent; class CList; }

enum QuestScreenType {
    QUEST_SCREEN_TYPE_AVAILABLE = 0,                                    // descriptive (unproven spelling)
    QUEST_SCREEN_TYPE_CURRENT = 1,                                      // descriptive
    QUEST_SCREEN_TYPE_OTHER = 2                                         // descriptive
};

class CTCInventoryQuests : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase {
public:
    void ConstructQuestListPC(NUISystem::CComponent* parent, const std::vector<CThing*>& quests, NUISystem::CList* list,
                              QuestScreenType type, NUISystem::CList* frame_list);   // 0x0061E6D0
protected:
    unsigned char InventoryQuestCardSelectPrimitiveHandles[0xc];        // +0x134 vector<CEnginePrimitiveHandle>
    unsigned char QuestCardItemList[0xc];                               // +0x140 vector<CQuestCardItem>
    bool IsGuildValid;                                                  // +0x14c
    bool InventoryScreen;                                               // +0x14d
    long QuestCardCount;                                                // +0x150
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
public:
    static void CheckRetailLayout() {
        FABLE_STATIC_ASSERT(offsetof(CTCInventoryQuests, InventoryQuestCardSelectPrimitiveHandles) == 0x134);
        FABLE_STATIC_ASSERT(offsetof(CTCInventoryQuests, IsGuildValid) == 0x14c);
        FABLE_STATIC_ASSERT(offsetof(CTCInventoryQuests, PreviousScreenType) == 0x168);
        FABLE_STATIC_ASSERT(offsetof(CTCInventoryQuests, AvailableQuests) == 0x16c);
        FABLE_STATIC_ASSERT(offsetof(CTCInventoryQuests, DisplayQuests) == 0x178);
        FABLE_STATIC_ASSERT(offsetof(CTCInventoryQuests, AccumFadeTime) == 0x184);
    }
};
FABLE_STATIC_ASSERT(sizeof(CTCInventoryQuests) == 0x188);
#endif // FABLE_ENGINE_CTCINVENTORYQUESTS_H
