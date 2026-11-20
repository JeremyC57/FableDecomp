// Behaviour test for 0x00B24850 CEngine::SetDefaultRenderStates (links the REAL row object: the row body and all 38 expanded
// SetRenderState / AddStateToUpdateList bodies plus the inline SetTextureEnable store run as compiled). No class models: the render
// manager is a raw 0x4000-byte buffer at the engine header offsets (CRenderStateManager: StateStack +0x8, StateUpdateList +0x2008,
// StateStackSize +0x2808, StateUpdateListSize +0x280C, CurrentBookmarkMask +0x2814, DeviceState +0x2818 (CStateInfo 0x14 bytes:
// CurrentState, DesiredState +4, BookmarkMask +8, param +0xC, DirtyListFlag +0x10); CRenderManagerCore TexturesEnabled +0x3A57 (retail
// 0x00B25733)); the engine is a raw buffer (EnableWireFrame +0x10, EnableTextures +0x13, AnisotropicFilteringLevel +0x158). The twelve
// D3D*Table arrays are test data with a distinct value per (table, index). Every out-of-line callee is bound by /alternatename to an
// extern "C" __fastcall stub (thiscall-compatible) that logs its `this` and arguments.
// Reference model: Model() below, written from the source statements and the header's SetRenderState / AddStateToUpdateList text,
// run on a copy of the initial buffer; the row's buffer must equal it byte for byte (every desired state, bookmark mask, dirty flag,
// state-stack entry and update-list entry, both sizes, TexturesEnabled), and the call log must equal the model's (order, `this`, args).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall Crash(void*) { printf("FAIL crash\n"); ExitProcess(3); return 0; }

#pragma comment(linker, "/alternatename:@CallRow@8=?SetDefaultRenderStates@CEngine@@AAEXXZ")
#pragma comment(linker, "/alternatename:?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A=_g_RM")
#pragma comment(linker, "/alternatename:?GPShaderRenderManager@NEngineInternal@@3PAVCShaderRenderManager@@A=_g_SRM")
#pragma comment(linker, "/alternatename:?GPLightingManager@NEngineInternal@@3PAVCEngineLightingManager@@A=_g_LM")
#pragma comment(linker, "/alternatename:?GPDisplayManager@NEngineInternal@@3PAVCDisplayManager@@A=_g_DM")
#pragma comment(linker, "/alternatename:?SetupDefaultFogStates@CEngineLightingManager@@QAEXXZ=@FogStub@8")
#pragma comment(linker, "/alternatename:?SetVertexShaderConstantLayout@CShaderRenderManager@@QAEXJ@Z=@LayoutStub@12")
#pragma comment(linker, "/alternatename:?SetTextureTransformCount@CShaderRenderManager@@QAEXJ@Z=@TTCountStub@12")
#pragma comment(linker, "/alternatename:?EnableXBoxShaderConstants@CRenderManagerCore@@QAEX_N@Z=@XBoxStub@12")
#pragma comment(linker, "/alternatename:?GetVertexShaderVersion@CDisplayManager@@QBEKXZ=@VSVerStub@8")
#pragma comment(linker, "/alternatename:?SetUVCompressionVertexShaderConstant@CShaderRenderManager@@QAEXMMMM@Z=@UVStub@24")
#pragma comment(linker, "/alternatename:?D3DAlphaBlendModeTable@CRenderStateManager@@0PAKA=_t_AlphaBlend")
#pragma comment(linker, "/alternatename:?D3DAlphaFuncModeTable@CRenderStateManager@@0PAKA=_t_AlphaFunc")
#pragma comment(linker, "/alternatename:?D3DPolyCullModeTable@CRenderStateManager@@0PAKA=_t_PolyCull")
#pragma comment(linker, "/alternatename:?D3DPolyFillModeTable@CRenderStateManager@@0PAKA=_t_PolyFill")
#pragma comment(linker, "/alternatename:?D3DPolyShadeModeTable@CRenderStateManager@@0PAKA=_t_PolyShade")
#pragma comment(linker, "/alternatename:?D3DTextureArgumentModeTable@CRenderStateManager@@0PAKA=_t_TexArg")
#pragma comment(linker, "/alternatename:?D3DTextureFilterModeTable@CRenderStateManager@@0PAKA=_t_TexFilter")
#pragma comment(linker, "/alternatename:?D3DTextureMipMapModeTable@CRenderStateManager@@0PAKA=_t_TexMip")
#pragma comment(linker, "/alternatename:?D3DTextureOperationModeTable@CRenderStateManager@@0PAKA=_t_TexOp")
#pragma comment(linker, "/alternatename:?D3DTextureTransformModeTable@CRenderStateManager@@0PAKA=_t_TexTransform")
#pragma comment(linker, "/alternatename:?D3DTextureUVModeTable@CRenderStateManager@@0PAKA=_t_TexUV")
#pragma comment(linker, "/alternatename:?D3DZBufferFuncModeTable@CRenderStateManager@@0PAKA=_t_ZFunc")

