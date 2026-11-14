// Behaviour test for 0x00945060 CAIStateGroup_WorkHammerAnvil::Update: move to the action point (repeat while in progress, abort when
// work time ends), require arrival and a successful look-at, play the intro (failure: terminate + pushable again, no loop), make the
// creature unpushable, hammer while each loop animation finishes, he stays within 0.5 (2D) of the point and the end frame is ahead
// (a failed loop animation makes him pushable, terminates and still plays the outro), then the outro: on success the next hammer frame
// is scheduled (frame + fps * (r + 0.5) * 20); always pushable again and terminate.
#include <cstdio>
#include <vector>
#include <string>
enum { NUL = 0, FINISHED = 2, FAILED = 3, IN_PROGRESS = 4 };
struct Sim {
    std::vector<int> moves; size_t mi; bool workTime; int look;
    std::vector<int> anims; size_t ai;          // results of successive animations
    std::vector<float> dist2; size_t di;        // squared 2D distance per loop check
    long frame, fps; float rnd;
    long next; int terminated; std::vector<std::string> played; std::vector<int> pushable; std::vector<int> scratch;
    Sim() : mi(0), workTime(true), look(FINISHED), ai(0), di(0), frame(100), fps(15), rnd(0.25f), next(-1), terminated(0) { }
    int Move() { return mi < moves.size() ? moves[mi++] : FINISHED; }
    int Play(const char* n) { played.push_back(n); return ai < anims.size() ? anims[ai++] : FINISHED; }
    float D2() { return di < dist2.size() ? dist2[di++] : 1.0f; }
    long GetFrame() { return frame++; }
};
static void Update(Sim& s)
{
    int result;
    do {
        result = s.Move();
        if (!s.workTime) { s.terminated++; return; }
    } while (result == IN_PROGRESS);
    if (result != FINISHED) { s.terminated++; return; }
    if (s.look != FINISHED) { s.terminated++; return; }
    s.pushable.push_back(0);
    if (s.Play("INTO") != FINISHED) { s.terminated++; s.pushable.push_back(1); return; }
    s.scratch.push_back(0);
    long end = (long)(s.GetFrame() + s.fps * (s.rnd + 0.5) * 30.0);
    do {
        result = s.Play("LOOP");
        if (result != FINISHED) { s.scratch.push_back(1); s.terminated++; break; }
    } while (s.D2() < 0.25f && end > s.GetFrame());
    if (s.Play("OUTOF") == FINISHED)
        s.next = (long)(s.GetFrame() + s.fps * (s.rnd + 0.5) * 20.0);
    s.pushable.push_back(1);
    s.terminated++;
}
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { ++fails; std::printf("FAIL %s\n", w); } }
int main()
{
    { Sim s; s.moves.push_back(IN_PROGRESS); s.moves.push_back(IN_PROGRESS); s.workTime = false; Update(s);
      check(s.terminated == 1 && s.played.empty() && s.mi == 1, "work time over while moving"); }
    { Sim s; s.moves.push_back(FAILED); Update(s); check(s.terminated == 1 && s.played.empty(), "move failed"); }
    { Sim s; s.look = FAILED; Update(s); check(s.terminated == 1 && s.played.empty() && s.pushable.empty(), "look failed"); }
    { Sim s; s.anims.push_back(FAILED); Update(s);
      check(s.played.size() == 1 && s.pushable.size() == 2 && s.pushable[1] == 1 && s.scratch.empty() && s.next == -1, "intro failed"); }
    { Sim s; s.dist2.push_back(0.1f); s.dist2.push_back(0.1f); s.dist2.push_back(0.3f); Update(s);
      check(s.played.size() == 5 && s.played[1] == "LOOP" && s.played[3] == "LOOP" && s.played[4] == "OUTOF", "hammers until he leaves the spot");
      check(s.next > 0 && s.terminated == 1 && s.pushable.back() == 1 && s.scratch.size() == 1 && s.scratch[0] == 0, "outro schedules next"); }
    { Sim s; s.anims.push_back(FINISHED); s.anims.push_back(FAILED); Update(s);
      check(s.played.size() == 3 && s.played[2] == "OUTOF" && s.scratch.size() == 2 && s.scratch[1] == 1 && s.terminated == 2, "loop failure still plays outro"); }
    { Sim s; s.fps = 0; for (int i = 0; i < 5; ++i) s.dist2.push_back(0.0f); Update(s);
      check(s.played.size() == 3, "zero-length session: one loop"); }
    { Sim s; s.anims.push_back(FINISHED); s.anims.push_back(FINISHED); s.anims.push_back(FAILED); s.dist2.push_back(0.5f); Update(s);
      check(s.next == -1 && s.terminated == 1, "outro failed: no schedule"); }
    if (fails == 0) std::printf("00945060_TEST PASS\n");
    return fails;
}
