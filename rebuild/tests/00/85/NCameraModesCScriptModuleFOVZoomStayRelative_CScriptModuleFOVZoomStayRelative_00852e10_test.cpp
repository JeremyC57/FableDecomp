// Behaviour test for 0x00852E10 NCameraModes::CScriptModuleFOVZoomStayRelative::CScriptModuleFOVZoomStayRelative (links the REAL row
// object with the COMDATs it carries: the two vftables, the inline CIntelligentPointer<CThing> copy). Raw native-layout storage only:
//   module   vptr +0x00, Thing +0x84 (vptr, PData), Axis +0x8C, Angle +0x98, Mag +0x9C (the base ctor's members are its own business)
//   CThing   PhysicsTC +0x60;  CTCPhysicsBase  vtable slot 70 (+0x118) GetFacingVector() const, C3DVector by hidden pointer
// Every callee logs a letter; the test compares the whole call sequence and every argument (the base FOV-zoom ctor gets look_at_pos,
// start_pos, end_pos, the fraction, the frame count and the flag in that order; Thing is built from the by-value thing with the base
// ctor, its vptr, GetPItem on the argument and SetPItem; the facing comes from the argument's thing; the argument is destroyed last),
// both vptrs, and Mag = |end - look|, rel = the fast-normalised end - look (GFFastOneOverSqrt: table estimate + one Newton step),
// Angle = acos(facing . rel) in turns, Axis = rel x facing against an independent model.
// GFastOneOverSqrtTable: the 128-entry mantissa table GFInitMaths fills at start-up (retail 0x013CE558, in .bss), as the landed
// 0x007AF0F0 test spells it.
#include <stdio.h>
#include <math.h>
#include <string.h>
#pragma comment(linker, "/alternatename:@CallRow@40=??0CScriptModuleFOVZoomStayRelative@NCameraModes@@QAE@V?$CIntelligentPointer@VCThing@@@@ABVC3DVector@@11MJ_N@Z")
#pragma comment(linker, "/alternatename:??0CScriptModuleFOVZoom@NCameraModes@@QAE@ABVC3DVector@@00MJ_N@Z=@BaseCtor@32")
#pragma comment(linker, "/alternatename:??0CBaseIntelligentPointer@@QAE@XZ=@IpCtor@4")
#pragma comment(linker, "/alternatename:??1CBaseIntelligentPointer@@UAE@XZ=@IpDtor@4")
#pragma comment(linker, "/alternatename:?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ=@IpGet@4")
#pragma comment(linker, "/alternatename:?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z=@IpSet@12")
#pragma comment(linker, "/alternatename:_StayVft=??_7CScriptModuleFOVZoomStayRelative@NCameraModes@@6B@")
#pragma comment(linker, "/alternatename:_IpVft=??_7?$CIntelligentPointer@VCThing@@@@6B@")
// vftable entries the ctor never calls
#pragma comment(linker, "/alternatename:??1CScriptModuleBase@NCameraModes@@UAE@XZ=_Never")
#pragma comment(linker, "/alternatename:??1CScriptModuleFOVZoom@NCameraModes@@UAE@XZ=_Never")
#pragma comment(linker, "/alternatename:?GetModuleFOV@CScriptModuleBase@NCameraModes@@UBE_NAAM@Z=_Never")
#pragma comment(linker, "/alternatename:?GetModuleRollAngle@CScriptModuleBase@NCameraModes@@UBE_NAAM@Z=_Never")
#pragma comment(linker, "/alternatename:?IsFinished@CScriptModuleBase@NCameraModes@@UBE_NXZ=_Never")
#pragma comment(linker, "/alternatename:?IsShortenOnOcclusion@CScriptModuleFOVZoom@NCameraModes@@UBE_NXZ=_Never")
#pragma comment(linker, "/alternatename:?IsStretchAroundHero@CScriptModuleBase@NCameraModes@@UBE_NXZ=_Never")
#pragma comment(linker, "/alternatename:?OnInitLeaf@CScriptModuleFOVZoom@NCameraModes@@MAEXXZ=_Never")
#pragma comment(linker, "/alternatename:?ResetCameraOnEnd@CScriptModuleFOVZoom@NCameraModes@@UBE_NXZ=_Never")
#pragma comment(linker, "/alternatename:?Update@CScriptModuleFOVZoomStayRelative@NCameraModes@@UAEXXZ=_Never")
unsigned long GFastOneOverSqrtTable[128]={106,103,100,98,95,92,90,87,85,83,80,78,76,74,72,70,68,66,64,62,60,58,56,54,52,51,49,47,46,44,42,41,39,38,36,35,33,32,31,29,28,26,25,24,22,21,20,19,17,16,15,14,13,12,10,9,8,7,6,5,4,3,2,1,255,252,248,244,240,237,233,230,226,223,220,217,214,211,208,205,202,199,196,193,191,188,185,183,180,178,176,173,171,168,166,164,162,160,158,155,153,151,149,147,145,143,142,140,138,136,134,133,131,129,127,126,124,122,121,119,118,116,115,113,112,110,109,107};
struct V3 { float X, Y, Z; };
extern "C" void* __fastcall CallRow(void* self, int edx, void* thing_vptr, void* thing_pdata, const V3* look, const V3* start,
                                    const V3* end, float fraction, long frames, int shorten);
