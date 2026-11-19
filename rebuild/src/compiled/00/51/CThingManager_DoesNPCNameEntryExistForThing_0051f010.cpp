// CThingManager::DoesNPCNameEntryExistForThing(unsigned __int64 uid) -- whether the NPC name map holds an entry for the thing's UID
// Leaf: ?DoesNPCNameEntryExistForThing@CThingManager@@QAE_N_K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051F010, 32 bytes, thiscall, `ret 8` (the 8-byte UID).
// Identity: Ego_r.pdb 0x00529030 and FableWin.pdb 0x01D44E70 ?DoesNPCNameEntryExistForThing@CThingManager@@QAE_N_K@Z; its one
//   caller CTCHasName::GetNPCShortName 0x007F4990 (call 0x007F49AB) passes the same UID it then hands to
//   GetNameForThing_OnlyCalledFromTCHasName 0x0051F040, this row's twin. Was catalogued _unattributed::Contains on a local
//   CKeyedSet_0051f010 model.
// Callees: NPCNameMap's _Rb_tree::_M_find(const unsigned __int64&) const 0x00527E00 (out of line, node pointer in eax; the
//   twin 0x0051F040 binds the same instance).
// Layout: retail CThingManager::NPCNameMap (map<unsigned __int64, pair<CCharString, CWideString> >) at +0xCC (header opt-in
//   FABLE_RETAIL_CTHINGMANAGER_NPCNAMES); its _M_header node pointer at +0 is end().
// Shape: the found iterator is a named local, then compared with end() -- retail reads the header word after the call
//   (0x0051F023 `mov edx,[esi]`); the one-expression `find(uid) != end()` evaluates end() first (VC7.1 right-to-left) and
//   loads it before the call (DIFFER 36v34). The map is addressed once (esi); the UID goes by reference (its own slot).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER_NPCNAMES
#define FABLE_RETAIL_CTHINGMANAGER_NPCSHORTNAME
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_MFIND_DECL
#include "fable_string.h"
#include "engine/CThingManager.h"

bool CThingManager::DoesNPCNameEntryExistForThing(unsigned __int64 uid)
{
    std::map<unsigned __int64, std::pair<CCharString, CWideString> >::iterator it = NPCNameMap.find(uid);
    return it != NPCNameMap.end();
}
