#ifndef FABLE_ENGINE_CTESTTHEINVENTORYSCRIPT_H
#define FABLE_ENGINE_CTESTTHEINVENTORYSCRIPT_H
// NScript::CTestTheInventoryScript (NF436, new header; no includer before NF436). Base CLevelScriptBase<NScript::CEmptyData> (FableWin.pdb LF_BCLASS) with the
// class's own members.
// Layout: Ego_r.pdb CTestTheInventoryScript (struct_layouts_egor.tsv, 96 bytes) at retail offsets = PDB - 0x14 (retail CScriptBase is 0x40; no STLport
// container in the class); retail size 0x4C (the landed Alloc 0x00EEEE40 news 0x4C). Member names / types: Ego_r.pdb and FableWin.pdb.
// Methods (FableWin.pdb LF_CLASS field list, all public; Ego_r.pdb has no out-of-line ctor copy, so it is inline; the destructor is
// compiler-generated): ctor (CScriptDataBase*, CGameScriptInterfaceBase&), static Alloc, and the virtual overrides below
// (FableWin.pdb ??_7CTestTheInventoryScript@NScript@@6B@ slot order; retail vftable 0x012F5998). The class's other methods are not declared yet.
// Includers define FABLE_RETAIL_CLEVELSCRIPTBASE as well: the implicit destructor then calls ~CScriptBase 0x00CBD510 directly, as retail.
#ifdef FABLE_RETAIL_NSCRIPT   // the retail NScript model only; no #error branch (VC7.1 under Wine spins on #error)
#include "rebuild_abi.h"
#include "engine/CLevelScriptBase.h"
class CScriptDataBase;
class CGameScriptInterfaceBase;
class CPersistContext;
namespace NScript {
class CEmptyData;
class CTestTheInventoryScript : public CLevelScriptBase<CEmptyData> {
public:
    CTestTheInventoryScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CEmptyData>(data, game) { }
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game); // 0x00EEEE40
    virtual void RegisterMain();                            // slot 1  0x00EEE9D0
    virtual void Main();                                    // slot 2  0x00EEEA90
    bool IgnoreNextLoad;                          // +0x48 (Ego_r.pdb +0x5c; FableWin.pdb bool)
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CTestTheInventoryScript) == 0x4C);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CTESTTHEINVENTORYSCRIPT_H
