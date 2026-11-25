// NScript::CV_TrophyDealerScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CV_TrophyDealerScript(data, game)
// Leaf: ?Alloc@CV_TrophyDealerScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE80A0, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Callees: scalar operator new 0x00BFEA1A (0x48 = sizeof), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable 0x012F51F4).
// Model (NF434): the shared engine/CV_TrophyDealerScript.h (new: Ego_r.pdb layout, FableWin.pdb overrides) on engine/CLevelScriptBase.h, replacing a local
//   class that declared none of the class's own virtuals (its vftable bound CScriptBase's slots) and an out-of-line destructor (FableWin.pdb
//   marks it compiler-generated). This TU now also emits the implicit ??_G with the vftable, as retail's TU did (the ??_G follows Alloc).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#include "engine/CV_TrophyDealerScript.h"
CScriptBase* FABLE_FASTCALL NScript::CV_TrophyDealerScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CV_TrophyDealerScript(data, game);
}
