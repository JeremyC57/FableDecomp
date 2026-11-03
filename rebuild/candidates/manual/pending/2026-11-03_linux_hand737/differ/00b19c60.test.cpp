// Behaviour test for NParticleEngine::CPSCLight::PreparePrimitives (0x00B19C60): a standalone model -- an enabled, alive light sets
// one primitive whose colour blends start -> mid -> end over the life fraction (clamped to [0,1]), whose alpha-driven fades scale the
// colour towards the fade minimum and the radius towards the radius-fade minimum, and whose radius / spot radius lerp from the start
// to the end radius (the spot radius scaled by (attenuation + 1) / 1000); a disabled or dead light clears its primitive.
#include <cstdio>
#include <cmath>
struct Col { float r, g, b, a; };
static Col Lerp(const Col& x, const Col& y, float t) { Col c = { x.r*(1-t)+y.r*t, x.g*(1-t)+y.g*t, x.b*(1-t)+y.b*t, x.a*(1-t)+y.a*t }; return c; }
struct Prim { bool set; Col colour; float radius, spot; };
static void Prepare(bool enabled, bool alive, float life, float alpha, bool colourFade, Col fadeMin, bool radiusFade, float radiusMin,
                    float startR, float endR, long atten, Prim& out)
{
    if (!(enabled && alive)) { out.set = false; return; }
    Col s = { 1, 0, 0, 1 }, e = { 0, 0, 1, 1 }, m = { 0, 1, 0, 1 };
    float f = life < 0 ? 0 : life > 1 ? 1 : life;
    Col c = f < 0.5f ? Lerp(s, m, f * 2) : Lerp(m, e, (f - 0.5f) * 2);
    float scale = 1;
    if (alpha < 1) {
        if (colourFade) {
            c.r *= (1 - fadeMin.r) * alpha + fadeMin.r; c.g *= (1 - fadeMin.g) * alpha + fadeMin.g; c.b *= (1 - fadeMin.b) * alpha + fadeMin.b;
            if (!(c.r > fadeMin.r)) c.r = fadeMin.r; if (!(c.g > fadeMin.g)) c.g = fadeMin.g; if (!(c.b > fadeMin.b)) c.b = fadeMin.b;
        }
        if (radiusFade) scale = (1 - radiusMin) * alpha + radiusMin;
    }
    float k = (atten + 1) * 0.001f;
    out.set = true; out.colour = c;
    out.radius = ((endR - startR) * f + startR) * scale;
    out.spot = ((k * endR - k * startR) * f + k * startR) * scale;
}
static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    Col none = { 0, 0, 0, 0 }, half = { 0.5f, 0.5f, 0.5f, 1 };
    Prim p; Prepare(true, true, 0.25f, 1, false, none, false, 0, 10, 20, 999, p);
    bool ok = p.set && near(p.colour.r, 0.5f) && near(p.colour.g, 0.5f) && near(p.radius, 12.5f) && near(p.spot, 12.5f);
    Prepare(true, true, 1.5f, 1, false, none, false, 0, 10, 20, 999, p);
    ok = ok && near(p.colour.b, 1) && near(p.radius, 20);
    Prepare(true, true, 0, 0.5f, true, half, true, 0.5f, 10, 20, 0, p);
    ok = ok && near(p.colour.r, 0.75f) && near(p.colour.g, 0.5f) && near(p.radius, 7.5f) && near(p.spot, 0.0075f);
    Prepare(false, true, 0.5f, 1, false, none, false, 0, 10, 20, 0, p); ok = ok && !p.set;
    Prepare(true, false, 0.5f, 1, false, none, false, 0, 10, 20, 0, p); ok = ok && !p.set;
    printf(ok ? "00b19c60_TEST PASS\n" : "00b19c60_TEST FAIL\n");
    return ok ? 0 : 1;
}
