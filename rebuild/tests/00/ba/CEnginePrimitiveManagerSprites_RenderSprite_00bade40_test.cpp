// Behaviour test for 0x00BADE40 CEnginePrimitiveManagerSprites::RenderSprite (links the REAL row object: the expanded capture block,
// distance, fade, radius, frustum test, graphic-frame lookup and sub-primitive construction run as compiled).
// Fixtures (raw native layout, every offset read by an instruction of the row):
//   CRenderStateManager (CRenderStateCaptureBlock::StateManager 0x013CB508): capture stack count +0x2808, entries of 8 bytes from +8
//     (the entry gets the manager's +0x3A24 block, 0x00BADE4C..0x00BADE67), state mask +0x2814 shifted left once (0x00BADE69).
//   CEngineInternalPrimitiveSprite: FadeDistance.FadeBeginDistance +0x14, MaxDrawDistance +0x18, WorldPos +0x48, BankIndex +0x54,
//     Frame +0x5C, SizeX / SizeY +0x60 / +0x64, OffsetFactorX / Y +0x68 / +0x6C, BSphereRadius +0x70, Colour.A +0x83, flags +0x84
//     (IsParticle 0x20, 0x00BAE069 `and byte ptr [esi+84h],0DFh`).
//   CEngineCamera (GPCamera 0x01436EA0): CameraDesc.WorldPosition +0x54, NearZPlane +0x6C, FarZPlane +0x70, LookVector +0x1B4,
//     FrustrumPlanes[0..3].Normal +0x1C0 / +0x1D0 / +0x1E0 / +0x1F0.
//   CEngineResourceManager (GPResourceManager 0x01436E98): graphic bank 0 pointer +0x954; CGraphicDataBank GraphicList begin / end
//     +0x1E0 / +0x1E4 (44-byte CGraphicInfo, Frames pointer +0x28, 16-byte CGraphicInfoFrame).
//   CEngineSubPrimitiveSprite (24 bytes from CEngineSubPrimitiveBase::operator new): vptr +0, Parent +0xC, NextSubPrimitive +0x10.
// Stubs (the row's out-of-line callees, recorded in call order): CRenderStateManager::RestoreCaptureBlock 0x00A05840,
//   CGraphicInfoFrame::BeginLoading 0x009FE620, CEngineSubPrimitiveBase::operator new 0x00BEB540, CBaseClassNonCopyable constructor
//   0x0099A2F0, CEngineSubPrimitiveRenderer::AddSubPrimitive 0x00B846E0.
// Reference model (independent, long double): distance = |WorldPos - camera|; past MaxDrawDistance nothing but the capture block;
//   past FadeBeginDistance alpha = round-to-nearest-even((Max - distance) * A / (Max - Begin)); BSphereRadius =
//   sqrt((|SizeX * OffsetFactorX| + SizeX)^2 + (|SizeY * OffsetFactorY| + SizeY)^2) (stored even when the sprite is then culled); the
//   frustum test (depth along the look vector against near / far, then planes 2, 3, 0, 1 against the radius); then IsParticle
//   cleared, BeginLoading(frame or null, 0, 0), a new sub-primitive (or null when the allocation fails) and
//   AddSubPrimitive(sub-primitive, distance, (unsigned char)alpha, 1, this); the capture block is restored exactly once, last.
// Checks: (1) exact dyadic cases on every boundary (distance == Max, == Begin, Max == Begin, each frustum plane at and past its
//   limit, bank index at / past the list size, allocation failure); (2) 6000 seeded random cases against the model (cases within
//   1e-4 of a decision boundary or of a rounding tie are skipped and counted); (3) bytes the row must not write are compared.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

