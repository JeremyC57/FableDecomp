// Test for 0x00A0BE80 CCamera::GetFOVFlags() const: returns FOVFlags (+0x28).
#include <stdio.h>
#include <string.h>
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Up, Forward; };

class CCamera {                                     // Ego_r.pdb CCamera, 0x38 bytes
public:
    C3DVector       WorldPosition;                  // +0x00
    CRightHandedSet RHSet;                          // +0x0C
    bool            HeightLocked;                   // +0x24
    bool            ViewVecZLocked;                 // +0x25
    unsigned long   FOVFlags;                       // +0x28
    float           HorizontalFOV;                  // +0x2C
    float           VerticalFOV;                    // +0x30
    float           Zoom;                           // +0x34
    unsigned long GetFOVFlags() const;
};
int main()
{
    CCamera c; memset(&c, 0, sizeof c);
    if (sizeof(CCamera) != 0x38 || (char*)&c.FOVFlags - (char*)&c != 0x28) { printf("00a0be80_TEST FAIL layout\n"); return 1; }
    c.FOVFlags = 7; c.HorizontalFOV = 1.25f; c.VerticalFOV = 0.75f; c.Zoom = 3.0f;
    const CCamera& r = c;
    if (r.GetFOVFlags() != c.FOVFlags) { printf("00a0be80_TEST FAIL value\n"); return 1; }
    c.FOVFlags = (unsigned long)5;
    if (r.GetFOVFlags() != (unsigned long)5) { printf("00a0be80_TEST FAIL reread\n"); return 1; }
    printf("00a0be80_TEST PASS\n");
    return 0;
}
