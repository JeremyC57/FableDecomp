// NQuestManager::CRuntimeQuestPersistence::LoadFromGameStateScript(CCharString&) -- empty both persist maps, then parse every START_NEW_QUEST <quest> <script> END_NEW_QUEST block of the game-state text into the quest-script map and every START_NEW_MASTER_QUEST_DATA block into the master-data map, re-sorting each map once if anything was added
// Leaf: ?LoadFromGameStateScript@CRuntimeQuestPersistence@NQuestManager@@QAEXAAVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B5080, 1149 bytes (int3 padded to 1152), `ret 4`.
// Identity: Ego_r.pdb (0x004BB5D0) / FableWin.pdb (0x01C5F1D0) ?LoadFromGameStateScript@CRuntimeQuestPersistence@NQuestManager@@QAEXAAVCCharString@@@Z
//   (a non-const reference: engine/CQuestManager.h declares it so and the landed caller CQuestManager::LoadGameState 0x004B5500 binds
//   AAV); the old row's const-reference leaf (ABV) was a stand-in.
// Callees: CRuntimeQuestPersistence::Clear 0x004B41F0 (Ego_r / FableWin), CStringParser ctor 0x00404720 and its implicit inline dtor
//   (~list 0x004046B0, two ~CCharString, ~CBaseClass 0x0099A2E0), SkipPastNextItemIfIs 0x009B9E00, ReadAsStringUntilString 0x009BA330,
//   ReadAsStringUntilWhitespace 0x009BA4A0, SkipWhitespace 0x009B9C60, SkipPastString 0x009B9BA0, CCharString() 0x0099E4B0 /
//   (const char*, long) 0x0099EBF0 / copy 0x0099EC30 / operator= 0x0099EFB0 / ~ 0x0099EAE0, the STLport _M_insert_overflow 0x004BB400
//   of push_back, and std::sort 0x004BBB40 (`__fastcall(first, last, compare)`).
// Shape: CVectorMap::Insert (push_back of a value_type temporary, then Dirty) per block, then each map's Update (Sort when Dirty).
// NF87: moved off the row-local CCharString / CStringParser / CRuntimeQuestPersistence / CPersistMap models onto the shared headers, so
//   Clear and std::sort bind their PDB / STLport spellings.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_LOADGAMESTATE
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVATEMULTIPLEQUESTS
#define FABLE_RETAIL_NQUESTMANAGER_CRUNTIMEQUESTPERSISTENCE_CLEAR
#define FABLE_RETAIL_CVECTORMAP_SORT
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#define FABLE_RETAIL_CVECTORMAP_INSERT_VALUE_TYPE
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CStringParser.h"
#include "engine/CScriptInfo.h"
#include "engine/CScriptInfoManager.h"
#include "engine/CQuestManager.h"
#include "engine/CVectorMap.h"

void NQuestManager::CRuntimeQuestPersistence::LoadFromGameStateScript(CCharString& text)
{
    Clear();
    CStringParser parser(text);
    parser.ReadAsStringUntilString("START_NEW_QUEST");
    while (parser.SkipPastNextItemIfIs("START_NEW_QUEST")) {
        parser.SkipWhitespace();
        CPersistInfo info;
        info.QuestName = parser.ReadAsStringUntilWhitespace();
        parser.SkipWhitespace();
        info.PersistScript = parser.ReadAsStringUntilString("END_NEW_QUEST");
        PersistScripts.Insert(info.QuestName, info);
        parser.SkipPastString("END_NEW_QUEST");
    }
    parser.ReadAsStringUntilString("START_NEW_MASTER_QUEST_DATA");
    while (parser.SkipPastNextItemIfIs("START_NEW_MASTER_QUEST_DATA")) {
        parser.SkipWhitespace();
        CPersistInfo info;
        info.QuestName = parser.ReadAsStringUntilWhitespace();
        parser.SkipWhitespace();
        info.PersistScript = parser.ReadAsStringUntilString("END_NEW_MASTER_QUEST_DATA");
        MasterDataPersistScripts.Insert(info.QuestName, info);
        parser.SkipPastString("END_NEW_MASTER_QUEST_DATA");
    }
    PersistScripts.Update();
    MasterDataPersistScripts.Update();
}