class CRenderStateManager { public: void RestoreCaptureBlock(); };
class CRenderStateCaptureBlock { static CRenderStateManager* StateManager; public: static void Set(CRenderStateManager* m) { StateManager = m; } };
CRenderStateManager* CRenderStateCaptureBlock::StateManager;
class CEngineCamera; class CEngineResourceManager; class CEnginePrimitiveManager; class CEngineInternalPrimitiveSprite;
class CGraphicInfoFrame { public: void BeginLoading(unsigned long, unsigned long) const; };
class CBaseClassNonCopyable { protected: CBaseClassNonCopyable(); public: unsigned long pad[2]; };
class CEngineSubPrimitiveBase : public CBaseClassNonCopyable
{
public:
    virtual ~CEngineSubPrimitiveBase();
    static void* __fastcall operator new(unsigned int size);
    static void __fastcall operator delete(void*);
    void* Parent; void* NextSubPrimitive;
};
class CEngineSubPrimitiveSprite : public CEngineSubPrimitiveBase { public: virtual ~CEngineSubPrimitiveSprite(); unsigned long tail; };
class CEngineSubPrimitiveRenderer { public: void AddSubPrimitive(CEngineSubPrimitiveBase*, float, unsigned char, unsigned long, CEnginePrimitiveManager*); };
namespace NEngineInternal
{
    CEngineCamera* GPCamera;
    CEngineResourceManager* GPResourceManager;
    CEngineSubPrimitiveRenderer* GPSubPrimitiveRenderer;
}
class CEnginePrimitiveManagerSprites
{
protected:
    void RenderSprite(CEngineInternalPrimitiveSprite& primitive);
public:
    static void Call(CEnginePrimitiveManagerSprites* m, CEngineInternalPrimitiveSprite* p) { m->RenderSprite(*p); }
};

template<class T> static T& At(void* base, int off) { return *(T*)((unsigned char*)base + off); }

enum { MGR_SIZE = 0x3A40, PRIM_SIZE = 0xA0, CAM_SIZE = 0x210, RES_SIZE = 0x960, BANK_SIZE = 0x200, NINFO = 5, NFRAME = 8 };
static unsigned char g_mgr[MGR_SIZE], g_prim[PRIM_SIZE], g_cam[CAM_SIZE], g_res[RES_SIZE], g_bank[BANK_SIZE];
static unsigned char g_infos[NINFO * 44], g_frames[NINFO][NFRAME * 16];
static unsigned char g_this[16], g_renderer[16];
static unsigned char g_subprim[2][64];
static int g_fail, g_cases, g_skipped, g_queued, g_faded;

enum { EV_RESTORE = 1, EV_BEGIN, EV_NEW, EV_CTOR, EV_ADD };
static int g_ev[16], g_nev;
static void* g_restore_this; static const void* g_begin_this; static unsigned long g_begin_a, g_begin_b;
static unsigned int g_new_size; static bool g_new_fail; static void* g_ctor_this;
static void* g_add_this; static void* g_add_sub; static float g_add_distance; static unsigned char g_add_alpha;
static unsigned long g_add_flags; static void* g_add_manager;
static unsigned long g_mask_at_restore, g_count_at_restore;

void CRenderStateManager::RestoreCaptureBlock()
{
    g_ev[g_nev++ & 15] = EV_RESTORE; g_restore_this = this;
    g_count_at_restore = At<unsigned long>(g_mgr, 0x2808); g_mask_at_restore = At<unsigned long>(g_mgr, 0x2814);
}
void CGraphicInfoFrame::BeginLoading(unsigned long a, unsigned long b) const { g_ev[g_nev++ & 15] = EV_BEGIN; g_begin_this = this; g_begin_a = a; g_begin_b = b; }
CBaseClassNonCopyable::CBaseClassNonCopyable() { g_ev[g_nev++ & 15] = EV_CTOR; g_ctor_this = this; }
CEngineSubPrimitiveBase::~CEngineSubPrimitiveBase() { }
CEngineSubPrimitiveSprite::~CEngineSubPrimitiveSprite() { }
void* __fastcall CEngineSubPrimitiveBase::operator new(unsigned int size)
{
    g_ev[g_nev++ & 15] = EV_NEW; g_new_size = size;
    if (g_new_fail) return 0;
    memset(g_subprim[0], 0xCD, sizeof g_subprim[0]);
    return g_subprim[0] + 16;
}
void __fastcall CEngineSubPrimitiveBase::operator delete(void*) { }
void CEngineSubPrimitiveRenderer::AddSubPrimitive(CEngineSubPrimitiveBase* sub, float distance, unsigned char alpha, unsigned long flags, CEnginePrimitiveManager* manager)
{
    g_ev[g_nev++ & 15] = EV_ADD; g_add_this = this; g_add_sub = sub; g_add_distance = distance; g_add_alpha = alpha; g_add_flags = flags; g_add_manager = manager;
}

