// Behaviour test for 0x00B53F60 CEngineWeatherRenderer::FrameStart (links the REAL row object, built with its /d2inlT94 flag: FrameStart
// and the same-TU UpdateBoxPositions / BuildBoxPositionMatrix bodies, the inline GetVolumeTexture / PeekGraphic / Interpolate and the
// GFFloatToLongFloor expansions run as compiled). The row's externals are defined here: NEngineInternal::GPCamera / GPResourceManager,
// g_pEngineEnvironmentDef, the private static CTextureManager::ReuseBusyTextures, and recording doubles for
// CGraphicDataBank::SetReduceSizeFrameDelay (stores +0x26C, as retail 0x009FD100) and CGraphicInfo::GetVolumeTexture.
// Fixtures are raw native-layout images, every offset read by an instruction of the row or its same-TU callees:
//   renderer: MistVolumeTexture +0x110 (0x00B5404D); Settings +0x114: ParticleBoxLocked +0x11A (UpdateBoxPositions), ParticleBoxSpan
//   +0x1D4.., ParticleBoxCenter +0x1F0.., ParticleBoxPos +0x1FC.., Raining / Snowing / Mist +0x5D0 / +0x5D1 / +0x5D2 (0x00B53F60,
//   0x00B53F6D, 0x00B5405D), Rain / Snow / MistTimePassed +0x5D4.. and their [2] histories +0x5E0.., WindVector +0x5F8, WindVectorHistory
//   +0x604 / +0x610;  params: +0x3C interpolate (0x00B53F80);  camera: position +0x54;  resource manager: graphic bank 0 at +0x954
//   (0x00B54080);  bank: GraphicList begin / end +0x1E0 / +0x1E4 (0x2C-byte CGraphicInfo, 0x00B540A3), ReduceSizeFrameDelay +0x26C
//   (0x00B54086);  environment def: MistAlphaGraphicPC +0xF0 (0x00B5408D).
// Reference model (independent): boxes rebuilt when raining or snowing, the boxes are not locked and the camera has moved half a box
// span (floored) from the centre on any axis; each time-passed = (h1 - h0) * t + h0 and the wind vector likewise (compared to 1e-6
// relative); MistVolumeTexture cleared, then with mist: delay set to 0 and busy textures reusable around one GetVolumeTexture(texture
// slot, level 0) on the bank's graphic `index` (null graphic when out of range), then both restored.
#include <stdio.h>
#include <string.h>
#include <math.h>

class CEngineCamera; class CEngineResourceManager; class CEnvironmentDef; class CVolumeTexture; class CEngineComponentParams;
namespace NEngineInternal { CEngineCamera* GPCamera; CEngineResourceManager* GPResourceManager; }
const CEnvironmentDef* g_pEngineEnvironmentDef;
class CEngineWeatherRenderer { public: virtual void FrameStart(const CEngineComponentParams& cparams); };
class CTextureManager { static bool ReuseBusyTextures; friend bool Reuse(); friend void SetReuse(bool); };
bool CTextureManager::ReuseBusyTextures = false;
bool Reuse() { return CTextureManager::ReuseBusyTextures; }
void SetReuse(bool b) { CTextureManager::ReuseBusyTextures = b; }

static int failures;
static void check(bool ok, const char* what, int k) { if (!ok) { ++failures; if (failures < 40) printf("FAIL %s (case %d)\n", what, k); } }
template<class T> static void put(void* base, int off, T v) { memcpy((unsigned char*)base + off, &v, sizeof v); }
template<class T> static T get(const void* base, int off) { T v; memcpy(&v, (const unsigned char*)base + off, sizeof v); return v; }

