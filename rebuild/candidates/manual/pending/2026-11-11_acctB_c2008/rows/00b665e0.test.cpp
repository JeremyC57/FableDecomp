// Behaviour test for CEngineSkyRenderer::GetEnvironmentMap (0x00B665E0): a host model of the call sequence and of the environment-map
// camera it builds (120-degree FOV, mirrored Y axis, eye at -6500 / tan(60 degrees) on Z, far plane (7000 - z) * 1.1, near 1, z range
// [sky z start, 1], window = the surface size), checked against the retail constants.
#include <cstdio>
#include <cstring>
#include <cmath>

struct Desc { float Window[4]; float M[12]; float Pos[3]; float HFOV, VFOV; bool Use2DFOV; float Near, Far, ZStart, ZEnd; };
static char log_[512]; static long nlog;
static void Log(const char* s) { std::strcat(log_, s); std::strcat(log_, ";"); ++nlog; }
static Desc camera; static Desc applied[4]; static long napplied;
static float vwindow[4]; static unsigned long clear_colour, clear_flags; static float sky_interp, sky_pos[3]; static bool sky_reflection;
static const float sky_z_start = 0.25f;
static float AXToVX(float x) { return x * 0.5f; }   // the virtual-coordinate conversions (scaled stand-ins)
static float AYToVY(float y) { return y * 0.25f; }

static void GetEnvironmentMap(unsigned long width, unsigned long height, float frame_interpolation)
{
    Log("capture");
    Desc old_desc = camera;
    Log("attach"); Log("settarget");
    vwindow[0] = 0.0f; vwindow[1] = 0.0f; vwindow[2] = AXToVX((float)width); vwindow[3] = AYToVY((float)height); Log("vwindow");
    clear_colour = 0xff000000; clear_flags = 4; Log("clear"); Log("fogoff");
    float interpolate = frame_interpolation;
    Desc desc = old_desc;
    const float fov = 2.0943952f;
    float z = -6500.0f / (float)std::tan(fov * 0.5f);
    static const float pos[3] = { 0.0f, 0.0f, -2550.0f };
    desc.HFOV = fov; desc.Far = (7000.0f - z) * 1.1f; desc.Near = 1.0f; desc.ZStart = sky_z_start; desc.ZEnd = 1.0f;
    desc.Pos[0] = 0.0f; desc.Pos[1] = 0.0f; desc.Pos[2] = z;
    static const float m[12] = { 1, 0, 0, 0, -1, 0, 0, 0, 1, 0, 0, 0 };
    std::memcpy(desc.M, m, sizeof(m));
    desc.Window[0] = 0.0f; desc.Window[1] = 0.0f; desc.Window[2] = (float)width; desc.Window[3] = (float)height;
    applied[napplied++] = desc; camera = desc; Log("setcamera"); Log("zoff");
    sky_interp = interpolate; std::memcpy(sky_pos, pos, sizeof(pos)); sky_reflection = true; Log("rendersky");
    Log("backscreen"); Log("detach"); Log("detachz"); Log("refresh"); Log("uninit");
    applied[napplied++] = old_desc; camera = old_desc; Log("setcamera"); Log("restore");
}

static bool Near(float a, float b) { return std::fabs(a - b) <= 0.01f * (std::fabs(b) + 1.0f); }

int main()
{
    std::memset(&camera, 0, sizeof(camera)); camera.HFOV = 1.0f; camera.Pos[0] = 5.0f; camera.Far = 100.0f;
    GetEnvironmentMap(256, 128, 0.75f);
    const Desc& d = applied[0];
    bool ok = napplied == 2 && Near(d.HFOV, 2.0943952f) && Near(d.Pos[2], -3752.777f) && Near(d.Far, (7000.0f + 3752.777f) * 1.1f)
        && d.Near == 1.0f && d.ZEnd == 1.0f && d.ZStart == sky_z_start && d.Pos[0] == 0.0f && d.M[0] == 1.0f && d.M[4] == -1.0f
        && d.M[8] == 1.0f && d.Window[2] == 256.0f && d.Window[3] == 128.0f && vwindow[2] == 128.0f && vwindow[3] == 32.0f
        && clear_colour == 0xff000000 && clear_flags == 4 && sky_interp == 0.75f && sky_pos[2] == -2550.0f && sky_reflection
        && applied[1].HFOV == 1.0f && camera.Pos[0] == 5.0f && camera.Far == 100.0f
        && std::strcmp(log_, "capture;attach;settarget;vwindow;clear;fogoff;setcamera;zoff;rendersky;backscreen;detach;detachz;refresh;uninit;setcamera;restore;") == 0;
    if (!ok) { std::printf("00b665e0_TEST FAIL\n"); return 1; }
    std::printf("00b665e0_TEST PASS\n");
    return 0;
}
