#ifndef FABLE_ENGINE_CTESTNEWNAV1SCRIPT_H
#define FABLE_ENGINE_CTESTNEWNAV1SCRIPT_H
// NScript::CTestNewNav1Script (NF434, new header; no includer before NF434). The quest-script default shape: CLevelScriptBase<NScript::CEmptyData> with no
// members of its own.
// Layout: Ego_r.pdb CTestNewNav1Script (struct_layouts_egor.tsv, 92 bytes) at retail size = PDB - 0x14 = 0x48 (retail CScriptBase is
// 0x40); the landed Alloc 0x00EEF9C0 news 0x48.
// Methods (FableWin.pdb LF_CLASS field list; the ctor is user-declared and Ego_r.pdb has no out-of-line copy, so it is inline; the
// destructor is compiler-generated): ctor (CScriptDataBase*, CGameScriptInterfaceBase&), static Alloc, and the public virtual overrides
// below, in the FableWin.pdb ??_7CTestNewNav1Script@NScript@@6B@ slot order (retail vftable 0x012F5C8C).
// Includers define FABLE_RETAIL_CLEVELSCRIPTBASE as well: the implicit destructor then calls ~CScriptBase 0x00CBD510 directly, as retail.
#ifdef FABLE_RETAIL_NSCRIPT   // the retail NScript model only; no #error branch (VC7.1 under Wine spins on #error)
#include "rebuild_abi.h"
#include "engine/CLevelScriptBase.h"
class CScriptDataBase;
class CGameScriptInterfaceBase;
namespace NScript {
class CEmptyData;
class CTestNewNav1Script : public CLevelScriptBase<CEmptyData> {
public:
    CTestNewNav1Script(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CEmptyData>(data, game) { }
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game); // 0x00EEF9C0
    virtual void RegisterMain();                            // slot 1  0x00EEF900
    virtual void Main();                                    // slot 2  0x00EEF8F0
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CTestNewNav1Script) == 0x48);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CTESTNEWNAV1SCRIPT_H
