// Behaviour test for 0x00B075D0 NParticleEngine::CPSCSingleSprite::AddToBoundingSphere (links the REAL row object, with what it
// carries: the expanded list / matrix / lookup helpers, the const CVectorMap::LowerBound COMDAT and ScaleParticleDimensions).
// The classes come from the shared headers only so that the out-of-line callees defined here get their retail names; every object
// is raw native-layout storage and every offset used is one the row reads:
//   CPSCSingleSprite  SpriteBankIndex +0x34, AnimationTimeSecs +0x44, StartRenderSize +0x48, EndRenderSize +0x4C, SpriteAlignment +0x50,
//                     PositionParam +0x6C, TrailBankIndex +0x80, TrailWidth +0x84, flag byte +0x93 (CrossedSprites 0x20,
//                     ForceAnimationTime 0x80), Lifetimer +0x9C, the trail list's head node pointer +0xB8
//                     (node: next +0, prev +4, Position +8, Width +0x14)
//   CParticleSystem   flag byte +0x50 (NormalisedTimeEnabled 0x08, StayWithEmitter 0x40, ScaleParticles 0x80), NormalisedTime +0x5C,
//                     Scale +0x60
//   CParticleUpdateParameters  UpdateParams start +0x14 / finish +0x18; 16-byte pair: key +0, desc vptr +4, Callback +8, Index +0xC
//   CGraphicDataBank  vptr (GetNoFramesInGraphic = slot +0x50), GraphicList start +0x1E0 / finish +0x1E4; CGraphicInfo 0x2C bytes,
//                     Frames +0x28; CGraphicInfoFrame 0x10 bytes, GraphicInfo +0xC; header FrameWidth +6, FrameHeight +8 (words)
// Expected values come from the independent model in Model() (plain double arithmetic on the raw fields).
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_TOC3DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CPARTICLESYSTEM
#define FABLE_RETAIL_CPARTICLESYSTEM_SCALE
#define FABLE_RETAIL_CPARTICLESYSTEM_UPDATE
#define FABLE_RETAIL_CPARTICLESYSTEM_BOUNDING_TRANSFORM_PASS24
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_ZYX_SUMS
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CGRAPHICINFO_SPRITE
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_ADDTOBOUNDS
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CPARTICLEUPDATEPARAMDESC_GETUPDATEPARAM_NESTED
#define FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_GETUPDATEPARAM
#define FABLE_RETAIL_CPSCSPLINEBASE_PASS23
#define FABLE_RETAIL_CPSCSINGLESPRITE_ADDTOBOUNDINGSPHERE
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CPSCSingleSprite.h"
#include "engine/CParticleSystem.h"
#include "engine/CParticleUpdateParameters.h"
#include "engine/C2DVector.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/CGraphicDataBank.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static int failures, g_case;
static void check(bool ok, const char* what) { if (!ok) { ++failures; if (failures < 40) printf("FAIL %s (case %d)\n", what, g_case); } }
static bool Near(double a, double b) { double d = fabs(a - b), m = fabs(b) > 1.0 ? fabs(b) : 1.0; return d <= 2e-5 * m; }

// ---- the writable float the row reads as ?DELTA_NOTIONAL_ZERO@@3MB ----
#pragma const_seg(".dlt")
extern const float DELTA_NOTIONAL_ZERO = 0.25f;
#pragma const_seg()
#pragma comment(linker, "/SECTION:.dlt,RW")
static void SetDelta(float v) { *(float*)&DELTA_NOTIONAL_ZERO = v; }
static float GetDelta() { return *(volatile float*)&DELTA_NOTIONAL_ZERO; }