enum { TB_ALPHABLEND, TB_ALPHAFUNC, TB_POLYCULL, TB_POLYFILL, TB_POLYSHADE, TB_TEXARG, TB_TEXFILTER, TB_TEXMIP, TB_TEXOP, TB_TEXTRANSFORM,
       TB_TEXUV, TB_ZFUNC, TB_COUNT };
extern "C" {
unsigned long t_AlphaBlend[16], t_AlphaFunc[16], t_PolyCull[16], t_PolyFill[16], t_PolyShade[16], t_TexArg[16], t_TexFilter[16],
              t_TexMip[16], t_TexOp[16], t_TexTransform[16], t_TexUV[16], t_ZFunc[16];
void* g_RM; void* g_SRM; void* g_LM; void* g_DM;
}
static unsigned long* const Tables[TB_COUNT] = { t_AlphaBlend, t_AlphaFunc, t_PolyCull, t_PolyFill, t_PolyShade, t_TexArg, t_TexFilter,
                                                  t_TexMip, t_TexOp, t_TexTransform, t_TexUV, t_ZFunc };
static unsigned long T(int table, int index) { return Tables[table][index]; }

// ---- call log ----
struct SEvent { int kind; void* self; unsigned long a, b, c, d; };
enum { EV_FOG = 1, EV_LAYOUT, EV_TTCOUNT, EV_XBOX, EV_VSVER, EV_UV };
static SEvent Log[64]; static int LogN;
static void Add(SEvent* log, int& n, int kind, void* self, unsigned long a = 0, unsigned long b = 0, unsigned long c = 0, unsigned long d = 0)
{ if (n < 64) { log[n].kind = kind; log[n].self = self; log[n].a = a; log[n].b = b; log[n].c = c; log[n].d = d; } ++n; }
static unsigned long Bits(float f) { unsigned long u; memcpy(&u, &f, 4); return u; }
static unsigned long VSVersion;
extern "C" void __fastcall FogStub(void* self, void*) { Add(Log, LogN, EV_FOG, self); }
extern "C" void __fastcall LayoutStub(void* self, void*, long layout) { Add(Log, LogN, EV_LAYOUT, self, (unsigned long)layout); }
extern "C" void __fastcall TTCountStub(void* self, void*, long count) { Add(Log, LogN, EV_TTCOUNT, self, (unsigned long)count); }
extern "C" void __fastcall XBoxStub(void* self, void*, bool enable) { Add(Log, LogN, EV_XBOX, self, enable ? 1u : 0u); }
extern "C" unsigned long __fastcall VSVerStub(void* self, void*) { Add(Log, LogN, EV_VSVER, self); return VSVersion; }
extern "C" void __fastcall UVStub(void* self, void*, float a, float b, float c, float d) { Add(Log, LogN, EV_UV, self, Bits(a), Bits(b), Bits(c), Bits(d)); }
extern "C" void __fastcall CallRow(void* engine, void*);

// ---- render manager layout (engine/CRenderStateManager.h, engine/CRenderManagerCore.h) ----
enum { RM_SIZE = 0x4000, STACK = 0x8, UPDATE = 0x2008, STACK_SIZE = 0x2808, UPDATE_SIZE = 0x280C, BOOKMARK = 0x2814, DEVICE = 0x2818,
       TEXTURES_ENABLED = 0x3A57 };
