// CEngineWaterSettings::KeepHistoryInRange(C2DVector*, float) const -- wrap a two-entry 2D scroll history back into [-range, range]: when the current X (or Y) has run past +range or -range, shift both history entries' X (or Y) by the range
// Leaf: ?KeepHistoryInRange@CEngineWaterSettings@@QBEXPAVC2DVector@@M@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE9530, length 143 bytes. No callees.
// Name: Ego_r.pdb 0x00A9F1B0 / FableWin.pdb 0x02E5A2C0 ?KeepHistoryInRange@CEngineWaterSettings@@QBEXPAVC2DVector@@M@Z (public const);
//   it sits in the CEngineWaterSettings TU after KeepHistoryInRange(float*) 0x00BE94E0 and before WorldFrameUpdate 0x00BE95F0,
//   whose four calls 0x00BE9874 / 0x00BE9881 / 0x00BE988E / 0x00BE989B are its only callers (Ego_r order: float* 0x00A9F160,
//   C2DVector* 0x00A9F1B0). The body is byte-identical to 0x00B61AA0 (CEngineSkyRenderer's private copy in the sky TU).
// Shape: `this` is never read (ecx is reused for the history pointer, `ret 8`); each axis is
//   `if (h[0].X > range) {...} else if (-range > h[0].X) {...}` (`fcomp ; test ah,41h ; jne`, the negation is `fld ; fchs`);
//   `x += range` loads the range first (`fld range ; fadd [x]`), `x -= range` loads x first; the Y tests end in their own `ret 8`.
// NF41 (2026-11-16): was a local CEngineSkyRenderer / C2DVector model with the sky renderer's spelling; now defined on the shared
//   engine/CEngineWaterSettings.h declaration with engine/C2DVector.h.
#include "engine/C2DVector.h"
#include "engine/CEngineWaterSettings.h"

void CEngineWaterSettings::KeepHistoryInRange(C2DVector* history, float range) const
{
    if (history[0].X > range) {
        history[0].X -= range;
        history[1].X -= range;
    } else if (-range > history[0].X) {
        history[0].X += range;
        history[1].X += range;
    }
    if (history[0].Y > range) {
        history[0].Y -= range;
        history[1].Y -= range;
    } else if (-range > history[0].Y) {
        history[0].Y += range;
        history[1].Y += range;
    }
}
