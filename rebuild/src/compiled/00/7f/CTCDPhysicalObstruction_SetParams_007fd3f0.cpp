// CTCDPhysicalObstruction::SetParams(const C3DVector& start, const C3DVector& end, float radius) -- copy the two end points into StartPos (+0xc) and EndPos (+0x18) and the radius into Radius (+0x28)
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FD3F0, length 64 bytes.
// Shape: the member addresses in eax (`lea eax,[ecx+0Ch]`), three dword copies each, `ret 0Ch`.
// Leaf: ?SetParams@CTCDPhysicalObstruction@@QAEXABVC3DVector@@0M@Z
// NF123 (2026-11-20, q5136): was the stand-in CInputManager_007fd3f0::SetVectors(const C3DVector&, const C3DVector&, long) (the chunk label
//   "CInputManager::UpdateChecksum" was propagated). Ego_r.pdb 0x007F86A0 + FableWin.pdb 0x0278A9F0 name
//   ?SetParams@CTCDPhysicalObstruction@@QAEXABVC3DVector@@0M@Z, in Ego_r member order right after SetLifeTimeSeconds (0x007FD3D0) and
//   before Construct (0x007FD430); the stores hit the PDB members StartPos +0x0c / EndPos +0x18 / Radius +0x28 (engine header), and the
//   landed caller 0x008CAF10 passes `radius * 2.0f` (a float) as the third argument. Ported onto engine/CTCDPhysicalObstruction.h.
#define FABLE_RETAIL_CTCDPHYSICALOBSTRUCTION_SETPARAMS
#include "engine/CTCDPhysicalObstruction.h"

void CTCDPhysicalObstruction::SetParams(const C3DVector& start, const C3DVector& end, float radius)
{
    StartPos = start;
    EndPos = end;
    Radius = radius;
}
