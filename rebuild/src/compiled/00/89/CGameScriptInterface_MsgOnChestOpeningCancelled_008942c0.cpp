// Leaf: ?MsgOnChestOpeningCancelled@CGameScriptInterface@@UBE_NXZ
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

struct CGameScriptInterface_Methods : CGameScriptInterface { void GSI_PreMsg(); void GSI_PostMsg(); };
class CMessageEvent;
class CMessageEventManager { public:
    void RemoveMessage(CMessageEvent const*a0);   // 0x00493B5B (Ego_r.pdb ?RemoveMessage@CMessageEventManager@@QAEXPBVCMessageEvent@@@Z)
};
struct CMgr { void* Find(int* pKey);  };
bool CGameScriptInterface::MsgOnChestOpeningCancelled() const
{
    CGameScriptInterface* self = const_cast<CGameScriptInterface*>(this);
 CGameScriptInterface_Methods* methods=(CGameScriptInterface_Methods*)self;
 methods->GSI_PreMsg();methods->GSI_PostMsg();
 CMgr* mgr=*(CMgr**)((char*)self->World+0x60);int key=0x48;void* obj=mgr->Find(&key);
 if(obj){((CMessageEventManager*)mgr)->RemoveMessage((CMessageEvent const*)obj);return true;}return false;
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_MsgOnChestOpeningCancelled -> CGameScriptInterface::MsgOnChestOpeningCancelled, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
