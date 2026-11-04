// NScript::CExpression_FollowScript::Init() -- empty: retail is a lone `ret`
// Leaf: ?Init@CExpression_FollowScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE9F70, 1 bytes, thiscall, plain `ret`.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00eea3e0 at 00cd9c15 with name "Expression_Follow"; Alloc news + stores script vtable 012f55b8; PDB ?Alloc@CExpression_FollowScript@NScript@@SIPAVCScriptBase@@; script vtable slot 3 (slot 1 pushes "Main" 0x012C2F94); PDB Init; refs=1; was sub_00ee9f70 (wave3/labels_alloc.tsv); PDB ?Init@CExpression_FollowScript@NScript@@UAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Hook() on a `struct T` pad model (00/ee/gapscan_sub00ee9f70_00ee9f70.cpp, `?Hook@T@@QAEXXZ`); same bytes.
// Model: engine/CScriptBase.h + Game +0x40 (PDB ctor CExpression_FollowScript(CGameScriptInterfaceBase&): no master data); Init declared public virtual as the PDB spells it.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
namespace NScript {
class CExpression_FollowScript : public CScriptBase {
public:
    virtual void Init();                          // slot 3
    CGameScriptInterfaceBase* Game;         // +0x40 (the ctor's CGameScriptInterfaceBase&)
};
void CExpression_FollowScript::Init()
{
}
}
