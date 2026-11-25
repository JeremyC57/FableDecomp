#ifndef FABLE_ENGINE_CQ_GUILDTRAININGDEPARTURESCRIPT_H
#define FABLE_ENGINE_CQ_GUILDTRAININGDEPARTURESCRIPT_H
// NScript::CQ_GuildTrainingDepartureScript (NF436, new header; no includer before NF436). Base CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> (FableWin.pdb LF_BCLASS) with the
// class's own members.
// Layout: Ego_r.pdb CQ_GuildTrainingDepartureScript (struct_layouts_egor.tsv, 108 bytes) at retail offsets = PDB - 0x14 (retail CScriptBase is 0x40; no STLport
// container in the class); retail size 0x58 (the landed Alloc 0x00D513C0 news 0x58). Member names / types: Ego_r.pdb and FableWin.pdb.
// Methods (FableWin.pdb LF_CLASS field list, all public; Ego_r.pdb has no out-of-line ctor copy, so it is inline; the destructor is
// compiler-generated): ctor (CScriptDataBase*, CGameScriptInterfaceBase&), static Alloc, and the virtual overrides below
// (FableWin.pdb ??_7CQ_GuildTrainingDepartureScript@NScript@@6B@ slot order; retail vftable 0x012CF670). The class's other methods are not declared yet.
// Includers define FABLE_RETAIL_CLEVELSCRIPTBASE as well: the implicit destructor then calls ~CScriptBase 0x00CBD510 directly, as retail.
#ifdef FABLE_RETAIL_NSCRIPT   // the retail NScript model only; no #error branch (VC7.1 under Wine spins on #error)
#include "rebuild_abi.h"
#include "engine/CLevelScriptBase.h"
class CScriptDataBase;
class CGameScriptInterfaceBase;
class CPersistContext;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_GuildTrainingDepartureScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_GuildTrainingDepartureScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game); // 0x00D513C0
    virtual void RegisterMain();                            // slot 1  0x00D50760
    virtual void Main();                                    // slot 2  0x00D50800
    virtual void Init();                                    // slot 3  0x00D506B0
    bool Finished;                                // +0x48 (Ego_r.pdb +0x5c; FableWin.pdb bool)
    long MeleeGrade;                              // +0x4c (Ego_r.pdb +0x60; FableWin.pdb long)
    long SkillGrade;                              // +0x50 (Ego_r.pdb +0x64; FableWin.pdb long)
    long WillGrade;                               // +0x54 (Ego_r.pdb +0x68; FableWin.pdb long)
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_GuildTrainingDepartureScript) == 0x58);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_GUILDTRAININGDEPARTURESCRIPT_H
