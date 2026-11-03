#ifndef FABLE_ENGINE_CQUESTCARDDEF_H
#define FABLE_ENGINE_CQUESTCARDDEF_H
// Retail model (chunk 729, hand-written): Ego_r.pdb CQuestCardDef (class key V, pdb_class_keys.tsv; 0x7C bytes in the PDB) on the
// packed retail CDefClassBase (0x25, first 4-aligned member at +0x28, CDefClassBase.h). The STLport vector RewardObjects is 0xC
// bytes in retail (PDB 0x10), so every member after it sits 4 below the PDB: IsCore +0x58 and IsVignette +0x59 (retail
// CTCQuestCard::IsCore 0x00702280 / IsVignette 0x007022E0 read [def+58h] / [def+59h]), MakeVignetteRouteAppearOnMinimap +0x5B
// (0x007022F0). Members before RewardObjects are at their PDB offsets. Requires FABLE_RETAIL_DEF_CLASS_BASE.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefString.h"
#include "stlport/fable_containers.h"
class CQuestCardDef : public CDefClassBase {
public:
    long QuestName;                                          // +0x28
    long QuestSummary;                                       // +0x2c
    long QuestObjective;                                     // +0x30
    long SuccessSummary;                                     // +0x34
    CDefString RegionName;                                   // +0x38
    CDefString TeleporterRegionName;                         // +0x3c
    long InventoryCategory;                                  // +0x40
    long RenownReward;                                       // +0x44
    long GoldReward;                                         // +0x48
    std::vector<long> RewardObjects;                         // +0x4c
    bool IsCore;                                             // +0x58
    bool IsVignette;                                         // +0x59
    bool IsExclusive;                                        // +0x5a
    bool MakeVignetteRouteAppearOnMinimap;                   // +0x5b
    unsigned char Prerequisites[0xc];                        // +0x5c map<EQuestCardPrerequisite,long>
    long NumBoasts;                                          // +0x68
    bool CanPlayerCancel;                                    // +0x6c
    unsigned long QuestEpilogue;                             // +0x70
    static void CheckRetailLayout() {
        FABLE_STATIC_ASSERT(offsetof(CQuestCardDef, QuestName) == 0x28);
        FABLE_STATIC_ASSERT(offsetof(CQuestCardDef, IsCore) == 0x58);
        FABLE_STATIC_ASSERT(offsetof(CQuestCardDef, IsVignette) == 0x59);
        FABLE_STATIC_ASSERT(offsetof(CQuestCardDef, QuestEpilogue) == 0x70);
    }
};
FABLE_STATIC_ASSERT(sizeof(CQuestCardDef) == 0x74);
#endif
