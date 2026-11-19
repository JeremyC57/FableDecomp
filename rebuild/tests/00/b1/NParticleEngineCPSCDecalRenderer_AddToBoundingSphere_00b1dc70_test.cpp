// Behaviour test for CPSCDecalRenderer::AddToBoundingSphere (0x00B1DC70), a portable model. No decal bank: false, nothing added.
// Each active decal (max life > -1 and the active bit) adds its position with radius 0.5 * |size|, where the animation point is the
// system's normalised time (0 when disabled) clamped to [0, 1 - 0.0001] or, with ForceAnimationTime, fmod(life * 1 / (anim secs *
// tick), 1); the frame is floor(frames * point) clamped to [0, frames - 1]; size.X = (end - start) * point + start, size.Y =
// height / width of that frame * size.X, both scaled by the system's largest scale when scaling is on. An inactive decal whose
// lifetimer passed its max life is cleared. The result is whether any decal was added.
#include <stdio.h>
#include <math.h>
#include <vector>
struct Decal { float x, y, z; long lifetimer, max_life; bool active, cleared; };
struct Frame { unsigned short w, h; };
struct Added { float x, radius; };
static const float DNZ = 0.0001f;
static bool AddToBoundingSphere(long bank_index, bool force, float start, float end, float anim_secs, long tick,
                                bool time_enabled, float system_time, float scale, const std::vector<Frame>& frames,
                                std::vector<Decal>& decals, float interp, std::vector<Added>& out, std::vector<long>& frames_used)
{
    if (bank_index == 0) return false;
    bool valid = false;
    for (size_t i = 0; i < decals.size(); ++i) {
        Decal& d = decals[i];
        if (d.max_life > -1 && d.active) {
            float life = d.lifetimer + interp;
            float t = time_enabled ? system_time : 0.0f; t = t < 0.0f ? 0.0f : (t > 1.0f ? 1.0f : t);
            float p;
            if (!force) { p = t; if (p < 0.0f) p = 0.0f; else if (p > 1.0f - DNZ) p = 1.0f - DNZ; }
            else p = (float)fmod((double)(life * (1.0f / (anim_secs * tick))), 1.0);
            long n = (long)frames.size(), f = (long)floor(n * p);
            if (f < 0) f = 0; else if (f > n - 1) f = n - 1;
            frames_used.push_back(f);
            float sx = (end - start) * p + start, sy = (float)frames[f].h / frames[f].w * sx;
            sx *= scale; sy *= scale;
            Added a = { d.x, (float)sqrt(sx * sx + sy * sy) * 0.5f }; out.push_back(a);
            valid = true;
        } else if (d.lifetimer + interp > d.max_life) d.cleared = true;
    }
    return valid;
}
int main()
{
    int bad = 0;
    std::vector<Frame> frames; Frame f0 = { 4, 4 }, f1 = { 4, 12 }; frames.push_back(f0); frames.push_back(f1);
    std::vector<Decal> decals; Decal a = { 1, 0, 0, 5, 10, true, false }, b = { 2, 0, 0, 20, 10, false, false }, c = { 3, 0, 0, 5, 10, false, false };
    decals.push_back(a); decals.push_back(b); decals.push_back(c);
    std::vector<Added> out; std::vector<long> used;
    if (AddToBoundingSphere(0, false, 0, 4, 1, 10, true, 0.75f, 1.0f, frames, decals, 0, out, used) || !out.empty()) ++bad;
    // point 0.75 -> frame floor(1.5) = 1 (h/w 3); size.X = 3, size.Y = 9 -> radius sqrt(90) / 2
    if (!AddToBoundingSphere(1, false, 0, 4, 1, 10, true, 0.75f, 1.0f, frames, decals, 0, out, used)) ++bad;
    if (out.size() != 1 || out[0].x != 1.0f || used[0] != 1 || fabs(out[0].radius - (float)sqrt(90.0) * 0.5f) > 1e-5f) ++bad;
    if (!decals[1].cleared || decals[2].cleared) ++bad;                          // only the expired inactive decal is cleared
    out.clear(); used.clear();
    // time disabled -> point 0 -> frame 0, size.X = start = 2 -> |(2, 2)| / 2; scale 2 doubles it
    AddToBoundingSphere(1, false, 2, 4, 1, 10, false, 0.75f, 2.0f, frames, decals, 0, out, used);
    if (used[0] != 0 || fabs(out[0].radius - (float)sqrt(32.0) * 0.5f) > 1e-5f) ++bad;
    out.clear(); used.clear();
    // forced: life 5 * 1 / (1 * 4) = 1.25 -> fmod 0.25 -> frame 0, size.X = 1
    AddToBoundingSphere(1, true, 0, 4, 1, 4, true, 0.9f, 1.0f, frames, decals, 0, out, used);
    if (used[0] != 0 || fabs(out[0].radius - (float)sqrt(2.0) * 0.5f) > 1e-5f) ++bad;
    printf(bad ? "00b1dc70_TEST FAIL\n" : "00b1dc70_TEST PASS\n");
    return bad ? 1 : 0;
}