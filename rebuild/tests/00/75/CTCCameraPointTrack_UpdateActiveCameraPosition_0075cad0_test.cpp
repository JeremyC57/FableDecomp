// 0075cad0 behaviour test: CTCCameraPointTrack::UpdateActiveCameraPosition (match.cpp body, host-modelled accessors)
#include <stdio.h>
#include <math.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
static bool near_(float a, float b) { return fabs(a - b) < 1e-4f; }

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
    float GetLengthSquared2D() const { return X * X + Y * Y; }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator/(float s) const { float inv = 1.0f / s; return C3DVector(X * inv, Y * inv, Z * inv); }
};
struct CThing { C3DVector Pos; const C3DVector& GetPosition() const { return Pos; } };
static CThing g_hero;
struct CPlayerManager { CThing* GetMainHero() { return &g_hero; } };
static CPlayerManager g_pm;
struct CGame { CPlayerManager* GetPlayerManager() { return &g_pm; } };
static CGame g_game;
struct CTCCameraPointTrack {
    float CameraStringLength, CameraCageRadius;
    C3DVector DragCamPos;
    CGame* GetGame() { return &g_game; }
    void UpdateActiveCameraPosition();
};

// ---- match.cpp body ----
void CTCCameraPointTrack::UpdateActiveCameraPosition()
{
    C3DVector heroPos = GetGame()->GetPlayerManager()->GetMainHero()->GetPosition();
    const C3DVector& cam = DragCamPos;
    C3DVector offset(heroPos.X - cam.X, heroPos.Y - cam.Y, 0.0f);
    float lengthSq = offset.GetLengthSquared2D();
    if (lengthSq > CameraStringLength * CameraStringLength)
        DragCamPos = heroPos - offset * CameraStringLength / (float)sqrt(lengthSq);
    else if (lengthSq < CameraCageRadius * CameraCageRadius)
        DragCamPos = heroPos - offset * CameraCageRadius / (float)sqrt(lengthSq);
}

int main()
{
    CTCCameraPointTrack t; t.CameraStringLength = 5.0f; t.CameraCageRadius = 2.0f;
    // too far: pulled onto the string circle (hero 10,0; cam 0,0 -> cam 5,0, Z = hero Z)
    g_hero.Pos = C3DVector(10, 0, 7); t.DragCamPos = C3DVector(0, 0, 1);
    t.UpdateActiveCameraPosition();
    CHECK(near_(t.DragCamPos.X, 5) && near_(t.DragCamPos.Y, 0) && near_(t.DragCamPos.Z, 7));
    // too close: pushed out to the cage radius (hero 0,0; cam 0,1 -> offset (0,-1) -> cam 0,2)
    g_hero.Pos = C3DVector(0, 0, 3); t.DragCamPos = C3DVector(0, 1, 9);
    t.UpdateActiveCameraPosition();
    CHECK(near_(t.DragCamPos.X, 0) && near_(t.DragCamPos.Y, 2) && near_(t.DragCamPos.Z, 3));
    // inside the band: unchanged
    g_hero.Pos = C3DVector(3, 4, 0); t.DragCamPos = C3DVector(0, 0, 1);   // distance 5 == string length, not greater
    t.UpdateActiveCameraPosition();
    CHECK(t.DragCamPos.X == 0 && t.DragCamPos.Y == 0 && t.DragCamPos.Z == 1);
    g_hero.Pos = C3DVector(3, 0, 0); t.DragCamPos = C3DVector(0, 0, 1);
    t.UpdateActiveCameraPosition();
    CHECK(t.DragCamPos.X == 0 && t.DragCamPos.Y == 0 && t.DragCamPos.Z == 1);
    if (!g_fail) printf("0075cad0_TEST PASS\n");
    return g_fail;
}