struct Event { char kind; const void* self; unsigned long a; const void* p; bool reuse; unsigned long delay; };
static Event events[16]; static int nev;
static unsigned char bank[0x300];
class CGraphicDataBank { public: void SetReduceSizeFrameDelay(unsigned long delay); };
void CGraphicDataBank::SetReduceSizeFrameDelay(unsigned long delay)
{
    if (nev < 16) { Event e = { 'S', this, delay, 0, Reuse(), get<unsigned long>(this, 0x26C) }; events[nev] = e; } ++nev;
    put(this, 0x26C, delay);
}
static CVolumeTexture* const fakeTexture = (CVolumeTexture*)0x7E57;
class CGraphicInfo { public: bool GetVolumeTexture(CVolumeTexture** texture, unsigned long level) const; };
bool CGraphicInfo::GetVolumeTexture(CVolumeTexture** texture, unsigned long level) const
{
    if (nev < 16) { Event e = { 'V', this, level, texture, Reuse(), get<unsigned long>(bank, 0x26C) }; events[nev] = e; } ++nev;
    if (this) *texture = fakeTexture;
    return true;
}

static union { unsigned char b[0x700]; double align; } renderer, params;
static unsigned char camera[0x80], resman[0x980], envdef[0x140];
static unsigned char graphics[6][0x2C];

struct Case {
    bool raining, snowing, mist, locked; float t;
    float camx, camy, camz; long cx, cy, cz; float sx, sy, sz;
    unsigned long index, oldDelay; bool oldReuse; bool zlocked;
};

static unsigned long seed = 5353;
static unsigned long Rnd() { seed = seed * 1103515245ul + 12345ul; return (seed >> 8) & 0xffffff; }
static float RndF(float lo, float hi) { return lo + (hi - lo) * (float)(Rnd() % 65536) / 65536.0f; }
static long Floor(float x) { return (long)floor((double)x); }
static bool Near(float got, double want) { return fabs((double)got - want) <= 1e-6 * (fabs(want) + 1.0); }

