// NScript::CQ_PrisonWardenGameScript::`scalar deleting destructor'(unsigned int flags) -- the destructor (expanded), then free the object when bit 0 is set
// Leaf: ??_GCQ_PrisonWardenGameScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DE02F0, 40 bytes, thiscall, `ret 4`; calls 004aa840=CScriptThing::~CScriptThing ; 00cbd510=CScriptBase::~CScriptBase ; 00bfe9bc=MSVCR71.DLL::operator_delete.
// Identity (naming wave 35, family sdd): Anniversary wave 2026-11-04 (high): vtable 0x012DCB40 slot 0; Anniversary RTTI NScript::CQ_PrisonWardenGameScript (string-literal match, validated 257/258); PDB name set has the sdd member of NScript::CQ_PrisonWardenGameScript; slot layout from named TLC siblings; function referenced by this vtable only.
//   ??_GCQ_PrisonWardenGameScript@NScript@@UAEPAXI@Z (rebuild/candidates/manual/naming/2026-11-04_anniversary/labels_high.tsv); PDB: Ego_r.pdb / FableWin.pdb ??_ECQ_PrisonWardenGameScript@NScript@@UAEPAXI@Z; FableWin.pdb ??1CQ_PrisonWardenGameScript@NScript@@UAE@XZ.
//   Was:  (catalog stand-in CVtable_012dcb40, Leaf `scalar_deleting_destructor').
// Shape: the deleting destructor is emitted with the vtable, i.e. in the TU that constructs the object (wave 33 ctor): the class model and
//   the PDB constructor ??0CQ_PrisonWardenGameScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z of the landed row 0x00DDDC90 (rebuild/src/compiled/00/dd/NScriptCQPrisonWardenGameScript_CQPrisonWardenGameScript_00dddc90.cpp), its own source with the FABLE_RETAIL_CLEVELSCRIPTBASE opt-in added; its user-declared destructor dropped (retail expands the destructor into the ??_G). The constructor body is not part of this graded body.
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
#define FABLE_RETAIL_CLEVELSCRIPTBASE   // retail's implicit ~CLevelScriptBase<T> (chunk 713 opt-in): the ??_G calls ~CScriptBase 0x00CBD510
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_PrisonWardenGameScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_PrisonWardenGameScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members[0x50 - 0x48];          // POD members up to the script thing
    CScriptThing  SThingBookOpened;                            // +0x50
};
CQ_PrisonWardenGameScript::CQ_PrisonWardenGameScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}