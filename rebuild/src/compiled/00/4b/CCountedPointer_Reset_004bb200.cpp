// CCountedPointer<CQuestManager::CActiveQuest>::Reset(CQuestManager::CActiveQuest* data) -- drop the shared count block (destroying the quest on last reference), then adopt the new object
// Leaf: ?Reset@?$CCountedPointer@VCActiveQuest@CQuestManager@@@@QAEXPAVCActiveQuest@CQuestManager@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BB200, 103 bytes, `ret 4`.
// Flags: none
// Name: FableWin.pdb 0x01C64310 ?Reset@?$CCountedPointer@VCActiveQuest@CQuestManager@@@@QAEXPAVCActiveQuest@CQuestManager@@@Z; the count
//   block this body builds stores 0x004BAEF0, ?DeleteData@?$CCountedPointer@VCActiveQuest@CQuestManager@@@@SIXPAX@Z (Ego_r.pdb 0x004BE0B0
//   + FableWin.pdb 0x01C67F10, landed row 0x004BAEF0). Name-fix pass (2026-11-12): was the stand-in CCountedPointer_004baef0::Reset with a
//   local count block and Destroy_004baef0; now the explicit instantiation of the engine/CCountedPointer.h Reset (the landed
//   CCountedPointer<CAIBrain>::Reset 0x00834E40 spelling) on engine/CQuestManager.h.
// Callees: operator delete 0x00BFE9BC, operator new 0x00BFEA1A; stores DeleteData 0x004BAEF0.
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

template void CCountedPointer<CQuestManager::CActiveQuest>::Reset(CQuestManager::CActiveQuest*);
