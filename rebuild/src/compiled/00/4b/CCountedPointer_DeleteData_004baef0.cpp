// CCountedPointer<CQuestManager::CActiveQuest>::DeleteData(void* data) -- the counted pointer's per-type deleter: destroy the active quest (out-of-line destructor) and free it
// Leaf: ?DeleteData@?$CCountedPointer@VCActiveQuest@CQuestManager@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BAEF0, 23 bytes, static __fastcall (`ret`).
// Flags: none
// Name: Ego_r.pdb 0x004BE0B0 + FableWin.pdb 0x01C67F10 ?DeleteData@?$CCountedPointer@VCActiveQuest@CQuestManager@@@@SIXPAX@Z; retail stores
//   0x004BAEF0 as the count block's deleter of CCountedPointer<CActiveQuest> in CQuestManager::ActivateQuests (0x004B3F94, 0x004B40CD;
//   the landed 0x004B3CE0 binds this symbol). Name-fix pass (2026-11-12): was the stand-in free function OnDie_3 on a CTCMapwho
//   placeholder (module CTCMapwho, a propagated label); now the explicit instantiation of the engine/CCountedPointer.h DeleteData on
//   engine/CQuestManager.h (the ActivateQuests 0x004B3CE0 header set).
// Callees: CQuestManager::CActiveQuest::~CActiveQuest 0x004B9B90 (out of line), operator delete 0x00BFE9BC.
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD_GAMESCRIPTINTERFACE
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#define FABLE_RETAIL_CSCRIPTINFO_ACCESSORS
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_LOADGAMESTATE
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVEQUEST
#define FABLE_RETAIL_CQUESTMANAGER_SAVEGAMESTATE
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVATEMULTIPLEQUESTS
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVATEQUESTS
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_LOADTHINGSWITHSECTIONNAMES
#define FABLE_RETAIL_CWORLDMAP_UPDATENAVMAPS
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CWorld.h"
#include "engine/CScriptInfo.h"
#include "engine/CScriptBase.h"
#include "engine/CScriptDataBase.h"
#include "engine/CMessageEventManager.h"
#include "engine/CTBaseSingleton.h"
#include "engine/CQuestManager.h"
#include "engine/CWorldMap.h"
#include "engine/CCountedPointer.h"

template void CCountedPointer<CQuestManager::CActiveQuest>::DeleteData(void*);
