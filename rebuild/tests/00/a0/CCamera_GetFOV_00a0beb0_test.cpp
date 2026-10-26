// Test for 0x00A0BEB0 CCamera::GetFOV() const: returns HorizontalFOV (+0x2C).
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
    float GetFOV() const;
};
int main()
{
    CCamera c; memset(&c, 0, sizeof c);
    if (sizeof(CCamera) != 0x38 || (char*)&c.HorizontalFOV - (char*)&c != 0x2C) { printf("00a0beb0_TEST FAIL layout\n"); return 1; }
    c.FOVFlags = 7; c.HorizontalFOV = 1.25f; c.VerticalFOV = 0.75f; c.Zoom = 3.0f;
    const CCamera& r = c;
    if (r.GetFOV() != c.HorizontalFOV) { printf("00a0beb0_TEST FAIL value\n"); return 1; }
    c.HorizontalFOV = (float)5;
    if (r.GetFOV() != (float)5) { printf("00a0beb0_TEST FAIL reread\n"); return 1; }
    printf("00a0beb0_TEST PASS\n");
    return 0;
}
