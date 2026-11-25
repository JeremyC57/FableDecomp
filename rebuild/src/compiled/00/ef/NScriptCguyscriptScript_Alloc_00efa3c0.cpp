// NScript::Cguy_scriptScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new Cguy_scriptScript(data, game)
// Leaf: ?Alloc@Cguy_scriptScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EFA3C0, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Callees: scalar operator new 0x00BFEA1A (0x48 = sizeof), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable 0x012F696C).
// Model (NF434): the shared engine/Cguy_scriptScript.h (new: Ego_r.pdb layout, FableWin.pdb overrides) on engine/CLevelScriptBase.h, replacing a local
//   class that declared none of the class's own virtuals (its vftable bound CScriptBase's slots) and an out-of-line destructor (FableWin.pdb
//   marks it compiler-generated). This TU now also emits the implicit ??_G with the vftable, as retail's TU did (the ??_G follows Alloc).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#include "engine/Cguy_scriptScript.h"
CScriptBase* FABLE_FASTCALL NScript::Cguy_scriptScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new Cguy_scriptScript(data, game);
}
