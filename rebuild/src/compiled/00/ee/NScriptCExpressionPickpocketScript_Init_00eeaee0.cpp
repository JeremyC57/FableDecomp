// NScript::CExpression_PickpocketScript::Init() -- empty: retail is a lone `ret`
// Leaf: ?Init@CExpression_PickpocketScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EEAEE0, 1 bytes, thiscall, plain `ret`.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00eeb730 at 00cd9de0 with name "Expression_Pickpocket"; Alloc news + stores script vtable 012f56a0; PDB ?Alloc@CExpression_PickpocketScript@NScript@@SIPAVCScriptBase@@; script vtable slot 3 (slot 1 pushes "Main" 0x012C2F94); PDB Init; refs=1; was sub_00eeaee0 (wave3/labels_alloc.tsv); PDB ?Init@CExpression_PickpocketScript@NScript@@UAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Hook() on a `struct T` pad model (00/ee/gapscan_sub00eeaee0_00eeaee0.cpp, `?Hook@T@@QAEXXZ`); same bytes.
// Model: engine/CScriptBase.h + Game +0x40 (PDB ctor CExpression_PickpocketScript(CGameScriptInterfaceBase&): no master data); Init declared public virtual as the PDB spells it.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
namespace NScript {
class CExpression_PickpocketScript : public CScriptBase {
public:
    virtual void Init();                          // slot 3
    CGameScriptInterfaceBase* Game;         // +0x40 (the ctor's CGameScriptInterfaceBase&)
};
void CExpression_PickpocketScript::Init()
{
}
}
