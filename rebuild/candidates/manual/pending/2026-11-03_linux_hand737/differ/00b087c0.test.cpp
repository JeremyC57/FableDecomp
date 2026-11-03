// Behaviour test for NParticleEngine::CPSCSingleSprite::PreparePrimitives (0x00B087C0): a standalone model of the frame and sprite
// selection -- the animation fraction is the clamped life fraction, or with ForceAnimationTime the life time over the animation
// length wrapped to [0,1); the graphic frame is floor(frames * fraction) clamped to [0, frames - 1]; the sprite height keeps the
// frame's aspect; crossed sprites are spread by (i + 0.5) * step / count (step 1 for alignments 2 and 4, else 0.5) and each set
// under its own child id (-1 past the id list); a single sprite with no width or no alpha clears its child instead.
#include <cstdio>
#include <cmath>
#include <vector>
static long Floor(float x) { return (long)std::floor(x); }
struct Child { long id; float angle; bool clear; };
static float AnimFraction(bool force, float life, float lifeTime, float tick, float animSecs)
{
    if (!force) { float limit = 1.0f - 1e-6f; return life < 0 ? 0 : life > limit ? limit : life; }
    return (float)std::fmod(lifeTime * (1.0f / (tick * animSecs)), 1.0);
}
static long Frame(long frames, float fraction)
{
    long f = Floor(frames * fraction);
    if (f < 0) f = 0; else if (f > frames - 1) f = frames - 1;
    return f;
}
static void Sprites(bool crossed, long count, long alignment, float angle, float width, unsigned char alpha,
                    const std::vector<long>& ids, std::vector<Child>& out)
{
    if (crossed) {
        float step = (alignment == 2 || alignment == 4) ? 1.0f : 0.5f;
        for (long i = 0; i < count; ++i) {
            Child c = { (unsigned long)i < ids.size() ? ids[i] : -1, (i + 0.5f) * step / count + angle, false };
            out.push_back(c);
        }
    } else {
        Child c = { ids.empty() ? -1 : ids[0], angle, !(width > 0 && alpha) };
        out.push_back(c);
    }
}
static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    bool ok = near(AnimFraction(false, 0.5f, 0, 15, 1), 0.5f) && AnimFraction(false, 2.0f, 0, 15, 1) < 1.0f
        && near(AnimFraction(false, -1, 0, 15, 1), 0) && near(AnimFraction(true, 0, 22.5f, 15, 1), 0.5f);
    ok = ok && Frame(8, 0.5f) == 4 && Frame(8, 0.999f) == 7 && Frame(8, 0) == 0 && Frame(8, 1.5f) == 7;
    float width = 4, aspect = 32.0f / 64.0f; ok = ok && near(aspect * width, 2);
    std::vector<long> ids; ids.push_back(11); ids.push_back(12);
    std::vector<Child> c; Sprites(true, 3, 2, 0.25f, 1, 255, ids, c);
    ok = ok && c.size() == 3 && c[0].id == 11 && c[1].id == 12 && c[2].id == -1 && near(c[0].angle, 0.25f + 0.5f / 3) && near(c[2].angle, 0.25f + 2.5f / 3);
    c.clear(); Sprites(true, 2, 1, 0, 1, 255, ids, c); ok = ok && near(c[1].angle, 0.75f * 0.5f);
    c.clear(); Sprites(false, 0, 0, 1, 0, 255, ids, c); ok = ok && c.size() == 1 && c[0].clear && c[0].id == 11;
    c.clear(); Sprites(false, 0, 0, 1, 2, 0, ids, c); ok = ok && c[0].clear;
    c.clear(); Sprites(false, 0, 0, 1, 2, 9, ids, c); ok = ok && !c[0].clear && near(c[0].angle, 1);
    printf(ok ? "00b087c0_TEST PASS\n" : "00b087c0_TEST FAIL\n");
    return ok ? 0 : 1;
}
