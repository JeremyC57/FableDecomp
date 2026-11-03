// Behaviour test for CUserProfileManager::RenderUIBackground (0x004075C0): a standalone model of the filter maths -- every term
// fades from its unfiltered value by alpha, the tint is TintX * tintScale + diffuse.X + ambient.X (diffuse / ambient scaled by
// the def), lifted by the minimum when any channel is below it, alpha channel 1; nothing is drawn without a captured texture.
#include <cstdio>
#include <cmath>

struct V4 { float X, Y, Z, W; };
struct Def { float Saturation, Contrast, Brightness, TintR, TintG, TintB, TintScale, DiffuseScale, AmbientScale, Minimum; };
struct Out { bool drawn; float saturation, contrast, brightness; V4 colour; };

static Out Render(const Def& d, V4 diffuse, V4 ambient, float alpha, bool haveTexture)
{
    static const V4 noFilter = { 1.0f, 1.0f, 1.0f, 1.0f };
    const float s0 = 0.0f, c0 = 0.0f, b0 = 0.0f;
    Out o; o.drawn = false;
    o.saturation = (d.Saturation - s0) * alpha + s0;
    o.contrast = (d.Contrast - c0) * alpha + c0;
    diffuse.X *= d.DiffuseScale; diffuse.Y *= d.DiffuseScale; diffuse.Z *= d.DiffuseScale; diffuse.W *= d.DiffuseScale;
    ambient.X *= d.AmbientScale; ambient.Y *= d.AmbientScale; ambient.Z *= d.AmbientScale; ambient.W *= d.AmbientScale;
    V4 c = { d.TintR * d.TintScale + diffuse.X + ambient.X, d.TintG * d.TintScale + diffuse.Y + ambient.Y,
             d.TintB * d.TintScale + diffuse.Z + ambient.Z, 1.0f };
    if (c.X < d.Minimum || c.Y < d.Minimum || c.Z < d.Minimum) { c.X += d.Minimum; c.Y += d.Minimum; c.Z += d.Minimum; }
    o.colour.X = (c.X - noFilter.X) * alpha + noFilter.X; o.colour.Y = (c.Y - noFilter.Y) * alpha + noFilter.Y;
    o.colour.Z = (c.Z - noFilter.Z) * alpha + noFilter.Z; o.colour.W = (c.W - noFilter.W) * alpha + noFilter.W;
    o.brightness = (d.Brightness - b0) * alpha + b0;
    o.drawn = haveTexture;
    return o;
}

static bool Near(float a, float b) { return std::fabs(a - b) < 1e-5f; }

int main()
{
    Def d = { 0.5f, 1.5f, 0.25f, 0.2f, 0.4f, 0.6f, 0.5f, 1.0f, 1.0f, 0.3f };
    V4 diffuse = { 0.1f, 0.1f, 0.1f, 1.0f }, ambient = { 0.0f, 0.0f, 0.0f, 0.0f };
    Out full = Render(d, diffuse, ambient, 1.0f, true);
    // tint (0.1, 0.2, 0.3) + diffuse 0.1 = (0.2, 0.3, 0.4); R < 0.3 so every channel is lifted by 0.3
    if (!full.drawn || !Near(full.saturation, 0.5f) || !Near(full.contrast, 1.5f) || !Near(full.brightness, 0.25f) ||
        !Near(full.colour.X, 0.5f) || !Near(full.colour.Y, 0.6f) || !Near(full.colour.Z, 0.7f) || !Near(full.colour.W, 1.0f)) {
        std::printf("004075c0_TEST FAIL\n"); return 1;
    }
    Out none = Render(d, diffuse, ambient, 0.0f, false);   // alpha 0: the unfiltered values, nothing drawn without a texture
    if (none.drawn || !Near(none.saturation, 0.0f) || !Near(none.colour.X, 1.0f) || !Near(none.colour.W, 1.0f)) {
        std::printf("004075c0_TEST FAIL\n"); return 1;
    }
    std::printf("004075c0_TEST PASS\n");
    return 0;
}