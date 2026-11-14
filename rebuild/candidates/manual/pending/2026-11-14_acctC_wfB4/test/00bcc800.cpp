// Behaviour test for 0x00BCC800 CEnginePrimitiveManagerWeaponTrails::RenderWeaponTrail: a host model of the row's decisions with
// recording stand-ins for the engine. Checked: the two bail-outs (frame not available -> nothing touched; fewer than two points
// left after the skip -> no state change, no draw), the skip (floor of SkipPoints, clamped at 0) and its fraction (clamped at 0),
// the vertex count (16 per segment + 2: nine steps for the first segment, eight for the others, two vertices a step), the tangents
// ((previous - next) * 0.5 for an inner point, * 1.0 at either end), the Hermite basis (t = 0 gives the current point, t = 1 the
// previous one; the first segment starts at the skip fraction), the age fade (1 - age / fall-off) * Alpha clamped at 0, rounded to
// nearest and packed 0xFFaaaaaa, the texture coordinates (u 0.99 / 0.01, v = (segment + t - fraction) / 4), fog off + the capture
// block around the draw, the triangle-strip draw of vertex_count - 2 primitives with the frame's texture on stage 0, and the vertex
// buffer pool: the count rounded up to the granularity (capped at 0x10000), the three-word key, a returned buffer reused for the
// same key unless it is busy, and the pool's free-byte accounting.
#include <cstdio>
#include <cmath>
#include <cstring>

struct V3 { float x, y, z; };
static V3 Sub(const V3& a, const V3& b) { V3 r = { a.x - b.x, a.y - b.y, a.z - b.z }; return r; }
static V3 Add(const V3& a, const V3& b) { V3 r = { a.x + b.x, a.y + b.y, a.z + b.z }; return r; }
static V3 Mul(const V3& a, float s) { V3 r = { a.x * s, a.y * s, a.z * s }; return r; }
struct SVertex { V3 position; unsigned long colour; float u, v; };

// The vertex buffer pool (CEngineVertexBufferPool / CEngineDynamic3DResourceManager): one free list per key.
struct SBuffer { unsigned long key[3]; SBuffer* pool_next; bool busy; long bytes; SVertex data[512]; };
struct SPool {
    long granularity, free_bytes, total_bytes, built; SBuffer storage[4]; SBuffer* head[4]; SBuffer* tail[4]; unsigned long keys[4][3]; long key_count;
    long FindID(const unsigned long* key) { for (long i = 0; i < key_count; ++i) if (!memcmp(keys[i], key, 3 * sizeof(unsigned long))) return i + 1; return 0; }
    long AddKey(const unsigned long* key) { long id = FindID(key); if (id == 0) { memcpy(keys[key_count], key, 3 * sizeof(unsigned long)); id = ++key_count; } return id; }
    SBuffer* GetBuffer(unsigned long type, long count, long stride, unsigned long usage, unsigned long pool)
    {
        long size = ((count - 1) / granularity + 1) * granularity;
        count = size < 0x10000 ? size : 0x10000;
        unsigned long key[3] = { type, (unsigned long)((stride << 16) + count), (pool << 16) + usage };
        long id = AddKey(key);
        SBuffer* resource = 0;
        if (head[id - 1] && !head[id - 1]->busy) {
            resource = head[id - 1];
            head[id - 1] = resource->pool_next;
            resource->pool_next = 0;
            if (!head[id - 1]) tail[id - 1] = 0;
            free_bytes -= resource->bytes;
        }
        if (!resource) {
            resource = &storage[built++];
            resource->bytes = count * stride; resource->busy = false; resource->pool_next = 0;
            memcpy(resource->key, key, 3 * sizeof(unsigned long));
            total_bytes += resource->bytes;
        }
        return resource;
    }
    void ReturnResource(SBuffer* resource)
    {
        long id = FindID(resource->key);
        if (id > 0) {
            if (tail[id - 1]) tail[id - 1]->pool_next = resource; else head[id - 1] = resource;
            tail[id - 1] = resource;
            free_bytes += resource->bytes;
        }
    }
};

