// Behaviour test for 0x00B65A20 CEngineSkyRenderer::RenderOuterSky (self-contained host model; plain C++98, no engine headers).
// Models: the theme channel's sky texture set (textures 0 / 1, blend 0) and colours 12 / 13, the early exit without a sky texture,
// the frame fetch (create + begin loading / begin loading while unloaded / touch + share), the second texture falling back to the
// first, the shared frame's reference counting across the two fetches, the shader constants and the additive blend states.
#include <cstdio>
#include <cmath>

struct Colour { float R, G, B, A; };
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
struct Channel { Colour Colours[19]; long Textures[2]; float Blends[2]; };

struct Output {
    bool Drawn; int Textures[2]; float PixelConstant5[4]; Colour Vertex0, Vertex1;
    int SourceBlend, DestBlend; bool BlendEnable;
};
enum { BLEND_ONE = 1 };

static void RenderOuterSky(const Channel& channel, Bank& bank, Output& out)
{
    out.Drawn = false;
    long texture0 = channel.Textures[0];
    long texture1 = channel.Textures[1];
    if (texture0 == 0)
        return;
    float blend = 1.0f - channel.Blends[0];
    Frame frame;
    if (!bank.PeekGraphicInfoFrame(texture0)->GetFrame(frame))
        return;
    int textures[2];
    textures[0] = frame.Data->Texture;
    if (texture1 != 0 && bank.PeekGraphicInfoFrame(texture1)->GetFrame(frame))
        textures[1] = frame.Data->Texture;
    else
        textures[1] = textures[0];
    for (int k = 0; k < 4; ++k) out.PixelConstant5[k] = blend;
    const Colour& sky_colour = channel.Colours[12];
    Colour colour0 = sky_colour;
    const Colour& horizon_colour = channel.Colours[13];
    Colour colour1 = horizon_colour;
    out.Vertex0 = colour0; out.Vertex1 = colour1;
    out.SourceBlend = BLEND_ONE; out.DestBlend = BLEND_ONE; out.BlendEnable = true;
    out.Textures[0] = textures[0]; out.Textures[1] = textures[1];
    out.Drawn = true;
}

static bool Near(float a, float b) { return std::fabs(a - b) < 1e-5f; }
static int Fail() { std::printf("00b65a20_TEST FAIL\n"); return 1; }

int main()
{
    static Channel channel;
    for (int c = 0; c < 19; ++c) { channel.Colours[c].R = c * 0.1f; channel.Colours[c].G = c * 0.2f; channel.Colours[c].B = c * 0.3f; channel.Colours[c].A = 1.0f; }
    channel.Blends[0] = 0.25f; channel.Blends[1] = 0.75f;
    Bank bank;
    Output out;
    // no sky texture: nothing happens, no frame fetched
    channel.Textures[0] = 0; channel.Textures[1] = 2;
    RenderOuterSky(channel, bank, out);
    if (out.Drawn || bank.Frames[0].Data || bank.Frames[2].Data) return Fail();
    // first texture not created yet: created, loading begun, nothing drawn
    channel.Textures[0] = 1;
    RenderOuterSky(channel, bank, out);
    if (out.Drawn || !bank.Frames[1].Data || bank.Frames[1].Data->BeginLoadingCalls != 1 || bank.Frames[2].Data) return Fail();
    // first loaded, second not created: second created, texture 1 falls back to texture 0
    FrameData* first = new FrameData(); first->ReferenceCount = 1; first->Texture = 11;
    bank.Frames[1].Data->Loaded = first;
    RenderOuterSky(channel, bank, out);
    if (!out.Drawn || out.Textures[0] != 11 || out.Textures[1] != 11 || !bank.Frames[2].Data || first->ReferenceCount != 1) return Fail();
    if (!Near(out.PixelConstant5[0], 0.75f) || !Near(out.PixelConstant5[3], 0.75f)) return Fail();
    if (!Near(out.Vertex0.G, 2.4f) || !Near(out.Vertex1.B, 3.9f) || out.SourceBlend != BLEND_ONE || out.DestBlend != BLEND_ONE || !out.BlendEnable) return Fail();
    // both loaded: two different textures; the shared frame drops the first reference before taking the second
    FrameData* second = new FrameData(); second->ReferenceCount = 1; second->Texture = 22;
    bank.Frames[2].Data->Loaded = second;
    RenderOuterSky(channel, bank, out);
    if (!out.Drawn || out.Textures[0] != 11 || out.Textures[1] != 22 || first->ReferenceCount != 1 || second->ReferenceCount != 1) return Fail();
    // no second texture: texture 1 = texture 0
    channel.Textures[1] = 0;
    RenderOuterSky(channel, bank, out);
    if (!out.Drawn || out.Textures[1] != 11) return Fail();
    delete first; delete second; delete bank.Frames[1].Data; delete bank.Frames[2].Data;
    std::printf("00b65a20_TEST PASS\n");
    return 0;
}