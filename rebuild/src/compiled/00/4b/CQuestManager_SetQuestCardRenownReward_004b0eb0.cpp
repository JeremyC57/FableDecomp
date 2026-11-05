// CQuestManager::SetQuestCardRenownReward(const CCharString&, long) -- store the renown reward on the quest-card component of the active quest card for the given script, when the card exists and is not dead
// Leaf: ?SetQuestCardRenownReward@CQuestManager@@QAEXABVCCharString@@J@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +4.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B0EB0, length 93 bytes.
// Callees: CQuestManager::GetActiveQuestCardFromScriptName(const CCharString&) @ 0x004B0C80 (manifest),
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (the thing's inlined GetInterface for the
// quest-card component, type 0x6c = word 3 / bit 12 of the presence bitset: `test ch,10h` on +0x2c).
// Identity: FableWin.pdb ?SetQuestCardRenownReward@CQuestManager@@QAEXABVCCharString@@J@Z; the twin @ 0x004B0E50 is
// SetQuestCardGoldReward.
// Layout: engine/CTCQuestCard.h (PDB) GoldReward +0x2c, RenownReward +0x30; the thing's dead flag is bit 0 of
// +0x91 (as in the landed CGameScriptInterface::DeactivateBoastUi model), the presence bitset at +0x20, the
// sorted {type, component} map at +0x44.
// Shape: the out-parameter GetInterface template (the landed 0x008CB5C0 / 0x007504E0 shape) with its result
// ignored: the key temporary, the component pointer and the quest argument share ONE dead argument slot, so the
// absent path tests whatever that slot holds (`mov eax,[esp+4]`) -- the out pointer is not initialised.
#include "engine/CTCQuestCard.h"

class CTCBase;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CQuestManager { public:
    void SetQuestCardRenownReward(const CCharString& scriptName, long reward);
public:
    CThing* GetActiveQuestCardFromScriptName(const CCharString&) const;   // 0x004B0C80
};

void CQuestManager::SetQuestCardRenownReward(const CCharString& scriptName, long reward)
{
    CThing* card = GetActiveQuestCardFromScriptName(scriptName);
    if (card && !card->IsDead()) {
        CTCQuestCard* questCard;
        card->GetTC(TCI_QUEST_CARD, &questCard);
        if (questCard)
            questCard->RenownReward = reward;
    }
}