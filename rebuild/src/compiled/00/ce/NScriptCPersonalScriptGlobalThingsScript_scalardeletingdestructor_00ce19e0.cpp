// NScript::CPersonalScript_GlobalThingsScript::`scalar deleting destructor'(unsigned int flags) -- the destructor (expanded), then free the object when bit 0 is set
// Leaf: ??_GCPersonalScript_GlobalThingsScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CE19E0, 30 bytes, thiscall, `ret 4`; calls 00cbd510=CScriptBase::~CScriptBase ; 00bfe9bc=MSVCR71.DLL::operator_delete.
// Naming wave 33 (family ctorB): slot 0 of vtable 0x012C30CC (no RTTI), the vtable the landed constructor 0x00CDE350 stores last
//   (??0CPersonalScript_GlobalThingsScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z, FableWin.pdb); FableWin.pdb ??_ECPersonalScript_GlobalThingsScript@NScript@@UAEPAXI@Z; FableWin.pdb ??1CPersonalScript_GlobalThingsScript@NScript@@UAE@XZ.
//   Was a free __fastcall stand-in (catalog CVtable_012c30cc::`scalar deleting destructor').
// Shape: the deleting destructor is emitted with the vtable, i.e. in the constructor's TU: the class model and the PDB constructor of
//   the landed row 0x00CDE350 (rebuild/src/compiled/00/cd/NScriptCPersonalScriptGlobalThingsScript_CPersonalScriptGlobalThingsScript_00cde350.cpp), its own source with the FABLE_RETAIL_CLEVELSCRIPTBASE opt-in added. The constructor body is not part of this graded body.
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
#define FABLE_RETAIL_CLEVELSCRIPTBASE   // retail's implicit ~CLevelScriptBase<T> (chunk 713 opt-in): the ??_G calls ~CScriptBase 0x00CBD510
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CPersonalScript_GlobalThingsScript : public CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> {
public:
    CPersonalScript_GlobalThingsScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual void RegisterMain();
    virtual void Main();
};
CPersonalScript_GlobalThingsScript::CPersonalScript_GlobalThingsScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<NScript::CQ_SunnyvaleMasterData>(data, game)
{
}
}