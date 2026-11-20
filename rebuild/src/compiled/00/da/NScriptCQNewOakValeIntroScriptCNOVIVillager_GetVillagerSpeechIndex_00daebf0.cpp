// NScript::CQ_NewOakValeIntroScript::CNOVI_Villager::GetVillagerSpeechIndex(long) -- pick a random villager speech index different from the last one used (waiting a frame on a repeat), record it in the quest script and return it; 0 once the script is done
// Leaf: ?GetVillagerSpeechIndex@CNOVI_Villager@CQ_NewOakValeIntroScript@NScript@@AAEJJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DAEBF0, 107 bytes (+ int3 padding; `ret 4`).
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?GetVillagerSpeechIndex@CNOVI_Villager@CQ_NewOakValeIntroScript@NScript@@AAEJJ@Z (private,
//   long(long)); the landed CNOVI_Villager::Main 0x00DADF80 calls it eight times on `this` with the speech list size and already binds this
//   spelling through engine/CQ_NewOakValeIntroScript.h.
//   Name-fix NF186 (q6347): was a global `struct CNOVI_Villager` with a local CQ_NewOakValeIntroScript model (no Leaf line; catalog
//   CNOVI_Villager::GetVillagerSpeechIndex without NScript / CQ_NewOakValeIntroScript nesting).
// Callees: CEntityScriptBase::IsActiveThreadTerminating() @ 0x00F35B30, rand @ 0x00BFEB16, the game script interface's slot 7 (+0x1c,
//   NewScriptFrame).
// Layout: engine/CQ_NewOakValeIntroScript.h (CNOVI_Villager: ParentClass +0x14; CQ_NewOakValeIntroScript lastVillagerSpeechIdx retail +0x98).
// Shape: quest-script style -- every statement after a possible yield re-tests IsActiveThreadTerminating(); the final
//   `IsActiveThreadTerminating() ? 0 : index` is the `neg al ; sbb eax,eax ; not eax ; and eax,edi` tail.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_NewOakValeIntroScript.h"
extern "C" int __cdecl rand();                     // 0x00BFEB16
namespace NScript {
long CQ_NewOakValeIntroScript::CNOVI_Villager::GetVillagerSpeechIndex(long count)
{
    while (!IsActiveThreadTerminating()) {
        long index = rand() % count;
        if (index != ParentClass.lastVillagerSpeechIdx) {
            if (IsActiveThreadTerminating())
                return 0;
            ParentClass.lastVillagerSpeechIdx = index;
            return IsActiveThreadTerminating() ? 0 : index;
        }
        Game->NewScriptFrame();
    }
    return 0;
}
}
