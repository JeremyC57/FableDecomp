// Behaviour test for 0x00B64FA0 CEngineSkyRenderer::RenderClouds(float) (self-contained host model; plain C++98, no engine headers).
// Models: the four cloud texture ids (texture sets 2 and 1, textures 0 / 1 each), the per-texture frame fetch (create + begin loading /
// begin loading while unloaded / touch + share the loaded frame) with the zero-RGBA texture and a missing count for each failure,
// the early exit when all four are missing, pixel constants 5 / 6 = (1 - blend) of sets 2 / 1 splatted, 7 = colour 7 with w 1, the
// wrap UV modes of stages 0..3, the blend states (ONE, INV_SRC_ALPHA, blending on, alpha test off), the cloud vector interpolation
// CloudVector[k] = (History[k][0] - History[k][1]) * t + History[k][1] uploaded as user vertex constant 0, and the draw.
#include <cstdio>
#include <cmath>

struct Colour { float R, G, B, A; };
struct Vec2 { float X, Y; };
struct FrameData { unsigned long ReferenceCount; int Texture; };
struct Frame {
    FrameData* Data;
    Frame() : Data(0) {}
    ~Frame() { if (Data) { if (--Data->ReferenceCount == 0) delete Data; Data = 0; } }
    void Assign(FrameData* data) { if (Data && --Data->ReferenceCount == 0) delete Data; Data = data; if (Data) ++Data->ReferenceCount; }
};
struct InfoFrameData { FrameData* Loaded; int BeginLoadingCalls; };
struct InfoFrame {
    InfoFrameData* Data;
    InfoFrame() : Data(0) {}
    bool GetFrame(Frame& frame)
    {
        if (Data) {
            if (Data->Loaded) { frame.Assign(Data->Loaded); return true; }
            ++Data->BeginLoadingCalls;
            return false;
        }
        Data = new InfoFrameData(); Data->Loaded = 0; Data->BeginLoadingCalls = 1;
        return false;
    }
};
struct Bank { InfoFrame Frames[8]; InfoFrame* PeekGraphicInfoFrame(long index) { return (index >= 0 && index < 8) ? &Frames[index] : 0; } };
struct TextureSet { long Textures[2]; float Blends[1]; };
struct Channel { Colour Colours[19]; TextureSet TextureSets[3]; };
enum { ZERO_RGBA_TEXTURE = -1, UV_WRAP = 1, BLEND_ONE = 2, BLEND_INV_SRC_ALPHA = 6 };

struct Output {
    bool Drawn; int Textures[4]; float PixelConstant5[4], PixelConstant6[4], PixelConstant7[4]; float VertexConstant0[4];
    int UVModeU[4], UVModeV[4]; int SourceBlend, DestBlend; bool BlendEnable, AlphaTestEnable;
};
struct Sky { Vec2 CloudVector[2]; Vec2 CloudVectorHistory[2][2]; };

static Vec2 Interpolate(const Vec2& a, const Vec2& b, float t)
{
    Vec2 d; d.X = a.X - b.X; d.Y = a.Y - b.Y;
    Vec2 r; r.X = d.X * t + b.X; r.Y = d.Y * t + b.Y;
    return r;
}

static void RenderClouds(Sky& sky, const Channel& channel, Bank& bank, Output& out, float interpolate)
{
    out.Drawn = false;
    long texture_ids[4];
    texture_ids[0] = channel.TextureSets[2].Textures[0];
    texture_ids[1] = channel.TextureSets[2].Textures[1];
    texture_ids[2] = channel.TextureSets[1].Textures[0];
    texture_ids[3] = channel.TextureSets[1].Textures[1];
    int textures[4];
    long num_missing = 0;
    for (long i = 0; i < 4; ++i) {
        Frame frame;
        if (texture_ids[i] != 0 && bank.PeekGraphicInfoFrame(texture_ids[i])->GetFrame(frame)) {
            textures[i] = frame.Data->Texture;
        } else {
            textures[i] = ZERO_RGBA_TEXTURE;
            ++num_missing;
        }
    }
    if (num_missing == 4)
        return;
    float blend0 = 1.0f - channel.TextureSets[2].Blends[0];
    float blend1 = 1.0f - channel.TextureSets[1].Blends[0];
    Colour colour = channel.Colours[7];
    for (int k = 0; k < 4; ++k) { out.PixelConstant5[k] = blend0; out.PixelConstant6[k] = blend1; }
    out.PixelConstant7[0] = colour.R; out.PixelConstant7[1] = colour.G; out.PixelConstant7[2] = colour.B; out.PixelConstant7[3] = 1.0f;
    for (int s = 0; s < 4; ++s) { out.UVModeU[s] = UV_WRAP; out.UVModeV[s] = UV_WRAP; }
    out.SourceBlend = BLEND_ONE; out.DestBlend = BLEND_INV_SRC_ALPHA; out.BlendEnable = true; out.AlphaTestEnable = false;
    sky.CloudVector[0] = Interpolate(sky.CloudVectorHistory[0][0], sky.CloudVectorHistory[0][1], interpolate);
    sky.CloudVector[1] = Interpolate(sky.CloudVectorHistory[1][0], sky.CloudVectorHistory[1][1], interpolate);
    out.VertexConstant0[0] = sky.CloudVector[0].X; out.VertexConstant0[1] = sky.CloudVector[0].Y;
    out.VertexConstant0[2] = sky.CloudVector[1].X; out.VertexConstant0[3] = sky.CloudVector[1].Y;
    for (int k = 0; k < 4; ++k) out.Textures[k] = textures[k];
    out.Drawn = true;
}

