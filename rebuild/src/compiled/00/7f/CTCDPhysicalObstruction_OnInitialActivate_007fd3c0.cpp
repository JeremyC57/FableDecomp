// CTCDPhysicalObstruction::OnInitialActivate() -- start the obstruction with no life-time limit (LifeTimer = -1)
// Leaf: ?OnInitialActivate@CTCDPhysicalObstruction@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FD3C0, 8 bytes: `mov dword ptr [ecx+24h],0FFFFFFFFh ; ret`.
// Identity (two sources): (1) slot 6 of the CTCDPhysicalObstruction vtable 0x01241C0C (RTTI .?AVCTCDPhysicalObstruction@@), the CTCBase
//   OnInitialActivate slot (engine/CTCBase.h; the catalog's "vfunc_6"); (2) Ego_r.pdb ?OnInitialActivate@CTCDPhysicalObstruction@@UAEXXZ,
//   first of the class's bodies in donor order (OnInitialActivate, SetLifeTimeSeconds, SetParams, Construct, Create, ...) exactly as
//   retail lays them out from 0x007FD3C0 (SetLifeTimeSeconds 0x007FD3D0 stores its frame count to the same +0x24), and +0x24 is the
//   Ego_r member LifeTimer.
// Header: engine/CTCDPhysicalObstruction.h (new), engine/CTCDriverBase.h, engine/CTCBase.h (FABLE_RETAIL_CTCBASE).
#include "engine/CTCDPhysicalObstruction.h"
void CTCDPhysicalObstruction::OnInitialActivate()
{
    LifeTimer = -1;
}