static void Run(const Case& c, int k)
{
    memset(renderer.b, 0x4D, sizeof renderer.b); memset(params.b, 0x2B, sizeof params.b);
    memset(camera, 0x11, sizeof camera); memset(resman, 0x22, sizeof resman); memset(envdef, 0x33, sizeof envdef); memset(bank, 0x44, sizeof bank);
    put(params.b, 0x3C, c.t);
    renderer.b[0x5D0] = c.raining; renderer.b[0x5D1] = c.snowing; renderer.b[0x5D2] = c.mist; renderer.b[0x11A] = c.locked; renderer.b[0x11B] = c.zlocked;
    put(renderer.b, 0x1D4, c.sx); put(renderer.b, 0x1D8, c.sy); put(renderer.b, 0x1DC, c.sz);
    put(renderer.b, 0x1F0, c.cx); put(renderer.b, 0x1F4, c.cy); put(renderer.b, 0x1F8, c.cz);
    float hist[3][2] = { { 1.5f, 4.25f }, { -2.0f, 6.0f }, { 10.0f, 9.5f } };
    for (int i = 0; i < 3; ++i) { put(renderer.b, 0x5E0 + i * 8, hist[i][0]); put(renderer.b, 0x5E4 + i * 8, hist[i][1]); }
    float w0[3] = { 0.5f, -1.25f, 3.0f }, w1[3] = { 2.5f, 0.75f, -4.0f };
    for (int i = 0; i < 3; ++i) { put(renderer.b, 0x604 + i * 4, w0[i]); put(renderer.b, 0x610 + i * 4, w1[i]); }
    put(renderer.b, 0x110, (void*)0x5151);                                          // stale texture pointer
    put(camera, 0x54, c.camx); put(camera, 0x58, c.camy); put(camera, 0x5C, c.camz);
    put<void*>(resman, 0x954, bank); put<void*>(bank, 0x1E0, graphics); put<void*>(bank, 0x1E4, graphics + 6); put(bank, 0x26C, c.oldDelay);
    put(envdef, 0xF0, c.index);
    NEngineInternal::GPCamera = (CEngineCamera*)(void*)camera; NEngineInternal::GPResourceManager = (CEngineResourceManager*)(void*)resman;
    g_pEngineEnvironmentDef = (const CEnvironmentDef*)(void*)envdef;
    SetReuse(c.oldReuse); nev = 0;
    unsigned char before[sizeof renderer.b]; memcpy(before, renderer.b, sizeof before);

    ((CEngineWeatherRenderer*)(void*)renderer.b)->CEngineWeatherRenderer::FrameStart(*(const CEngineComponentParams*)(void*)params.b);

    // boxes
    bool moved = false;
    if ((c.raining || c.snowing) && !c.locked) {
        long hx = Floor(c.sx * 0.5f), hy = Floor(c.sy * 0.5f), hz = Floor(c.sz * 0.5f);
        long dx = Floor((float)fabs((float)c.cx - c.camx)), dy = Floor((float)fabs((float)c.cy - c.camy)), dz = Floor((float)fabs((float)c.cz - c.camz));
        moved = dx >= hx || dy >= hy || dz >= hz;
    }
    bool centreSame = get<long>(renderer.b, 0x1F0) == c.cx && get<long>(renderer.b, 0x1F4) == c.cy && get<long>(renderer.b, 0x1F8) == c.cz;
    bool boxesSame = memcmp(renderer.b + 0x1FC, before + 0x1FC, 81 * 12) == 0;
    if (!moved) check(centreSame && boxesSame, "boxes untouched when not moved / locked / dry", k);
    else {
        long fx = Floor(c.camx), fy = Floor(c.camy);
        long sx = Floor(c.sx), sy = Floor(c.sy);
        long ex = fx / sx * sx + (fx % sx >= sx / 2 ? sx : 0), ey = fy / sy * sy + (fy % sy >= sy / 2 ? sy : 0);
        long fz = Floor(c.camz), sz = Floor(c.sz);
        long ez = c.zlocked ? c.cz : fz / sz * sz + (fz % sz >= sz / 2 ? sz : 0);
        check(get<long>(renderer.b, 0x1F0) == ex && get<long>(renderer.b, 0x1F4) == ey && get<long>(renderer.b, 0x1F8) == ez, "boxes rebuilt around the camera (Z kept when Z-locked)", k);
        check(get<float>(renderer.b, 0x1FC + 40 * 12) == (float)ex && get<float>(renderer.b, 0x1FC + 40 * 12 + 4) == (float)ey
              && get<float>(renderer.b, 0x1FC + 40 * 12 + 8) == (float)ez, "middle box at the centre", k);
        const float corner = (float)((double)-4 * c.sx + (double)ex);                 // stored: one rounding to float, as the row stores it
        check(get<float>(renderer.b, 0x1FC) == corner, "corner box X", k);
    }
    // interpolation
    for (int i = 0; i < 3; ++i)
        check(Near(get<float>(renderer.b, 0x5D4 + i * 4), ((double)hist[i][1] - hist[i][0]) * c.t + hist[i][0]), "time passed interpolated", k);
    for (int i = 0; i < 3; ++i)
        check(Near(get<float>(renderer.b, 0x5F8 + i * 4), ((double)w1[i] - w0[i]) * c.t + w0[i]), "wind vector interpolated", k);
    for (int i = 0; i < 3; ++i)
        check(get<float>(renderer.b, 0x5E0 + i * 8) == hist[i][0] && get<float>(renderer.b, 0x604 + i * 4) == w0[i], "histories untouched", k);
    // mist texture
    void* slot = renderer.b + 0x110;
    if (!c.mist) {
        check(get<void*>(renderer.b, 0x110) == 0 && nev == 0, "no mist: texture cleared, no bank calls", k);
        check(Reuse() == c.oldReuse && get<unsigned long>(bank, 0x26C) == c.oldDelay, "no mist: state untouched", k);
    } else {
        const void* info = c.index < 6 ? (const void*)graphics[c.index] : 0;
        bool seq = nev == 3
            && events[0].kind == 'S' && events[0].self == bank && events[0].a == 0
            && events[1].kind == 'V' && events[1].self == info && events[1].p == slot && events[1].a == 0 && events[1].reuse && events[1].delay == 0
            && events[2].kind == 'S' && events[2].self == bank && events[2].a == c.oldDelay && events[2].reuse;
        check(seq, "delay 0 + busy reuse around one GetVolumeTexture(texture slot, 0) on graphic[index], then delay restored", k);
        check(Reuse() == c.oldReuse && get<unsigned long>(bank, 0x26C) == c.oldDelay, "reuse flag and delay restored", k);
        check(get<void*>(renderer.b, 0x110) == (info ? (void*)fakeTexture : (void*)0), "texture slot filled (cleared first)", k);
    }
}