extern "C" const void* const StayVft[];
extern "C" const void* const IpVft[];
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall OnFault(void*) { ExitProcess(3); return 0; }

static int g_fail = 0;
static void Check(bool ok, int line, int scene) { if (!ok) { if (g_fail < 20) printf("check failed at line %d (scene %d)\n", line, scene); g_fail = 1; } }
static char g_log[64]; static int g_logn;
static void Log(char c) { if (g_logn < 60) g_log[g_logn++] = c; g_log[g_logn] = 0; }
extern "C" void __cdecl Never() { Log('!'); }

// ---- the scene ----
static unsigned char g_row[0xa0 + 16];
static unsigned char g_thing[0x100], g_item[0x10];
static struct { const void* vptr; } g_physics;
static const void* g_physvt[80];
static V3 g_facing, g_look, g_start, g_end; static float g_frac; static long g_frames; static int g_shorten;
static struct { void* self; const V3* look; const V3* start; const V3* end; float frac; long frames; int shorten; } g_base;
static void* g_ipctor_self; static void* g_ipset_self; static void* g_ipset_item; static void* g_ipdtor_self;
static void* g_get_self[4]; static int g_gets; static void* g_facing_self; static void* g_param;

extern "C" void* __fastcall BaseCtor(void* self, int, const V3* look, const V3* start, const V3* end, float frac, long frames, int shorten)
{
    Log('B'); g_base.self = self; g_base.look = look; g_base.start = start; g_base.end = end; g_base.frac = frac; g_base.frames = frames;
    g_base.shorten = shorten & 0xff;
    *(const void**)self = (const void*)0x1234;         // the base's own vptr, overwritten by the row
    return self;
}
extern "C" void* __fastcall IpCtor(void* self) { Log('c'); g_ipctor_self = self; ((void**)self)[1] = 0; return self; }
extern "C" void __fastcall IpDtor(void* self) { Log('d'); g_ipdtor_self = self; }
extern "C" void* __fastcall IpGet(const void* self) { Log('g'); if (g_gets < 4) g_get_self[g_gets] = (void*)self; g_gets++; return ((void* const*)self)[1]; }
extern "C" void __fastcall IpSet(void* self, int, void* item) { Log('s'); g_ipset_self = self; g_ipset_item = item; ((void**)self)[1] = item; }
extern "C" V3* __fastcall Facing(const void* self, int, V3* out) { Log('f'); g_facing_self = (void*)self; *out = g_facing; return out; }

// ---- the model ----
static float FastOneOverSqrt(float value)
{
    unsigned long bits; memcpy(&bits, &value, 4);
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) | (GFastOneOverSqrtTable[(bits >> 17) & 0x7f] << 15);
    float r; memcpy(&r, &estimate, 4);
    return (float)((3.0 - (double)r * r * value) * r * 0.5);
}
static bool Near(double a, double b, double scale) { if (a != a && b != b) return true; return fabs(a - b) <= 2e-6 * (scale > 1.0 ? scale : 1.0); }
static unsigned g_seed = 12345u;
static float Rf(float lo, float hi) { g_seed = g_seed * 1664525u + 1013904223u; return lo + (hi - lo) * (float)((g_seed >> 8) & 0xffff) / 65535.0f; }

