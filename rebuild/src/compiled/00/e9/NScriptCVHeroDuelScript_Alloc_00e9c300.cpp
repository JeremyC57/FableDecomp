// NScript::CV_HeroDuelScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script factory: new
// CV_HeroDuelScript(data, game) with the constructor inlined (CScriptBase(), Game +0x40, MasterScriptData +0x44, vtable 0x012ED0D4)
// Leaf: ?Alloc@CV_HeroDuelScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E9C300, 54 bytes, __fastcall (ecx = data, edx = game).
// Identity: `new` of 0x5C bytes = sizeof(NScript::CV_HeroDuelScript) (engine/NScript_CV_HeroDuelScript.h) with vtable 0x012ED0D4, the
//   vtable the landed NScript::CV_HeroDuelScript::CV_HeroDuelScript 0x00E986A0 stores (slot 0 the landed ??_G 0x00E9C340, slots 1..3
//   the landed CV_HeroDuelScript::RegisterMain 0x00E98700 / Main 0x00E987C0 / Init 0x00E986D0); Ego_r.pdb / FableWin.pdb
//   ?Alloc@CV_HeroDuelScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z (the static allocator
//   every script class has). The catalog's "Alloc_CVtable_012ed0d4" was a placeholder.
// Shape: the constructor is inline in this TU (the out-of-line copy 0x00E986A0 is the same body).
#define FABLE_RETAIL_NSCRIPT
#include "fable_string.h"
#include "engine/CScriptBase.h"
#include "engine/NScript_CV_HeroDuelScript.h"

inline NScript::CV_HeroDuelScript::CV_HeroDuelScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
CScriptBase* __fastcall NScript::CV_HeroDuelScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CV_HeroDuelScript(data, game);
}
