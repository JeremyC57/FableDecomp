// Behaviour test for C3DFrustrum::UpdateFrustrum (0x0084D0E0): Focus = weighted mean (the weight total truncated to a
// long after each add), FOV = atan(sqrt(max_i ((p_i - Apex) . side)^2 / |Focus - Apex|^2)) / (2 pi) with
// side = (Focus - Apex) x Up.
#include <cstdio>
#include <cmath>

struct Vec { float X, Y, Z; };
struct Point { Vec pos; float weight; };
struct Frustrum {
    Vec Apex, Up;
    const Point* first;
    const Point* last;
    float FOV;
    Vec Focus;
};

static void UpdateFrustrum(Frustrum& f)
{
    float sx = 0, sy = 0, sz = 0;
    long total = 0;
    for (const Point* it = f.first; it != f.last; ++it) {
        sx += it->pos.X * it->weight; sy += it->pos.Y * it->weight; sz += it->pos.Z * it->weight;
        total = (long)(total + it->weight);
    }
    float inv = 1.0f / total;
    f.Focus.X = sx * inv; f.Focus.Y = sy * inv; f.Focus.Z = sz * inv;
    Vec t = { f.Focus.X - f.Apex.X, f.Focus.Y - f.Apex.Y, f.Focus.Z - f.Apex.Z };
    Vec side = { t.Y * f.Up.Z - t.Z * f.Up.Y, t.Z * f.Up.X - t.X * f.Up.Z, t.X * f.Up.Y - t.Y * f.Up.X };
    float maxSq = 0;
    for (const Point* it = f.first; it != f.last; ++it) {
        float d = (it->pos.X - f.Apex.X) * side.X + (it->pos.Y - f.Apex.Y) * side.Y + (it->pos.Z - f.Apex.Z) * side.Z;
        float d2 = d * d;
        if (d2 > maxSq) maxSq = d2;
    }
    f.FOV = (float)std::atan((float)std::sqrt(maxSq / (t.X * t.X + t.Y * t.Y + t.Z * t.Z))) * 0.15915493667125702f;
}

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main()
{
    int fails = 0;
    // Apex at the origin looking along +Y, up +Z: side = (0,10,0) x (0,0,1) = (10,0,0).
    Point pts[2] = { { { -10, 10, 0 }, 1.0f }, { { 10, 10, 0 }, 1.0f } };
    Frustrum f = { { 0, 0, 0 }, { 0, 0, 1 }, pts, pts + 2, 0, { 0, 0, 0 } };
    UpdateFrustrum(f);
    // focus (0,10,0); offsets . side = -100, 100 -> max 10000; |t|^2 = 100 -> sqrt(100) = 10; atan(10)/(2pi)
    if (!Near(f.Focus.X, 0) || !Near(f.Focus.Y, 10) || !Near(f.Focus.Z, 0)) ++fails;
    if (!Near(f.FOV, (float)(std::atan(10.0) / 6.283185307179586))) ++fails;
    // Weighted: weights 3 and 1 -> focus x = (-30 + 10) / 4 = -5.
    Point w[2] = { { { -10, 10, 0 }, 3.0f }, { { 10, 10, 0 }, 1.0f } };
    Frustrum g = { { 0, 0, 0 }, { 0, 0, 1 }, w, w + 2, 0, { 0, 0, 0 } };
    UpdateFrustrum(g);
    if (!Near(g.Focus.X, -5) || !Near(g.Focus.Y, 10)) ++fails;
    // Fractional weights: the long total truncates after each add (0.6 -> 0, 0.6 -> 0, 1.5 -> 1).
    Point h[3] = { { { 2, 0, 0 }, 0.6f }, { { 4, 0, 0 }, 0.6f }, { { 6, 0, 0 }, 1.5f } };
    Frustrum k = { { 0, -1, 0 }, { 0, 0, 1 }, h, h + 3, 0, { 0, 0, 0 } };
    UpdateFrustrum(k);
    if (!Near(k.Focus.X, (2 * 0.6f + 4 * 0.6f + 6 * 1.5f) / 1.0f)) ++fails;
    if (fails) { std::printf("0084d0e0_TEST FAIL\n"); return 1; }
    std::printf("0084d0e0_TEST PASS\n");
    return 0;
}