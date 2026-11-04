// NScript::CQ_PrisonWardenGameScript::OnPersist(CPersistContext&) -- empty override (the level script persists nothing of its own)
// Leaf: ?OnPersist@CQ_PrisonWardenGameScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DDDCD0, 3 bytes: ret 4.
// Identity (naming wave 35, family lop): outer script vtable 012dcb40 slot 4 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00ddde20 (slot 2) registers 4 entity scripts whose PDB Alloc outer is CQ_PrisonWardenGameScript; PDB ?OnPersist@CQ_PrisonWardenGameScript@NScript@@; refs=1; was sub_00dddcd0.
//   ?OnPersist@CQ_PrisonWardenGameScript@NScript@@UAEXAAVCPersistContext@@@Z in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Empty member hook. __fastcall this=ecx. (catalog stand-in _gapscan).
// Class: the class model of the landed constructor row 0x00DDDC90 (rebuild/src/compiled/00/dd/NScriptCQPrisonWardenGameScript_CQPrisonWardenGameScript_00dddc90.cpp); the PDB OnPersist declaration added where missing; only this member is graded.
class CScriptDataBase;
class CGameScriptInterfaceBase;
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() { }
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_PrisonWardenGameScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_PrisonWardenGameScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual ~CQ_PrisonWardenGameScript();
    unsigned char _members[0x50 - 0x48];          // POD members up to the script thing
    CScriptThing  SThingBookOpened;                            // +0x50
public:
    virtual void OnPersist(CPersistContext& context);   // PDB ?OnPersist@CQ_PrisonWardenGameScript@NScript@@UAEXAAVCPersistContext@@@Z (slot 4)
};
CQ_PrisonWardenGameScript::CQ_PrisonWardenGameScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}

void NScript::CQ_PrisonWardenGameScript::OnPersist(CPersistContext&)
{
}