struct STrail { float skip_points; const V3* trail[2]; long size; const float* created; float now, fall_off; unsigned char alpha; };
struct SLog { int frame_asked, shader, world_reset, capture_open, capture_close, fog_off, prepare, lock, unlock, draws, flush; long primitives; long texture; SBuffer* buffer; long vertices; };

static long Floor(float f) { return (long)floor(f); }
static long Near(float f) { return (long)floor(f + 0.5f); }   // the test never feeds an exact .5

static void RenderWeaponTrail(SPool& pool, const STrail& trail, bool frame_ok, long frame_texture, SLog& log)
{
    ++log.frame_asked;
    if (!frame_ok)
        return;
    long trail_size = trail.size;
    long skip = Floor(trail.skip_points);
    if (skip < 0)
        skip = 0;
    long points = trail_size - skip;
    if (points < 2)
        return;
    ++log.shader; ++log.world_reset;
    float fraction = trail.skip_points - skip;
    if (fraction < 0.0f)
        fraction = 0.0f;
    long segments = points - 1;
    long vertex_count = segments * 16 + 2;
    V3 tangents[2][64];
    for (long i = 0; i < points; ++i) {
        long n = points - i;
        long next = trail_size - 1 < n ? trail_size - 1 : n;
        long previous = n - 2 > 0 ? n - 2 : 0;
        float scale = next == previous + 2 ? 0.5f : 1.0f;
        tangents[0][n - 1] = Mul(Sub(trail.trail[0][previous], trail.trail[0][next]), scale);
        tangents[1][n - 1] = Mul(Sub(trail.trail[1][previous], trail.trail[1][next]), scale);
    }
    ++log.capture_open; ++log.fog_off; ++log.prepare;
    SBuffer* buffer = pool.GetBuffer(2, vertex_count, sizeof(SVertex), 0x208, 0);
    ++log.lock;
    SVertex* vertices = buffer->data;
    unsigned long first_step = 0;
    for (long segment = 0; segment < segments; ++segment) {
        long n = points - segment;
        long current = n - 1;
        long previous = n - 2 > 0 ? n - 2 : 0;
        float alpha = (1.0f - (trail.now - trail.created[current]) / trail.fall_off) * trail.alpha;
        if (alpha < 0.0f)
            alpha = 0.0f;
        for (unsigned long step = first_step; step <= 8; ++step) {
            long a = Near(alpha);
            unsigned long colour = 0xff000000 | (a << 16) | (a << 8) | a;
            float t = step * 0.125f;
            if (segment == 0 && t < fraction)
                t = fraction;
            float t2 = t * t, t3 = t2 * t;
            float h1 = 2.0f * t3 - 3.0f * t2 + 1.0f, h2 = 3.0f * t2 - 2.0f * t3, h3 = t3 - 2.0f * t2 + t, h4 = t3 - t2;
            float v = (segment + t - fraction) * 0.25f;
            for (int side = 0; side < 2; ++side) {
                SVertex* vertex = vertices++;
                vertex->position = Add(Add(Mul(trail.trail[side][current], h1), Mul(trail.trail[side][previous], h2)),
                                       Add(Mul(tangents[side][current], h3), Mul(tangents[side][previous], h4)));
                vertex->colour = colour;
                vertex->u = side == 0 ? 0.99f : 0.01f;
                vertex->v = v;
            }
        }
        first_step = 1;
    }
    ++log.unlock;
    log.vertices = (long)(vertices - buffer->data);
    log.texture = frame_texture; log.primitives = vertex_count - 2; ++log.draws; ++log.flush;
    pool.ReturnResource(buffer);
    log.buffer = buffer;
    ++log.capture_close;
}

static bool Close(float a, float b) { return fabs(a - b) < 0.0005f; }
static bool Same(const V3& a, const V3& b) { return Close(a.x, b.x) && Close(a.y, b.y) && Close(a.z, b.z); }

