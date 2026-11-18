// NQuestManager::CRuntimeQuestPersistence::Clear() -- empty both persist-script maps (erase every entry, then release their storage)
// Leaf: ?Clear@CRuntimeQuestPersistence@NQuestManager@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B41F0, length 107 bytes. Only caller LoadFromGameStateScript 0x004B508E.
// Identity: Ego_r.pdb (0x004BB590) / FableWin.pdb (0x01C5FA60) ?Clear@CRuntimeQuestPersistence@NQuestManager@@QAEXXZ; Ego_d.pdb holds
//   `this` only and a 49-byte /Od body (four calls). The catalog's "ClearAll" (and the older "CQuestManager::RotateQuestPersistenceVectors")
//   were descriptive. `this` is the CRuntimeQuestPersistence (CQuestManager +0xc) whose two CVectorMap<CCharString, CPersistInfo> members
//   PersistScripts (+0x00) and MasterDataPersistScripts (+0x10) are cleared.
// Callees: STLport __copy(first, last, result, random_access_iterator_tag, ptrdiff_t*) @ 0x004B84F0 (`__fastcall`),
//   __destroy_aux(first, last, __false_type) @ 0x004B93E0 (`__fastcall`), CVectorMap::DeallocateMemory @ 0x004BB6B0.
// Shape: per map STLport vector::clear() (erase(begin(), end()), inline) then the out-of-line DeallocateMemory; `this` moves to the
//   second map with `add esi,10h`.
// NF87: moved off the row-local CCharString / CPersistPair / CPersistMap / CRuntimeQuestPersistence models onto the shared headers
//   (overlay opt-ins FABLE_RETAIL_NQUESTMANAGER_CRUNTIMEQUESTPERSISTENCE_CLEAR, FABLE_RETAIL_CVECTORMAP_DEALLOCATEMEMORY), so the callees
//   bind the STLport __copy / __destroy_aux and CVectorMap::DeallocateMemory spellings.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_LOADGAMESTATE
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVATEMULTIPLEQUESTS
#define FABLE_RETAIL_NQUESTMANAGER_CRUNTIMEQUESTPERSISTENCE_CLEAR
#define FABLE_RETAIL_CVECTORMAP_DEALLOCATEMEMORY
#define FABLE_RETAIL_STLP_COPY_NOINLINE
#define FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE_FASTCALL
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CScriptInfo.h"
#include "engine/CScriptInfoManager.h"
#include "engine/CQuestManager.h"
#include "engine/CVectorMap.h"

void NQuestManager::CRuntimeQuestPersistence::Clear()
{
    PersistScripts.clear();
    PersistScripts.DeallocateMemory();
    MasterDataPersistScripts.clear();
    MasterDataPersistScripts.DeallocateMemory();
}