static void* g_sprite_vptr;
static void LearnVptr()
{
    // the vftable the row stores is ??_7CEngineSubPrimitiveSprite@@6B@: build one object here to read the same symbol's address
    g_new_fail = false;
    CEngineSubPrimitiveSprite* s = new CEngineSubPrimitiveSprite;
    g_sprite_vptr = At<void*>(s, 0);
}

static unsigned long g_seed = 777;
static unsigned long Rnd() { g_seed = g_seed * 1664525u + 1013904223u; return g_seed >> 8; }
static float RndF(float lo, float hi) { return lo + (hi - lo) * (float)(Rnd() & 0xFFFF) / 65536.0f; }

struct Case
{
    float wp[3], cam[3], look[3], plane[4][3], nearz, farz, begin, maxd, sx, sy, ofx, ofy;
    unsigned char a; unsigned long bank, frame, ninfo; unsigned char flags; bool new_fail;
};
static void Fail(const char* what, double got, double want)
{
    if (g_fail++ < 16) printf("FAIL case %d: %s got %.9g want %.9g\n", g_cases, what, got, want);
}
static long double RoundEven(long double v)
{
    long double f = floorl(v), r = v - f;
    if (r > 0.5L) return f + 1; if (r < 0.5L) return f;
    return (fmodl(f, 2) == 0) ? f : f + 1;
}
// returns false when the case sits on a decision boundary the float model cannot call (exact == false)
static bool Run(const Case& c, bool exact)
{
    int i, k;
    // ---- model
    long double dx = (long double)c.wp[0] - c.cam[0], dy = (long double)c.wp[1] - c.cam[1], dz = (long double)c.wp[2] - c.cam[2];
    long double dist_ld = sqrtl(dx * dx + dy * dy + dz * dz);
    float dist = (float)dist_ld;
    const long double EPS = 1e-4L;
    #define NEAR_TIE(x, y) (!exact && fabsl((long double)(x) - (long double)(y)) <= EPS * (1 + fabsl((long double)(y))))
    if (NEAR_TIE(dist_ld, c.maxd)) return false;
    bool drawn = !(dist_ld > c.maxd);
    long double alpha = c.a;
    if (drawn)
    {
        if (NEAR_TIE(dist, c.begin)) return false;
        if (dist > c.begin)
        {
            long double v = ((long double)c.maxd - dist) * c.a / ((long double)c.maxd - c.begin);
            v = (long double)(float)v;
            long double fr = v - floorl(v);
            if (!exact && fabsl(fr - 0.5L) < 1e-3L) return false;
            alpha = RoundEven(v);
        }
    }
    long double rx = fabsl((long double)c.sx * c.ofx) + c.sx, ry = fabsl((long double)c.sy * c.ofy) + c.sy;
    float radius = (float)sqrtl(rx * rx + ry * ry);
    bool visible = false;
    if (drawn)
    {
        long double vx = (long double)(float)dx, vy = (long double)(float)dy, vz = (long double)(float)dz;
        long double depth = vx * c.look[0] + vy * c.look[1] + vz * c.look[2];
        if (NEAR_TIE(depth + radius, c.nearz) || NEAR_TIE(depth - radius, c.farz)) return false;
        visible = !(depth + radius < c.nearz) && !(depth - radius > c.farz);
        static const int order[4] = { 2, 3, 0, 1 };
        for (k = 0; k < 4 && visible; ++k)
        {
            const float* n = c.plane[order[k]];
            long double dot = vx * n[0] + vy * n[1] + vz * n[2];
            if (NEAR_TIE(dot, radius)) return false;
            if (dot > radius) visible = false;
        }
    }
    ++g_cases;
    // ---- fixtures
    for (i = 0; i < MGR_SIZE; ++i) g_mgr[i] = (unsigned char)(i * 7 + 3);
    unsigned long count0 = Rnd() % 5, mask0 = Rnd() | 1;
    At<unsigned long>(g_mgr, 0x2808) = count0; At<unsigned long>(g_mgr, 0x2814) = mask0;
    CRenderStateCaptureBlock::Set((CRenderStateManager*)g_mgr);
    for (i = 0; i < PRIM_SIZE; ++i) g_prim[i] = (unsigned char)(0x40 + i);
    At<float>(g_prim, 0x14) = c.begin; At<float>(g_prim, 0x18) = c.maxd;
    for (i = 0; i < 3; ++i) At<float>(g_prim, 0x48 + 4 * i) = c.wp[i];
    At<unsigned long>(g_prim, 0x54) = c.bank; At<unsigned long>(g_prim, 0x5C) = c.frame;
    At<float>(g_prim, 0x60) = c.sx; At<float>(g_prim, 0x64) = c.sy; At<float>(g_prim, 0x68) = c.ofx; At<float>(g_prim, 0x6C) = c.ofy;
    At<float>(g_prim, 0x70) = -123.0f; At<unsigned char>(g_prim, 0x83) = c.a; At<unsigned char>(g_prim, 0x84) = c.flags;
    for (i = 0; i < CAM_SIZE; ++i) g_cam[i] = 0;
    for (i = 0; i < CAM_SIZE / 4; ++i) At<float>(g_cam, 4 * i) = 1000.0f + i;   // any field read by mistake is far off
    for (i = 0; i < 3; ++i) { At<float>(g_cam, 0x54 + 4 * i) = c.cam[i]; At<float>(g_cam, 0x1B4 + 4 * i) = c.look[i]; }
    At<float>(g_cam, 0x6C) = c.nearz; At<float>(g_cam, 0x70) = c.farz;
    for (k = 0; k < 4; ++k) for (i = 0; i < 3; ++i) At<float>(g_cam, 0x1C0 + 16 * k + 4 * i) = c.plane[k][i];
    NEngineInternal::GPCamera = (CEngineCamera*)g_cam;
    memset(g_res, 0x11, sizeof g_res); memset(g_bank, 0x22, sizeof g_bank); memset(g_infos, 0x33, sizeof g_infos);
    At<void*>(g_res, 0x954) = g_bank;
    At<void*>(g_bank, 0x1E0) = g_infos; At<void*>(g_bank, 0x1E4) = g_infos + c.ninfo * 44;
    for (i = 0; i < NINFO; ++i) At<void*>(g_infos, i * 44 + 0x28) = g_frames[i];
    NEngineInternal::GPResourceManager = (CEngineResourceManager*)g_res;
    NEngineInternal::GPSubPrimitiveRenderer = (CEngineSubPrimitiveRenderer*)g_renderer;
    memset(g_this, 0x77, sizeof g_this);
    g_nev = 0; memset(g_ev, 0, sizeof g_ev); g_new_fail = c.new_fail; g_restore_this = 0; g_begin_this = (void*)1; g_add_sub = (void*)1;
    unsigned char prim0[PRIM_SIZE], cam0[CAM_SIZE], mgr0[MGR_SIZE], res0[RES_SIZE], bank0[BANK_SIZE], infos0[sizeof g_infos], this0[16];
    memcpy(prim0, g_prim, sizeof prim0); memcpy(cam0, g_cam, sizeof cam0); memcpy(mgr0, g_mgr, sizeof mgr0);
    memcpy(res0, g_res, sizeof res0); memcpy(bank0, g_bank, sizeof bank0); memcpy(infos0, g_infos, sizeof infos0); memcpy(this0, g_this, 16);
    // ---- run
    CEnginePrimitiveManagerSprites::Call((CEnginePrimitiveManagerSprites*)g_this, (CEngineInternalPrimitiveSprite*)g_prim);
    // ---- capture block: one entry pushed (the manager's +0x3A24 block), the mask shifted, restored exactly once and last
    if (At<unsigned long>(g_mgr, 0x2808) != count0 + 1) Fail("capture count", At<unsigned long>(g_mgr, 0x2808), count0 + 1);
    if (At<void*>(g_mgr, 8 + 8 * count0) != (void*)(g_mgr + 0x3A24)) Fail("capture entry", 0, 0);
    if (At<unsigned long>(g_mgr, 0x2814) != mask0 << 1) Fail("capture mask", At<unsigned long>(g_mgr, 0x2814), mask0 << 1);
    At<unsigned long>(mgr0, 0x2808) = count0 + 1; At<void*>(mgr0, 8 + 8 * count0) = (void*)(g_mgr + 0x3A24); At<unsigned long>(mgr0, 0x2814) = mask0 << 1;
    if (memcmp(mgr0, g_mgr, sizeof mgr0)) Fail("manager bytes", 0, 0);
    int nrestore = 0; for (i = 0; i < g_nev; ++i) if (g_ev[i] == EV_RESTORE) ++nrestore;
    if (nrestore != 1 || g_ev[g_nev - 1] != EV_RESTORE) Fail("restore once and last", nrestore, 1);
    if (g_restore_this != (void*)g_mgr) Fail("restore this", 0, 0);
    if (g_count_at_restore != count0 + 1 || g_mask_at_restore != mask0 << 1) Fail("restore before the capture", 0, 0);
    // ---- primitive
    float want_radius = drawn ? radius : -123.0f;
    float got_radius = At<float>(g_prim, 0x70);
    if (exact ? got_radius != want_radius : fabs((double)got_radius - want_radius) > 2e-6 * (1 + fabs((double)want_radius))) Fail("BSphereRadius", got_radius, want_radius);
    unsigned char want_flags = (drawn && visible) ? (unsigned char)(c.flags & ~0x20) : c.flags;
    if (At<unsigned char>(g_prim, 0x84) != want_flags) Fail("flags", At<unsigned char>(g_prim, 0x84), want_flags);
    At<float>(prim0, 0x70) = got_radius; At<unsigned char>(prim0, 0x84) = At<unsigned char>(g_prim, 0x84);
    if (memcmp(prim0, g_prim, sizeof prim0)) Fail("primitive bytes", 0, 0);
    if (memcmp(cam0, g_cam, sizeof cam0) || memcmp(res0, g_res, sizeof res0) || memcmp(bank0, g_bank, sizeof bank0) ||
        memcmp(infos0, g_infos, sizeof infos0) || memcmp(this0, g_this, 16)) Fail("read-only fixture written", 0, 0);
    // ---- calls
    if (!(drawn && visible))
    {
        if (g_nev != 1) Fail("culled sprite calls", g_nev, 1);
        return true;
    }
    ++g_queued; if (dist > c.begin) ++g_faded;
    int want_ev[6], n = 0;
    want_ev[n++] = EV_BEGIN; want_ev[n++] = EV_NEW; if (!c.new_fail) want_ev[n++] = EV_CTOR; want_ev[n++] = EV_ADD; want_ev[n++] = EV_RESTORE;
    if (g_nev != n) Fail("call count", g_nev, n);
    for (i = 0; i < n && i < g_nev; ++i) if (g_ev[i] != want_ev[i]) Fail("call order", g_ev[i], want_ev[i]);
    const void* want_frame = c.bank < c.ninfo ? (const void*)(g_frames[c.bank] + c.frame * 16) : 0;
    if (g_begin_this != want_frame) Fail("BeginLoading this", 0, 0);
    if (g_begin_a != 0 || g_begin_b != 0) Fail("BeginLoading arguments", g_begin_a, g_begin_b);
    if (g_new_size != 24) Fail("new size", g_new_size, 24);
    void* sub = c.new_fail ? 0 : (void*)(g_subprim[0] + 16);
    if (g_add_sub != sub) Fail("AddSubPrimitive subprim", 0, 0);
    if (!c.new_fail)
    {
        if (g_ctor_this != sub) Fail("base constructor this", 0, 0);
        if (At<void*>(sub, 0) != g_sprite_vptr) Fail("vptr", 0, 0);
        if (At<void*>(sub, 0xC) != (void*)g_prim) Fail("Parent", 0, 0);
        if (At<void*>(sub, 0x10) != 0) Fail("NextSubPrimitive", 0, 0);
        for (i = 0; i < 16; ++i) if (g_subprim[0][i] != 0xCD || g_subprim[0][16 + 24 + i] != 0xCD) { Fail("sub-primitive guard", i, 0); break; }
        if (At<unsigned long>(sub, 0x14) != 0xCDCDCDCD) Fail("sub-primitive tail written", 0, 0);
    }
    if (g_add_this != (void*)g_renderer) Fail("renderer this", 0, 0);
    if (g_add_manager != (void*)g_this) Fail("manager argument", 0, 0);
    if (g_add_flags != 1) Fail("render flags", g_add_flags, 1);
    if (exact ? g_add_distance != dist : fabs((double)g_add_distance - dist) > 2e-6 * (1 + fabs((double)dist))) Fail("distance", g_add_distance, dist);
    if (g_add_alpha != (unsigned char)(long)alpha) Fail("alpha", g_add_alpha, (double)alpha);
    return true;
}

