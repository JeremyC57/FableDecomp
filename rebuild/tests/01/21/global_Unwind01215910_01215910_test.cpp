// Behaviour test for CAsyncDecoder_00c48640::Run 0x00C47F10: a scripted request queue drives open (valid / invalid
// stream), seek (no stream, negative position, success), read (count stored, clamped at 0 on error, position answered)
// and close, and the loop ends after the exit request with one acknowledgement to the exit queue.
#include <cstdio>
#include <cstring>

struct Stream { long channels, rate, unit, position, user, readResult, seekMode; float seekAmount; int seeks, reads; };
static Stream* g_live; static int g_deleted;
struct Frame { unsigned long channels, samples, valid; };
struct Req { int type; Stream proto; long mode; float amount; Frame* frame; };
struct Replies { int open, openOk, seek, seekOk, read, close, exit; long lastPos, lastUser, lastChannels; Frame* lastFrame; };
static Replies R;
static Req g_q[16]; static int g_n, g_i;

static void Run()
{
    bool exitSeen = false;
    Stream* stream = 0;
    do {
        Req r = g_q[g_i++];
        switch (r.type) {
        case 2: if (stream) { ++g_deleted; g_live = 0; } stream = 0; ++R.close; break;
        case 3: {
            bool ok = false; long pos = 0, user = 0;
            if (stream) { stream->seekMode = r.mode; stream->seekAmount = r.amount; ++stream->seeks;
                          long p = stream->position; if (p >= 0) { ok = true; pos = p; user = stream->user; } }
            ++R.seek; if (ok) { ++R.seekOk; R.lastPos = pos; R.lastUser = user; }
            break;
        }
        case 4: {
            long position = stream->position; long read = -1;
            if (position >= 0) { ++stream->reads; read = stream->readResult; if (read >= 0) position = stream->position + read; }
            r.frame->valid = read < 0 ? 0 : read;
            ++R.read; R.lastPos = position < 0 ? 0 : position; R.lastFrame = r.frame; R.lastUser = stream->user;
            break;
        }
        case 1: {
            static Stream s; s = r.proto; stream = &s; g_live = &s;
            ++R.open;
            if (s.channels >= 1 && s.rate >= 1 && s.unit >= 0) { ++R.openOk; R.lastChannels = s.channels; }
            else { ++g_deleted; g_live = 0; stream = 0; }
            break;
        }
        case 5: exitSeen = true; break;
        }
    } while (!exitSeen);
    ++R.exit;
}
int main()
{
    bool ok = true;
    Stream bad = { 0, 44100, 2, 0, 0, 0, 0, 0, 0, 0 }, good = { 2, 22050, 4, 10, 7, 100, 0, 0, 0, 0 };
    Frame f = { 2, 128, 999 };
    Req q[] = { { 3 }, { 1, bad }, { 1, good }, { 3, {}, 1, 2.5f }, { 4, {}, 0, 0, &f }, { 2 }, { 9 }, { 5 }, { 2 } };
    std::memcpy(g_q, q, sizeof q); g_n = 9;
    Run();
    ok = ok && g_i == 8 && R.exit == 1 && R.seek == 2 && R.seekOk == 1 && R.open == 2 && R.openOk == 1 && R.lastChannels == 2
            && g_deleted == 2 && R.read == 1 && f.valid == 100 && R.lastPos == 110 && R.lastFrame == &f && R.lastUser == 7
            && R.close == 1 && g_live == 0;
    std::memset(&R, 0, sizeof R); g_i = 0;
    Stream errs = { 1, 8000, 0, 5, 3, -4, 0, 0, 0, 0 };
    Req q2[] = { { 1, errs }, { 4, {}, 0, 0, &f }, { 5 } };
    std::memcpy(g_q, q2, sizeof q2);
    Run();
    ok = ok && R.openOk == 1 && f.valid == 0 && R.lastPos == 5 && R.exit == 1;
    if (ok) std::printf("01215910_TEST PASS\n"); else std::printf("00c47f10_TEST FAIL\n");
    return ok ? 0 : 1;
}