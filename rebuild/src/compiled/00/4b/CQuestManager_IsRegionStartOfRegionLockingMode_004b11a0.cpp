// CQuestManager::IsRegionStartOfRegionLockingMode(const CRegion&) const -- whether some active non-vignette, non-feat quest card's running quest contains the region
// Leaf: ?IsRegionStartOfRegionLockingMode@CQuestManager@@QBE_NABVCRegion@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B11A0, length 193 bytes, `ret 4`
// (PDB ?IsRegionStartOfRegionLockingMode@CQuestManager@@QBE_NABVCRegion@@@Z).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const
// 0x004365B0 (TC type 0x6c, the quest card), CQuestManager::GetActiveQuestFromScriptName 0x004AF5B0 (landed),
// CQuestManager::CActiveQuest::ContainsRegion(const CRegion&) const 0x004AFD80 (a set<const CRegion*> find; the
// manifest's CHitParameters::IsDivineWrathStrike label is propagated), the definition's release (virtual slot 1).
// Layout: retail ActiveQuestCards +0x5c (PDB +0x88); CTCQuestCard as the PDB (PDef +0x0c, FeatIndex +0x14,
// ScriptQuestName +0x28); CQuestCardDef refcount +4, IsVignette +0x59 (PDB +0x5d, defs sit 4 below the PDB).
// Shape: the landed IsAnyQuestActive (0x004B1080) loop: the quest card from the unchecked GetQuestCard helper (its
// register seeded from the argument word before the walk), the definition held by a scoped counted copy whose
// AddRef/DecRef run in memory, one null test shared by the copy, the `&&` guard and the release; `FeatIndex <= -1`
// gives `cmp -1 ; jg`; the early `return true` duplicates the release before the `mov al,1` exit.
// NF66 (round 7): ported onto engine/CQuestManager.h (the define block of the landed GetActiveQuestFromScriptName 0x004AF5B0 plus
//   ANYQUEST_LOCKSTART for the self and the NF66 opt-in FABLE_RETAIL_CQUESTMANAGER_ACTIVEQUEST_CONTAINSREGION), engine/CTCQuestCard.h,
//   engine/CQuestCardDef.h and engine/CThing.h: the local CCharString / CDefString / CThing / CQuestCardDef / CTCQuestCard /
//   CQuestManager / CActiveQuest models deleted, so the lookup binds
//   ?GetActiveQuestFromScriptName@CQuestManager@@QBEPAVCActiveQuest@1@ABVCCharString@@@Z (the nested CActiveQuest; was a global struct).
//   The quest card comes from the const PeekTC (const LowerBound 0x004365B0) with an uninitialised out pointer (retail keeps the caller's register when the thing has no card).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CBASICSTRING_COMPARE_BODY
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_LOADGAMESTATE
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVEQUEST
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVEQUEST_CONTAINSREGION
#define FABLE_RETAIL_CQUESTMANAGER_SAVEGAMESTATE
#define FABLE_RETAIL_CQUESTMANAGER_GETACTIVEQUESTFROMSCRIPTNAME
#define FABLE_RETAIL_CQUESTMANAGER_ANYQUEST_LOCKSTART
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCQUESTCARD
#define FABLE_RETAIL_CTCQUESTCARD_ACCESSORS
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CQuestManager.h"
#include "engine/CTCQuestCard.h"
#include "engine/CQuestCardDef.h"

bool CQuestManager::IsRegionStartOfRegionLockingMode(const CRegion& region) const
{
    for (std::list<CIntelligentPointer<CThing> >::const_iterator it = ActiveQuestCards.begin(); it != ActiveQuestCards.end(); ++it) {
        const CTCQuestCard* card;
        (*it).Get()->PeekTC(TCI_QUEST_CARD, &card);
        if (card) {
            CDefPointer<const CQuestCardDef> def = card->PDef;
            if (def.Get() && !def->IsVignette && card->FeatIndex <= -1) {
                CActiveQuest* quest = GetActiveQuestFromScriptName(card->ScriptQuestName);
                if (quest && quest->ContainsRegion(region))
                    return true;
            }
        }
    }
    return false;
}
