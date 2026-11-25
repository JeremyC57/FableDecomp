// Leaf: ?IsPlayerCreatureBlocking@CGameScriptInterface@@UBE_NXZ
// CGameScriptInterface::IsPlayerCreatureBlocking @ 00899ba0
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CThingPlayerCreature;
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
 /* 0x00449970 CPlayerManager::GetMainPlayer (Ego_r 0x004450F0) */
class CTCBase;
// Naming (NF437, 2026-11-25): the LowerBound split -- the row-local free stand-in (the stand-in ?LowerBound@@YIPAUNode@@PAXUKeyArg@@@Z) over a raw map pointer replaced by the shared
//   CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<..> > typedef of engine/CVectorMap.h that the landed AI rows use (0x00929F10): CThing::Components at +0x44 (0x10 bytes),
//   so the call binds the established ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@...@@QBE spelling at 0x004365B0.
#include "engine/ETCInterfaceType.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
extern char  __fastcall CheckBlocking(void* obj);

bool CGameScriptInterface::IsPlayerCreatureBlocking() const
{
    void* self = (void*)this;
    CPlayer* mgr = (&((CPlayerManager*)(*(void**)((char*)self + 0x14)))->GetMainPlayer());
    CThingPlayerCreature* e = mgr->GetPControlledCreature();
    if (e && !(*(unsigned char*)((char*)e + 0x91) & 1)
          && (*(unsigned int*)((char*)e + 0x24) & 0x10000))
    {
        const CInterfaceMap* map = (const CInterfaceMap*)((char*)e + 0x44);
        ETCInterfaceType key = (ETCInterfaceType)0x30;
        const CInterfaceMap::value_type* found = map->LowerBound(key);
        const CInterfaceMap::value_type* end = map->end();
        if (found == end || found->first > 0x30)
            found = end;
        if (CheckBlocking(found->second))
            return true;
    }
    return false;
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_IsPlayerCreatureBlocking -> CGameScriptInterface::IsPlayerCreatureBlocking, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