static Case Base()
{
    // camera at the origin looking down +Z; side planes along X and Y; sprite straight ahead: distance 16, radius 5 (3-4-5)
    Case c; memset(&c, 0, sizeof c);
    c.wp[2] = 16.0f; c.look[2] = 1.0f;
    c.plane[2][0] = 1.0f; c.plane[3][0] = -1.0f; c.plane[0][1] = 1.0f; c.plane[1][1] = -1.0f;
    c.nearz = 1.0f; c.farz = 100.0f; c.begin = 32.0f; c.maxd = 64.0f; c.sx = 3.0f; c.sy = 4.0f;
    c.a = 200; c.bank = 2; c.frame = 3; c.ninfo = NINFO; c.flags = 0xFF; c.new_fail = false;
    return c;
}
static void Exact()
{
    Case c = Base(); Run(c, true);                                     // plain visible sprite, no fade
    c = Base(); c.flags = 0x20; Run(c, true);                          // only IsParticle set
    c = Base(); c.flags = 0xDF; Run(c, true);                          // every flag but IsParticle
    // draw distance: == Max is drawn, just past it is not
    c = Base(); c.maxd = 16.0f; c.begin = 8.0f; Run(c, true);          // distance == Max, past Begin: alpha (16-16)*200/8 = 0
    c = Base(); c.maxd = 15.5f; c.begin = 8.0f; Run(c, true);          // past Max: nothing
    c = Base(); c.maxd = 16.0f; c.begin = 16.0f; Run(c, true);         // distance == Begin == Max: no fade (a `>=` would divide 0 by 0)
    // fade: distance 16, Begin 8, Max 24: alpha = 8 * A / 16
    c = Base(); c.begin = 8.0f; c.maxd = 24.0f; c.a = 200; Run(c, true);   // 100
    c = Base(); c.begin = 8.0f; c.maxd = 24.0f; c.a = 5; Run(c, true);     // 2.5 -> 2 (ties to even)
    c = Base(); c.begin = 8.0f; c.maxd = 24.0f; c.a = 7; Run(c, true);     // 3.5 -> 4
    c = Base(); c.begin = 12.0f; c.maxd = 20.0f; c.a = 255; Run(c, true);  // 4 * 255 / 8 = 127.5 -> 128
    c = Base(); c.begin = 0.0f; c.maxd = 64.0f; c.a = 255; Run(c, true);   // 48 * 255 / 64 = 191.25 -> 191
    c = Base(); c.begin = 16.0f; Run(c, true);                         // distance == Begin: no fade
    // radius: offset factors of either sign add |size * factor|
    c = Base(); c.ofx = 1.0f; c.ofy = -0.5f; Run(c, true);             // (6, 6): radius sqrt(72), inexact -> compared exactly as float
    c = Base(); c.ofx = -1.0f; c.ofy = 0.0f; c.sx = 4.0f; c.sy = 6.0f; Run(c, true);   // (8, 6): 10
    c = Base(); c.sx = 0.0f; c.sy = 2.0f; c.ofy = 3.0f; Run(c, true);  // (0, 8): 8
    // 3D distance and non-zero camera: (2, 3, 6) from the camera -> 7
    c = Base(); c.cam[0] = 1.0f; c.cam[1] = 2.0f; c.cam[2] = 3.0f; c.wp[0] = 3.0f; c.wp[1] = 5.0f; c.wp[2] = 9.0f; c.begin = 6.0f; c.maxd = 8.0f; Run(c, true);
    c = Base(); c.wp[0] = 1.0f; c.wp[1] = 4.0f; c.wp[2] = 8.0f; c.begin = 8.0f; c.maxd = 10.0f; c.a = 100; Run(c, true);   // distance 9: alpha 50
    // near / far: depth 16, radius 5
    c = Base(); c.nearz = 21.0f; Run(c, true);                         // depth + radius == near: kept
    c = Base(); c.nearz = 21.5f; Run(c, true);                         // culled by the near plane
    c = Base(); c.farz = 11.0f; Run(c, true);                          // depth - radius == far: kept
    c = Base(); c.farz = 10.5f; Run(c, true);                          // culled by the far plane
    // side planes 2, 3, 0, 1: dot == radius kept, past it culled (sprite moved sideways; the distance stays dyadic-inexact-free: 3-4-5 scaled)
    for (int p = 0; p < 4; ++p)
    {
        c = Base(); c.wp[0] = 0; c.wp[1] = 0; c.wp[2] = 16.0f;
        memset(c.plane, 0, sizeof c.plane); c.plane[p][2] = 0.3125f;   // dot = 5 == radius
        Run(c, true);
        c.plane[p][2] = 0.375f;                                        // dot = 6 > radius
        Run(c, true);
        c.plane[p][2] = -4.0f;                                         // far inside
        Run(c, true);
    }
    // graphic lookup: index at / past the list size gives a null frame; frame index scaled by 16
    c = Base(); c.bank = NINFO - 1; c.frame = 7; Run(c, true);
    c = Base(); c.bank = NINFO; Run(c, true);
    c = Base(); c.ninfo = 2; c.bank = 2; Run(c, true);
    c = Base(); c.ninfo = 0; c.bank = 0; Run(c, true);
    c = Base(); c.bank = 0; c.frame = 0; Run(c, true);
    // allocation failure: a null sub-primitive is still queued
    c = Base(); c.new_fail = true; Run(c, true);
    // alpha 0 and 255 without fade
    c = Base(); c.a = 0; Run(c, true);
    c = Base(); c.a = 255; Run(c, true);
}

