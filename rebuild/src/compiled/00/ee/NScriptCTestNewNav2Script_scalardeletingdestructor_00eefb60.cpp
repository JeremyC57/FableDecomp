// NScript::CTestNewNav2Script::`scalar deleting destructor'(unsigned int flags) -- the destructor (expanded), then free the object when bit 0 is set
// Leaf: ??_GCTestNewNav2Script@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EEFB60, 30 bytes, thiscall, `ret 4`; calls 00cbd510=CScriptBase::~CScriptBase ; 00bfe9bc=MSVCR71.DLL::operator_delete.
// Naming wave 33 (family ctorB): slot 0 of vtable 0x012F5CBC (no RTTI), the vtable the landed constructor 0x00EEFA20 stores last
//   (??0CTestNewNav2Script@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z, FableWin.pdb); FableWin.pdb ??_ECTestNewNav2Script@NScript@@UAEPAXI@Z; FableWin.pdb ??1CTestNewNav2Script@NScript@@UAE@XZ.
//   Was a free __fastcall stand-in (catalog CVtable_012f5cbc::`scalar deleting destructor').
// Shape: the deleting destructor is emitted with the vtable, i.e. in the constructor's TU: the class model and the PDB constructor of
//   the landed row 0x00EEFA20 (rebuild/src/compiled/00/ee/NScriptCTestNewNav2Script_CTestNewNav2Script_00eefa20.cpp), its own source with the FABLE_RETAIL_CLEVELSCRIPTBASE opt-in added. The constructor body is not part of this graded body.
class CScriptDataBase {
public:
    virtual ~CScriptDataBase();
};
namespace NScript { class CEmptyData; }
class CGameScriptInterfaceBase;
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE   // retail's implicit ~CLevelScriptBase<T> (chunk 713 opt-in): the ??_G calls ~CScriptBase 0x00CBD510
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CTestNewNav2Script : public CLevelScriptBase<NScript::CEmptyData> {
public:
    CTestNewNav2Script(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual void RegisterMain();
    virtual void Main();
};
CTestNewNav2Script::CTestNewNav2Script(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<NScript::CEmptyData>(data, game)
{
}
}