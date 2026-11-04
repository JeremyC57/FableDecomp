// CCamera::SetFOV(float horizontalFOV, float verticalFOV) -- store both fields of view; the flags say horizontal only (1) or horizontal and vertical (3, when the vertical one is positive)
// Leaf: ?SetFOV@CCamera@@QAEXMM@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0BED0, 48 bytes, `ret 8`. Name: Ego_r/FableWin PDB ?SetFOV@CCamera@@QAEXMM@Z
//   (the catalog row agrees). Global: 0.0f @ 0x0122DEDC.
// Layout: Ego_r.pdb CCamera: FOVFlags +0x28, HorizontalFOV +0x2C, VerticalFOV +0x30.
// Shape: the float members copied as dwords; flags set to 1, then raised to 3 under `if (v > 0)` (`test ah,41h ; jne`).
#pragma pack(push, 1)
#include "engine/C3DVector.h"
#pragma pack(pop)
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };

class CCamera {
public:
    C3DVector       WorldPosition;      // +0x00
    CRightHandedSet RHSet;              // +0x0c
    bool            HeightLocked;       // +0x24
    bool            ViewVecZLocked;     // +0x25
    unsigned long   FOVFlags;           // +0x28
    float           HorizontalFOV;      // +0x2c
    float           VerticalFOV;        // +0x30
    float           Zoom;               // +0x34
    void SetFOV(float horizontalFOV, float verticalFOV);
};

void CCamera::SetFOV(float horizontalFOV, float verticalFOV)
{
    HorizontalFOV = horizontalFOV;
    VerticalFOV = verticalFOV;
    FOVFlags = 1;
    if (verticalFOV > 0.0f)
        FOVFlags = 3;
}