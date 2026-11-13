// Behaviour test for 0x00B891B0 CEngineScreenEffectOutlineGlow::CEngineScreenEffectOutlineGlow(const CEngineInit&): a host model of
// the constructor body (same statement order, same arguments) over a mock scratch texture manager and mock shader resources with
// reference-counted shader data (the assignment releases the old data and shares the new; the by-value temporaries release theirs).
#include <cstdio>
#include <cstring>

struct ShaderData {
    char Name[48]; long ReferenceCount;
};
static ShaderData g_shaders[16]; static int g_shaderCount = 0; static int g_deleted = 0;
static ShaderData* LookUp(const char* name)
{
    for (int i = 0; i < g_shaderCount; ++i)
        if (std::strcmp(g_shaders[i].Name, name) == 0) { ++g_shaders[i].ReferenceCount; return &g_shaders[i]; }
    ShaderData* d = &g_shaders[g_shaderCount++];
    std::strcpy(d->Name, name); d->ReferenceCount = 1;
    return d;
}
struct Shader {
    ShaderData* Data;
    Shader() : Data(0) { }
    Shader(ShaderData* d) : Data(d) { }
    Shader(const Shader& o) : Data(o.Data) { if (Data) ++Data->ReferenceCount; }
    ~Shader() { if (Data && --Data->ReferenceCount == 0) ++g_deleted; }
    void operator=(const Shader& other)
    {
        ShaderData* data = Data;
        if (data) { if (--data->ReferenceCount == 0) ++g_deleted; Data = 0; }
        Data = other.Data;
        ++Data->ReferenceCount;
    }
};
static Shader GetShader(const char* name) { return Shader(LookUp(name)); }

struct PixelFormat { long FormatIndex; PixelFormat() : FormatIndex(-1) { } void Initialise(long f) { FormatIndex = f; } };
struct Extents { long X, Y; Extents(long x, long y) : X(x), Y(y) { } };
struct Alloc { long page; long w, h, mips; long format; unsigned long usage; bool compressed; };
struct Scratch {
    long pages; Alloc allocs[8]; long count; long lastPageSize; bool lastRenderTarget;
    Scratch() : pages(0), count(0), lastPageSize(-5), lastRenderTarget(false) { }
    long AllocteMemoryPage(long size, bool rt) { lastPageSize = size; lastRenderTarget = rt; return 40 + ++pages; }
    long AllocateTexture(long page, const Extents& e, long mips, const PixelFormat& f, unsigned long usage, bool compressed)
    {
        Alloc a = { page, e.X, e.Y, mips, f.FormatIndex, usage, compressed }; allocs[count] = a; return 1000 + count++;
    }
};
static Scratch g_scratch;

struct Vertex { float X, Y, Z; unsigned long DiffuseColour; float U1, V1; };
struct Colour { unsigned char B, G, R, A; void Initialise(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; } };
struct Init { long InitFlags; };

struct OutlineGlow {
    void* OutlineTexture; void* Blur1Texture; void* Blur2Texture;
    long OutlineTextureHandle, Blur1TextureHandle, Blur2TextureHandle, DynamicTexturePageHandle;
    Vertex OutlineToBlurVerts[4], BlurToBlurVerts[4], BlurToBackBufferVerts[4];
    Shader OutlineToBlurVertexShader, OutlineToBlurPixelShader, BlurToBlurVertexShader, BlurToBlurPixelShader,
           BlurToBackBufferVertexShader, BlurToBackBufferPixelShader;
    float BlurFactor, SharpenFactor, OuterStencilExtraNormalExtrusion, OutlineGlowScaleFactor;
    bool ParametersOverridden; float OverriddenParamsWidthWorldSpace, OverriddenParamsWidthScreenSpace; Colour OverriddenParamsColour;
    long OverriddenParamsTextureAlphaRef, OverriddenParamsTextureAlphaBias, BlurStepCount; float DepthBufferBias;
    bool CaptureOpen; bool EnableDrawDebugTextures;

