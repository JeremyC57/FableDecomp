#ifndef FABLE_ENGINE_CALEXTESTSCRIPT_H
#define FABLE_ENGINE_CALEXTESTSCRIPT_H
// NScript::CalextestScript (NF434, new header; no includer before NF434). The quest-script default shape: CLevelScriptBase<NScript::CalextestData> with no
// members of its own.
// Layout: Ego_r.pdb CalextestScript (struct_layouts_egor.tsv, 92 bytes) at retail size = PDB - 0x14 = 0x48 (retail CScriptBase is
// 0x40); the landed Alloc 0x00EED220 news 0x48.
// Methods (FableWin.pdb LF_CLASS field list; the ctor is user-declared and Ego_r.pdb has no out-of-line copy, so it is inline; the
// destructor is compiler-generated): ctor (CScriptDataBase*, CGameScriptInterfaceBase&), static Alloc, and the public virtual overrides
// below, in the FableWin.pdb ??_7CalextestScript@NScript@@6B@ slot order (retail vftable 0x012F57E4).
// Includers define FABLE_RETAIL_CLEVELSCRIPTBASE as well: the implicit destructor then calls ~CScriptBase 0x00CBD510 directly, as retail.
#ifdef FABLE_RETAIL_NSCRIPT   // the retail NScript model only; no #error branch (VC7.1 under Wine spins on #error)
#include "rebuild_abi.h"
#include "engine/CLevelScriptBase.h"
class CScriptDataBase;
class CGameScriptInterfaceBase;
namespace NScript {
class CalextestData;
class CalextestScript : public CLevelScriptBase<CalextestData> {
public:
    CalextestScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CalextestData>(data, game) { }
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game); // 0x00EED220
    virtual void RegisterMain();                            // slot 1  0x00EED160
    virtual void Main();                                    // slot 2  0x00EED0E0
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CalextestScript) == 0x48);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CALEXTESTSCRIPT_H