// ---- recorded out-of-line callees ----
struct BoundsCall { void* self; float x, y, z, radius; };
static BoundsCall g_bounds[64]; static int g_nbounds;
void C3DBoundingSphere::AddToBounds(const C3DVector& point, float radius)
{
    if (g_nbounds < 64) { BoundsCall& c = g_bounds[g_nbounds]; c.self = this; memcpy(&c.x, &point, 12); c.radius = radius; }
    ++g_nbounds;
}
static float g_matrix[12]; static int g_transforms; static const void* g_transform_self;
static long g_tick; static int g_ticks;
static unsigned char* g_bank; static int g_banks;
namespace NParticleEngine {
void CParticleSystem::GetSystemTransform(CMatrix3x4& m) const { ++g_transforms; g_transform_self = this; memcpy(&m, g_matrix, 48); }
long FABLE_FASTCALL GetTickRate() { ++g_ticks; return g_tick; }
const CGraphicDataBank& FABLE_FASTCALL GetGraphicBank() { ++g_banks; return *(const CGraphicDataBank*)g_bank; }
}

// ---- the two virtual callees, reached through hand-built vtables (thiscall = fastcall with a dummy edx) ----
struct CallbackCall { void* self; void* out; unsigned long index; float time; int local; const void* params; unsigned long flags; };
static CallbackCall g_cb; static int g_cbs; static bool g_cb_result; static float g_cb_pos[3];
static bool __fastcall CallbackGetUpdateParam(void* self, void*, float* out, unsigned long index, float time, bool local, const void* params, unsigned long flags)
{
    ++g_cbs; g_cb.self = self; g_cb.out = out; g_cb.index = index; g_cb.time = time; g_cb.local = local ? 1 : 0; g_cb.params = params; g_cb.flags = flags;
    if (g_cb_result) { out[0] = g_cb_pos[0]; out[1] = g_cb_pos[1]; out[2] = g_cb_pos[2]; }
    return g_cb_result;
}
static void __fastcall BadSlot(void*, void*) { printf("FAIL unexpected virtual slot\n"); ++failures; }
static void* g_callback_vtable[4] = { (void*)BadSlot, (void*)CallbackGetUpdateParam, (void*)BadSlot, (void*)BadSlot };
static long g_noframes; static int g_noframe_calls; static unsigned long g_noframe_index; static void* g_noframe_self;
static unsigned long __fastcall BankGetNoFramesInGraphic(void* self, void*, unsigned long index)
{
    ++g_noframe_calls; g_noframe_index = index; g_noframe_self = self; return (unsigned long)g_noframes;
}
static void* g_bank_vtable[24];

// ---- raw objects ----
static unsigned char g_sprite[0x138], g_system[0x200], g_params[0x24], g_bankmem[0x200], g_sphere[0x20];
struct TrailNode { TrailNode* next; TrailNode* prev; float pos[3]; float width; };
static TrailNode g_nodes[8];                               // [0] is the list's head (sentinel) node
struct ParamPair { unsigned long key; void* vptr; void* callback; unsigned long index; };
static ParamPair g_pairs[6];
static void* g_callback_object[2];
static unsigned char g_infos[8 * 0x2C], g_frames[8][16 * 0x10], g_headers[8][16][0x10];
template<class T> static void Put(unsigned char* base, int off, T v) { memcpy(base + off, &v, sizeof(T)); }

struct Rng { unsigned long s; unsigned long Next() { s = s * 1664525UL + 1013904223UL; return s >> 8; }
             float Float(float lo, float hi) { return lo + (hi - lo) * (float)(Next() & 0xFFFF) / 65535.0f; } };

struct Scenario {
    long trail_bank; int trail_count; float trail_width; float trail_pos[7][3];
    unsigned char system_flags; float normalised_time; float scale[3];
    long sprite_bank; unsigned long position_param; int pair_count; unsigned long pair_keys[6]; bool cb_result; float cb_pos[3];
    unsigned char sprite_flags; long alignment; float anim_secs, start_size, end_size; long lifetimer; float interpolate;
    long tick; long noframes; int graphic_count; float delta;
};

