// Behaviour test for 0x00B64550 CEngineSkyRenderer::RenderSunFlare() (self-contained host model; plain C++98, no engine headers).
// Models: the capture block (captured on entry, restored on every exit), the occlusion visibility (negative -> nothing, above 1 -> 1,
// below 1/255 -> nothing), the additive blend / z-buffer states, the sun position 2000 units along the sun vector from the camera taken
// through the 3x4 view transform (X row E21*y+E31*z+E11*x, Y / Z rows left to right), the behind-the-camera test (z <= 0.01 -> no
// sprites), the view-to-screen projection, the lens-flare elements (texture 0 ends the list, an unloaded texture is requested and
// skipped, a loaded one is drawn at (screen - centre) * Position + centre with half size GFVXToAX(Radius) and colour element colour *
// theme colour 11 * visibility * clamp(|screen - centre| / |centre|, 0, 1)) and the final sun sprite (theme colour 10 * visibility;
// unloaded -> requested, nothing drawn).
#include <cstdio>
#include <cmath>
#include <cstring>

struct Colour { float R, G, B, A; };
struct ByteColour { unsigned char B, G, R, A; };
struct Vec3 { float X, Y, Z; };
struct Matrix { float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43; };
struct Camera {
    Vec3 Position; Matrix View; float HalfWidth, HalfHeight, CentreX, CentreY;
    void WorldToView(Vec3& p) const
    {
        const float xx = p.X, yy = p.Y, zz = p.Z;
        p.X = View.E11 * xx + (View.E21 * yy + View.E31 * zz) + View.E41;
        p.Y = View.E12 * xx + View.E22 * yy + View.E32 * zz + View.E42;
        p.Z = View.E13 * xx + View.E23 * yy + View.E33 * zz + View.E43;
    }
    void ViewToScreen(Vec3& p) const
    {
        const float one_over_z = 1.0f / p.Z;
        p.X = CentreX + one_over_z * HalfWidth * p.X;
        p.Y = CentreY - one_over_z * HalfHeight * p.Y;
    }
};
// GFVXToAX / GFVYToAY: virtual (0..65536) to absolute screen coordinates of a 640 x 480 display.
static float VXToAX(float x) { return x * 640.0f / 65536.0f; }
static float VYToAY(float y) { return y * 480.0f / 65536.0f; }

struct Texture { bool Loaded; int BeginLoadingCalls; };
struct Element { long Texture; float Radius; float Position; ByteColour Colour; };
struct SkyDef { long SunFlareRadius; long SunFlareTexture; Element Elements[4]; long ElementCount; };
struct Sprite { int Texture; float X, Y, Width, Height; ByteColour Colour; };
struct State {
    int Captures, Restores; int SourceBlend, DestBlend; bool BlendEnable, ZEnable;
    Sprite Sprites[8]; int SpriteCount;
};
enum { BLEND_ONE = 2, BLEND_INV_SRC_COLOR = 4 };

static unsigned char ToByte(float c) { return (unsigned char)(long)(c * 255.0f); }
static void Draw(State& st, int texture, float x, float y, float w, float h, const Colour& c)
{
    Sprite& s = st.Sprites[st.SpriteCount++];
    s.Texture = texture; s.X = x; s.Y = y; s.Width = w; s.Height = h;
    s.Colour.R = ToByte(c.R); s.Colour.G = ToByte(c.G); s.Colour.B = ToByte(c.B); s.Colour.A = ToByte(c.A);
}
static float Length(float x, float y) { return (float)std::sqrt(y * y + x * x); }

struct Scope { State& St; Scope(State& st) : St(st) { ++St.Captures; } ~Scope() { ++St.Restores; } };