enum { POLYSHADE = 0x000, POLYFILL = 0x014, ZWRITE = 0x03C, ZENABLE = 0x050, ZFUNC = 0x064, POLYCULL = 0x078, ALPHAFUNC = 0x08C,
       ALPHABLEND_EN = 0x0A0, ALPHAREF = 0x0C8, SRCBLEND = 0x104, DESTBLEND = 0x118, FOGENABLE = 0x12C, FOGCOLOUR = 0x140,
       FOGSTART = 0x154, FOGEND = 0x168, SPECULAR = 0x1CC, DITHER = 0x1F4, CLIPPING = 0x208, LIGHTING = 0x21C,
       MAXANISO = 0x4EC, LODBIAS = 0x58C, MIPMAP = 0x62C, UVU = 0x6CC, UVV = 0x76C, UVW = 0x80C, FILTERMIN = 0x8AC, FILTERMAG = 0x94C,
       TEXOP = 0x9EC, ALPHAOP = 0xA8C, ARG1 = 0xB2C, ARG2 = 0xBCC, ALPHAARG1 = 0xC6C, ALPHAARG2 = 0xD0C, TRANSFORM = 0xE4C };
static unsigned char RM[RM_SIZE], MRM[RM_SIZE], INIT[RM_SIZE];
static unsigned char Engine[0x200];
static unsigned long& U32(unsigned char* b, int off) { return *(unsigned long*)(b + off); }