static void Build(const Scenario& s)
{
    memset(g_sprite, 0xCD, sizeof g_sprite); memset(g_system, 0xCD, sizeof g_system); memset(g_params, 0xCD, sizeof g_params);
    memset(g_bankmem, 0xCD, sizeof g_bankmem);
    Put<long>(g_sprite, 0x80, s.trail_bank); Put<float>(g_sprite, 0x84, s.trail_width);
    g_nodes[0].next = g_nodes[0].prev = &g_nodes[0];
    for (int i = 0; i < s.trail_count; ++i) {
        TrailNode* n = &g_nodes[i + 1]; memcpy(n->pos, s.trail_pos[i], 12); n->width = 77.0f;
        n->prev = g_nodes[0].prev; n->next = &g_nodes[0]; g_nodes[0].prev->next = n; g_nodes[0].prev = n;
    }
    Put<void*>(g_sprite, 0xB8, &g_nodes[0]);
    Put<long>(g_sprite, 0x34, s.sprite_bank); Put<unsigned long>(g_sprite, 0x6C, s.position_param);
    Put<float>(g_sprite, 0x44, s.anim_secs); Put<float>(g_sprite, 0x48, s.start_size); Put<float>(g_sprite, 0x4C, s.end_size);
    Put<long>(g_sprite, 0x50, s.alignment); g_sprite[0x93] = s.sprite_flags; Put<long>(g_sprite, 0x9C, s.lifetimer);
    g_system[0x50] = s.system_flags; Put<float>(g_system, 0x5C, s.normalised_time); memcpy(g_system + 0x60, s.scale, 12);
    g_callback_object[0] = g_callback_vtable;
    for (int j = 0; j < s.pair_count; ++j) { g_pairs[j].key = s.pair_keys[j]; g_pairs[j].vptr = 0; g_pairs[j].callback = g_callback_object; g_pairs[j].index = 1000 + j; }
    Put<void*>(g_params, 0x14, g_pairs); Put<void*>(g_params, 0x18, g_pairs + s.pair_count);
    for (int v = 0; v < 24; ++v) g_bank_vtable[v] = (void*)BadSlot;
    g_bank_vtable[0x50 / 4] = (void*)BankGetNoFramesInGraphic;
    Put<void*>(g_bankmem, 0, g_bank_vtable);
    Put<void*>(g_bankmem, 0x1E0, g_infos); Put<void*>(g_bankmem, 0x1E4, g_infos + s.graphic_count * 0x2C);
    for (int g = 0; g < 8; ++g) {
        Put<void*>(g_infos + g * 0x2C, 0x28, g_frames[g]);
        for (int f = 0; f < 16; ++f) {
            Put<void*>(g_frames[g] + f * 0x10, 0xC, g_headers[g][f]);
            Put<unsigned short>(g_headers[g][f], 6, (unsigned short)(16 + 8 * g + 3 * f));       // FrameWidth
            Put<unsigned short>(g_headers[g][f], 8, (unsigned short)(40 + 5 * g + 7 * f));       // FrameHeight
        }
    }
    g_bank = g_bankmem; g_tick = s.tick; g_noframes = s.noframes; g_cb_result = s.cb_result; memcpy(g_cb_pos, s.cb_pos, 12);
    SetDelta(s.delta);
    g_nbounds = g_transforms = g_ticks = g_banks = g_cbs = g_noframe_calls = 0;
}

struct Expected { bool valid; int nbounds; float pts[10][3]; float radius[10]; int transforms; int cbs; unsigned long cb_index; int ticks, banks, noframe_calls; };

static void Transform(const float* m, float* p)
{
    double x = p[0], y = p[1], z = p[2];
    p[0] = (float)(m[0] * x + m[3] * y + m[6] * z + m[9]);
    p[1] = (float)(m[1] * x + m[4] * y + m[7] * z + m[10]);
    p[2] = (float)(m[2] * x + m[5] * y + m[8] * z + m[11]);
}

