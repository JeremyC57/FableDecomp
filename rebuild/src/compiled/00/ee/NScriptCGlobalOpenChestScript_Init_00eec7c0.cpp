// NScript::CGlobal_OpenChestScript::Init() -- empty: retail is a lone `ret`
// Leaf: ?Init@CGlobal_OpenChestScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EEC7C0, 1 bytes, thiscall, plain `ret`.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00eecea0 at 00cda176 with name "Global_OpenChest"; Alloc news + stores script vtable 012f5760; PDB ?Alloc@CGlobal_OpenChestScript@NScript@@SIPAVCScriptBase@@; script vtable slot 3 (slot 1 pushes "Main" 0x012C2F94); PDB Init; refs=1; was sub_00eec7c0 (wave3/labels_alloc.tsv); PDB ?Init@CGlobal_OpenChestScript@NScript@@UAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Hook() on a `struct T` pad model (00/ee/gapscan_sub00eec7c0_00eec7c0.cpp, `?Hook@T@@QAEXXZ`); same bytes.
// Model: engine/CScriptBase.h + Game +0x40 (PDB ctor CGlobal_OpenChestScript(CGameScriptInterfaceBase&): no master data); Init declared public virtual as the PDB spells it.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
namespace NScript {
class CGlobal_OpenChestScript : public CScriptBase {
public:
    virtual void Init();                          // slot 3
    CGameScriptInterfaceBase* Game;         // +0x40 (the ctor's CGameScriptInterfaceBase&)
};
void CGlobal_OpenChestScript::Init()
{
}
}