// The header text of SetRenderState / AddStateToUpdateList (default branch), applied to the model buffer; pointers are stored as the
// row's buffer addresses (RM) so the two buffers compare byte for byte.
static void ModelSet(unsigned char* m, int state_off, unsigned long value)
{
    int s = DEVICE + state_off;
    if (U32(m, s + 4) != value) {
        if ((U32(m, s + 8) & U32(m, BOOKMARK)) == 0) {
            U32(m, s + 8) |= U32(m, BOOKMARK);
            long n = (long)U32(m, STACK_SIZE);
            U32(m, STACK + 8 * n) = (unsigned long)(RM + s);
            U32(m, STACK + 8 * n + 4) = U32(m, s + 4);
            U32(m, STACK_SIZE) = (unsigned long)(n + 1);
        }
        U32(m, s + 4) = value;
        if (!m[s + 0x10]) {
            m[s + 0x10] = 1;
            long n = (long)U32(m, UPDATE_SIZE);
            U32(m, UPDATE + 4 * n) = (unsigned long)(RM + s);
            U32(m, UPDATE_SIZE) = (unsigned long)(n + 1);
        }
    }
}
static SEvent MLog[64]; static int MLogN;
// enum values: engine/RenderStateEnums.h
static void Model(unsigned char* m, bool wire, bool textures, long aniso)
{
    for (long n = 0; n < 8; ++n) {
        ModelSet(m, ALPHAOP + 0x14 * n, T(TB_TEXOP, 0));       // TEXTURE_OP_DISABLE
        ModelSet(m, ARG1 + 0x14 * n, T(TB_TEXARG, 2));         // TEXTURE_ARG_TEXTURE
        ModelSet(m, ARG2 + 0x14 * n, T(TB_TEXARG, 1));         // TEXTURE_ARG_DIFFUSE
        ModelSet(m, ALPHAARG1 + 0x14 * n, T(TB_TEXARG, 2));
        ModelSet(m, ALPHAARG2 + 0x14 * n, T(TB_TEXARG, 1));
    }
    ModelSet(m, ALPHAFUNC, T(TB_ALPHAFUNC, 7));                // ALPHA_TEST_ALWAYS
    ModelSet(m, POLYSHADE, T(TB_POLYSHADE, 1));                // POLY_SHADE_GOURAUD
    ModelSet(m, POLYFILL, T(TB_POLYFILL, 0));                  // POLY_FILL_SOLID
    ModelSet(m, POLYCULL, T(TB_POLYCULL, 0));                  // POLY_CULL_CCW
    ModelSet(m, ZWRITE, 1);
    ModelSet(m, ZFUNC, T(TB_ZFUNC, 3));                        // ZBUFFER_FUNC_LESS_EQUAL
    ModelSet(m, ZENABLE, 1);
    ModelSet(m, FOGENABLE, 0);
    ModelSet(m, FOGCOLOUR, 0xffffff);
    ModelSet(m, FOGSTART, Bits(0.0f));
    ModelSet(m, FOGEND, Bits(1.0f));
    ModelSet(m, ALPHAREF, 0x80);
    ModelSet(m, SPECULAR, 0);
    ModelSet(m, DITHER, 1);
    for (long n = 0; n < 8; ++n) {
        ModelSet(m, MAXANISO + 0x14 * n, (unsigned long)aniso);
        ModelSet(m, LODBIAS + 0x14 * n, Bits(0.0f));
        ModelSet(m, MIPMAP + 0x14 * n, T(TB_TEXMIP, 1));       // TEXTURE_MIPMAP_LINEAR
        ModelSet(m, FILTERMIN + 0x14 * n, T(TB_TEXFILTER, 1)); // TEXTURE_FILTER_LINEAR
        ModelSet(m, FILTERMAG + 0x14 * n, T(TB_TEXFILTER, 1));
        ModelSet(m, UVU + 0x14 * n, T(TB_TEXUV, 2));           // TEXTURE_UV_CLAMP
        ModelSet(m, UVV + 0x14 * n, T(TB_TEXUV, 2));
        ModelSet(m, UVW + 0x14 * n, T(TB_TEXUV, 2));
        ModelSet(m, TRANSFORM + 0x14 * 0, T(TB_TEXTRANSFORM, 0)); // stage 0, TEXTURE_TRANSFORM_DIABLED
    }
    ModelSet(m, SRCBLEND, T(TB_ALPHABLEND, 1));                // ALPHA_BLEND_ONE
    ModelSet(m, DESTBLEND, T(TB_ALPHABLEND, 0));               // ALPHA_BLEND_ZERO
    ModelSet(m, TEXOP + 0x14 * 1, T(TB_TEXOP, 0));
    ModelSet(m, ALPHAOP + 0x14 * 1, T(TB_TEXOP, 0));
    ModelSet(m, TEXOP + 0x14 * 0, T(TB_TEXOP, 3));             // TEXTURE_OP_MODULATE
    ModelSet(m, ALPHAOP + 0x14 * 0, T(TB_TEXOP, 3));
    m[TEXTURES_ENABLED] = textures ? 1 : 0;
    ModelSet(m, ALPHABLEND_EN, 0);
    ModelSet(m, CLIPPING, 1);
    ModelSet(m, LIGHTING, 0);
    Add(MLog, MLogN, EV_FOG, g_LM);
    Add(MLog, MLogN, EV_LAYOUT, g_SRM, 3);
    Add(MLog, MLogN, EV_TTCOUNT, g_SRM, 0);
    Add(MLog, MLogN, EV_XBOX, g_RM, 1);
    Add(MLog, MLogN, EV_VSVER, g_DM);
    Add(MLog, MLogN, EV_UV, g_SRM, Bits(1.0f / 2048.0f), Bits(1.0f / 2048.0f), Bits(-8.0f), Bits(-8.0f));
    if (wire) ModelSet(m, POLYFILL, T(TB_POLYFILL, 1));        // POLY_FILL_WIRE
}

// All the states the row touches (offsets within DeviceState) -- the random set-up below seeds each one.
static int States[200]; static int StateN;
static void AddState(int off) { for (int i = 0; i < StateN; ++i) if (States[i] == off) return; States[StateN++] = off; }
static void CollectStates()
{
    static const int single[] = { POLYSHADE, POLYFILL, ZWRITE, ZENABLE, ZFUNC, POLYCULL, ALPHAFUNC, ALPHABLEND_EN, ALPHAREF, SRCBLEND,
                                  DESTBLEND, FOGENABLE, FOGCOLOUR, FOGSTART, FOGEND, SPECULAR, DITHER, CLIPPING, LIGHTING };
    static const int staged[] = { MAXANISO, LODBIAS, MIPMAP, UVU, UVV, UVW, FILTERMIN, FILTERMAG, TEXOP, ALPHAOP, ARG1, ARG2, ALPHAARG1,
                                  ALPHAARG2, TRANSFORM };
    for (int i = 0; i < (int)(sizeof(single) / sizeof(single[0])); ++i) AddState(single[i]);
    for (int i = 0; i < (int)(sizeof(staged) / sizeof(staged[0])); ++i) for (int n = 0; n < 8; ++n) AddState(staged[i] + 0x14 * n);
}

