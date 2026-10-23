// Behaviour test for CTCCreatureGenerator::GetUndeadGenerationPoint (0x0074A260): up to three random tries; a point is
// accepted when on the map, passable, outside the exclusion area, near the creature's height and reachable.
#include <cstdio>
#include <cmath>

struct C3DVector { float X, Y, Z; };
static unsigned long g_seed = 1;
static unsigned long ROR13(unsigned long v) { return (v >> 13) | (v << 19); }
static unsigned long GFRandom() { g_seed = (unsigned long)(g_seed * 0x24a1 + 0x24df) & 0xffffffffUL; g_seed = ROR13(g_seed) & 0xffffffffUL; return g_seed; }
static int g_passableAfter = 0, g_calls = 0, g_navigable = 1;
static float g_ground = 0.0f;

struct CGen {
    float GenerationRadius;
    bool  HasExclusion;
    bool Get(const C3DVector& creaturePos, bool hasNav, C3DVector* out)
    {
        for (int attempt = 0; attempt < 3; ++attempt) {
            float distance = ((GenerationRadius - 1.5f) * 0.01f) * (float)(GFRandom() % 100) + 1.5f;
            float angle = (float)(GFRandom() % 10) * 0.2f - 1.0f;
            float a = angle * angle * 0.5f;
            C3DVector f = { distance, 0.0f, 0.0f };          // forward (1,0,0) scaled
            out->X = f.X * (float)std::cos(a) + creaturePos.X;
            out->Y = f.X * (float)std::sin(a) + creaturePos.Y;
            out->Z = creaturePos.Z;
            ++g_calls;
            if (g_calls <= g_passableAfter) continue;      // not passable
            if (HasExclusion) continue;
            out->Z += 1.0f;
            float height = g_ground;
            out->Z = height;
            if (!((float)std::fabs(height - creaturePos.Z) < 1.0f)) continue;
            if (!hasNav) return true;
            if (g_navigable) return true;
        }
        return false;
    }
};

int main()
{
    CGen g; g.GenerationRadius = 10.0f; g.HasExclusion = false;
    C3DVector pos = { 5.0f, 5.0f, 0.5f };
    C3DVector out;
    if (!g.Get(pos, false, &out)) { std::printf("0074a260_TEST FAIL\n"); return 1; }
    float dx = out.X - pos.X, dy = out.Y - pos.Y;
    float d = (float)std::sqrt(dx * dx + dy * dy);
    if (d < 1.5f - 1e-3f || d > 10.0f + 1e-3f || out.Z != 0.0f) { std::printf("0074a260_TEST FAIL\n"); return 1; }
    g_calls = 0; g_passableAfter = 2;
    if (!g.Get(pos, true, &out) || g_calls != 3) { std::printf("0074a260_TEST FAIL\n"); return 1; }
    g_calls = 0; g_passableAfter = 3;
    if (g.Get(pos, true, &out)) { std::printf("0074a260_TEST FAIL\n"); return 1; }
    g_calls = 0; g_passableAfter = 0; g_navigable = 0;
    if (g.Get(pos, true, &out) || g_calls != 3) { std::printf("0074a260_TEST FAIL\n"); return 1; }
    g_ground = 3.0f; g_navigable = 1;
    if (g.Get(pos, false, &out)) { std::printf("0074a260_TEST FAIL\n"); return 1; }
    std::printf("0074a260_TEST PASS\n");
    return 0;
}
