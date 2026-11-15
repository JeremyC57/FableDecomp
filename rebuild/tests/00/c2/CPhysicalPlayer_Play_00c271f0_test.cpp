// Behaviour test for CPhysicalPlayer::Play(CSoundEventManager*, SPlayDesc*, CSoundEvent*, SEventLevel*, float, float) 0x00C271F0 --
// portable model of the row with a recording CODA API: 3D source only for a positioned mono event (else 2D), start offset =
// length * fraction rounded down to the block alignment, fade-in only when starting mid-sample, sample type from the format tag,
// loop window (format loop minus ceil(bytes per sample), loop point in the middle) or the whole length when not looping, then
// position + range callback (3D) or the distance property (2D), pitch, sample, play cursor, play(looping) and the master-scaled
// volume for 2D sources; nothing is started when the offset is past the end.
#include <cstdio>
#include <cmath>
#include <vector>

struct Call { int id; double a; double b; };
static std::vector<Call> g_calls;
static void rec(int id, double a = 0, double b = 0) { Call c = { id, a, b }; g_calls.push_back(c); }
enum { C3D = 1, C2D, ENV, SAMPLE, POS, RANGE, DIST, VOL, PITCH, SETSRC, CURSOR, PLAY, ADD, TICK, TIME };

struct V3 { float X, Y, Z; V3 SwapYZ() const { V3 r; r.X = X; r.Y = Z; r.Z = Y; return r; } };
struct Env { float startVol, fadeIn, holdA, holdB, endVol, fadeOut; };
struct SampleDesc { long type, channels, rate; unsigned long length, loopStart, loopEnd; long data; };
static Env g_env; static SampleDesc g_sample; static V3 g_pos;
struct Format {
    unsigned long length; unsigned short channels, tag; long rate; unsigned long blockAlign, loopStart, loopEnd; float bps; long hasLoop;
};
struct Buffer { Format format; long data; };
struct Event { long priority; Buffer* buffer; };
struct Manager { float fadeIn, fadeOut, master; };
struct Desc { long mode; V3 pos; float distance; };
struct Level { float fraction; long loops; };

struct Player {
    Event* ev; long sample; long source; long loopsLeft; unsigned long loopPoint; long armed; long priority; unsigned long length;
    bool looping, is3D, started, stopped;
    void Play(Manager* m, Desc* d, Event* e, Level* lv, float pitch, float gain)
    {
        ev = e;
        loopsLeft = lv->loops;
        priority = ev->priority;
        is3D = d->mode != 0;
        if (d->mode != 0 && ev->buffer->format.channels > 1) is3D = false;
        if (is3D) { rec(C3D); source = 3; } else { rec(C2D); source = 2; }
        Buffer* b = ev->buffer;
        unsigned long offset = (unsigned long)(b->format.length * lv->fraction);
        unsigned long aligned = offset - offset % b->format.blockAlign;
        if (offset != aligned) offset = aligned;
        Env env;
        env.fadeIn = offset ? m->fadeIn * 0.001f : 0.0f;
        env.fadeOut = m->fadeOut * 0.001f;
        env.holdB = 0.0f; env.startVol = 0.0f; env.holdA = 0.0f; env.endVol = 1.0f;
        g_env = env; rec(ENV);
        SampleDesc s;
        s.type = 0;
        switch (b->format.tag) { case 0: s.type = 1; break; case 1: s.type = 8; break; }
        s.channels = b->format.channels;
        s.rate = b->format.rate;
        if (loopsLeft != 0) {
            armed = 1; looping = true;
            if (b->format.hasLoop) {
                s.loopStart = b->format.loopStart;
                float tail = (float)std::ceil((double)b->format.bps);
                s.loopEnd = b->format.loopEnd - (unsigned long)tail;
                loopPoint = s.loopStart + (s.loopEnd - s.loopStart) / 2;
            } else { s.loopStart = 0; s.loopEnd = 0; loopPoint = b->format.length / 2; }
        } else { s.loopStart = 0; s.loopEnd = 0; loopPoint = b->format.length; armed = 0; looping = false; }
        s.length = b->format.length;
        s.data = b->data;
        g_sample = s; rec(SAMPLE); sample = 77;
        length = b->format.length;
        started = false; stopped = false;
        rec(ADD);
        if (offset < length) {
            if (d->mode != 0) {
                if (is3D) { g_pos = d->pos.SwapYZ(); rec(POS); rec(RANGE); }
            } else rec(DIST, d->distance);
            rec(TIME);
            rec(VOL, 0.0f);
            rec(PITCH, pitch);
            rec(SETSRC, (double)sample);
            rec(CURSOR, (double)offset);
            rec(TICK);
            rec(PLAY, loopsLeft != 0);
            if (!is3D) gain *= m->master;
            rec(VOL, gain);
        }
    }
};

