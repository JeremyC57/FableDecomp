// CQuestManager::ClearQuestPersistScript(const CCharString& quest_name) -- drop the quest's runtime persistence entries and clear the thing manager's persistence data for that section
// Leaf: ?ClearQuestPersistScript@CQuestManager@@QAEXABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B39B0, length 106 bytes (`ret 4`).
// Identity: Ego_r.pdb 0x004B9D90 / FableWin.pdb ?ClearQuestPersistScript@CQuestManager@@QAEXABVCCharString@@@Z; Ego_d.pdb locals this,
//   quest_name. Its inline callees: ?ClearPersistScript@CRuntimeQuestPersistence@NQuestManager@@QAEXABVCCharString@@@Z (Ego_r / FableWin)
//   and FableWin.pdb ?EraseFromKey@?$CVectorMap@VCCharString@@VCPersistInfo@..@@QAEXABVCCharString@@@Z (Ego_d locals key, lower, upper).
// Callees: CVectorMap<CCharString, CPersistInfo>::LowerBound @ 0x004B9750 / UpperBound @ 0x004B9850, STLport
//   __copy(first, last, result, random_access_iterator_tag, ptrdiff_t*) @ 0x004B84F0 (`__fastcall`), __destroy_aux(first, last,
//   __false_type) @ 0x004B93E0 (`__fastcall`), CWorld::GetThingManager (slot 3), CThingManager::ClearPersistenceDataForSection @ 0x00520C70.
// Shape: EraseFromKey's range erase is STLport vector::erase(first, last) inline; with UpperBound's body in the TU VC7.1 knows it writes no
//   memory, so the finish read for the end() test is reused as __copy's `last`.
// NF85 (q11087): moved off the row-local CCharString / CPersistMap / CQuestManager / CWorld / CThingManager models onto the shared
//   headers (overlay opt-ins FABLE_RETAIL_CQUESTMANAGER_CLEARQUESTPERSISTSCRIPT, FABLE_RETAIL_CVECTORMAP_ERASEFROMKEY,
//   FABLE_RETAIL_CVECTORMAP_UPPERBOUND_COUNT_FIRST), so LowerBound binds the CPersistInfo spelling.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CTHINGMANAGER_ACCESSORS
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_LOADGAMESTATE
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVATEMULTIPLEQUESTS
#define FABLE_RETAIL_CQUESTMANAGER_CLEARQUESTPERSISTSCRIPT
#define FABLE_RETAIL_CVECTORMAP_PODIUM_UPPERBOUND
#define FABLE_RETAIL_CVECTORMAP_UPPERBOUND_COUNT_FIRST
#define FABLE_RETAIL_CVECTORMAP_LOWERBOUND_COUNT_FIRST
#define FABLE_RETAIL_CVECTORMAP_ERASEFROMKEY
#define FABLE_RETAIL_STLP_COPY_NOINLINE
#define FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE_FASTCALL
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingManager.h"
#include "engine/CWorld.h"
#include "engine/CScriptInfo.h"
#include "engine/CScriptInfoManager.h"
#include "engine/CQuestManager.h"
#include "engine/CVectorMap.h"

void CQuestManager::ClearQuestPersistScript(const CCharString& quest_name)
{
    RuntimeQuestPersistence.ClearPersistScript(quest_name);
    World->GetThingManager().ClearPersistenceDataForSection(quest_name);
}
