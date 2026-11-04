// NScript::CExpression_WaitScript::Init() -- empty: retail is a lone `ret`
// Leaf: ?Init@CExpression_WaitScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EEA450, 1 bytes, thiscall, plain `ret`.
// Identity: Anniversary wave 2026-11-04 (high): vtable 0x012F55F8 slot 3; Anniversary RTTI NScript::CExpression_WaitScript (string-literal match, validated 257/258); PDB name set has the Init member of NScript::CExpression_WaitScript; slot layout from named TLC siblings; function referenced by this vtable only (2026-11-04_anniversary/labels_high.tsv); PDB ?Init@CExpression_WaitScript@NScript@@UAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Hook() on a `struct T` pad model (00/ee/gapscan_sub00eea450_00eea450.cpp, `?Hook@T@@QAEXXZ`); same bytes.
// Model: engine/CScriptBase.h + Game +0x40 (PDB ctor CExpression_WaitScript(CGameScriptInterfaceBase&): no master data); Init declared public virtual as the PDB spells it.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
namespace NScript {
class CExpression_WaitScript : public CScriptBase {
public:
    virtual void Init();                          // slot 3
    CGameScriptInterfaceBase* Game;         // +0x40 (the ctor's CGameScriptInterfaceBase&)
};
void CExpression_WaitScript::Init()
{
}
}