static void Run(int scene)
{
    memset(g_row, 0xcd, sizeof g_row); g_logn = 0; g_log[0] = 0; g_gets = 0; memset(g_get_self, 0, sizeof g_get_self);
    *(void**)(g_thing + 0x60) = &g_physics;
    void* param[2] = { (void*)0x5678, g_thing };          // the by-value CIntelligentPointer<CThing>: vptr, PData
    g_param = param;
    void* ret = CallRow(g_row, 0, param[0], param[1], &g_look, &g_start, &g_end, g_frac, g_frames, g_shorten);
    // calls: base ctor, Thing's base ctor, GetPItem(argument), SetPItem, GetPItem(argument), GetFacingVector, the argument's dtor
    Check(strcmp(g_log, "Bcgsgfd") == 0, __LINE__, scene);
    Check(ret == g_row, __LINE__, scene);
    Check(g_base.self == g_row && g_base.look == &g_look && g_base.start == &g_start && g_base.end == &g_end, __LINE__, scene);
    Check(g_base.frac == g_frac && g_base.frames == g_frames && g_base.shorten == g_shorten, __LINE__, scene);
    Check(g_ipctor_self == g_row + 0x84 && g_ipset_self == g_row + 0x84 && g_ipset_item == g_thing, __LINE__, scene);
    Check(g_gets == 2, __LINE__, scene);
    Check(g_get_self[0] != g_row + 0x84 && g_get_self[1] != g_row + 0x84, __LINE__, scene);     // both reads are of the argument
    Check(g_ipdtor_self == g_get_self[0] && g_get_self[0] == g_get_self[1], __LINE__, scene);    // ... which is destroyed at the end
    Check(g_facing_self == &g_physics, __LINE__, scene);
    Check(*(const void* const**)g_row == StayVft && *(const void* const**)(g_row + 0x84) == IpVft, __LINE__, scene);
    Check(*(void**)(g_row + 0x88) == g_thing, __LINE__, scene);
    // the values
    double x = (double)g_end.X - g_look.X, y = (double)g_end.Y - g_look.Y, z = (double)g_end.Z - g_look.Z;
    float rx = (float)x, ry = (float)y, rz = (float)z;
    double mag = sqrt((double)rx * rx + (double)ry * ry + (double)rz * rz);
    float sq = (float)((double)rx * rx + (double)ry * ry + (double)rz * rz);
    float k = FastOneOverSqrt(sq);
    float nx = (float)(rx * (double)k), ny = (float)(ry * (double)k), nz = (float)(rz * (double)k);
    double dot = (double)g_facing.X * nx + (double)g_facing.Y * ny + (double)g_facing.Z * nz;
    double angle = acos(dot) * 0.15915494;
    double ax = (double)ny * g_facing.Z - (double)nz * g_facing.Y, ay = (double)nz * g_facing.X - (double)nx * g_facing.Z,
           az = (double)nx * g_facing.Y - (double)ny * g_facing.X;
    float* axis = (float*)(g_row + 0x8c);
    float fl = (float)(fabs(g_facing.X) + fabs(g_facing.Y) + fabs(g_facing.Z));
    Check(Near(*(float*)(g_row + 0x9c), mag, mag), __LINE__, scene);
    Check(Near(*(float*)(g_row + 0x98), angle, 1.0), __LINE__, scene);
    Check(Near(axis[0], ax, fl) && Near(axis[1], ay, fl) && Near(axis[2], az, fl), __LINE__, scene);
    // nothing else of the module is written by the row (bytes 0x04..0x83 belong to the base ctor stub, which wrote none but the vptr)
    for (int i = 4; i < 0x84; ++i) Check(g_row[i] == 0xcd, __LINE__, scene);
    for (int i = 0xa0; i < (int)sizeof g_row; ++i) Check(g_row[i] == 0xcd, __LINE__, scene);
}
int main()
{
    SetUnhandledExceptionFilter((void*)OnFault);
    for (int i = 0; i < 80; ++i) g_physvt[i] = (const void*)Never;
    g_physvt[70] = (const void*)Facing; g_physics.vptr = g_physvt;
    for (int scene = 0; scene < 4000; ++scene) {
        g_look.X = Rf(-500, 500); g_look.Y = Rf(-500, 500); g_look.Z = Rf(-50, 50);
        g_start.X = Rf(-500, 500); g_start.Y = Rf(-500, 500); g_start.Z = Rf(-50, 50);
        float s = (scene & 3) == 0 ? 0.01f : (scene & 3) == 1 ? 1.0f : 40.0f;
        g_end.X = g_look.X + Rf(-s, s); g_end.Y = g_look.Y + Rf(-s, s); g_end.Z = g_look.Z + Rf(-s, s);
        if (scene % 7 == 0) { g_end.Z = g_look.Z; }
        g_facing.X = Rf(-1, 1); g_facing.Y = Rf(-1, 1); g_facing.Z = (scene % 5 == 0) ? 0.0f : Rf(-1, 1);
        if (scene % 3 == 0) { float l = (float)sqrt(g_facing.X * g_facing.X + g_facing.Y * g_facing.Y + g_facing.Z * g_facing.Z); if (l > 0.1f) { g_facing.X /= l; g_facing.Y /= l; g_facing.Z /= l; } }
        g_frac = Rf(0.2f, 2.0f); g_frames = scene % 31; g_shorten = scene & 1;
        Run(scene);
    }
    if (!g_fail) printf("00852e10_TEST PASS\n");
    return g_fail;
}