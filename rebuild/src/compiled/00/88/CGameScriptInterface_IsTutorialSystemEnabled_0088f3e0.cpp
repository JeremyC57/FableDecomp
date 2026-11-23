// Leaf: ?IsTutorialSystemEnabled@CGameScriptInterface@@UBE_NXZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CGameScriptInterface.h under its class-model switch FABLE_RETAIL_NSCRIPT (was the flattened
//   free stand-in ?CGameScriptInterface_IsTutorialSystemEnabled@@YIDPAX@Z); the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"   // switch route: the real class model
struct TutMgr { char pad0[0xc]; bool flagC; char pad1[0x17-0xd]; bool flag17; char pad2[0xd7-0x18]; bool flagD7; };   // the three flags are bool (the PDB member returns bool)
class CUserProfileManager : public TutMgr { public: static CUserProfileManager& __fastcall Get(); };   // 0x0040D2A0 ?Get@CUserProfileManager@@SIAAV1@XZ

bool CGameScriptInterface::IsTutorialSystemEnabled() const
{
    TutMgr* p = &CUserProfileManager::Get();
    if (p->flagC)
        return p->flagD7;
    return p->flag17;
}