    static void Quad(Vertex* v, float x0, float y0, float x1, float y1, float u0, float u1)
    {
        v[0].X = x0; v[0].Y = y0; v[0].Z = 0.0f; v[1].X = x1; v[1].Y = y0; v[1].Z = 0.0f;
        v[2].X = x1; v[2].Y = y1; v[2].Z = 0.0f; v[3].X = x0; v[3].Y = y1; v[3].Z = 0.0f;
        v[0].U1 = u0; v[0].V1 = u1; v[1].U1 = u1; v[1].V1 = u1; v[2].U1 = u1; v[2].V1 = u0; v[3].U1 = u0; v[3].V1 = u0;
    }
    OutlineGlow(const Init& init) : CaptureOpen(false)
    {
        OutlineGlowScaleFactor = 0.333f;
        PixelFormat format;
        format.Initialise(21);
        Scratch& scratch = g_scratch;
        DynamicTexturePageHandle = scratch.AllocteMemoryPage(0, true);
        OutlineTextureHandle = scratch.AllocateTexture(DynamicTexturePageHandle, Extents(256, 256), 1, format, 1, false);
        OutlineTexture = 0;
        Blur1TextureHandle = scratch.AllocateTexture(DynamicTexturePageHandle, Extents(128, 128), 1, format, 1, false);
        Blur2TextureHandle = scratch.AllocateTexture(DynamicTexturePageHandle, Extents(128, 128), 1, format, 1, false);
        Blur1Texture = 0;
        Blur2Texture = 0;
        Quad(OutlineToBlurVerts, -1.0f, -1.0f, 1.0f, 1.0f, 0.001953125f, 1.001953125f);
        Quad(BlurToBlurVerts, -1.0f, -1.0f, 1.0f, 1.0f, 0.00390625f, 1.00390625f);
        Quad(BlurToBackBufferVerts, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f);
        for (long i = 0; i < 4; ++i) {
            OutlineToBlurVerts[i].DiffuseColour = 0xffffffff;
            BlurToBlurVerts[i].DiffuseColour = 0xffffffff;
            BlurToBackBufferVerts[i].DiffuseColour = 0xffffffff;
        }
        OutlineToBlurVertexShader = GetShader("VSHADER_SCREEN_FX_GLOW_BLUR");
        BlurToBlurVertexShader = GetShader("VSHADER_SCREEN_FX_GLOW_BLUR");
        if (init.InitFlags & 0x40) {
            OutlineToBlurPixelShader = GetShader("PSHADER_TEXTURE_COPY_OUTLINE_SIMPLE");
            BlurToBlurPixelShader = GetShader("PSHADER_TEXTURE_COPY_OUTLINE_SIMPLE");
        } else {
            OutlineToBlurPixelShader = GetShader("PSHADER_TEXTURE_COPY_OUTLINE");
            BlurToBlurPixelShader = GetShader("PSHADER_TEXTURE_COPY_OUTLINE");
        }
        BlurToBackBufferVertexShader = GetShader("VSHADER_SCREEN_FX_COPY");
        BlurToBackBufferPixelShader = GetShader("PSHADER_TEXTURE_APPLY_OUTLINE");
        ParametersOverridden = false;
        OverriddenParamsWidthWorldSpace = 0.0f;
        OverriddenParamsWidthScreenSpace = 0.0f;
        OverriddenParamsColour.Initialise(255, 255, 255, 255);
        OverriddenParamsTextureAlphaRef = 0x80;
        OverriddenParamsTextureAlphaBias = 0;
        EnableDrawDebugTextures = false;
        OuterStencilExtraNormalExtrusion = 4.0f;
        BlurStepCount = 1;
        DepthBufferBias = -0.002f;
    }
};

static bool Near(float a, float b) { float d = a - b; return d < 1e-6f && d > -1e-6f; }
static bool CheckQuad(const Vertex* v, float u0, float u1)
{
    return Near(v[0].X, -1.0f) && Near(v[0].Y, -1.0f) && Near(v[1].X, 1.0f) && Near(v[1].Y, -1.0f) && Near(v[2].X, 1.0f) && Near(v[2].Y, 1.0f)
        && Near(v[3].X, -1.0f) && Near(v[3].Y, 1.0f) && Near(v[0].Z, 0.0f) && Near(v[3].Z, 0.0f)
        && Near(v[0].U1, u0) && Near(v[0].V1, u1) && Near(v[1].U1, u1) && Near(v[1].V1, u1) && Near(v[2].U1, u1) && Near(v[2].V1, u0)
        && Near(v[3].U1, u0) && Near(v[3].V1, u0)
        && v[0].DiffuseColour == 0xffffffffUL && v[1].DiffuseColour == 0xffffffffUL && v[2].DiffuseColour == 0xffffffffUL && v[3].DiffuseColour == 0xffffffffUL;
}
static const char* Name(const Shader& s) { return s.Data ? s.Data->Name : ""; }

