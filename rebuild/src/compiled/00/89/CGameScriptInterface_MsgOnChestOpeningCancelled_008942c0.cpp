// Leaf: ?MsgOnChestOpeningCancelled@CGameScriptInterface@@UBE_NXZ
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

struct CGameScriptInterface_Methods : CGameScriptInterface { void GSI_PreMsg(); void GSI_PostMsg(); };
class CMessageEvent;
class CMessageEventManager { public:
    void RemoveMessage(CMessageEvent const*a0);   // 0x00493B5B (Ego_r.pdb ?RemoveMessage@CMessageEventManager@@QAEXPBVCMessageEvent@@@Z)
};
struct CMgr { void* Find(int* pKey);  };
// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x006E7530 ?GetMaxWorldFrameForMessages@CGameScriptInterface@@IBEJXZ (was ?GSI_PostMsg@CGameScriptInterface_Methods@@QAEXXZ)
//   0x006E7510 ?GetMinWorldFrameForMessages@CGameScriptInterface@@IBEJXZ (was ?GSI_PreMsg@CGameScriptInterface_Methods@@QAEXXZ)
bool CGameScriptInterface::MsgOnChestOpeningCancelled() const
{
    CGameScriptInterface* self = const_cast<CGameScriptInterface*>(this);
 CGameScriptInterface_Methods* methods=(CGameScriptInterface_Methods*)self;
 reinterpret_cast<const CGameScriptInterface*>(methods)->GetMinWorldFrameForMessages();reinterpret_cast<const CGameScriptInterface*>(methods)->GetMaxWorldFrameForMessages();
 CMgr* mgr=*(CMgr**)((char*)self->World+0x60);int key=0x48;void* obj=mgr->Find(&key);
 if(obj){((CMessageEventManager*)mgr)->RemoveMessage((CMessageEvent const*)obj);return true;}return false;
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_MsgOnChestOpeningCancelled -> CGameScriptInterface::MsgOnChestOpeningCancelled, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
