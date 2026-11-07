// NUISystem::CComponent::ChangePositionDelta(const C2DVector& delta, float time, bool immediate) -- move the component by a delta from its current position: a component using relative coordinates first un-converts the delta from back-buffer units and scales it by the UI scale
// Leaf: ?ChangePositionDelta@CComponent@NUISystem@@UAEXABVC2DVector@@M_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052EA30, 181 bytes (192 with padding), `ret 0Ch`; NUISystem::CComponent vtable slot 32
//   (engine/NUISystem_CComponent.h; Ego_r.pdb / FableWin.pdb ?ChangePositionDelta@CComponent@NUISystem@@UAEXABVC2DVector@@M_N@Z).
// Callees: IsUsingRelativePosition (slot 117, +0x1d4) and ChangePosition (slot 31, +0x7c) through the vtable (slot names from
//   engine/NUISystem_CComponent.h / the PDB name sets; the catalog's "MoveTo" / "UsesScreenCoordinates" were descriptive);
//   NUISystem::CUIScreenUnitConverter::UnConvertVector 0x0052E530 (FableWin.pdb ?UnConvertVector@CUIScreenUnitConverter@NUISystem@@
//   SI?AVC2DVector@@V3@@Z: static __fastcall, C2DVector by value, hidden result in ecx; it divides by the back-buffer size and
//   multiplies by the 640x480 screen size, the inverse of the PDB-named ConvertVector twin 0x0052E580 that the landed 0x0052EEC0
//   binds; the manifest's "UnConvertVector float(float)" signature and the parked rows' free "ConvertX" were wrong);
//   NUISystem::CManager::GetInstance 0x0041E5F2 and CManager::GetUIScale 0x0041CF47 (Ego_r.pdb ?GetUIScale@CManager@NUISystem@@
//   QBE?AVC2DVector@@XZ, C2DVector by hidden pointer, called once per axis as retail does).
// Layout: engine/NUISystem_CComponent.h (Position +0x34); C2DVector from engine/C2DVector.h (retail opt-in: user copy ctor).
// Shape: the delta is copied into a local before the virtual test; the un-converted vector is assigned back to the local
//   (hidden-result temporary, then an 8-byte copy), each axis is scaled in place (`fld [d] ; fmul [scale]`), and the new
//   position is the `d + Position` temporary of C2DVector::operator+ passed by reference to ChangePosition. Writing the sum
//   into the local instead (`d += Position`) leaves the same instructions but reorders the un-convert argument loads round
//   `sub esp,8` and the result stores round the UI-scale push (chunk 840: 4 lines -> RELOCATION_MATCH on that change alone).
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_VECTOR
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CUIScreenUnitConverter.h"

namespace NUISystem {

void CComponent::ChangePositionDelta(const C2DVector& delta, float time, bool immediate)
{
    C2DVector d = delta;
    if (IsUsingRelativePosition()) {
        d = CUIScreenUnitConverter::UnConvertVector(d);
        d.X *= CManager::GetInstance()->GetUIScale().X;
        d.Y *= CManager::GetInstance()->GetUIScale().Y;
    }
    ChangePosition(d + Position, time, immediate);
}

}