int main()
{
    static SPool pool; pool.granularity = 64;
    V3 base[6], tip[6]; float created[6];
    for (int i = 0; i < 6; ++i) {
        base[i].x = (float)i; base[i].y = (float)(i * i); base[i].z = 1.0f;
        tip[i].x = (float)i; tip[i].y = (float)(i * i) + 2.0f; tip[i].z = 3.0f;
        created[i] = 10.0f + i;
    }
    STrail trail = { 0.0f, { base, tip }, 6, created, 16.0f, 8.0f, 200 };
    int fail = 0;

    // 1. frame not available: nothing but the frame request.
    SLog log; memset(&log, 0, sizeof log);
    RenderWeaponTrail(pool, trail, false, 7, log);
    if (log.frame_asked != 1 || log.shader || log.capture_open || log.draws || pool.built) fail |= 1;

    // 2. whole trail: 5 segments -> 82 vertices, 80 primitives, buffer rounded to 128 vertices.
    memset(&log, 0, sizeof log);
    RenderWeaponTrail(pool, trail, true, 7, log);
    if (log.vertices != 82 || log.primitives != 80 || log.draws != 1 || log.texture != 7) fail |= 2;
    if (log.shader != 1 || log.world_reset != 1 || log.fog_off != 1 || log.prepare != 1 || log.capture_open != 1 || log.capture_close != 1 ||
        log.lock != 1 || log.unlock != 1 || log.flush != 1) fail |= 2;
    SBuffer* first = log.buffer;
    if (!first || first->key[0] != 2 || first->key[1] != ((sizeof(SVertex) << 16) + 128) || first->key[2] != 0x208) fail |= 4;
    if (pool.built != 1 || pool.total_bytes != 128 * (long)sizeof(SVertex) || pool.free_bytes != pool.total_bytes) fail |= 4;
    // first pair: t = 0 of segment 0 = the newest point (index 5); u 0.99 then 0.01; v 0.
    if (!Same(first->data[0].position, base[5]) || !Same(first->data[1].position, tip[5])) fail |= 8;
    if (!Close(first->data[0].u, 0.99f) || !Close(first->data[1].u, 0.01f) || !Close(first->data[0].v, 0.0f)) fail |= 8;
    // age fade of segment 0 (point 5): (1 - (16 - 15) / 8) * 200 = 175 -> 0xFFAFAFAF.
    if (first->data[0].colour != 0xffafafafUL || first->data[1].colour != 0xffafafafUL) fail |= 16;
    // last pair of segment 0 (step 8, t = 1) = point 4; segment 1 starts at step 1 (pair index 9).
    if (!Same(first->data[16].position, base[4]) || !Close(first->data[16].v, 0.25f)) fail |= 32;
    if (!Close(first->data[18].v, (1.0f + 0.125f) * 0.25f)) fail |= 32;
    // segment 1 colour (point 4): (1 - 2 / 8) * 200 = 150 -> 0x96; last segment (point 1): (1 - 5 / 8) * 200 = 75 -> 0x4B.
    if (first->data[18].colour != 0xff969696UL || first->data[81].colour != 0xff4b4b4bUL) fail |= 64;
    // very last pair = point 0, v = (4 + 1) / 4.
    if (!Same(first->data[80].position, base[0]) || !Same(first->data[81].position, tip[0]) || !Close(first->data[81].v, 1.25f)) fail |= 64;
    // midpoint of segment 0 (step 4, t = 0.5): h1 = h2 = 0.5, h3 = 0.125, h4 = -0.125; tangent at 5 = (P4 - P5) * 1 (end point),
    // tangent at 4 = (P3 - P5) * 0.5 (inner point). y: 0.5 * 25 + 0.5 * 16 + 0.125 * (16 - 25) - 0.125 * 0.5 * (9 - 25) = 20.375.
    if (!Close(first->data[8].position.y, 20.375f) || !Close(first->data[8].position.x, 4.5f)) fail |= 128;

    // 3. same key again: the returned buffer is reused, nothing new built, the free bytes go back up after the return.
    memset(&log, 0, sizeof log);
    RenderWeaponTrail(pool, trail, true, 9, log);
    if (log.buffer != first || pool.built != 1 || pool.free_bytes != pool.total_bytes) fail |= 256;
    // busy head: a new buffer is built for the same key and queued behind the first.
    first->busy = true;
    memset(&log, 0, sizeof log);
    RenderWeaponTrail(pool, trail, true, 9, log);
    if (log.buffer == first || pool.built != 2 || first->pool_next != log.buffer || pool.key_count != 1) fail |= 512;
    first->busy = false;

    // 4. skip 1.5: skip 1, fraction 0.5, 5 points, 4 segments -> 66 vertices (64-vertex key no longer fits: 128 again).
    trail.skip_points = 1.5f;
    memset(&log, 0, sizeof log);
    RenderWeaponTrail(pool, trail, true, 7, log);
    SBuffer* b = log.buffer;
    if (log.vertices != 66 || log.primitives != 64) fail |= 1024;
    // steps 0..4 of segment 0 are held at t = 0.5 (v = 0); step 5 has t = 0.625 -> v = 0.125 / 4; the newest drawn point is 4.
    if (!Close(b->data[0].v, 0.0f) || !Close(b->data[8].v, 0.0f) || !Close(b->data[10].v, 0.03125f)) fail |= 1024;
    if (!Close(b->data[0].position.x, 3.5f) || !Same(b->data[16].position, base[3])) fail |= 1024;
    // tangent at the newest drawn point 4 is now an inner one: next = 5, previous = 3 -> (P3 - P5) * 0.5.
    // y at t = 0.5: 0.5 * 16 + 0.5 * 9 + 0.125 * 0.5 * (9 - 25) - 0.125 * 0.5 * (4 - 16) = 12.25.
    if (!Close(b->data[0].position.y, 12.25f)) fail |= 2048;

    // 5. negative skip: floor(-0.5) = -1 -> skip 0, fraction clamped to 0: the full trail again.
    trail.skip_points = -0.5f;
    memset(&log, 0, sizeof log);
    RenderWeaponTrail(pool, trail, true, 7, log);
    if (log.vertices != 82 || !Close(log.buffer->data[0].v, 0.0f) || !Same(log.buffer->data[0].position, base[5])) fail |= 4096;

    // 6. fewer than two points left: the frame is asked for, nothing is set up or drawn.
    trail.skip_points = 5.0f;
    memset(&log, 0, sizeof log);
    long built = pool.built;
    RenderWeaponTrail(pool, trail, true, 7, log);
    if (log.frame_asked != 1 || log.shader || log.capture_open || log.draws || pool.built != built) fail |= 8192;

    // 7. old trail: the fade is clamped at 0 -> alpha byte 0; one segment -> 18 vertices, 64-vertex buffer (a second key).
    trail.skip_points = 4.0f; trail.now = 100.0f;
    memset(&log, 0, sizeof log);
    RenderWeaponTrail(pool, trail, true, 7, log);
    if (log.vertices != 18 || log.primitives != 16 || log.buffer->data[0].colour != 0xff000000UL) fail |= 16384;
    if (log.buffer->key[1] != ((sizeof(SVertex) << 16) + 64) || pool.key_count != 2) fail |= 16384;
    // points = 2: both points are end points -> tangent scale 1.0: at t = 0.5 x = 0.5 * 1 + 0.5 * 0 + 0.125 * (0 - 1) - 0.125 * (0 - 1) = 0.5.
    if (!Close(log.buffer->data[8].position.x, 0.5f)) fail |= 32768;

    if (fail) { printf("00bcc800_TEST FAIL %x\n", fail); return 1; }
    printf("00bcc800_TEST PASS\n");
    return 0;
}
