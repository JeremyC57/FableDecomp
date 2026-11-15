// Behaviour test for CEnvironment::UpdateSunAndMoonPositions() 0x006B2CA0 -- portable model of the row: nothing when the
// non-colour values are forced; else sun and moon start at (cos t, sin t, 0) of the time of day (in turns), are turned by
// SetYRotate(-Scalers[8|11]) then SetZRotate(Scalers[9|12]) (GFCos / GFSin of turns, E31 = -E13 / E12 = -E21 entry form,
// TransformPoint rows E1k*x + E2k*y + E3k*z + E4k), get Scalers[10|13] added to Z and are normalised; the diffuse direction
// is -Sun * (1 - Scalers[5]) - Moon * Scalers[5], normalised, and held at least DiffuseClampAngle below the horizon (Z set to
// -sin(clamp), X / Y rescaled to keep unit length, normalised again). Checked against independent rotation formulas.
#include <cstdio>
#include <cmath>

static int fails = 0;
static void check(bool ok, const char* what) { if (!ok) { std::printf("FAIL: %s\n", what); ++fails; } }

struct V3 { float X, Y, Z; };
static double GFCos(float a) { return cos(a * (double)6.2831855f); }
static double GFSin(float a) { return sin(a * (double)6.2831855f); }
static void Normalise(V3& v) { float l = (float)sqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z); if (l > 0) { v.X /= l; v.Y /= l; v.Z /= l; } }

struct M34 {
    float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43;
    void SetIdentity() { E11 = 1; E12 = 0; E13 = 0; E21 = 0; E22 = 1; E23 = 0; E31 = 0; E32 = 0; E33 = 1; E41 = 0; E42 = 0; E43 = 0; }
    void SetYRotate(float a) { SetIdentity(); E11 = (float)GFCos(a); E33 = (float)GFCos(a); E13 = (float)GFSin(a); E31 = -E13; }
    void SetZRotate(float a) { SetIdentity(); E11 = (float)GFCos(a); E22 = (float)GFCos(a); E21 = (float)GFSin(a); E12 = -E21; }
    void TransformPoint(V3& p) const {
        const float xx = p.X, yy = p.Y, zz = p.Z;
        p.X = E11 * xx + E21 * yy + E31 * zz + E41;
        p.Y = E12 * xx + E22 * yy + E32 * zz + E42;
        p.Z = E13 * xx + E23 * yy + E33 * zz + E43;
    }
};

struct Env {
    bool ForcedNonColourValues; float Time; float Scalers[16]; float DiffuseClampAngle;
    V3 SunVector, MoonVector, DiffuseVector;
    void UpdateSunAndMoonPositions()
    {
        if (ForcedNonColourValues) return;
        M34 rotation;
        V3 sun = { (float)GFCos(Time), (float)GFSin(Time), 0.0f };
        rotation.SetYRotate(-Scalers[8]); rotation.TransformPoint(sun);
        rotation.SetZRotate(Scalers[9]); rotation.TransformPoint(sun);
        sun.Z += Scalers[10]; Normalise(sun);
        V3 moon = { (float)GFCos(Time), (float)GFSin(Time), 0.0f };
        rotation.SetYRotate(-Scalers[11]); rotation.TransformPoint(moon);
        rotation.SetZRotate(Scalers[12]); rotation.TransformPoint(moon);
        moon.Z += Scalers[13]; Normalise(moon);
        SunVector = sun; MoonVector = moon;
        float s5 = Scalers[5];
        V3 d = { -SunVector.X * (1.0f - s5) - MoonVector.X * s5, -SunVector.Y * (1.0f - s5) - MoonVector.Y * s5,
                 -SunVector.Z * (1.0f - s5) - MoonVector.Z * s5 };
        Normalise(d);
        float sc = (float)GFSin(DiffuseClampAngle);
        if (-d.Z < sc) {
            d.Z = -sc;
            float scale = (float)sqrt((1.0f - sc * sc) / (d.X * d.X + d.Y * d.Y));
            d.X *= scale; d.Y *= scale;
            Normalise(d);
        }
        DiffuseVector = d;
    }
};

