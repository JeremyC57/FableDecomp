// NQuestManager::CRuntimeQuestPersistence::GetQuestMasterDataPersistScript(const CCharString&) const -- the master-data persist script recorded for the quest, or null when the quest has none
// Leaf: ?GetQuestMasterDataPersistScript@CRuntimeQuestPersistence@NQuestManager@@QBEPBVCCharString@@ABV3@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B27F0, length 82 bytes.
// Callees: CVectorMap<CCharString, CPersistInfo>::LowerBound(const CCharString&) @ 0x004B97D0,
// CBasicString<char>::operator< @ 0x00429950 (on the string blocks, the inlined CCharString <).
// The manifest label "CQuestManager::LookupPersistentQuestInfo" is a guess and is rejected: `this` holds the
// sorted map at +0x10 -- PDB CRuntimeQuestPersistence {PersistScripts +0, MasterDataPersistScripts +0x14}, the
// shipped CVectorMap being 16 bytes, so MasterDataPersistScripts sits at +0x10 (the sibling
// SetQuestMasterDataPersistScript @ 0x004B4F30, called by CQuestManager on its member at +0xc, uses the same
// +0x10 map); FableWin.pdb names ?GetQuestMasterDataPersistScript@CRuntimeQuestPersistence@NQuestManager@@QBEPBVCCharString@@ABV3@@Z.
// The element is pair<CCharString, CPersistInfo {QuestName, PersistScript}>: the result is &second.PersistScript (+8).
// Shape: the map's inline find (`(it == last || key < it->first) ? last : it` with `last` a local read once into
// esi after the call) and the caller's own `it != end()` test re-reading +0x14, the found return first;
// CCharString < compares the blocks (equal -> false, null key -> true, null element -> false, else the block compare).
// NF80 (q10923): moved onto the shared headers -- engine/CQuestManager.h (FABLE_RETAIL_CQUESTMANAGER_LOADGAMESTATE +
//   _ACTIVATEMULTIPLEQUESTS: CRuntimeQuestPersistence with its two CVectorMap<CCharString, CPersistInfo> members and the const
//   declaration both PDBs give), engine/CVectorMap.h (Find over the out-of-line LowerBound 0x004B97D0) and fable_string.h
//   (CCharString::operator< over the one CBasicString<char>::operator< body, FABLE_RETAIL_CBASICSTRING_LESS_BODY). The function is
//   const (QBE) as FableWin.pdb names it and the landed caller 0x004B3760 binds it; the local CCharString / CCharStringData /
//   CRuntimeQuestPersistence / map models are gone. Preamble: the landed quest-manager TU rows (0x004BAEF0 / 0x004B3CE0).
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
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

const CCharString* NQuestManager::CRuntimeQuestPersistence::GetQuestMasterDataPersistScript(const CCharString& quest_name) const
{
    CVectorMap<CCharString, CPersistInfo>::const_iterator it = MasterDataPersistScripts.Find(quest_name);
    if (it != MasterDataPersistScripts.end())
        return &it->second.PersistScript;
    return 0;
}
