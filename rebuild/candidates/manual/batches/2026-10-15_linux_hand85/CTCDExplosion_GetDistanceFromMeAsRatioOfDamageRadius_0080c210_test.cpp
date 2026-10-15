// Test for 0080c210 CTCDExplosion::GetDistanceFromMeAsRatioOfDamageRadius: segment distance / DamageRadius.
#include <cstdio>
#include <math.h>
struct C3DVector { float X, Y, Z; };
struct CThing { C3DVector Pos; const C3DVector& GetPosition() const { return Pos; } };
static float Geometry_DistancePointToLineSegment(const C3DVector& a, const C3DVector& b, const C3DVector& p)
{
    float dx = b.X - a.X, dy = b.Y - a.Y, dz = b.Z - a.Z;
    float len2 = dx * dx + dy * dy + dz * dz;
    float t = len2 > 0.0f ? ((p.X - a.X) * dx + (p.Y - a.Y) * dy + (p.Z - a.Z) * dz) / len2 : 0.0f;
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    float ex = a.X + t * dx - p.X, ey = a.Y + t * dy - p.Y, ez = a.Z + t * dz - p.Z;
    return (float)sqrt(ex * ex + ey * ey + ez * ez);
}
struct CTCDExplosion {
    float DamageRadius; C3DVector StartPos; C3DVector EndPos;
    float GetDistanceFromMeAsRatioOfDamageRadius(const CThing* thing) const
    {
        return Geometry_DistancePointToLineSegment(StartPos, EndPos, thing->GetPosition()) / DamageRadius;
    }
};
int main()
{
    CTCDExplosion e = { 4.0f, { 0.0f, 0.0f, 0.0f }, { 10.0f, 0.0f, 0.0f } };
    CThing above = { { 5.0f, 2.0f, 0.0f } }, beyond = { { 13.0f, 4.0f, 0.0f } };
    float r1 = e.GetDistanceFromMeAsRatioOfDamageRadius(&above);    // 2 / 4
    float r2 = e.GetDistanceFromMeAsRatioOfDamageRadius(&beyond);   // 5 / 4
    if (fabs(r1 - 0.5f) > 1e-5f || fabs(r2 - 1.25f) > 1e-5f) { std::printf("0080c210_TEST FAIL\n"); return 1; }
    std::printf("0080c210_TEST PASS\n");
    return 0;
}
