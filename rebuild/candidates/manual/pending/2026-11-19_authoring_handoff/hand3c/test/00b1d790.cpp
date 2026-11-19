// Behaviour test for CPSCDecalRenderer::PoolDecal (0x00B1D790), a portable model. The decal's normalised life is clamped to [0, 1];
// its pooled size is min(((end - start) * t + start) * size factor, MaxPoolSize). Without a collider nothing changes and the result
// is false. With one: life pooling sets the collider's max life to (long)min(max life + tick * life secs * (1 - t), tick * MaxPoolLife)
// and adds the decal's frame plus the frame rate to the collider's frame; size pooling adds size / collider size to its size factor
// once per frame (old factor kept); alpha pooling adds min(band alpha * fade / 255 * decal alpha factor, MaxPoolAlpha) to its alpha
// factor once per frame; the decal is deactivated and the result is true.
#include <stdio.h>
#include <math.h>
struct Decal { long lifetimer, max_life; float size_factor[2], alpha_factor[2], frame; long update_timer; bool active, size_hit, alpha_hit; };
struct Colours { bool use_start, use_mid, use_end; unsigned char start_a, mid_a, end_a; };
struct Pool { bool life, size, alpha; float life_secs, max_pool_size, max_pool_life, max_pool_alpha, frame_rate; Colours c; };
static const long TICK = 10;
static bool PoolDecal(const Pool& p, float start, float end, Decal& d, Decal* collider, float collider_size, float fade_alpha)
{
    float t = (float)d.lifetimer / d.max_life; t = t < 0.0f ? 0.0f : (t > 1.0f ? 1.0f : t);
    float size = ((end - start) * t + start) * d.size_factor[0]; if (size > p.max_pool_size) size = p.max_pool_size;
    if (!collider) return false;
    if (p.life) {
        float a = collider->max_life + TICK * p.life_secs * (1.0f - t), b = TICK * p.max_pool_life;
        collider->max_life = (long)(b < a ? b : a);
        collider->frame = collider->frame + d.frame + p.frame_rate;
    }
    if (p.size && !collider->size_hit) {
        collider->size_hit = true; collider->update_timer = collider->lifetimer;
        collider->size_factor[1] = collider->size_factor[0]; collider->size_factor[0] += size / collider_size;
    }
    if (p.alpha) {
        const Colours& c = p.c;
        float sa = c.use_start ? c.start_a : c.use_mid ? c.mid_a : c.use_end ? c.end_a : 255;
        float ea = c.use_end ? c.end_a : c.use_mid ? c.mid_a : c.use_start ? c.start_a : 255;
        float ma = c.use_mid ? c.mid_a : (sa + ea) * 0.5f;
        float alpha = t < 0.5f ? (sa + ma) * 0.5f : (ma + ea) * 0.5f;
        float da = alpha * (fade_alpha / 255.0f) * d.alpha_factor[0]; if (da > p.max_pool_alpha) da = p.max_pool_alpha;
        if (!collider->alpha_hit) {
            collider->alpha_hit = true; collider->update_timer = collider->lifetimer;
            collider->alpha_factor[1] = collider->alpha_factor[0]; collider->alpha_factor[0] += da;
        }
    }
    d.active = false;
    return true;
}
int main()
{
    int bad = 0;
    Pool p = { true, true, true, 2.0f, 4.0f, 100.0f, 0.5f, 1.0f, { true, false, true, 200, 0, 100 } };
    Decal d = { 5, 10, { 1.0f, 1.0f }, { 1.0f, 1.0f }, 2.0f, 0, true, false, false };
    Decal c = { 7, 30, { 1.0f, 1.0f }, { 0.25f, 0.25f }, 3.0f, 0, true, false, false };
    if (PoolDecal(p, 0.0f, 4.0f, d, 0, 1.0f, 255.0f) || !d.active) ++bad;               // no collider
    if (!PoolDecal(p, 0.0f, 4.0f, d, &c, 4.0f, 255.0f) || d.active) ++bad;
    // t = 0.5: size 2; life min(30 + 10 * 2 * 0.5, 1000) = 40; frame 3 + 2 + 1 = 6; size factor 1 + 2/4 = 1.5
    // alphas: start 200, end 100, mid 150; t >= 0.5 -> (150 + 100) / 2 = 125; * 1 * 1 = 125 -> capped 0.5
    if (c.max_life != 40 || c.frame != 6.0f || c.size_factor[0] != 1.5f || c.size_factor[1] != 1.0f || c.update_timer != 7) ++bad;
    if (c.alpha_factor[0] != 0.75f || c.alpha_factor[1] != 0.25f) ++bad;
    d.active = true; d.lifetimer = -3;                                                   // t clamps to 0; second pooling this frame
    if (!PoolDecal(p, 0.0f, 4.0f, d, &c, 4.0f, 255.0f) || c.size_factor[0] != 1.5f || c.alpha_factor[0] != 0.75f) ++bad;
    if (c.max_life != 60 || c.frame != 9.0f) ++bad;                                     // life + 10 * 2 * 1; frame + 2 + 1
    printf(bad ? "00b1d790_TEST FAIL\n" : "00b1d790_TEST PASS\n");
    return bad ? 1 : 0;
}
