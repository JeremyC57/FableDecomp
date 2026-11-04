// NScript::CQ_PrisonRaceScript::OnPersist(CPersistContext&) -- empty override (the level script persists nothing of its own)
// Leaf: ?OnPersist@CQ_PrisonRaceScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DDAD90, 3 bytes: ret 4.
// Identity (naming wave 35, family lop): outer script vtable 012dc88c slot 4 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00ddae40 (slot 2) registers 6 entity scripts whose PDB Alloc outer is CQ_PrisonRaceScript; PDB ?OnPersist@CQ_PrisonRaceScript@NScript@@; refs=1; was sub_00ddad90.
//   ?OnPersist@CQ_PrisonRaceScript@NScript@@UAEXAAVCPersistContext@@@Z in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Empty member hook. __fastcall this=ecx. (catalog stand-in _gapscan).
// Class: the class model of the landed constructor row 0x00DDAD30 (rebuild/src/compiled/00/dd/NScriptCQPrisonRaceScript_CQPrisonRaceScript_00ddad30.cpp); the PDB OnPersist declaration added where missing; only this member is graded.
class CScriptDataBase {
public:
    virtual ~CScriptDataBase();
};
namespace NScript { class CQ_SunnyvaleMasterData; }
class CGameScriptInterfaceBase;
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_PrisonRaceScript : public CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> {
public:
    CQ_PrisonRaceScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual void RegisterMain();
    virtual void Main();
public:
    virtual void OnPersist(CPersistContext& context);   // PDB ?OnPersist@CQ_PrisonRaceScript@NScript@@UAEXAAVCPersistContext@@@Z (slot 4)
};
CQ_PrisonRaceScript::CQ_PrisonRaceScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<NScript::CQ_SunnyvaleMasterData>(data, game)
{
}
}

void NScript::CQ_PrisonRaceScript::OnPersist(CPersistContext&)
{
}