static void Model(const Scenario& s, Expected& e)
{
    memset(&e, 0, sizeof e);
    if (s.trail_bank != 0 && s.trail_count > 0) {
        for (int i = 0; i < s.trail_count; ++i) {
            float p[3]; memcpy(p, s.trail_pos[i], 12);
            if (s.system_flags & 0x40) { Transform(g_matrix, p); ++e.transforms; }
            memcpy(e.pts[e.nbounds], p, 12); e.radius[e.nbounds] = (float)(s.trail_width * 0.5); ++e.nbounds;
        }
        e.valid = true;
    }
    if (s.sprite_bank == 0 || s.position_param == 0) return;
    int hit = -1;
    for (int j = 0; j < s.pair_count; ++j) if (s.pair_keys[j] == s.position_param) hit = j;
    if (hit < 0) return;
    e.cbs = 1; e.cb_index = 1000 + hit;
    if (!s.cb_result) return;
    float p[3]; memcpy(p, s.cb_pos, 12); Transform(g_matrix, p); ++e.transforms;
    e.ticks = 1; e.banks = 1; e.noframe_calls = 1;
    float tick = (float)s.tick, life = (float)((double)s.lifetimer + s.interpolate);
    float nt = (s.system_flags & 0x08) ? s.normalised_time : 0.0f;
    if (nt < 0.0f) nt = 0.0f; else if (nt > 1.0f) nt = 1.0f;
    float anim;
    if (!(s.sprite_flags & 0x80)) {
        float top = (float)(1.0 - (double)s.delta);
        anim = nt; if (anim < 0.0f) anim = 0.0f; else if (anim > top) anim = top;
    } else {
        anim = (float)fmod((1.0 / ((double)s.anim_secs * tick)) * life, 1.0);
    }
    long frame = (long)floor((double)(float)((double)s.noframes * anim));
    if (frame < 0) frame = 0; else if (frame > s.noframes - 1) frame = s.noframes - 1;
    unsigned short w, h; memcpy(&w, g_headers[s.sprite_bank][frame] + 6, 2); memcpy(&h, g_headers[s.sprite_bank][frame] + 8, 2);
    float sx = (float)(s.start_size + ((double)s.end_size - s.start_size) * anim);
    float sy = (float)((double)h / w * sx);
    if (s.system_flags & 0x80) {
        float m = s.scale[0] > s.scale[1] ? s.scale[0] : s.scale[1]; if (s.scale[2] > m) m = s.scale[2];
        sx = (float)((double)sx * m); sy = (float)((double)sy * m);
    }
    double factor = ((s.sprite_flags & 0x20) && s.alignment != 0) ? 1.0 : 0.5;
    memcpy(e.pts[e.nbounds], p, 12); e.radius[e.nbounds] = (float)(sqrt((double)sy * sy + (double)sx * sx) * factor); ++e.nbounds;
    e.valid = true;
}

static void Run(const Scenario& s)
{
    Build(s);
    Expected e; Model(s, e);
    NParticleEngine::CPSCSingleSprite* sprite = (NParticleEngine::CPSCSingleSprite*)g_sprite;
    bool valid = sprite->NParticleEngine::CPSCSingleSprite::AddToBoundingSphere(*(C3DBoundingSphere*)g_sphere, s.interpolate,
        *(NParticleEngine::CParticleSystem*)g_system, *(const NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>*)g_params);
    check(valid == e.valid, "return value");
    check(g_nbounds == e.nbounds, "AddToBounds call count");
    for (int i = 0; i < e.nbounds && i < g_nbounds; ++i) {
        check(g_bounds[i].self == g_sphere, "AddToBounds this");
        check(Near(g_bounds[i].x, e.pts[i][0]) && Near(g_bounds[i].y, e.pts[i][1]) && Near(g_bounds[i].z, e.pts[i][2]), "AddToBounds point");
        check(Near(g_bounds[i].radius, e.radius[i]), "AddToBounds radius");
    }
    check(g_transforms == e.transforms, "GetSystemTransform call count");
    if (e.transforms) check(g_transform_self == g_system, "GetSystemTransform this");
    check(g_cbs == e.cbs, "callback call count");
    if (e.cbs && g_cbs) {
        check(g_cb.self == g_callback_object, "callback this"); check(g_cb.index == e.cb_index, "callback index");
        check(g_cb.time == s.interpolate, "callback time"); check(g_cb.local == 0, "callback tangent flag");
        check(g_cb.params == g_params, "callback params"); check(g_cb.flags == 1, "callback nesting depth");
    }
    check(g_ticks == e.ticks, "GetTickRate call count"); check(g_banks == e.banks, "GetGraphicBank call count");
    check(g_noframe_calls == e.noframe_calls, "GetNoFramesInGraphic call count");
    if (e.noframe_calls && g_noframe_calls) { check(g_noframe_index == (unsigned long)s.sprite_bank, "GetNoFramesInGraphic index"); check(g_noframe_self == g_bankmem, "GetNoFramesInGraphic this"); }
    // the trail entries and the raw fields must be untouched
    for (int i = 0; i < s.trail_count; ++i) check(memcmp(g_nodes[i + 1].pos, s.trail_pos[i], 12) == 0 && g_nodes[i + 1].width == 77.0f, "trail entry untouched");
    check(g_sprite[0x93] == s.sprite_flags && g_system[0x50] == s.system_flags, "flag bytes untouched");
}

