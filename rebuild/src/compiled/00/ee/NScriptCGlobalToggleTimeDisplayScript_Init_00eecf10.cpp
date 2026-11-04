// NScript::CGlobal_ToggleTimeDisplayScript::Init() -- reset the script state: TimeDisplayActive = false
// Leaf: ?Init@CGlobal_ToggleTimeDisplayScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EECF10, 5 bytes, thiscall, plain `ret`.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00eed060 at 00cda044 with name "Global_ToggleTimeDisplay"; Alloc news + stores script vtable 012f57a0; PDB ?Alloc@CGlobal_ToggleTimeDisplayScript@NScript@@SIPAVCScriptBase@@; script vtable slot 3 (slot 1 pushes "Main" 0x012C2F94); PDB Init; refs=1; was sub_00eecf10 (wave3/labels_alloc.tsv); PDB ?Init@CGlobal_ToggleTimeDisplayScript@NScript@@UAEXXZ in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Clear() on a `struct T` pad model (00/ee/gapscan_sub00eecf10_00eecf10.cpp, `?Clear@T@@QAEXXZ`); same bytes.
// Model: engine/CScriptBase.h + Game +0x40 (PDB ctor CGlobal_ToggleTimeDisplayScript(CGameScriptInterfaceBase&): no master data); the members this body stores are named from
//   Ego_r.pdb CGlobal_ToggleTimeDisplayScript at +0x14 (the retail script base is 0x14 smaller), the rest is not modelled here.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
namespace NScript {
class CGlobal_ToggleTimeDisplayScript : public CScriptBase {
public:
    virtual void Init();                          // slot 3
    CGameScriptInterfaceBase* Game;         // +0x40 (the ctor's CGameScriptInterfaceBase&)
    bool TimeDisplayActive;             // +0x44 (Ego_r.pdb +0x58)
};
void CGlobal_ToggleTimeDisplayScript::Init()
{
    TimeDisplayActive = false;
}
}
