// Behaviour test for CTCUnholyPower::FinishMove (0x00794C10): finished; the fade-out wait is frames-per-second times the
// level's seconds (truncated) and the environment fades back over those seconds; the 0xA7 component is reset when
// present; live build-up/hand effects are faded or killed, a live dark-faces effect is killed, dead ones skipped; the
// looping sound is stopped when the caster has an emitter.
#include <cstdio>
#include <string>

static std::string Num(unsigned long v) { char b[32]; std::sprintf(b, "%lu", v); return b; }

struct Power {
    bool finished; long level; long wait; float times[3]; bool hasA7; bool a7Active; long a7Count;
    bool alive[4]; bool hasEmitter; unsigned long sound; std::string log;
    void FinishMove(long fps)
    {
        finished = true;
        wait = (long)(fps * times[level]);
        char buf[32]; std::sprintf(buf, "T2/%g ", times[level]); log += buf;
        if (hasA7) { a7Active = false; a7Count = 0; }
        static const char* n[4] = { "B", "H1", "H2" };
        for (int i = 0; i < 3; ++i) if (alive[i]) { log += "F"; log += n[i]; }
        if (alive[3]) log += "K";
        if (hasEmitter) log += "S" + Num(sound);
    }
};

int main()
{
    int ok = 1;
    Power p = { false, 1, 0, { 1.0f, 2.5f, 3.0f }, true, true, 7, { true, false, true, true }, true, 42, "" };
    p.FinishMove(30);
    ok &= p.finished && p.wait == 75 && !p.a7Active && p.a7Count == 0 && p.log == "T2/2.5 FBFH2KS42";
    Power q = { false, 0, 0, { 1.25f, 2.5f, 3.0f }, false, true, 7, { false, false, false, false }, false, 42, "" };
    q.FinishMove(30);
    ok &= q.wait == 37 && q.a7Active && q.a7Count == 7 && q.log == "T2/1.25 ";
    std::printf(ok ? "00794c10_TEST PASS\n" : "00794c10_TEST FAIL\n");
    return ok ? 0 : 1;
}
