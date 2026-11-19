// Behaviour test for CPSCDecalRenderer::DecalCollides (0x00B1D550), a portable model. Start / end render sizes unpack from 11 bits
// over [0, 20] rounded to two places; the fade band is [FadeInEnd, FadeOutBegin] * 0.001. Every other decal of the system that is
// active (max life > -1 and the active bit), whose min(lifetimer / max life, 1) lies inside the band, gets the pooled size
// min(((end - start) * t + start) * size factor, MaxPoolSize); the first one whose squared distance to the given decal is below
// (decal_size + that size)^2 is returned with its size. The given decal itself is skipped.
#include <stdio.h>
#include <math.h>
#include <list>
struct Decal { float x, y, z; long lifetimer, max_life; bool active; float size_factor; };
static float Unpack(unsigned long packed) { float v = packed / 2047.0f * 20.0f; return (float)floor(v * 100.0f + 0.5f) / 100.0f; }
struct Renderer {
    unsigned long start_packed, end_packed, fade_in_end, fade_out_begin; float max_pool_size;
    bool DecalCollides(std::list<Decal*>& decals, std::list<Decal*>::iterator it_decal, float decal_size,
                       std::list<Decal*>::iterator& out, float& out_size)
    {
        float start = Unpack(start_packed), end = Unpack(end_packed);
        float fob = fade_out_begin * 0.001f, fie = fade_in_end * 0.001f;
        for (std::list<Decal*>::iterator it = decals.begin(); it != decals.end(); ++it) {
            if (it == it_decal) continue;
            Decal* d = *it;
            if (!(d->max_life > -1 && d->active)) continue;
            float t = (float)d->lifetimer / d->max_life; if (t > 1.0f) t = 1.0f;
            if (!(t >= fie && t <= fob)) continue;
            Decal* me = *it_decal;
            float size = ((end - start) * t + start) * d->size_factor; if (size > max_pool_size) size = max_pool_size;
            float dx = me->x - d->x, dy = me->y - d->y, dz = me->z - d->z, r = decal_size + size;
            if (dx * dx + dy * dy + dz * dz < r * r) { out_size = size; out = it; return true; }
        }
        return false;
    }
};
int main()
{
    int bad = 0;
    Renderer r = { 0, 2047, 100, 900, 15.0f };                                   // sizes 0 -> 20, band [0.1, 0.9]
    Decal me = { 0, 0, 0, 0, 10, true, 1.0f };
    Decal far_ = { 100, 0, 0, 5, 10, true, 1.0f };                               // t 0.5, size 10: too far
    Decal young = { 1, 0, 0, 0, 10, true, 1.0f };                                // t 0: before the band
    Decal dead = { 1, 0, 0, 5, -1, true, 1.0f };                                 // max life -1: inactive
    Decal off = { 1, 0, 0, 5, 10, false, 1.0f };                                 // active bit clear
    Decal hit = { 3, 4, 0, 5, 10, true, 2.0f };                                  // dist 5, size min(10 * 2, 15) = 15
    std::list<Decal*> l; l.push_back(&me); l.push_back(&far_); l.push_back(&young); l.push_back(&dead); l.push_back(&off); l.push_back(&hit);
    std::list<Decal*>::iterator out = l.end(); float size = -1.0f;
    if (!r.DecalCollides(l, l.begin(), 0.5f, out, size) || *out != &hit || size != 15.0f) ++bad;
    hit.x = 30.0f;                                                                // 30 >= 0.5 + 15
    out = l.end(); size = -1.0f;
    if (r.DecalCollides(l, l.begin(), 0.5f, out, size) || out != l.end() || size != -1.0f) ++bad;
    hit.x = 3.0f; hit.lifetimer = 50;                                              // t capped at 1: past the band
    if (r.DecalCollides(l, l.begin(), 0.5f, out, size)) ++bad;
    r.fade_out_begin = 1000;                                                      // band now includes 1
    if (!r.DecalCollides(l, l.begin(), 0.5f, out, size) || *out != &hit || size != 15.0f) ++bad;
    printf(bad ? "00b1d550_TEST FAIL\n" : "00b1d550_TEST PASS\n");
    return bad ? 1 : 0;
}