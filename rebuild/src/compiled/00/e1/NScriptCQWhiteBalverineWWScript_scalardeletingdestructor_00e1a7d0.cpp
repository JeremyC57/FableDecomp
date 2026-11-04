// NScript::CQ_WhiteBalverineWWScript::`scalar deleting destructor'(unsigned int flags) -- the destructor (expanded), then free the object when bit 0 is set
// Leaf: ??_GCQ_WhiteBalverineWWScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E1A7D0, 40 bytes, thiscall, `ret 4`; calls 004aa840=CScriptThing::~CScriptThing ; 00cbd510=CScriptBase::~CScriptBase ; 00bfe9bc=MSVCR71.DLL::operator_delete.
// Identity (naming wave 35, family sdd): outer script vtable 012e0ce4 slot 0 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00e18630 (slot 2) registers 3 entity scripts whose PDB Alloc outer is CQ_WhiteBalverineWWScript; PDB ??_G/??1CQ_WhiteBalverineWWScript@NScript@@; refs=1; was CVtable_012e0ce4::`scalar deleting destructor'.
//   ??_GCQ_WhiteBalverineWWScript@NScript@@UAEPAXI@Z (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv); PDB: FableWin.pdb ??_GCQ_WhiteBalverineWWScript@NScript@@UAEPAXI@Z; Ego_r.pdb / FableWin.pdb ??1CQ_WhiteBalverineWWScript@NScript@@UAE@XZ.
//   Was:  (catalog stand-in CVtable_012e0ce4, Leaf `scalar_deleting_destructor').
// Shape: the deleting destructor is emitted with the vtable, i.e. in the TU that constructs the object (wave 33 ctor): the class model and
//   the PDB constructor ??0CQ_WhiteBalverineWWScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z of the landed row 0x00E18440 (rebuild/src/compiled/00/e1/NScriptCQWhiteBalverineWWScript_CQWhiteBalverineWWScript_00e18440.cpp), its own source with the FABLE_RETAIL_CLEVELSCRIPTBASE opt-in added; its user-declared destructor dropped (retail expands the destructor into the ??_G). The constructor body is not part of this graded body.
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
class CQ_WhiteBalverineWWScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_WhiteBalverineWWScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members[0x50 - 0x48];          // POD members up to the script thing
    CScriptThing  WhiteBalverine;                            // +0x50
};
CQ_WhiteBalverineWWScript::CQ_WhiteBalverineWWScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}