static void RenderSunFlare(State& st, const SkyDef& def, Texture* textures, const Camera& camera, const Vec3& sun,
    float occlusion, const Colour* theme)
{
    Scope capture(st);
    float visibility = occlusion;
    if (visibility < 0.0f)
        return;
    if (visibility > 1.0f)
        visibility = 1.0f;
    else if (visibility < 1.0f / 255.0f)
        return;
    st.SourceBlend = BLEND_ONE; st.DestBlend = BLEND_INV_SRC_COLOR; st.BlendEnable = true; st.ZEnable = false;
    Vec3 screen;
    screen.X = sun.X * 2000.0f + camera.Position.X;
    screen.Y = sun.Y * 2000.0f + camera.Position.Y;
    screen.Z = sun.Z * 2000.0f + camera.Position.Z;
    camera.WorldToView(screen);
    if (screen.Z > 0.01) {
        camera.ViewToScreen(screen);
        float cx = VXToAX(32768.0f), cy = VYToAY(32768.0f);
        for (long i = 0; i < def.ElementCount; ++i) {
            long id = def.Elements[i].Texture;
            if (id == 0)
                break;
            if (!textures[id].Loaded) {
                ++textures[id].BeginLoadingCalls;
            } else {
                long size = (long)VXToAX(def.Elements[i].Radius);
                float ratio = Length(screen.X - cx, screen.Y - cy) / Length(cx, cy);
                ratio = ratio < 0.0f ? 0.0f : (ratio > 1.0f ? 1.0f : ratio);
                const ByteColour& b = def.Elements[i].Colour;
                const Colour& t = theme[11];
                Colour c;
                c.R = b.R / 255.0f * t.R * visibility * ratio; c.G = b.G / 255.0f * t.G * visibility * ratio;
                c.B = b.B / 255.0f * t.B * visibility * ratio; c.A = b.A / 255.0f * t.A * visibility * ratio;
                float position = def.Elements[i].Position;
                float x = (screen.X - cx) * position + cx;
                float y = (screen.Y - cy) * position + cy;
                Draw(st, (int)id, x - size, y - size, (float)(size * 2), (float)(size * 2), c);
            }
        }
        long sun_texture = def.SunFlareTexture;
        if (sun_texture != 0) {
            if (!textures[sun_texture].Loaded) {
                ++textures[sun_texture].BeginLoadingCalls;
                return;
            }
            long size = (long)VXToAX((float)def.SunFlareRadius);
            Colour c;
            c.R = theme[10].R * visibility; c.G = theme[10].G * visibility; c.B = theme[10].B * visibility; c.A = theme[10].A * visibility;
            Draw(st, (int)sun_texture, screen.X - size, screen.Y - size, (float)(size * 2), (float)(size * 2), c);
        }
    }
}

