// NScript::CQ_GuildTrainingScript::`scalar deleting destructor'(unsigned int flags) -- the destructor (expanded), then free the object when bit 0 is set
// Leaf: ??_GCQ_GuildTrainingScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D50650, 40 bytes, thiscall, `ret 4`; calls 00cd4470=CTimer::~CTimer ; 00cbd510=CScriptBase::~CScriptBase ; 00bfe9bc=MSVCR71.DLL::operator_delete.
// Identity (naming wave 35, family sdd): outer script vtable 012cd458 slot 0 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00d3bc60 (slot 2) registers 14 entity scripts whose PDB Alloc outer is CQ_GuildTrainingScript; PDB ??_G/??1CQ_GuildTrainingScript@NScript@@; refs=1; was CVtable_012cd458::`scalar deleting destructor'.
//   ??_GCQ_GuildTrainingScript@NScript@@UAEPAXI@Z (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv); PDB: FableWin.pdb ??_ECQ_GuildTrainingScript@NScript@@UAEPAXI@Z; FableWin.pdb ??1CQ_GuildTrainingScript@NScript@@UAE@XZ.
//   Was:  (catalog stand-in CVtable_012cd458, Leaf `scalar_deleting_destructor').
// Shape: the deleting destructor is emitted with the vtable, i.e. in the TU that constructs the object (wave 33 ctor): the class model and
//   the PDB constructor ??0CQ_GuildTrainingScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z of the landed row 0x00D3B390 (rebuild/src/compiled/00/d3/NScriptCQGuildTrainingScript_CQGuildTrainingScript_00d3b390.cpp), its own source with the FABLE_RETAIL_CLEVELSCRIPTBASE opt-in added; its user-declared destructor dropped (retail expands the destructor into the ??_G). The constructor body is not part of this graded body.
class CScriptDataBase;
class CGameScriptInterfaceBase;

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)

// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }

// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_CLEVELSCRIPTBASE   // retail's implicit ~CLevelScriptBase<T> (chunk 713 opt-in): the ??_G calls ~CScriptBase 0x00CBD510
#include "engine/CLevelScriptBase.h"


namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_GuildTrainingScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_GuildTrainingScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members[0x60 - 0x48];           // POD members up to the timer
    CTimer        WillHelpTimer;                            // +0x60
};

CQ_GuildTrainingScript::CQ_GuildTrainingScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}