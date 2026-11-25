#ifndef FABLE_ENGINE_CJOSS_TEST_SCRIPTSSCRIPT_H
#define FABLE_ENGINE_CJOSS_TEST_SCRIPTSSCRIPT_H
// NScript::Cjoss_test_scriptsScript (NF434, new header; no includer before NF434). The quest-script default shape: CLevelScriptBase<NScript::Cjoss_test_scriptsData> with no
// members of its own.
// Layout: Ego_r.pdb Cjoss_test_scriptsScript (struct_layouts_egor.tsv, 92 bytes) at retail size = PDB - 0x14 = 0x48 (retail CScriptBase is
// 0x40); the landed Alloc 0x00EF9EE0 news 0x48.
// Methods (FableWin.pdb LF_CLASS field list; the ctor is user-declared and Ego_r.pdb has no out-of-line copy, so it is inline; the
// destructor is compiler-generated): ctor (CScriptDataBase*, CGameScriptInterfaceBase&), static Alloc, and the public virtual overrides
// below, in the FableWin.pdb ??_7Cjoss_test_scriptsScript@NScript@@6B@ slot order (retail vftable 0x012F68D0).
// Includers define FABLE_RETAIL_CLEVELSCRIPTBASE as well: the implicit destructor then calls ~CScriptBase 0x00CBD510 directly, as retail.
#ifdef FABLE_RETAIL_NSCRIPT   // the retail NScript model only; no #error branch (VC7.1 under Wine spins on #error)
#include "rebuild_abi.h"
#include "engine/CLevelScriptBase.h"
class CScriptDataBase;
class CGameScriptInterfaceBase;
namespace NScript {
class Cjoss_test_scriptsData;
class Cjoss_test_scriptsScript : public CLevelScriptBase<Cjoss_test_scriptsData> {
public:
    Cjoss_test_scriptsScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<Cjoss_test_scriptsData>(data, game) { }
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game); // 0x00EF9EE0
    virtual void RegisterMain();                            // slot 1  0x00EF91C0
    virtual void Main();                                    // slot 2  0x00EF9280
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::Cjoss_test_scriptsScript) == 0x48);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CJOSS_TEST_SCRIPTSSCRIPT_H