static bool Near(float a, float b) { return std::fabs(a - b) < 1e-5f; }
static int Fail() { std::printf("00b64fa0_TEST FAIL\n"); return 1; }

int main()
{
    static Channel channel;
    for (int c = 0; c < 19; ++c) { channel.Colours[c].R = c * 0.1f; channel.Colours[c].G = c * 0.2f; channel.Colours[c].B = c * 0.3f; channel.Colours[c].A = 0.5f; }
    channel.TextureSets[1].Blends[0] = 0.25f; channel.TextureSets[2].Blends[0] = 0.75f;
    Bank bank;
    Sky sky;
    sky.CloudVectorHistory[0][0].X = 2.0f; sky.CloudVectorHistory[0][0].Y = 4.0f; sky.CloudVectorHistory[0][1].X = 1.0f; sky.CloudVectorHistory[0][1].Y = 0.0f;
    sky.CloudVectorHistory[1][0].X = -2.0f; sky.CloudVectorHistory[1][0].Y = 6.0f; sky.CloudVectorHistory[1][1].X = 2.0f; sky.CloudVectorHistory[1][1].Y = 2.0f;
    sky.CloudVector[0].X = sky.CloudVector[0].Y = sky.CloudVector[1].X = sky.CloudVector[1].Y = 99.0f;
    Output out;
    // no cloud texture at all: four misses, nothing drawn, no frame created, cloud vectors untouched
    for (int s = 1; s < 3; ++s) channel.TextureSets[s].Textures[0] = channel.TextureSets[s].Textures[1] = 0;
    RenderClouds(sky, channel, bank, out, 0.5f);
    if (out.Drawn || bank.Frames[1].Data || !Near(sky.CloudVector[0].X, 99.0f)) return Fail();
    // set 2 textures 1 / 2 not created yet, set 1 empty: created and loading begun, still four misses
    channel.TextureSets[2].Textures[0] = 1; channel.TextureSets[2].Textures[1] = 2;
    RenderClouds(sky, channel, bank, out, 0.5f);
    if (out.Drawn || !bank.Frames[1].Data || !bank.Frames[2].Data || bank.Frames[1].Data->BeginLoadingCalls != 1) return Fail();
    // texture 1 loaded: drawn with texture 1 and the zero texture for the other three
    FrameData* first = new FrameData(); first->ReferenceCount = 1; first->Texture = 11;
    bank.Frames[1].Data->Loaded = first;
    RenderClouds(sky, channel, bank, out, 0.5f);
    if (!out.Drawn || out.Textures[0] != 11 || out.Textures[1] != ZERO_RGBA_TEXTURE || out.Textures[2] != ZERO_RGBA_TEXTURE
        || out.Textures[3] != ZERO_RGBA_TEXTURE || bank.Frames[2].Data->BeginLoadingCalls != 2 || first->ReferenceCount != 1) return Fail();
    if (!Near(out.PixelConstant5[0], 0.25f) || !Near(out.PixelConstant5[3], 0.25f) || !Near(out.PixelConstant6[2], 0.75f)) return Fail();
    if (!Near(out.PixelConstant7[0], 0.7f) || !Near(out.PixelConstant7[2], 2.1f) || !Near(out.PixelConstant7[3], 1.0f)) return Fail();
    if (out.UVModeU[3] != UV_WRAP || out.UVModeV[0] != UV_WRAP || out.SourceBlend != BLEND_ONE || out.DestBlend != BLEND_INV_SRC_ALPHA
        || !out.BlendEnable || out.AlphaTestEnable) return Fail();
    // CloudVector[0] = ((2,4) - (1,0)) * 0.5 + (1,0) = (1.5, 2); CloudVector[1] = ((-2,6) - (2,2)) * 0.5 + (2,2) = (0, 4)
    if (!Near(sky.CloudVector[0].X, 1.5f) || !Near(sky.CloudVector[0].Y, 2.0f) || !Near(sky.CloudVector[1].X, 0.0f) || !Near(sky.CloudVector[1].Y, 4.0f)) return Fail();
    if (!Near(out.VertexConstant0[0], 1.5f) || !Near(out.VertexConstant0[3], 4.0f)) return Fail();
    // all four loaded (set 1 uses the same two graphics): four textures, references back to one each
    FrameData* second = new FrameData(); second->ReferenceCount = 1; second->Texture = 22;
    bank.Frames[2].Data->Loaded = second;
    channel.TextureSets[1].Textures[0] = 2; channel.TextureSets[1].Textures[1] = 1;
    RenderClouds(sky, channel, bank, out, 1.0f);
    if (!out.Drawn || out.Textures[0] != 11 || out.Textures[1] != 22 || out.Textures[2] != 22 || out.Textures[3] != 11
        || first->ReferenceCount != 1 || second->ReferenceCount != 1) return Fail();
    if (!Near(sky.CloudVector[0].X, 2.0f) || !Near(sky.CloudVector[1].Y, 6.0f)) return Fail();
    delete first; delete second; delete bank.Frames[1].Data; delete bank.Frames[2].Data;
    std::printf("00b64fa0_TEST PASS\n");
    return 0;
}