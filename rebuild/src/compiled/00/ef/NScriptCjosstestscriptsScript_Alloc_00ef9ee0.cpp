// NScript::Cjoss_test_scriptsScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new Cjoss_test_scriptsScript(data, game)
// Leaf: ?Alloc@Cjoss_test_scriptsScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EF9EE0, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Callees: scalar operator new 0x00BFEA1A (0x48 = sizeof), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable 0x012F68D0).
// Model (NF434): the shared engine/Cjoss_test_scriptsScript.h (new: Ego_r.pdb layout, FableWin.pdb overrides) on engine/CLevelScriptBase.h, replacing a local
//   class that declared none of the class's own virtuals (its vftable bound CScriptBase's slots) and an out-of-line destructor (FableWin.pdb
//   marks it compiler-generated). This TU now also emits the implicit ??_G with the vftable, as retail's TU did (the ??_G follows Alloc).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#include "engine/Cjoss_test_scriptsScript.h"
CScriptBase* FABLE_FASTCALL NScript::Cjoss_test_scriptsScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new Cjoss_test_scriptsScript(data, game);
}
