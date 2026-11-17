// CQuestManager::GetActiveQuestFromScriptName(const CCharString& name) const -- the active quest whose QuestName equals the name, or 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AF5B0, length 87 bytes.
// Leaf: ?GetActiveQuestFromScriptName@CQuestManager@@QBEPAVCActiveQuest@1@ABVCCharString@@@Z
// Retail bytes: push ebx ; push ebp ; mov ebp,[ecx+38h] ; push esi ; push edi ; mov edi,[ebp] ; cmp edi,ebp ; je N ; mov eax,[esp+14h] ; mov esi,[eax] ;
//               L: mov ebx,[edi+8] ; mov eax,[ebx+30h] ; cmp eax,esi ; je F ; (null/length tests, Compare, neg/sbb/inc) ; jne F ; mov edi,[edi] ; cmp edi,ebp ; jne L ;
//               N: ... xor eax,eax ; ret 4 ; F: ... mov eax,ebx ; ret 4
// Callee: Compare(const char*, const char*) @ 0x00411570 (`__fastcall`) inside the inlined CCharString equality.
// The manifest label "CScriptBase::OnRegionLoaded" is propagated (that one is 0x00CB7B00): the callers are CQuestManager
// code (ProcessQuestRegions 0x004B01B0, 0x004B11A0, SetQuestAsCompleted 0x004B1D30), `this` + 0x38 is ActiveQuests (retail
// CQuestManager: RegisteredQuestNames is the 12-byte vector at +0x2c, as in the landed GetRegisteredQuestFromIndex, so the
// 4-byte STLport list follows at +0x38), each node holds a CCountedPointer<CActiveQuest> (object at node + 8), and
// CActiveQuest::QuestName sits at +0x30 (PDB +0x34 less the 4 bytes of the retail Regions vector). The PDB lists
// ?GetActiveQuestFromScriptName@CQuestManager@@QBEPAVCActiveQuest@1@ABVCCharString@@@Z as the only const CQuestManager
// member returning a CActiveQuest* from a name. Shape: retail keeps the list head in ebp and the argument's string block in
// esi for the whole walk although the loop calls Compare (the retail build knows that callee writes no memory; the lane
// re-reads both after the call), so both are explicit locals here, the block read once after the empty test; the found
// quest is returned from ebx.
// NF56 (round 7, queue q10445): Ego_r.pdb 0x004B48E0 / FableWin.pdb 0x01C516E0 ?GetActiveQuestFromScriptName@CQuestManager@@QBEPAVCActiveQuest@1@ABVCCharString@@@Z
//   (CActiveQuest is nested in CQuestManager, class key V). The row now defines it on engine/CQuestManager.h: the typed ActiveQuests list
//   (+0x38, std::list<CCountedPointer<CActiveQuest> >, FABLE_RETAIL_CQUESTMANAGER_SAVEGAMESTATE, which also makes the manager a friend of the
//   record for QuestName +0x30) and the overlay opt-in FABLE_RETAIL_CQUESTMANAGER_GETACTIVEQUESTFROMSCRIPTNAME. Were the stand-ins
//   CQuestManagerRetail, a global CActiveQuest and a hand-made list node.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CBASICSTRING_COMPARE_BODY
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_LOADGAMESTATE
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVEQUEST
#define FABLE_RETAIL_CQUESTMANAGER_SAVEGAMESTATE
#define FABLE_RETAIL_CQUESTMANAGER_GETACTIVEQUESTFROMSCRIPTNAME
#include "fable_string.h"
#include "engine/CQuestManager.h"

CQuestManager::CActiveQuest* CQuestManager::GetActiveQuestFromScriptName(const CCharString& name) const
{
    for (std::list<CCountedPointer<CActiveQuest> >::const_iterator it = ActiveQuests.begin(); it != ActiveQuests.end(); ++it) {
        CActiveQuest* quest = it->Data;
        if (quest->QuestName == name)
            return quest;
    }
    return 0;
}