int main()
{
    bool ok = true;
    Init plain = { 0x80 };
    OutlineGlow* a = new OutlineGlow(plain);
    ok = ok && g_scratch.pages == 1 && g_scratch.count == 3 && g_scratch.lastPageSize == 0 && g_scratch.lastRenderTarget;
    ok = ok && a->DynamicTexturePageHandle == 41 && a->OutlineTextureHandle == 1000 && a->Blur1TextureHandle == 1001 && a->Blur2TextureHandle == 1002;
    ok = ok && g_scratch.allocs[0].page == 41 && g_scratch.allocs[0].w == 256 && g_scratch.allocs[0].h == 256 && g_scratch.allocs[0].mips == 1
            && g_scratch.allocs[0].format == 21 && g_scratch.allocs[0].usage == 1 && !g_scratch.allocs[0].compressed;
    ok = ok && g_scratch.allocs[1].w == 128 && g_scratch.allocs[2].h == 128 && g_scratch.allocs[2].page == 41;
    ok = ok && a->OutlineTexture == 0 && a->Blur1Texture == 0 && a->Blur2Texture == 0;
    ok = ok && CheckQuad(a->OutlineToBlurVerts, 0.001953125f, 1.001953125f) && CheckQuad(a->BlurToBlurVerts, 0.00390625f, 1.00390625f)
            && CheckQuad(a->BlurToBackBufferVerts, 0.0f, 1.0f);
    ok = ok && std::strcmp(Name(a->OutlineToBlurVertexShader), "VSHADER_SCREEN_FX_GLOW_BLUR") == 0
            && a->OutlineToBlurVertexShader.Data == a->BlurToBlurVertexShader.Data && a->BlurToBlurVertexShader.Data->ReferenceCount == 2;
    ok = ok && std::strcmp(Name(a->OutlineToBlurPixelShader), "PSHADER_TEXTURE_COPY_OUTLINE") == 0
            && std::strcmp(Name(a->BlurToBlurPixelShader), "PSHADER_TEXTURE_COPY_OUTLINE") == 0
            && std::strcmp(Name(a->BlurToBackBufferVertexShader), "VSHADER_SCREEN_FX_COPY") == 0
            && std::strcmp(Name(a->BlurToBackBufferPixelShader), "PSHADER_TEXTURE_APPLY_OUTLINE") == 0
            && a->BlurToBackBufferPixelShader.Data->ReferenceCount == 1 && g_deleted == 0;
    ok = ok && Near(a->OutlineGlowScaleFactor, 0.333f) && Near(a->OuterStencilExtraNormalExtrusion, 4.0f) && a->BlurStepCount == 1
            && Near(a->DepthBufferBias, -0.002f) && !a->ParametersOverridden && Near(a->OverriddenParamsWidthWorldSpace, 0.0f)
            && a->OverriddenParamsColour.R == 255 && a->OverriddenParamsColour.A == 255 && a->OverriddenParamsTextureAlphaRef == 0x80
            && a->OverriddenParamsTextureAlphaBias == 0 && !a->EnableDrawDebugTextures && !a->CaptureOpen;
    Init simple = { 0x40 };
    OutlineGlow* b = new OutlineGlow(simple);
    ok = ok && std::strcmp(Name(b->OutlineToBlurPixelShader), "PSHADER_TEXTURE_COPY_OUTLINE_SIMPLE") == 0
            && b->OutlineToBlurPixelShader.Data == b->BlurToBlurPixelShader.Data && b->BlurToBlurPixelShader.Data->ReferenceCount == 2
            && a->OutlineToBlurVertexShader.Data->ReferenceCount == 4 && g_scratch.pages == 2 && b->DynamicTexturePageHandle == 42;
    delete b; delete a;
    ok = ok && g_deleted == 5;
    if (!ok) { std::printf("00b891b0_TEST FAIL\n"); return 1; }
    std::printf("00b891b0_TEST PASS\n");
    return 0;
}