// Behaviour test for CInstance::Update 0x00C38D60: the state machine runs until a port has no message; 1 -> 2 creates the
// streamer and primes `prime` silenced frames (refused start -> 0, stop request -> detach and 4); 2 -> 3 attaches the ready
// streamer; 3 loops back to 2 while the frame is flagged and the ramp is not settled, else detaches and goes to 4, always
// advancing the ramp and handing the frame back; 4 -> 0 deletes the streamer and clears the ids; 5 -> 6; 0 / 6 / other exit.
#include <cstdio>
#include <cstring>

struct Item { long a, b, flag; };
struct Streamer { int taken, silenced, gets; unsigned long frames, samples; float scale; long playing, position; Item item; };
struct Inst {
    bool ports[5];                     // 0c, 40, 74, ac, d8
    long state; Streamer* s; long ids[3]; float scale;
    bool queryOk, stopFlag; long ready; bool settled; int detaches, attaches, stops, finals, asserts, deletes; long advancedBy;
    void Update(unsigned long frames, unsigned long samples, unsigned long prime)
    {
        static Streamer store;
        bool done = false;
        while (!done) {
            switch (state) {
            case 0: done = true; break;
            case 1:
                if (!ports[3] || !ports[1]) return;
                if (!queryOk) { if (stopFlag) { state = 4; ++detaches; } else state = 0; return; }
                std::memset(&store, 0, sizeof store); store.frames = frames; store.samples = samples; store.scale = scale; s = &store;
                if (prime > frames) ++asserts;
                for (unsigned long i = 0; i < prime; ++i) { ++s->gets; ++s->silenced; ++s->taken; }
                state = 2; break;
            case 2: if (!ready) return; ++attaches; state = 3; break;
            case 3:
                if (!ports[2]) return;
                if (s->item.flag && !settled) state = 2; else { state = 4; ++detaches; }
                advancedBy = s->position; ++s->taken;
                if (state == 2) ports[2] = false;   // the next frame is not back yet
                break;
            case 4:
                if (!ports[4]) return;
                if (s && s->playing) return;
                ++stops; if (s) { ++deletes; s = 0; }
                state = 0; ids[1] = ids[2] = ids[0] = -1; return;
            case 5: if (!ports[0]) return; ++finals; state = 6; return;
            case 6: done = true; break;
            default: done = true; break;
            }
        }
    }
};
int main()
{
    bool ok = true;
    Inst in; std::memset(&in, 0, sizeof in); in.scale = 0.5f;
    in.state = 1; in.Update(4, 256, 2);                                   // ports empty: stays
    ok = ok && in.state == 1 && in.s == 0;
    in.ports[3] = in.ports[1] = true; in.queryOk = false; in.stopFlag = false;
    in.Update(4, 256, 2); ok = ok && in.state == 0;                        // refused
    in.state = 1; in.stopFlag = true; in.Update(4, 256, 2); ok = ok && in.state == 4 && in.detaches == 1;
    in.state = 1; in.queryOk = true; in.ready = 0; in.Update(4, 256, 3);   // created, primed, waits in 2
    ok = ok && in.state == 2 && in.s && in.s->silenced == 3 && in.s->taken == 3 && in.s->frames == 4 && in.s->scale == 0.5f && in.asserts == 0;
    in.ready = 1; in.ports[2] = true; in.s->item.flag = 1; in.settled = false; in.s->position = 77;
    in.Update(4, 256, 0);                                                  // 2 -> 3 -> 2 (flagged), then 3 waits
    ok = ok && in.state == 3 && in.attaches == 2 && in.advancedBy == 77 && in.s->taken == 4;
    in.ports[2] = true; in.settled = true; in.ports[4] = true; in.s->playing = 1;
    in.Update(4, 256, 0);                                                  // 3 -> 4, streamer busy: waits
    ok = ok && in.state == 4 && in.detaches == 2 && in.s != 0;
    in.s->playing = 0; in.Update(4, 256, 0);
    ok = ok && in.state == 0 && in.s == 0 && in.deletes == 1 && in.ids[0] == -1 && in.ids[2] == -1 && in.stops == 1;
    in.state = 1; in.Update(2, 256, 5); ok = ok && in.asserts == 1;       // priming more than the frame count asserts
    in.state = 5; in.ports[0] = true; in.Update(0, 0, 0); ok = ok && in.state == 6 && in.finals == 1;
    in.Update(0, 0, 0); ok = ok && in.state == 6;
    in.state = 9; in.Update(0, 0, 0); ok = ok && in.state == 9;
    if (ok) std::printf("01214aaa_TEST PASS\n"); else std::printf("00c38d60_TEST FAIL\n");
    return ok ? 0 : 1;
}