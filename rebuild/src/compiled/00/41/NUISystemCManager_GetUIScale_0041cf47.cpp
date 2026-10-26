// NUISystem::CManager::GetUIScale() -- the UI scale factors: (1, 1) unless the engine is up and the screen is smaller than 1024 x 768 in either direction, in which case (width / 1024, height / 768)
// Leaf: GetUIScale
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041CF47, length 107 bytes. Frame-pointer size TU: `#pragma optimize("y", off)` + `("s", on)`.
// Callees: the screen-width / screen-height getters @ 0x0041CC14 / 0x0041CC2B (they read the override floats 0x013B876C/0x013B8770 when
//   the flag 0x013B8768 is set, else the display size 0x01375CD4/0x01375CD8; manifest label CTCLook::GetHeadAngularVelocityMaxXY is a
//   byte-twin). Globals: the engine singleton 0x013B86A0; floats 0x01230010 = 1024, 0x0123000C = 768, 0x01230008 = 1/1024, 0x01230004 = 1/768.
// Shape: the result is a C2DVector LOCAL (X at [ebp-8], Y at [ebp-4]) returned through its user copy constructor into the hidden result
//   (`fld X ; fstp [eax] ; fstp [eax+4]` with Y still on the x87 stack); `a < b || c < d` gives the `test ah,5 ; jnp / jp` pair.
#pragma optimize("y", off)
#pragma optimize("s", on)
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { }
};
class CEngine;
extern CEngine* g_pEngine;                         // 0x013B86A0
float GetScreenWidth();                            // 0x0041CC14
float GetScreenHeight();                           // 0x0041CC2B
namespace NUISystem {
class CManager { public:
    C2DVector GetUIScale();
};
C2DVector CManager::GetUIScale()
{
    C2DVector scale(1.0f, 1.0f);
    if (g_pEngine) {
        if (GetScreenWidth() < 1024.0f || GetScreenHeight() < 768.0f) {
            scale.X = GetScreenWidth() * 0.0009765625f;
            scale.Y = GetScreenHeight() * 0.0013020834f;
        }
    }
    return scale;
}
}