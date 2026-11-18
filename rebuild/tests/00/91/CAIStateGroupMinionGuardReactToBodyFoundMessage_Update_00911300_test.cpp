// Behaviour test for CAIStateGroup_MinionGuardReactToBodyFoundMessage::Update (0x00911300): no message -> terminate; dead / missing body
// -> terminate; otherwise the message is consumed, and unless the guard is strictly within 2.5 of the body it plays the curiosity action
// (must finish) and walks to the body while it stays alive (repeat while IN_PROGRESS, must finish); then, if the body is alive and the
// look-at finishes, it walks to the reported position (must finish) and orders a security sweep; it always terminates once.
// Model of the row's logic: the AI actions return scripted values and are logged.
#include <cstdio>
#include <cstring>
enum { TERMINATE_PROCESS = 1, FINISHED = 2, FAILED = 3, IN_PROGRESS = 4 };
struct Body { bool exists, dead; float x; };
static Body body; static bool have_message; static int removed, terminated;
static char log_[64]; static int nlog;
static int script[16]; static int nscript, iscript;
static int act(char c) { log_[nlog++] = c; return iscript < nscript ? script[iscript++] : FINISHED; }
static bool alive() { return body.exists && !body.dead; }
static int dying_after_moves; static int moves;
static void Update()
{
    if (have_message) {
        if (!alive()) { ++terminated; return; }
        ++removed;
        float d = body.x;                                  // guard at 0, body at x (3D distance)
        if (!(d * d < 2.5f * 2.5f)) {
            if (act('C') != FINISHED) { ++terminated; return; }
            int ret;
            do {
                if (!alive()) { ++terminated; return; }
                ret = act('M');
                if (++moves == dying_after_moves) body.dead = true;
            } while (ret == IN_PROGRESS);
            if (ret != FINISHED) { ++terminated; return; }
        }
        if (alive()) {
            if (act('L') == FINISHED) {
                if (act('P') != FINISHED) { ++terminated; return; }
                if (act('S') != FINISHED) { ++terminated; return; }
            }
        }
    }
    ++terminated;
}
static void run(bool msg, bool exists, bool dead, float x, const int* s, int n, int die = 0)
{
    have_message = msg; body.exists = exists; body.dead = dead; body.x = x; removed = terminated = 0; nlog = 0; moves = 0;
    dying_after_moves = die; std::memset(log_, 0, sizeof(log_)); nscript = n; iscript = 0;
    for (int i = 0; i < n; ++i) script[i] = s[i];
    Update();
}
static int fail(int n) { std::printf("00911300_TEST FAIL %d\n", n); return 1; }
int main()
{
    int ok[1] = { FINISHED };
    run(false, true, false, 5.0f, ok, 0); if (removed || terminated != 1 || nlog) return fail(1);           // no message
    run(true, false, false, 5.0f, ok, 0); if (removed || terminated != 1 || nlog) return fail(2);           // no body
    run(true, true, true, 5.0f, ok, 0); if (removed || terminated != 1 || nlog) return fail(3);             // dead body
    run(true, true, false, 1.0f, ok, 0); if (removed != 1 || terminated != 1 || std::strcmp(log_, "LPS")) return fail(4);   // close
    run(true, true, false, 2.5f, ok, 0); if (std::strcmp(log_, "CMLPS") || terminated != 1) return fail(5);  // exactly 2.5: walks
    { int s[] = { FAILED }; run(true, true, false, 5.0f, s, 1); if (std::strcmp(log_, "C") || terminated != 1) return fail(6); }
    { int s[] = { FINISHED, IN_PROGRESS, IN_PROGRESS, FINISHED }; run(true, true, false, 5.0f, s, 4);
      if (std::strcmp(log_, "CMMMLPS") || terminated != 1) return fail(7); }                               // repeat while in progress
    { int s[] = { FINISHED, IN_PROGRESS, FINISHED }; run(true, true, false, 5.0f, s, 3, 1);                // dies during the walk
      if (std::strcmp(log_, "CM") || terminated != 1) return fail(8); }
    { int s[] = { FINISHED, FAILED }; run(true, true, false, 5.0f, s, 2); if (std::strcmp(log_, "CM") || terminated != 1) return fail(9); }
    { int s[] = { FINISHED, FINISHED }; run(true, true, false, 5.0f, s, 2, 1);                               // dead after arriving
      if (std::strcmp(log_, "CM") || terminated != 1) return fail(10); }
    { int s[] = { FAILED }; run(true, true, false, 1.0f, s, 1); if (std::strcmp(log_, "L") || terminated != 1) return fail(11); }
    { int s[] = { FINISHED, FAILED }; run(true, true, false, 1.0f, s, 2); if (std::strcmp(log_, "LP") || terminated != 1) return fail(12); }
    { int s[] = { FINISHED, FINISHED, FAILED }; run(true, true, false, 1.0f, s, 3); if (std::strcmp(log_, "LPS") || terminated != 1) return fail(13); }
    std::printf("00911300_TEST PASS\n");
    return 0;
}