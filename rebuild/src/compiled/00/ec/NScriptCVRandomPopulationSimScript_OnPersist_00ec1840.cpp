// NScript::CV_RandomPopulationSimScript::OnPersist(CPersistContext&) -- empty override (the level script persists nothing of its own)
// Leaf: ?OnPersist@CV_RandomPopulationSimScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EC1840, 3 bytes: ret 4.
// Identity (naming wave 35, family lop): outer script vtable 012f12d8 slot 4 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00ec1910 (slot 2) registers 3 entity scripts whose PDB Alloc outer is CV_RandomPopulationSimScript; PDB ?OnPersist@CV_RandomPopulationSimScript@NScript@@; refs=1; was sub_00ec1840.
//   ?OnPersist@CV_RandomPopulationSimScript@NScript@@UAEXAAVCPersistContext@@@Z in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Empty member hook. __fastcall this=ecx. (catalog stand-in _gapscan).
// Class: the class model of the landed constructor row 0x00EC17E0 (rebuild/src/compiled/00/ec/NScriptCVRandomPopulationSimScript_CVRandomPopulationSimScript_00ec17e0.cpp); the PDB OnPersist declaration added where missing; only this member is graded.
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
class CV_RandomPopulationSimScript : public CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> {
public:
    CV_RandomPopulationSimScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual void RegisterMain();
    virtual void Main();
public:
    virtual void OnPersist(CPersistContext& context);   // PDB ?OnPersist@CV_RandomPopulationSimScript@NScript@@UAEXAAVCPersistContext@@@Z (slot 4)
};
CV_RandomPopulationSimScript::CV_RandomPopulationSimScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<NScript::CQ_SunnyvaleMasterData>(data, game)
{
}
}

void NScript::CV_RandomPopulationSimScript::OnPersist(CPersistContext&)
{
}
