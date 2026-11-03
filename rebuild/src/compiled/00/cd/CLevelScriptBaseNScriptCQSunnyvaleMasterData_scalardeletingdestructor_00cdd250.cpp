// CLevelScriptBase<NScript::CQ_SunnyvaleMasterData>::`scalar deleting destructor'(unsigned int flags) -- the destructor (expanded), then free the object when bit 0 is set
// Leaf: ??_G?$CLevelScriptBase@VCQ_SunnyvaleMasterData@NScript@@@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDD250, 30 bytes, thiscall, `ret 4`; calls 00cbd510=CScriptBase::~CScriptBase ; 00bfe9bc=MSVCR71.DLL::operator_delete.
// Naming wave 33 (family ctorB): slot 0 of vtable 0x012C2EEC (no RTTI), the vtable the landed constructor 0x00CDD160 stores last
//   (??0?$CLevelScriptBase@VCQ_SunnyvaleMasterData@NScript@@@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z, FableWin.pdb); FableWin.pdb ??_G?$CLevelScriptBase@VCQ_SunnyvaleMasterData@NScript@@@@UAEPAXI@Z; Ego_r.pdb ??1?$CLevelScriptBase@VCQ_SunnyvaleMasterData@NScript@@@@UAE@XZ; FableWin.pdb ??1?$CLevelScriptBase@VCQ_SunnyvaleMasterData@NScript@@@@UAE@XZ.
//   Was a free __fastcall stand-in (catalog CVtable_012c2eec::`scalar deleting destructor').
// Shape: the deleting destructor is emitted with the vtable, i.e. in the constructor's TU: the class model and the PDB constructor of
//   the landed row 0x00CDD160 (rebuild/src/compiled/00/cd/CLevelScriptBase_CLevelScriptBase_00cdd160.cpp), its own source with the FABLE_RETAIL_CLEVELSCRIPTBASE opt-in added. The constructor body is not part of this graded body.
class CScriptDataBase;
class CGameScriptInterfaceBase;
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44;
//   its ctor is inline (wave 7): this row is the explicit instantiation of it, the five retail copies 0x00CDD160..0x00CDD220).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE   // retail's implicit ~CLevelScriptBase<T> (chunk 713 opt-in): the ??_G calls ~CScriptBase 0x00CBD510
#include "engine/CLevelScriptBase.h"
namespace NScript { class CQ_SunnyvaleMasterData; }
template CLevelScriptBase<NScript::CQ_SunnyvaleMasterData >::CLevelScriptBase(CScriptDataBase*, CGameScriptInterfaceBase&);