int main()
{
    int k = 0;
    Case base = { true, false, true, false, 0.25f, 100.0f, 50.0f, 10.0f, 100, 50, 10, 20.0f, 16.0f, 10.0f, 2, 7, false, false };
    Case c;
    c = base; Run(c, ++k);                                                   // raining, not moved
    c = base; c.camx = 111.0f; Run(c, ++k);                                  // X moved half a span -> rebuild
    c = base; c.camy = 41.9f; Run(c, ++k);                                   // Y |50-41.9| floor 8 >= 8 -> rebuild
    c = base; c.camz = 14.9f; Run(c, ++k);                                   // Z floor 4 < 5: no rebuild
    c = base; c.camz = 15.0f; Run(c, ++k);                                   // Z 5 >= 5: rebuild
    c = base; c.camx = 200.0f; c.locked = true; Run(c, ++k);                 // locked: never
    c = base; c.camx = 200.0f; c.raining = false; Run(c, ++k);               // dry: never
    c = base; c.camx = 200.0f; c.raining = false; c.snowing = true; Run(c, ++k);   // snowing: rebuild
    c = base; c.mist = false; Run(c, ++k);                                   // no mist
    c = base; c.index = 5; c.oldReuse = true; c.oldDelay = 0; Run(c, ++k);   // last graphic, reuse already on
    c = base; c.index = 6; Run(c, ++k);                                      // out of range: null graphic
    c = base; c.index = 0x80000000ul; Run(c, ++k);                           // far out of range (unsigned compare)
    c = base; c.t = 0.0f; Run(c, ++k);
    c = base; c.t = 1.0f; c.oldDelay = 123; Run(c, ++k);
    c = base; c.t = -0.5f; Run(c, ++k);
    c = base; c.camz = 30.0f; c.zlocked = true; Run(c, ++k);                 // Z-locked rebuild keeps centre Z
    for (int r = 0; r < 3000; ++r) {
        Case q;
        q.raining = (Rnd() & 1) != 0; q.snowing = (Rnd() & 3) == 0; q.mist = (Rnd() & 1) != 0; q.locked = (Rnd() & 7) == 0; q.zlocked = (Rnd() & 3) == 0;
        q.t = RndF(-0.5f, 1.5f); q.sx = RndF(1.0f, 64.0f); q.sy = RndF(1.0f, 64.0f); q.sz = RndF(1.0f, 64.0f);
        q.cx = (long)(Rnd() % 2001) - 1000; q.cy = (long)(Rnd() % 2001) - 1000; q.cz = (long)(Rnd() % 201) - 100;
        q.camx = (float)q.cx + RndF(-40.0f, 40.0f); q.camy = (float)q.cy + RndF(-40.0f, 40.0f); q.camz = (float)q.cz + RndF(-40.0f, 40.0f);
        q.index = Rnd() % 8; q.oldDelay = Rnd() % 5; q.oldReuse = (Rnd() & 1) != 0;
        Run(q, 1000 + r);
    }
    if (failures) { printf("00b53f60_TEST FAIL %d\n", failures); return 1; }
    printf("00b53f60_TEST PASS (%d targeted, 3000 random)\n", k);
    return 0;
}