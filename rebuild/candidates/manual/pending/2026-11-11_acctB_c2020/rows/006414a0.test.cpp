// Behaviour test for 0x006414A0 CTCLight::DrawLight: a host model of the light's draw step -- the light slot grown on demand, the
// colour (override info or def) as floats, the flicker blend, the daylight curve over the def's four fade times (with the midnight
// wrap), the sunlight attenuator (facing against the flattened sun direction, clamped), inversion, the scaled colour / radii, the
// lighting-channel mask and the light primitive handed to the appearance; the light counter goes up once per draw.
#include <cstdio>
#include <cmath>
#include <vector>

struct Colour { unsigned char B, G, R, A; };
struct FColour { float R, G, B, A; };
struct Vec { float X, Y, Z; };
struct LightInfo { Colour colour; float inner, outer; bool inverted; };
struct LightDef { Colour colour; float inner, outer; bool sunlight, daylight; float t[4]; long effect; bool inverted; };
struct Prim { Vec pos; Colour colour; float radius, spot_radius; unsigned int mask; int set; };

static long g_lights_drawn = 0;

struct Light {
    LightInfo info; LightDef def; bool overridden; float acc, old_acc; std::vector<int> lights;
    // world / appearance
    float time; Vec sun, facing; float fade_start, fade_end; FColour diffuse; unsigned char channel; std::vector<Prim> prims;
    const Colour& GetColour() const { return overridden ? info.colour : def.colour; }
    float GetInnerRadius() const { return overridden ? info.inner : def.inner; }
    float GetOuterRadius() const { return overridden ? info.outer : def.outer; }
    bool IsInverted() const { return overridden ? info.inverted : def.inverted; }
    void DrawLight(long light_no, const Vec& pos, float interpolate)
    {
        if ((unsigned int)light_no >= lights.size()) {
            lights.resize(light_no + 1);
            prims.push_back(Prim()); prims.back().set = 0;
            lights[light_no] = (int)prims.size() - 1;
        }
        const Colour& c = GetColour();
        FColour colour = { c.R / 255.0f, c.G / 255.0f, c.B / 255.0f, c.A / 255.0f };
        float flicker = old_acc * (1.0f - interpolate) + acc * interpolate;
        float factor = 1.0f;
        if (def.daylight) {
            float times[4]; for (int i = 0; i < 4; ++i) times[i] = def.t[i] * (1.0f / 24.0f);
            float lerp = 0.0f; long segment;
            for (segment = 0; segment < 3; ++segment) {
                float start = times[segment], end = times[segment + 1];
                if (start > end) {
                    if (time >= start) { lerp = (time - start) / ((1.0f - start) + end); break; }
                    if (time <= end) { lerp = (time + (1.0f - start)) / ((1.0f - start) + end); break; }
                } else if (time >= start && time <= end) { lerp = (time - start) / (end - start); break; }
            }
            if (segment == 0) factor = lerp; else if (segment == 1) factor = 1.0f; else if (segment == 2) factor = 1.0f - lerp; else factor = 0.0f;
        } else if (def.sunlight) {
            Vec s = { -sun.X, -sun.Y, 0.0f }; float r = 1.0f / std::sqrt(s.X * s.X + s.Y * s.Y); s.X *= r; s.Y *= r;
            Vec f = facing; f.Z = 0.0f; r = 1.0f / std::sqrt(f.X * f.X + f.Y * f.Y); f.X *= r; f.Y *= r;
            float dot = s.X * f.X + s.Y * f.Y;
            factor = (dot - fade_start) / (fade_end - fade_start);
            if (factor < 0.0f) factor = 0.0f; else if (factor > 1.0f) factor = 1.0f;
            colour = diffuse;
        }
        if (IsInverted()) factor = 1.0f - factor;
        float scale = factor * flicker;
        colour.R *= scale; colour.G *= scale; colour.B *= scale; colour.A *= scale;
        float inner = GetInnerRadius() * scale, outer = GetOuterRadius() * scale;
        unsigned int mask = 0;
        switch (def.effect) {
        case 0: mask = 1u << channel; break;
        case 1: mask = 0xfffffffeu; break;
        case 2: mask = 1; break;
        case 3: mask = 0xffffffffu; break;
        }
        Prim& p = prims[lights[light_no]];
        p.pos = pos; p.colour.R = (unsigned char)(colour.R * 255.0f); p.colour.G = (unsigned char)(colour.G * 255.0f);
        p.colour.B = (unsigned char)(colour.B * 255.0f); p.colour.A = (unsigned char)(colour.A * 255.0f);
        p.radius = outer; p.spot_radius = inner; p.mask = mask; p.set = 1;
        ++g_lights_drawn;
    }
};

