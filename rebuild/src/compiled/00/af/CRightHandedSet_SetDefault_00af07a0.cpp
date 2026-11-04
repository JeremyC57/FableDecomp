// CRightHandedSet::SetDefault() -- reset the orientation basis: Up = (0, 0, 1), Forward = (0, 1, 0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF07A0, length 96 bytes. Callers: the CParticleEmitter constructor
// (0x00AF0380 @ 0x00AF057D / 0x00AF0585) on OrientationInterpolator.StartOrientation / EndOrientation.
// Leaf: SetDefault
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// PDB CRightHandedSet {C3DVector Up +0x00, C3DVector Forward +0x0c}. The catalog label "CVsyncCallbackTimerInfo" is propagated.
// Shape: a 12-byte frame temporary per assignment (`Up = C3DVector(0,0,1)`): each float field is stored as an immediate and
// re-read as a dword for the bitwise copy (VC does not forward a float store into an integer load). It is NOT a constructor:
// eax ends as the last copied word, and only the void form gives retail's register choice (eax = destination of Up,
// `add ecx,0Ch` for Forward); the constructor form keeps eax = this and swaps the two.
#include "engine/C3DVector.h"
class CRightHandedSet { public:
    C3DVector Up;           // +0x00
    C3DVector Forward;      // +0x0c
    void SetDefault();
};
void CRightHandedSet::SetDefault()
{
    Up = C3DVector(0.0f, 0.0f, 1.0f);
    Forward = C3DVector(0.0f, 1.0f, 0.0f);
}