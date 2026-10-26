// Behaviour test for 006626e0: src/006626e0.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CThingCreatureBase::GetAnimMovementVector(long animIndex)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CThingCreatureBase::GetAnimMovementVector(long animIndex) -- the animation's movement vector rotated into world space by the creature's appearance position matrix (scale 1)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006626E0, length 178 bytes, `ret 8` (hidden C3DVector result + index).
// Callees: CThing::PeekWorld @ 0x004C7AB0, the main mesh bank's GetAnimation(long) @ 0x00A242C0,
//   CTCGraphicAppearance::GetPositionMatrix(float) @ 0x004BC800 (CMatrix3x4 by hidden pointer; engine_api.tsv).
// Layout: retail CThing PTCGraphicAppearance +0x64; C3DAnimationInfo MovementVector +0x18; CWorld PMainMeshBank +0x3c.
// Shape: the movement vector is copied into a frame local and rotated IN PLACE by an inline matrix helper that first copies
//   the components into float locals (x = X*m00 + Y*m10 + Z*m20, ...; the helper's name is not from the PDB): VC7.1 keeps
//   the copies of X (dead index slot) and Y (a fourth frame word) because the first stores overwrite them, Z is read from
//   the local; the local is then returned through C3DVector's memberwise copy constructor (`fxch st(2)` + three `fstp`).
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
class CMatrix3x4 { public:
    float m[12];
    CMatrix3x4() { }
    void RotateInPlace(C3DVector& v) const
    {
        float x = v.X;
        float y = v.Y;
        float z = v.Z;
        v.X = m[0] * x + m[3] * y + m[6] * z;
        v.Y = m[1] * x + m[4] * y + m[7] * z;
        v.Z = m[2] * x + m[5] * y + m[8] * z;
    }
};
class C3DAnimationInfo { public:
    unsigned char _pad[0x18];
    C3DVector MovementVector;                  // +0x18
};
class CMeshDataBank { public:
    C3DAnimationInfo* GetAnimation(long index);   // 0x00A242C0
};
class CWorld { public:
    unsigned char _pad[0x3c];
    CMeshDataBank* PMainMeshBank;              // +0x3c
};
struct CTCGraphicAppearance {
    CMatrix3x4 GetPositionMatrix(float scale);  // 0x004BC800
};
class CThing { public:
    unsigned char _pad[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance; // +0x64
    const CWorld& PeekWorld() const;           // 0x004C7AB0
};
class CThingCreatureBase : public CThing { public:
    C3DVector GetAnimMovementVector(long animIndex);
};

C3DVector CThingCreatureBase::GetAnimMovementVector(long animIndex)
{
    CMeshDataBank* bank = PeekWorld().PMainMeshBank;
    C3DVector movement = bank->GetAnimation(animIndex)->MovementVector;
    PTCGraphicAppearance->GetPositionMatrix(1.0f).RotateInPlace(movement);
    return movement;
}

// ---- stubs and driver ----
static CWorld g_world; static CMeshDataBank g_bank; static C3DAnimationInfo g_anim; static long g_lastIndex = -1;
static float g_lastScale = 0.0f; static CTCGraphicAppearance g_app;
const CWorld& CThing::PeekWorld() const { return g_world; }
C3DAnimationInfo* CMeshDataBank::GetAnimation(long index) { g_lastIndex = index; return &g_anim; }
CMatrix3x4 CTCGraphicAppearance::GetPositionMatrix(float scale)
{
    g_lastScale = scale;
    CMatrix3x4 r;
    // rows: (0,1,0), (-1,0,0), (0,0,2): a quarter turn about Z with Z doubled; translation must be ignored
    float v[12] = { 0, 1, 0,  -1, 0, 0,  0, 0, 2,  100, 200, 300 };
    for (int i = 0; i < 12; ++i) r.m[i] = v[i];
    return r;
}
int main()
{
    g_world.PMainMeshBank = &g_bank;
    g_anim.MovementVector.X = 3.0f; g_anim.MovementVector.Y = 4.0f; g_anim.MovementVector.Z = 5.0f;
    CThingCreatureBase c; c.PTCGraphicAppearance = &g_app;
    C3DVector r = c.GetAnimMovementVector(17);
    // x = X*m0 + Y*m3 + Z*m6 = -4; y = X*m1 + Y*m4 + Z*m7 = 3; z = X*m2 + Y*m5 + Z*m8 = 10
    CHECK(r.X == -4.0f && r.Y == 3.0f && r.Z == 10.0f);
    CHECK(g_lastIndex == 17 && g_lastScale == 1.0f);
    // the animation's own vector is untouched (copied into a local first)
    CHECK(g_anim.MovementVector.X == 3.0f && g_anim.MovementVector.Y == 4.0f && g_anim.MovementVector.Z == 5.0f);
    if (fails == 0) printf("006626e0_TEST PASS\n");
    return fails;
}