int main()
{
    Rng r; r.s = 20261120UL; int cases = 0;
    for (int m = 0; m < 12; ++m) g_matrix[m] = 0.0f;
    for (g_case = 0; g_case < 6000; ++g_case) {
        Scenario s; memset(&s, 0, sizeof s);
        for (int m = 0; m < 12; ++m) g_matrix[m] = r.Float(-2.0f, 2.0f);
        s.trail_bank = (r.Next() % 4 == 0) ? 0 : (long)(r.Next() % 9) - 3; s.trail_count = (int)(r.Next() % 6); if (r.Next() % 5 == 0) s.trail_count = 0;
        s.trail_width = r.Float(0.1f, 9.0f);
        for (int i = 0; i < 7; ++i) for (int k = 0; k < 3; ++k) s.trail_pos[i][k] = r.Float(-50.0f, 50.0f);
        s.system_flags = (unsigned char)(r.Next() & 0xFF);
        s.normalised_time = r.Float(-0.4f, 1.4f); if (r.Next() % 7 == 0) s.normalised_time = (r.Next() & 1) ? 0.0f : 1.0f;
        for (int k = 0; k < 3; ++k) s.scale[k] = r.Float(0.2f, 3.0f);
        s.graphic_count = 8;
        s.sprite_bank = (r.Next() % 5 == 0) ? 0 : 1 + (long)(r.Next() % 7);
        s.pair_count = (int)(r.Next() % 7); unsigned long key = 3 + r.Next() % 5;
        for (int j = 0; j < s.pair_count; ++j) { s.pair_keys[j] = key; key += 2 + r.Next() % 4; }
        unsigned long pick = r.Next() % 8;
        if (pick == 0) s.position_param = 0;
        else if (pick < 6 && s.pair_count) s.position_param = s.pair_keys[r.Next() % s.pair_count];          // present
        else if (pick == 6) s.position_param = 1 + r.Next() % 2;                                             // below every key
        else s.position_param = (s.pair_count ? s.pair_keys[r.Next() % s.pair_count] : 40) + 1;             // between keys / above
        s.cb_result = (r.Next() % 6) != 0; for (int k = 0; k < 3; ++k) s.cb_pos[k] = r.Float(-30.0f, 30.0f);
        s.sprite_flags = (unsigned char)(r.Next() & 0xFF); s.alignment = (r.Next() % 3 == 0) ? 0 : (long)(r.Next() % 5) - 2;
        s.anim_secs = r.Float(0.2f, 6.0f); s.start_size = r.Float(0.1f, 8.0f); s.end_size = r.Float(0.1f, 8.0f);
        s.lifetimer = (long)(r.Next() % 4000) - 600; s.interpolate = r.Float(0.0f, 1.0f); s.tick = 20 + (long)(r.Next() % 41);
        s.noframes = 1 + (long)(r.Next() % 16);
        s.delta = (r.Next() % 3 == 0) ? 0.0f : ((r.Next() & 1) ? 0.25f : 0.001f);
        Run(s); ++cases;
    }
    check(GetDelta() == 0.0f || GetDelta() == 0.25f || GetDelta() == 0.001f, "delta storage");
    if (failures == 0) printf("00b075d0_TEST PASS (%d cases)\n", cases); else printf("00b075d0_TEST FAIL (%d failures)\n", failures);
    return failures ? 1 : 0;
}