static unsigned long Rnd() { return ((unsigned long)rand() << 16) ^ (unsigned long)rand(); }
// The value the model will write to a state, when known for a fixed engine set-up: used to seed "already desired" states so the
// early-out path (DesiredState == value) runs. Picked by running the model on a scratch buffer with every DesiredState = a sentinel.
static unsigned long Target[0x1300];
static int Fails;
static bool RunCase(const char* name, unsigned long seed, int mode, bool wire, bool textures, long aniso)
{
    srand(seed);
    memset(INIT, 0, sizeof(INIT));
    // Pre-compute each touched state's final value for this engine set-up.
    {
        static unsigned char scratch[RM_SIZE]; memset(scratch, 0, sizeof(scratch));
        U32(scratch, BOOKMARK) = 1;
        for (int i = 0; i < StateN; ++i) U32(scratch, DEVICE + States[i] + 4) = 0xA5A5A5A5;
        int saved = MLogN; Model(scratch, wire, textures, aniso); MLogN = saved;
        for (int i = 0; i < StateN; ++i) Target[States[i]] = U32(scratch, DEVICE + States[i] + 4);
    }
    // mode 0: everything differs, nothing bookmarked, nothing dirty, empty lists. mode 1: everything already desired.
    // mode 2: random mixture (desired / differs / bookmarked / dirty, non-empty lists, random current bookmark bit).
    U32(INIT, STACK_SIZE) = mode == 2 ? (unsigned long)(rand() % 50) : 0;
    U32(INIT, UPDATE_SIZE) = mode == 2 ? (unsigned long)(rand() % 40) : 0;
    U32(INIT, BOOKMARK) = mode == 2 ? (1ul << (rand() % 32)) : 1;
    for (int i = 0; i < StateN; ++i) {
        int s = DEVICE + States[i];
        U32(INIT, s + 0) = Rnd();
        U32(INIT, s + 0xC) = Rnd();
        INIT[s + 0x11] = (unsigned char)rand(); INIT[s + 0x12] = (unsigned char)rand();
        if (mode == 0) { U32(INIT, s + 4) = Target[States[i]] ^ 0x80000000; }
        else if (mode == 1) { U32(INIT, s + 4) = Target[States[i]]; U32(INIT, s + 8) = Rnd(); INIT[s + 0x10] = (unsigned char)(rand() & 1); }
        else {
            int r = rand() % 3;
            U32(INIT, s + 4) = r == 0 ? Target[States[i]] : r == 1 ? Rnd() : (Target[States[i]] ^ (1ul << (rand() % 32)));
            U32(INIT, s + 8) = (rand() & 1) ? Rnd() : 0;
            INIT[s + 0x10] = (unsigned char)(rand() & 1);
        }
    }
    INIT[TEXTURES_ENABLED] = (unsigned char)(rand() & 1);
    for (int k = 0; k < 64; ++k) { U32(INIT, STACK + 8 * k) = Rnd(); U32(INIT, STACK + 8 * k + 4) = Rnd(); U32(INIT, UPDATE + 4 * k) = Rnd(); }
    memcpy(RM, INIT, sizeof(RM)); memcpy(MRM, INIT, sizeof(MRM));
    memset(Engine, 0xCD, sizeof(Engine));
    Engine[0x10] = wire ? 1 : 0; Engine[0x13] = textures ? 1 : 0; *(long*)(Engine + 0x158) = aniso;
    VSVersion = (unsigned long)(rand() % 3000);
    LogN = 0; MLogN = 0;
    CallRow(Engine, 0);
    Model(MRM, wire, textures, aniso);
    bool ok = true;
    if (memcmp(RM, MRM, sizeof(RM)) != 0) {
        for (int i = 0; i < RM_SIZE; ++i) if (RM[i] != MRM[i]) { printf("FAIL %s: render manager byte +0x%X row %02X model %02X\n", name, i, RM[i], MRM[i]); break; }
        ok = false;
    }
    if (LogN != MLogN) { printf("FAIL %s: %d calls, model %d\n", name, LogN, MLogN); ok = false; }
    else for (int i = 0; i < LogN; ++i)
        if (memcmp(&Log[i], &MLog[i], sizeof(SEvent)) != 0) { printf("FAIL %s: call %d (kind %d) differs from the model (kind %d)\n", name, i, Log[i].kind, MLog[i].kind); ok = false; break; }
    for (int i = 0; i < (int)sizeof(Engine); ++i) {
        unsigned char want = i == 0x10 ? (wire ? 1 : 0) : i == 0x13 ? (textures ? 1 : 0) : (i >= 0x158 && i < 0x15C) ? Engine[i] : 0xCD;
        if (Engine[i] != want) { printf("FAIL %s: engine byte +0x%X written\n", name, i); ok = false; break; }
    }
    if (!ok) ++Fails;
    return ok;
}

