// Behaviour test for NUISystem::CComponent::UpdateColour (0x0052F900): the colour eases from the initial to the target colour
// (constant deceleration: v0 = 2d/T, a = -2d/T^2, so the colour lands on the target at T), is clamped to [0,1] and truncated to
// bytes, snaps to the target (and the initial colour follows) once the elapsed time reaches ColourTime; the render colour is
// the own colour times the parent colour unless the component is independent. Standalone model of the same arithmetic.
#include <cstdio>

struct Col { unsigned char B, G, R, A; };
struct FCol { float R, G, B, A; };

static FCol ToF(Col c) { FCol f = { c.R * (1.0f / 255.0f), c.G * (1.0f / 255.0f), c.B * (1.0f / 255.0f), c.A * (1.0f / 255.0f) }; return f; }
static float Cl(float x) { if (x > 1.0f) return 1.0f; if (x < 0.0f) return 0.0f; return x; }
static Col ToB(FCol f) { Col c; c.R = (unsigned char)(f.R * 255.0f); c.G = (unsigned char)(f.G * 255.0f); c.B = (unsigned char)(f.B * 255.0f); c.A = (unsigned char)(f.A * 255.0f); return c; }

struct Comp { Col Colour, Target, Initial, Parent, Render; float Elapsed, Time; bool Independent; };

static void Update(Comp& c, float dt)
{
    if (c.Elapsed < c.Time) {
        c.Elapsed += dt;
        FCol i = ToF(c.Initial), t = ToF(c.Target);
        float d[4] = { t.R - i.R, t.G - i.G, t.B - i.B, t.A - i.A }, s[4] = { i.R, i.G, i.B, i.A }, o[4];
        for (int k = 0; k < 4; ++k) {
            float acc = d[k] * -2.0f / (c.Time * c.Time), vel = d[k] * 2.0f * (1.0f / c.Time);
            o[k] = Cl(acc * 0.5f * c.Elapsed * c.Elapsed + vel * c.Elapsed + s[k]);
        }
        FCol r = { o[0], o[1], o[2], o[3] };
        c.Colour = ToB(r);
        if (c.Elapsed >= c.Time) { c.Colour = c.Target; c.Initial = c.Colour; }
    }
    if (!c.Independent) {
        FCol p = ToF(c.Parent), own = ToF(c.Colour);
        FCol r = { own.R * p.R, own.G * p.G, own.B * p.B, own.A * p.A };
        c.Render = ToB(r);
    } else
        c.Render = c.Colour;
}

static bool Eq(Col a, Col b) { return a.R == b.R && a.G == b.G && a.B == b.B && a.A == b.A; }

int main()
{
    Col black = { 0, 0, 0, 255 }, white = { 255, 255, 255, 255 };
    Comp c = { black, white, black, white, black, 0.0f, 2.0f, false };
    Update(c, 1.0f);                       // t = T/2: 0 + 1*(1) - 0.5*1*... = 0.75 of the way
    if (c.Colour.R < 185 || c.Colour.R > 192 || c.Colour.A != 255) { std::printf("0052f900_TEST FAIL half %d\n", c.Colour.R); return 1; }
    if (!Eq(c.Render, c.Colour)) { std::printf("0052f900_TEST FAIL render\n"); return 1; }
    Update(c, 1.5f);                       // past T: snaps to the target, initial follows
    if (!Eq(c.Colour, white) || !Eq(c.Initial, white)) { std::printf("0052f900_TEST FAIL snap\n"); return 1; }
    Col half = { 128, 128, 128, 255 };
    c.Parent = half;
    Update(c, 0.1f);                       // finished transition: only the render colour is rebuilt
    if (c.Render.R != 128 && c.Render.R != 127) { std::printf("0052f900_TEST FAIL parent %d\n", c.Render.R); return 1; }
    c.Independent = true; Update(c, 0.1f);
    if (!Eq(c.Render, white)) { std::printf("0052f900_TEST FAIL independent\n"); return 1; }
    std::printf("0052f900_TEST PASS\n");
    return 0;
}