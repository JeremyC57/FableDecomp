#ifndef FABLE_ENGINE_CQ_GUILDTRAININGWOODSMELEESCRIPT_H
#define FABLE_ENGINE_CQ_GUILDTRAININGWOODSMELEESCRIPT_H
// NScript::CQ_GuildTrainingWoodsMeleeScript (NF436, new header; no includer before NF436). Base CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> (FableWin.pdb LF_BCLASS) with the
// class's own members.
// Layout: Ego_r.pdb CQ_GuildTrainingWoodsMeleeScript (struct_layouts_egor.tsv, 96 bytes) at retail offsets = PDB - 0x14 (retail CScriptBase is 0x40; no STLport
// container in the class); retail size 0x4C (the landed Alloc 0x00D67740 news 0x4C). Member names / types: Ego_r.pdb and FableWin.pdb.
// Methods (FableWin.pdb LF_CLASS field list, all public; Ego_r.pdb has no out-of-line ctor copy, so it is inline; the destructor is
// compiler-generated): ctor (CScriptDataBase*, CGameScriptInterfaceBase&), static Alloc, and the virtual overrides below
// (FableWin.pdb ??_7CQ_GuildTrainingWoodsMeleeScript@NScript@@6B@ slot order; retail vftable 0x012D1D1C). The class's other methods are not declared yet.
// Includers define FABLE_RETAIL_CLEVELSCRIPTBASE as well: the implicit destructor then calls ~CScriptBase 0x00CBD510 directly, as retail.
#ifdef FABLE_RETAIL_NSCRIPT   // the retail NScript model only; no #error branch (VC7.1 under Wine spins on #error)
#include "rebuild_abi.h"
#include "engine/CLevelScriptBase.h"
class CScriptDataBase;
class CGameScriptInterfaceBase;
class CPersistContext;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_GuildTrainingWoodsMeleeScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_GuildTrainingWoodsMeleeScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game); // 0x00D67740
    virtual void RegisterMain();                            // slot 1  0x00D66560
    virtual void Main();                                    // slot 2  0x00D66620
    bool MissionSucceeded;                        // +0x48 (Ego_r.pdb +0x5c; FableWin.pdb bool)
    bool MissionFailed;                           // +0x49 (Ego_r.pdb +0x5d; FableWin.pdb bool)
    bool MissionOver;                             // +0x4a (Ego_r.pdb +0x5e; FableWin.pdb bool)
    bool ScorpionsAlive;                          // +0x4b (Ego_r.pdb +0x5f; FableWin.pdb bool)
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_GuildTrainingWoodsMeleeScript) == 0x4C);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_GUILDTRAININGWOODSMELEESCRIPT_H
