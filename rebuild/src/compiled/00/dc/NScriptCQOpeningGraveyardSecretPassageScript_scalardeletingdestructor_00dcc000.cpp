// NScript::CQ_OpeningGraveyardSecretPassageScript::`scalar deleting destructor'(unsigned int flags) -- the implicit destructor inline
//   (eh vector destructor iterator over hero_object[4] with ~CCharString, then ~CScriptBase out of line), then free this when bit 0 is set
// Leaf: ??_GCQ_OpeningGraveyardSecretPassageScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DCC000, 50 bytes, `ret 4`; built without /EHsc like the landed rows of this script TU.
// Model (NF434): engine/CQ_OpeningGraveyardSecretPassageScript.h (Ego_r.pdb layout at retail offsets, 0x74; hero_object +0x58) on the shared
//   engine/CLevelScriptBase.h / engine/CScriptBase.h, replacing the local CScriptBase model whose in-class virtual bodies emitted pick-any
//   copies of CScriptBase::RegisterMain / Main / Init / OnPersist. FABLE_RETAIL_CLEVELSCRIPTBASE: the implicit destructor calls ~CScriptBase
//   0x00CBD510 directly (retail 0x00DCC017). The destructor is compiler-generated (FableWin.pdb), so the vftable this TU emits carries the
//   class's own RegisterMain / Main / Init / OnPersist (header) and the ??_G copy is the leaf.
// Emission: the PDB constructor (header opt-in FABLE_RETAIL_CQ_OPENINGGRAVEYARDSECRETPASSAGESCRIPT_CTORS) is defined inline here and
//   expanded by an anonymous-namespace helper (Fidelity: scaffolding, not retail; retail's own constructor is 0x00DC2570).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CQ_OPENINGGRAVEYARDSECRETPASSAGESCRIPT_CTORS
#include "engine/CQ_OpeningGraveyardSecretPassageScript.h"
inline NScript::CQ_OpeningGraveyardSecretPassageScript::CQ_OpeningGraveyardSecretPassageScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
namespace {
// Fidelity: scaffolding, not retail -- expands the constructor so the TU emits the vftable and the ??_G leaf (anonymous namespace,
//   hashed name; the 0x00BB1740 / 0x00BCA060 pattern).
CScriptBase* EmitScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new NScript::CQ_OpeningGraveyardSecretPassageScript(data, game);
}
}