static bool Near(float a, float b) { return std::fabs(a - b) < 1e-3f; }
static int Fail(int line) { std::printf("00b64550_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(x) if (!(x)) return Fail(__LINE__)

int main()
{
    Camera camera;
    camera.Position.X = 10.0f; camera.Position.Y = 0.0f; camera.Position.Z = -5.0f;
    // identity view transform
    Matrix m = { 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0 };
    camera.View = m;
    camera.HalfWidth = 320.0f; camera.HalfHeight = 240.0f; camera.CentreX = 320.0f; camera.CentreY = 240.0f;
    Colour theme[19];
    for (int k = 0; k < 19; ++k) { theme[k].R = theme[k].G = theme[k].B = theme[k].A = 1.0f; }
    theme[10].R = 0.5f; theme[10].G = 0.25f; theme[10].B = 1.0f; theme[10].A = 1.0f;
    Texture textures[8];
    for (int k = 0; k < 8; ++k) { textures[k].Loaded = true; textures[k].BeginLoadingCalls = 0; }
    SkyDef def;
    def.SunFlareRadius = 4096; def.SunFlareTexture = 5; def.ElementCount = 3;
    ByteColour white = { 255, 255, 255, 255 };
    def.Elements[0].Texture = 1; def.Elements[0].Radius = 2048.0f; def.Elements[0].Position = 1.0f; def.Elements[0].Colour = white;
    def.Elements[1].Texture = 2; def.Elements[1].Radius = 1024.0f; def.Elements[1].Position = -0.5f; def.Elements[1].Colour = white;
    def.Elements[2].Texture = 0; def.Elements[2].Radius = 512.0f; def.Elements[2].Position = 0.0f; def.Elements[2].Colour = white;
    // sun straight ahead along +Z, shifted right: view = (10, 0, 1995) -> screen x = 320 + 320 * 10 / 1995
    Vec3 sun = { 0.0f, 0.0f, 1.0f };
    State st;
    std::memset(&st, 0, sizeof(st));
    // occluded (negative), and nearly invisible: captured and restored, nothing set or drawn
    RenderSunFlare(st, def, textures, camera, sun, -1.0f, theme);
    CHECK(st.Captures == 1 && st.Restores == 1 && !st.BlendEnable && st.SpriteCount == 0);
    RenderSunFlare(st, def, textures, camera, sun, 0.001f, theme);
    CHECK(st.Captures == 2 && st.Restores == 2 && !st.BlendEnable && st.SpriteCount == 0);
    // visible (clamped to 1): two flare sprites (the third element has no texture and ends the list) and the sun sprite
    RenderSunFlare(st, def, textures, camera, sun, 3.0f, theme);
    CHECK(st.Restores == 3 && st.SourceBlend == BLEND_ONE && st.DestBlend == BLEND_INV_SRC_COLOR && st.BlendEnable && !st.ZEnable);
    CHECK(st.SpriteCount == 3);
    const float sx = 320.0f + 320.0f * 10.0f / 1995.0f, sy = 240.0f;
    // centre = (320, 240); ratio = |(sx - 320, 0)| / 400
    const float ratio = (sx - 320.0f) / 400.0f;
    // element 0: half size (long)VXToAX(2048) = 20, position 1 -> at the sun
    CHECK(st.Sprites[0].Texture == 1 && Near(st.Sprites[0].X, sx - 20.0f) && Near(st.Sprites[0].Y, sy - 20.0f));
    CHECK(Near(st.Sprites[0].Width, 40.0f) && Near(st.Sprites[0].Height, 40.0f));
    CHECK(st.Sprites[0].Colour.R == ToByte(ratio) && st.Sprites[0].Colour.A == ToByte(ratio));
    // element 1: half size 10, position -0.5 -> mirrored through the centre
    CHECK(st.Sprites[1].Texture == 2 && Near(st.Sprites[1].X, 320.0f - (sx - 320.0f) * 0.5f - 10.0f) && Near(st.Sprites[1].Width, 20.0f));
    // sun sprite: half size (long)VXToAX(4096) = 40, colour theme 10 * 1
    CHECK(st.Sprites[2].Texture == 5 && Near(st.Sprites[2].X, sx - 40.0f) && Near(st.Sprites[2].Width, 80.0f));
    CHECK(st.Sprites[2].Colour.R == ToByte(0.5f) && st.Sprites[2].Colour.G == ToByte(0.25f) && st.Sprites[2].Colour.B == 255);
    // unloaded flare texture: requested and skipped; unloaded sun texture: requested, nothing more drawn, still restored
    textures[1].Loaded = false; textures[5].Loaded = false;
    st.SpriteCount = 0;
    RenderSunFlare(st, def, textures, camera, sun, 0.5f, theme);
    CHECK(st.Restores == 4 && st.SpriteCount == 1 && st.Sprites[0].Texture == 2);
    CHECK(textures[1].BeginLoadingCalls == 1 && textures[5].BeginLoadingCalls == 1 && textures[2].BeginLoadingCalls == 0);
    CHECK(st.Sprites[0].Colour.G == ToByte(0.5f * ratio));
    // sun behind the camera: states set, no sprites, restored
    Vec3 back = { 0.0f, 0.0f, -1.0f };
    st.SpriteCount = 0;
    RenderSunFlare(st, def, textures, camera, back, 1.0f, theme);
    CHECK(st.Restores == 5 && st.SpriteCount == 0 && textures[1].BeginLoadingCalls == 1);
    std::printf("00b64550_TEST PASS\n");
    return 0;
}