int main()
{
    LearnVptr();
    Exact();
    int exact_cases = g_cases;
    for (int n = 0; n < 6000; ++n)
    {
        Case c = Base(); int i, k;
        for (i = 0; i < 3; ++i) { c.cam[i] = RndF(-4.0f, 4.0f); c.wp[i] = c.cam[i] + RndF(-12.0f, 12.0f); c.look[i] = RndF(-1.0f, 1.0f); }
        for (k = 0; k < 4; ++k) for (i = 0; i < 3; ++i) c.plane[k][i] = RndF(-1.0f, 1.0f) * ((Rnd() & 3) ? 0.25f : 1.5f);
        c.nearz = RndF(-30.0f, 2.0f); c.farz = RndF(-2.0f, 40.0f);
        c.begin = RndF(0.0f, 14.0f); c.maxd = c.begin + RndF(0.5f, 16.0f);
        c.sx = RndF(0.0f, 3.0f); c.sy = RndF(0.0f, 3.0f); c.ofx = RndF(-1.0f, 1.0f); c.ofy = RndF(-1.0f, 1.0f);
        c.a = (unsigned char)Rnd(); c.bank = Rnd() % (NINFO + 2); c.frame = Rnd() % NFRAME; c.ninfo = Rnd() % (NINFO + 1);
        c.flags = (unsigned char)Rnd(); c.new_fail = (Rnd() % 7) == 0;
        if (!Run(c, false)) ++g_skipped;
    }
    if (g_cases - exact_cases < 5000) { printf("00bade40_TEST FAIL (only %d random cases ran, %d skipped)\n", g_cases - exact_cases, g_skipped); return 1; }
    if (g_fail) { printf("00bade40_TEST FAIL (%d of %d)\n", g_fail, g_cases); return 1; }
    if (g_queued < 600 || g_faded < 200) { printf("00bade40_TEST FAIL (coverage: %d queued, %d faded)\n", g_queued, g_faded); return 1; }
    printf("00bade40_TEST PASS (%d cases: %d queued, %d of them faded; %d boundary cases skipped)\n", g_cases, g_queued, g_faded, g_skipped);
    return 0;
}