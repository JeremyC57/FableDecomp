// Behaviour test for 0x00944940 CAIStateGroup_WorkSawingWood::Update: repeated work sessions -- move to the action point (repeat while in
// progress; when work time ends terminate at once), require arrival and the look-at, play the intro; the session end frame is chosen once
// (frame + fps * (r + 0.5) * 30, only while still 0); saw while each loop animation finishes, he stays within 0.5 (2D) and the end is
// ahead (a failed loop animation terminates immediately, no outro); the outro must finish and the group must not be stopping; the whole
// session repeats while the end frame is ahead. Terminate once at the end.
#include <cstdio>
#include <vector>
#include <string>
enum { FINISHED = 2, FAILED = 3, IN_PROGRESS = 4 };
struct Sim {
    std::vector<int> moves; size_t mi; std::vector<int> work; size_t wi; int look;
    std::vector<int> anims; size_t ai; std::vector<float> dist2; size_t di; bool stopping;
    long frame, fps; int terminated, randoms; std::vector<std::string> played;
    Sim() : mi(0), wi(0), look(FINISHED), ai(0), di(0), stopping(false), frame(100), fps(15), terminated(0), randoms(0) { }
    int Move() { return mi < moves.size() ? moves[mi++] : FINISHED; }
    bool Work() { return wi < work.size() ? work[wi++] != 0 : true; }
    int Play(const char* n) { played.push_back(n); return ai < anims.size() ? anims[ai++] : FINISHED; }
    float D2() { return di < dist2.size() ? dist2[di++] : 1.0f; }
    long GetFrame() { return frame++; }
};
static void Update(Sim& s)
{
    long end = 0;
    do {
        int result;
        do {
            result = s.Move();
            if (!s.Work()) { s.terminated++; return; }
        } while (result == IN_PROGRESS);
        if (result != FINISHED) break;
        if (s.look != FINISHED) break;
        if (s.Play("INTO") != FINISHED) break;
        if (end == 0) { s.randoms++; end = (long)(s.GetFrame() + s.fps * (0.25f + 0.5) * 30.0); }
        do {
            if (s.Play("LOOP") != FINISHED) { s.terminated++; return; }
        } while (s.D2() < 0.25f && end > s.GetFrame());
        if (s.Play("OUTOF") != FINISHED) break;
        if (s.stopping) break;
    } while (end > s.GetFrame());
    s.terminated++;
}
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { ++fails; std::printf("FAIL %s\n", w); } }
int main()
{
    { Sim s; s.moves.push_back(IN_PROGRESS); s.work.push_back(1); s.work.push_back(0); Update(s);
      check(s.terminated == 1 && s.played.empty() && s.wi == 2, "work time ends while moving"); }
    { Sim s; s.moves.push_back(FAILED); Update(s); check(s.terminated == 1 && s.played.empty(), "move failed"); }
    { Sim s; s.look = FAILED; Update(s); check(s.terminated == 1 && s.played.empty(), "look failed"); }
    { Sim s; s.anims.push_back(FAILED); Update(s); check(s.played.size() == 1 && s.randoms == 0, "intro failed: no session"); }
    { Sim s; s.anims.push_back(FINISHED); s.anims.push_back(FAILED); Update(s);
      check(s.played.size() == 2 && s.terminated == 1, "loop failed: no outro"); }
    { Sim s; s.stopping = true; s.dist2.push_back(0.1f); Update(s);
      check(s.played.size() == 4 && s.played[3] == "OUTOF" && s.terminated == 1, "stopping after outro"); }
    { Sim s; s.fps = 1; Update(s);   // end frame 100 + 22 = 122: several sessions, the end chosen once
      check(s.randoms == 1 && s.played.size() > 3 && s.played.size() % 3 == 0 && s.terminated == 1, "sessions repeat, end chosen once"); }
    { Sim s; s.anims.push_back(FINISHED); s.anims.push_back(FINISHED); s.anims.push_back(FAILED); Update(s);
      check(s.played.size() == 3 && s.terminated == 1, "outro failed"); }
    if (fails == 0) std::printf("00944940_TEST PASS\n");
    return fails;
}
