// CCountedPointer::Reset(T* data) -- (CCountedPointer<NCameraModes::CScriptModuleBase>) drop the current share (destroying the pointee on the last reference), then adopt data with a fresh count block
// Leaf: ?Reset@?$CCountedPointer@VCScriptModuleBase@NCameraModes@@@@QAEXPAVCScriptModuleBase@NCameraModes@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069C3D0, 103 bytes, thiscall, `ret 4`. Stores DeleteData 0x005ECF50.
// Names (NF115, PRIORITY q4423; was the stand-in CCountedPointer_005ecf50::Reset with a local count-block model): FableWin.pdb
//   ?Reset@?$CCountedPointer@VCScriptModuleBase@NCameraModes@@@@QAEXPAVCScriptModuleBase@NCameraModes@@@Z; the body stores
//   CCountedPointer<NCameraModes::CScriptModuleBase>::DeleteData 0x005ECF50 (landed under that FableWin spelling) as the count block's
//   DeleteFunc; callers CGameScriptInterface::CameraUseCameraPoint 0x0089E190 (both ctor branches) and
//   NTCScriptedControl::CActionTalkBase::SetUpCamera 0x00715A70 already bind this spelling.
// Shape: engine/CCountedPointer.h Reset (the 0x00834E40 recipe): ReleaseData expanded (its trailing Data store is dead), Data = data,
//   then the count block for a non-null data re-reads Data through `this`; a failed new leaves Info 0.
#include "engine/CCountedPointer.h"
namespace NCameraModes { class CScriptModuleBase; }
template void CCountedPointer<NCameraModes::CScriptModuleBase>::Reset(NCameraModes::CScriptModuleBase*);
