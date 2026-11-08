#ifndef FABLE_ENGINE_CQUESTCARDDEF_ENGINE_H
#define FABLE_ENGINE_CQUESTCARDDEF_ENGINE_H
// CQuestCardDef -- the quest card definition (name / summary / objective text indices, region, rewards, core / vignette flags,
// prerequisites, boast count). New shared header (chunk 882, row 0x0061E6D0 CTCInventoryQuests::ConstructQuestListPC); the
// generated flat layout stays in engine/_quarantine/CQuestCardDef.h (same offsets).
// Retail-only model: requires the retail def chain (FABLE_RETAIL_DEF_CLASS_BASE, FABLE_RETAIL_CDEFSTRINGTABLE, FABLE_RETAIL_CCHARSTRING).
// Base: CSubDefClassBase (packed 0x25 bytes; Ego_r.pdb CQuestCardDef 0x7C carries 0x28 bytes of base and 16-byte Dinkumware
// containers). Members: Ego_r.pdb (struct_layouts_egor.tsv) minus 4 after RewardObjects (12-byte STLport vector) -- retail reads
// IsCore +0x58 (landed CTCQuestCard::IsOptional 0x007022C0), IsVignette +0x59 (0x007022E0), IsExclusive +0x5A (0x007022A0),
// Prerequisites node count +0x60 and header +0x5C (0x0061F734 / 0x0061F76B), NumBoasts +0x68 and RegionName +0x38 (0x0062049A).
// The derived members are naturally aligned (QuestName at +0x28 after the 0x25-byte packed base). sizeof 0x74.
// EQuestCardPrerequisite: Ego_r.pdb `W4EQuestCardPrerequisite@@` (global); enumerator names are not in either PDB's name set --
// the one value the row uses (0, the renown level the hero needs) is named descriptively.
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#error engine/CQuestCardDef.h is the retail def model: define FABLE_RETAIL_DEF_CLASS_BASE before including it
#else
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CDefString.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"

enum EQuestCardPrerequisite {
    QUEST_CARD_PREREQUISITE_RENOWN = 0                                  // descriptive (unproven spelling): the renown level needed
};

class CQuestCardDef : public CSubDefClassBase {
public:
    long        QuestName;                                               // +0x28 text-bank index
    long        QuestSummary;                                            // +0x2c
    long        QuestObjective;                                          // +0x30
    long        SuccessSummary;                                          // +0x34
    CDefString  RegionName;                                              // +0x38
    CDefString  TeleporterRegionName;                                    // +0x3c
    long        InventoryCategory;                                       // +0x40
    long        RenownReward;                                            // +0x44
    long        GoldReward;                                              // +0x48
    std::vector<long> RewardObjects;                                     // +0x4c
    bool        IsCore;                                                  // +0x58
    bool        IsVignette;                                              // +0x59
    bool        IsExclusive;                                             // +0x5a
    bool        MakeVignetteRouteAppearOnMinimap;                        // +0x5b
    std::map<EQuestCardPrerequisite, long> Prerequisites;               // +0x5c
    long        NumBoasts;                                               // +0x68
    bool        CanPlayerCancel;                                         // +0x6c
    unsigned long QuestEpilogue;                                         // +0x70
};
FABLE_STATIC_ASSERT(sizeof(CQuestCardDef) == 0x74);
FABLE_STATIC_ASSERT(offsetof(CQuestCardDef, QuestName) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CQuestCardDef, RegionName) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CQuestCardDef, IsCore) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CQuestCardDef, IsVignette) == 0x59);
FABLE_STATIC_ASSERT(offsetof(CQuestCardDef, Prerequisites) == 0x5c);
FABLE_STATIC_ASSERT(offsetof(CQuestCardDef, NumBoasts) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CQuestCardDef, QuestEpilogue) == 0x70);

#endif // FABLE_RETAIL_DEF_CLASS_BASE
#endif // FABLE_ENGINE_CQUESTCARDDEF_ENGINE_H