int main()
{
    SetUnhandledExceptionFilter((void*)Crash);
    for (int t = 0; t < TB_COUNT; ++t) for (int i = 0; i < 16; ++i) Tables[t][i] = 0x10000000ul * (unsigned long)(t + 1) + 0x101ul * (unsigned long)(i + 1);
    g_RM = RM; g_SRM = (void*)0x5A000010; g_LM = (void*)0x5A000020; g_DM = (void*)0x5A000030;
    CollectStates();
    int k = 0;
    // Targeted: each path of the expanded setter, wireframe on / off, textures on / off.
    RunCase("all-differ", 1, 0, false, true, 4); ++k;
    RunCase("all-differ-wire", 2, 0, true, false, 16); ++k;
    RunCase("all-desired", 3, 1, false, false, 1); ++k;
    RunCase("all-desired-wire", 4, 1, true, true, 8); ++k;
    RunCase("mixed", 5, 2, false, true, 2); ++k;
    RunCase("mixed-wire", 6, 2, true, false, 0); ++k;
    // Spot checks against the source, independent of the model: the first loop's stage-7 alpha argument 2, stage 0's final alpha op
    // (MODULATE, set twice), stage 1's texture op (DISABLE), the fill mode after the wireframe override, TexturesEnabled, the stage-0
    // transform mode and an untouched stage-1 transform mode.
    RunCase("spot", 7, 0, true, true, 5); ++k;
    {
        bool ok = U32(RM, DEVICE + ALPHAARG2 + 0x14 * 7 + 4) == T(TB_TEXARG, 1) && U32(RM, DEVICE + ALPHAOP + 4) == T(TB_TEXOP, 3)
               && U32(RM, DEVICE + TEXOP + 0x14 + 4) == T(TB_TEXOP, 0) && U32(RM, DEVICE + POLYFILL + 4) == T(TB_POLYFILL, 1)
               && RM[TEXTURES_ENABLED] == 1 && U32(RM, DEVICE + TRANSFORM + 4) == T(TB_TEXTRANSFORM, 0)
               && U32(RM, DEVICE + TRANSFORM + 0x14 + 4) == U32(INIT, DEVICE + TRANSFORM + 0x14 + 4)
               && U32(RM, DEVICE + MAXANISO + 0x14 * 3 + 4) == 5 && U32(RM, DEVICE + FOGEND + 4) == Bits(1.0f);
        if (!ok) { printf("FAIL spot values\n"); ++Fails; }
    }
    for (int r = 0; r < 20000; ++r) {
        char name[32]; sprintf(name, "random %d", r);
        RunCase(name, 1000 + r, r % 7 == 0 ? r % 2 : 2, (r & 1) != 0, (r & 2) != 0, (long)(rand() % 17));
        if (Fails > 5) break;
    }
    if (Fails) { printf("00b24850_TEST FAIL (%d)\n", Fails); return 1; }
    printf("00b24850_TEST PASS (%d targeted, 20000 random)\n", k);
    return 0;
}