// independent reference: rotate (cos t, sin t, 0) about Y by -a (turns), then about Z by b, add c to Z, normalise
static void Ref(float t, float a, float b, float c, double out[3])
{
    const double tp = 6.283185307179586;
    double x = cos(t * tp), y = sin(t * tp), z = 0;
    double A = -a * tp; double x1 = cos(A) * x - sin(A) * z, z1 = sin(A) * x + cos(A) * z; x = x1; z = z1;
    double B = b * tp; double x2 = cos(B) * x + sin(B) * y, y2 = -sin(B) * x + cos(B) * y; x = x2; y = y2;
    z += c; double l = sqrt(x * x + y * y + z * z); out[0] = x / l; out[1] = y / l; out[2] = z / l;
}
static bool near(float a, double b) { return fabs(a - b) < 1e-4; }
static bool nearV(const V3& v, const double r[3]) { return near(v.X, r[0]) && near(v.Y, r[1]) && near(v.Z, r[2]); }
static Env make() { Env e = {}; e.DiffuseClampAngle = 0.05f; e.SunVector.X = 7; e.MoonVector.X = 7; e.DiffuseVector.X = 7; return e; }

int main() {
    { Env e = make(); e.ForcedNonColourValues = true; e.Time = 0.3f; e.UpdateSunAndMoonPositions();
      check(e.SunVector.X == 7 && e.MoonVector.X == 7 && e.DiffuseVector.X == 7, "forced: nothing written"); }
    { Env e = make(); e.Time = 0.1f; e.Scalers[8] = 0.05f; e.Scalers[9] = 0.2f; e.Scalers[10] = 0.3f;
      e.Scalers[11] = -0.1f; e.Scalers[12] = 0.4f; e.Scalers[13] = -0.2f; e.UpdateSunAndMoonPositions();
      double rs[3], rm[3]; Ref(0.1f, 0.05f, 0.2f, 0.3f, rs); Ref(0.1f, -0.1f, 0.4f, -0.2f, rm);
      check(nearV(e.SunVector, rs), "sun: Y(-S8) then Z(S9), +S10, normalised");
      check(nearV(e.MoonVector, rm), "moon: Y(-S11) then Z(S12), +S13, normalised"); }
    { // sun high above the horizon, Scalers[5] = 0: diffuse = -sun points down, no clamp
      Env e = make(); e.Time = 0.0f; e.Scalers[10] = 3.0f; e.Scalers[13] = 0.0f; e.UpdateSunAndMoonPositions();
      double l = sqrt(1.0 + 9.0); check(near(e.DiffuseVector.X, -1 / l) && near(e.DiffuseVector.Z, -3 / l), "diffuse = -sun, unclamped"); }
    { // Scalers[5] = 1: diffuse follows -moon (moon high: moon Z = 2)
      Env e = make(); e.Time = 0.25f; e.Scalers[5] = 1.0f; e.Scalers[10] = -5.0f; e.Scalers[13] = 2.0f; e.UpdateSunAndMoonPositions();
      double l = sqrt(1.0 + 4.0); check(near(e.DiffuseVector.Y, -1 / l) && near(e.DiffuseVector.Z, -2 / l), "diffuse = -moon at S5 = 1"); }
    { // sun on the horizon: diffuse horizontal -> clamped to -sin(clamp) with unit length kept
      Env e = make(); e.Time = 0.0f; e.DiffuseClampAngle = 0.1f; e.UpdateSunAndMoonPositions();
      double sc = sin(0.1 * 6.283185307179586), cc = sqrt(1 - sc * sc);
      check(near(e.DiffuseVector.Z, -sc) && near(e.DiffuseVector.X, -cc) && near(e.DiffuseVector.Y, 0), "clamped below the horizon"); }
    { // half blend of opposite sun / moon heights, diffuse just below the clamp limit is left alone
      Env e = make(); e.Time = 0.5f; e.Scalers[5] = 0.5f; e.Scalers[10] = 1.0f; e.Scalers[13] = 1.0f; e.DiffuseClampAngle = 0.01f;
      e.UpdateSunAndMoonPositions(); double l = sqrt(2.0);
      check(near(e.DiffuseVector.X, 1 / l) && near(e.DiffuseVector.Z, -1 / l), "blend of equal sun and moon, unclamped"); }
    if (!fails) std::printf("006b2ca0_TEST PASS\n");
    return fails;
}