static bool near(float a, float b) { return std::fabs(a - b) < 1e-3f; }

int main()
{
    bool ok = true;
    Light l;
    Colour red = { 0, 0, 255, 255 }, half = { 0, 128, 0, 255 };
    l.def.colour = red; l.def.inner = 2.0f; l.def.outer = 10.0f; l.def.sunlight = false; l.def.daylight = false;
    l.def.t[0] = 18.0f; l.def.t[1] = 20.0f; l.def.t[2] = 6.0f; l.def.t[3] = 8.0f; l.def.effect = 0; l.def.inverted = false;
    l.info.colour = half; l.info.inner = 1.0f; l.info.outer = 4.0f; l.info.inverted = false;
    l.overridden = false; l.acc = 1.0f; l.old_acc = 0.5f; l.time = 0.0f; l.channel = 3;
    l.fade_start = 0.0f; l.fade_end = 1.0f; FColour sky = { 0.5f, 0.25f, 1.0f, 1.0f }; l.diffuse = sky;
    l.sun.X = 1.0f; l.sun.Y = 0.0f; l.sun.Z = -1.0f; l.facing.X = -2.0f; l.facing.Y = 0.0f; l.facing.Z = 5.0f;
    Vec pos = { 1.0f, 2.0f, 3.0f };
    // plain light: grows the slot list, blends the flicker (0.5 -> 1.0 at 0.5 = 0.75), mask from the appearance channel
    l.DrawLight(2, pos, 0.5f);
    Prim& p = l.prims[l.lights[2]];
    ok = ok && l.lights.size() == 3 && p.set && near(p.radius, 7.5f) && near(p.spot_radius, 1.5f) && p.mask == 8u
         && p.colour.R == 191 && p.colour.G == 0 && p.colour.A == 191 && p.pos.Y == 2.0f && g_lights_drawn == 1;
    // overridden: the info's colour / radii; inverted factor gives a dark light
    l.overridden = true; l.info.inverted = true; l.def.effect = 3;
    l.DrawLight(2, pos, 1.0f);
    ok = ok && l.lights.size() == 3 && near(l.prims[l.lights[2]].radius, 0.0f) && l.prims[l.lights[2]].mask == 0xffffffffu && g_lights_drawn == 2;
    l.overridden = false; l.info.inverted = false;
    // daylight curve (fade in 18..20 h, out 6..8 h, wrapping midnight)
    l.def.daylight = true; l.def.effect = 2;
    l.time = 19.0f / 24.0f; l.DrawLight(0, pos, 1.0f); ok = ok && near(l.prims[l.lights[0]].radius, 5.0f) && l.prims[l.lights[0]].mask == 1u;
    l.time = 1.0f / 24.0f;  l.DrawLight(0, pos, 1.0f); ok = ok && near(l.prims[l.lights[0]].radius, 10.0f);
    l.time = 7.0f / 24.0f;  l.DrawLight(0, pos, 1.0f); ok = ok && near(l.prims[l.lights[0]].radius, 5.0f);
    l.time = 12.0f / 24.0f; l.DrawLight(0, pos, 1.0f); ok = ok && near(l.prims[l.lights[0]].radius, 0.0f);
    // a curve whose first segment wraps midnight (23 h .. 1 h): before midnight, after midnight
    l.def.t[0] = 23.0f; l.def.t[1] = 1.0f; l.def.t[2] = 5.0f; l.def.t[3] = 6.0f;
    l.time = 23.5f / 24.0f; l.DrawLight(0, pos, 1.0f); ok = ok && near(l.prims[l.lights[0]].radius, 2.5f);
    l.time = 0.5f / 24.0f;  l.DrawLight(0, pos, 1.0f); ok = ok && near(l.prims[l.lights[0]].radius, 7.5f);
    // sunlight attenuator: facing against the sun direction (dot 1) -> full, with the environment's diffuse colour
    l.def.daylight = false; l.def.sunlight = true; l.def.effect = 1;
    l.DrawLight(1, pos, 1.0f);
    ok = ok && near(l.prims[l.lights[1]].radius, 10.0f) && l.prims[l.lights[1]].colour.B == 255 && l.prims[l.lights[1]].colour.R == 127
         && l.prims[l.lights[1]].mask == 0xfffffffeu;
    l.facing.X = 2.0f; l.DrawLight(1, pos, 1.0f); ok = ok && near(l.prims[l.lights[1]].radius, 0.0f);
    ok = ok && g_lights_drawn == 10;
    if (!ok) { std::printf("006414a0_TEST FAIL\n"); return 1; }
    std::printf("006414a0_TEST PASS\n");
    return 0;
}
