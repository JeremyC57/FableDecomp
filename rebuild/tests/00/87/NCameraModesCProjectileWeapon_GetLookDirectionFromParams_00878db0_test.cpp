// Behaviour test for 0x00878DB0 NCameraModes::CProjectileWeapon::GetLookDirectionFromParams(float phi, float heightParam) const
// The model mirrors the source: the camera position and the horizontal look direction come from the two param helpers; the
// target offset is the observed thing's position raised by 1.6 minus the camera position; each pitch is
// atan2(z, |xy|) / (2*pi) wrapped into [0, 1); the look direction is rotated about (-look.Y, look.X, 0) by
// targetPitch - lookPitch and returned.
#include <cstdio>
#include <cmath>

struct Vec { float X, Y, Z; };
static Vec g_camera, g_thing, g_lookXY;
static Vec g_rotAxis; static float g_rotAngle; static int g_rotations = 0;
static float g_lastPhi, g_lastHeight;

static Vec GetPosFromParams(float phi, float height) { g_lastPhi = phi; g_lastHeight = height; return g_camera; }
static Vec GetLookDirectionXYFromParams(float, float) { return g_lookXY; }
static void Rotate(Vec& v, const Vec& axis, float angle) { ++g_rotations; g_rotAxis = axis; g_rotAngle = angle; v.Z += angle; }

static float PitchFraction(const Vec& v)
{
    float xy = (float)std::sqrt(v.X * v.X + v.Y * v.Y);
    float fraction = (float)std::atan2(v.Z, xy) * 0.15915493667125702f;
    if (fraction < 0.0f || fraction >= 1.0f) {
        fraction = (float)std::fmod((double)fraction, 1.0);
        if (fraction < 0.0f)
            fraction += 1.0f;
    }
    return fraction;
}

static Vec GetLookDirectionFromParams(float phi, float heightParam)
{
    Vec camera = GetPosFromParams(phi, heightParam);
    Vec toTarget = { g_thing.X - camera.X, g_thing.Y - camera.Y, (g_thing.Z + 1.6f) - camera.Z };
    Vec look = GetLookDirectionXYFromParams(phi, heightParam);
    Vec side = { look.Y, -look.X, 0.0f };
    Vec axis = { -side.X, -side.Y, side.Z };
    float targetPitch = PitchFraction(toTarget);
    float lookPitch = PitchFraction(look);
    Rotate(look, axis, targetPitch - lookPitch);
    return look;
}

static bool Near(float a, float b) { return std::fabs(a - b) < 1e-5f; }

int main()
{
    int fail = 0;
    Vec cam = { 0.0f, 0.0f, 1.6f }; g_camera = cam;
    Vec thing = { 10.0f, 0.0f, 10.0f }; g_thing = thing;   // aim point 10 up, 10 along: 45 degrees = 0.125 turn
    Vec lookXY = { 1.0f, 0.0f, 0.0f }; g_lookXY = lookXY;  // horizontal: pitch 0
    Vec r = GetLookDirectionFromParams(0.3f, 0.7f);
    if (g_rotations != 1 || !Near(g_rotAngle, 0.125f)) fail |= 1;
    if (g_rotAxis.X != 0.0f || g_rotAxis.Y != 1.0f || g_rotAxis.Z != 0.0f) fail |= 2;   // (-look.Y, look.X, 0)
    if (!Near(r.Z, 0.125f) || r.X != 1.0f) fail |= 4;
    if (g_lastPhi != 0.3f || g_lastHeight != 0.7f) fail |= 8;
    Vec below = { 10.0f, 0.0f, -10.0f }; g_thing = below;   // aim point 10 below the camera: -0.125 wraps to 0.875
    Vec cam2 = { 0.0f, 0.0f, 1.6f }; g_camera = cam2;
    r = GetLookDirectionFromParams(0.0f, 0.0f);
    if (!Near(g_rotAngle, 0.875f)) fail |= 16;
    Vec down = { 0.0f, 1.0f, -1.0f }; g_lookXY = down;     // look pitched down: also 0.875, difference 0
    g_thing = below;
    r = GetLookDirectionFromParams(0.0f, 0.0f);
    if (!Near(g_rotAngle, 0.0f) || g_rotAxis.X != -1.0f || g_rotAxis.Y != 0.0f) fail |= 32;
    if (fail) { std::printf("00878db0_TEST FAIL %d\n", fail); return 1; }
    std::printf("00878db0_TEST PASS\n");
    return 0;
}