static int fails = 0;
static void check(bool c, const char* w) { if (!c) { ++fails; std::printf("FAIL %s\n", w); } }
static bool has(int id) { for (size_t i = 0; i < g_calls.size(); ++i) if (g_calls[i].id == id) return true; return false; }
static const Call* last(int id) { const Call* r = 0; for (size_t i = 0; i < g_calls.size(); ++i) if (g_calls[i].id == id) r = &g_calls[i]; return r; }

int main()
{
    Manager m = { 500.0f, 250.0f, 0.5f };
    Buffer b; b.format.length = 1000; b.format.channels = 1; b.format.tag = 1; b.format.rate = 22050; b.format.blockAlign = 4;
    b.format.loopStart = 100; b.format.loopEnd = 900; b.format.bps = 1.5f; b.format.hasLoop = 1; b.data = 1234;
    Event e = { 7, &b };
    Desc d = { 1, { 1.0f, 2.0f, 3.0f }, 9.0f };
    Level lv = { 0.0f, 2 };
    Player p;
    // positioned mono, looping with a format loop, from the start
    g_calls.clear(); p.Play(&m, &d, &e, &lv, 1.25f, 0.8f);
    check(has(C3D) && !has(C2D) && p.is3D, "positioned mono event gets a 3D source");
    check(g_env.fadeIn == 0.0f && g_env.fadeOut == 0.25f && g_env.endVol == 1.0f, "no fade-in from the start, fade-out from the manager");
    check(g_sample.type == 8 && g_sample.channels == 1 && g_sample.rate == 22050 && g_sample.data == 1234, "ADPCM sample description");
    check(g_sample.loopStart == 100 && g_sample.loopEnd == 898 && p.loopPoint == 499, "loop window minus ceil(bytes per sample), mid loop point");
    check(p.armed == 1 && p.looping && p.priority == 7 && p.length == 1000, "looping state and copies");
    check(g_pos.X == 1.0f && g_pos.Y == 3.0f && g_pos.Z == 2.0f && has(RANGE) && !has(DIST), "Y/Z swapped position and range callback");
    check(last(PLAY)->a == 1 && last(VOL)->a == (double)0.8f && last(CURSOR)->a == 0, "looping play, unscaled 3D volume, cursor 0");
    // stereo positioned: 2D source; not looping; mid start rounded to the block alignment with a fade-in; PCM
    b.format.channels = 2; b.format.tag = 0; lv.loops = 0; lv.fraction = 0.3335f;
    g_calls.clear(); p.Play(&m, &d, &e, &lv, 1.0f, 0.8f);
    check(has(C2D) && !p.is3D && !has(POS) && !has(DIST), "a positioned stereo event is 2D without position or distance");
    check(g_env.fadeIn == 0.5f && last(CURSOR)->a == 332, "mid start: block-aligned offset and fade-in");
    check(g_sample.type == 1 && g_sample.loopStart == 0 && g_sample.loopEnd == 0 && p.loopPoint == 1000 && p.armed == 0 && !p.looping, "no loop: whole length");
    check(last(PLAY)->a == 0 && last(VOL)->a == (double)(0.8f * 0.5f), "not looping, 2D volume scaled by the master");
    // unpositioned: distance property; looping without a format loop
    d.mode = 0; lv.loops = -1; b.format.hasLoop = 0; lv.fraction = 0.0f;
    g_calls.clear(); p.Play(&m, &d, &e, &lv, 1.0f, 1.0f);
    check(has(DIST) && last(DIST)->a == 9.0f && p.loopPoint == 500 && g_sample.loopEnd == 0, "2D distance property; half-length loop point");
    // offset at the end: nothing started
    lv.fraction = 1.0f;
    g_calls.clear(); p.Play(&m, &d, &e, &lv, 1.0f, 1.0f);
    check(has(SAMPLE) && has(ADD) && !has(PLAY) && !has(CURSOR), "offset past the end: sample made, nothing played");
    if (fails) { std::printf("00c271f0_TEST FAIL\n"); return 1; }
    std::printf("00c271f0_TEST PASS\n");
    return 0;
}