// Behaviour test for CEngineInternalPrimitiveSpriteTrail::GetWorldPosition (0x00BCC1C0): centre of the box around every
// trail point grown by the width; false for an empty trail.
#include <stdio.h>
struct V { float X, Y, Z; };
static bool GetWorldPosition(const V* pts, int n, float w, V& out)
{
    if (n == 0) return false;
    V mn = { 3.402823466e+38f, 3.402823466e+38f, 3.402823466e+38f }, mx = { -3.402823466e+38f, -3.402823466e+38f, -3.402823466e+38f };
    for (int i = 0; i < n; ++i) {
        V p = pts[i];
        mn.X = mn.X < p.X - w ? mn.X : p.X - w; mn.Y = mn.Y < p.Y - w ? mn.Y : p.Y - w; mn.Z = mn.Z < p.Z - w ? mn.Z : p.Z - w;
        mx.X = mx.X > p.X + w ? mx.X : p.X + w; mx.Y = mx.Y > p.Y + w ? mx.Y : p.Y + w; mx.Z = mx.Z > p.Z + w ? mx.Z : p.Z + w;
    }
    out.X = (mx.X + mn.X) * 0.5f; out.Y = (mx.Y + mn.Y) * 0.5f; out.Z = (mx.Z + mn.Z) * 0.5f;
    return true;
}
int main()
{
    V pts[2] = { { 0, 0, 0 }, { 4, 2, -6 } }; V c = { 9, 9, 9 };
    if (GetWorldPosition(pts, 0, 1, c) || c.X != 9) { printf("00bcc1c0_TEST FAIL\n"); return 1; }
    if (!GetWorldPosition(pts, 2, 1, c) || c.X != 2 || c.Y != 1 || c.Z != -3) { printf("00bcc1c0_TEST FAIL\n"); return 1; }
    printf("00bcc1c0_TEST PASS\n"); return 0;
}