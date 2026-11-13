// Behaviour test for NScript::CQ_WaspBossScript::CWaspChaseWoman::Main (0x00E10E60): a host model of the script's statements
// (same control flow as src/00e10e60.cpp) driven by a scripted fake game interface. Checks the StartChase wait, the
// ChasedWomanNav route ((nav + 1) % 6 order, re-issued when the task ends or the next nav is reached), the saved-by-hero
// conversation, the SavedVillagerCount < 2 morality gate, the escape run and the fade, and every terminate exit.
#include <cstdio>
#include <cstring>

static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { std::printf("check failed line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)

struct Thing { char name[32]; bool alive; };

struct World {
    // scripted inputs
    int terminate_at;        // number of IsActiveThreadTerminating calls before it returns true (-1 = never)
    int t_calls;
    int start_chase_after;   // frames until StartChase becomes true
    int chaser_dies_after;   // frames until the chaser is dead
    int task_busy_frames;    // the route task reports busy this many polls after each FollowPreCalculatedRoute
    int escape_frames;       // frames until the woman is within 2 of VillagerEscapePos
    bool queen_attacks;
    long saved_count;
    // observed
    int frames;
    int task_left;
    int follows; char follow_names[64][32];
    int clears;
    bool removed;
    bool scared_off;
    int lines; char line_text[64];
    int morality_given; float morality;
    int moves;
    int fades; float fade_time;
    bool timer_registered, timer_released; long timer_value;
};
static World W;

static bool IsActiveThreadTerminating()
{
    if (W.terminate_at >= 0 && W.t_calls++ >= W.terminate_at) return true;
    return false;
}
static void NewScriptFrame() { ++W.frames; if (W.task_left > 0) --W.task_left; }
static Thing GetThingWithScriptName(const char* name)
{
    Thing t; std::strncpy(t.name, name, 31); t.name[31] = 0; t.alive = true;
    return t;
}
static void FollowPreCalculatedRoute(const Thing& t)
{
    if (W.follows < 64) std::strcpy(W.follow_names[W.follows], t.name);
    ++W.follows; W.task_left = W.task_busy_frames;
}
static bool IsPerformingScriptTask() { return W.task_left > 0; }
static bool IsDistanceBetweenThingsUnder(const Thing&, float) { return false; }
static bool ChaserAlive() { return W.frames < W.chaser_dies_after; }
static bool IsDistanceBetweenThingsOver(float) { return W.frames < W.escape_frames; }

struct Timer {   // CTimer: register on construction, deregister on destruction, operator= sets the time
    Timer() { W.timer_registered = true; }
    ~Timer() { W.timer_released = true; }
    Timer& operator=(long t) { W.timer_value = t; return *this; }
};

static void NavName(char* out, long n) { std::sprintf(out, "ChasedWomanNav%ld", n); }

// The model: src/00e10e60.cpp statement for statement.
static void Main()
{
    NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    // StartScriptingEntity succeeds at once in this model (the wait loop is the CWaspVictim one, tested there).
    if (IsActiveThreadTerminating()) return;
    if (W.queen_attacks) {
        if (IsActiveThreadTerminating()) return;
        W.removed = true;
    }
    while (!(W.frames >= W.start_chase_after)) {
        NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Thing chaser = GetThingWithScriptName("WaspChaser");
    long nav = 2;
    char buf[32];
    NavName(buf, nav + 1);
    Thing next_nav = GetThingWithScriptName(buf);
    NavName(buf, nav);
    FollowPreCalculatedRoute(GetThingWithScriptName(buf));
    Timer timer;
    timer = 11;
    for (;; NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return;
        if (!IsPerformingScriptTask() || IsDistanceBetweenThingsUnder(next_nav, 2.0f)) {
            if (IsActiveThreadTerminating()) return;
            nav = (nav + 1) % 6;
            NavName(buf, (nav + 1) % 6);
            next_nav = GetThingWithScriptName(buf);
            ++W.clears;
            NavName(buf, nav);
            FollowPreCalculatedRoute(GetThingWithScriptName(buf));
        }
        (void)chaser;
        if (!ChaserAlive()) {
            if (IsActiveThreadTerminating()) return;
            W.scared_off = true;
            ++W.lines; std::strcpy(W.line_text, "TEXT_QST_072_WASP_CHASE_FEMALE_ON_SAVED_10");
            if (W.saved_count < 2) {
                if (IsActiveThreadTerminating()) return;
                ++W.morality_given; W.morality = 0.01f;
                W.saved_count++;
            }
            while (IsDistanceBetweenThingsOver(2.0f)) {
                NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
                if (!IsPerformingScriptTask()) {
                    if (IsActiveThreadTerminating()) return;
                    ++W.moves; W.task_left = 3;
                }
            }
            if (IsActiveThreadTerminating()) return;
            ++W.fades; W.fade_time = 2.5f;
            break;
        }
    }
}

static void Reset()
{
    std::memset(&W, 0, sizeof W);
    W.terminate_at = -1;
}

int main()
{
    // 1. full run: chase for 20 frames, route tasks last 3 polls, then saved; second villager gets morality
    Reset();
    W.start_chase_after = 4; W.chaser_dies_after = 24; W.task_busy_frames = 3; W.escape_frames = 40; W.saved_count = 1;
    Main();
    CHECK(W.follows >= 2);
    CHECK(std::strcmp(W.follow_names[0], "ChasedWomanNav2") == 0);
    CHECK(std::strcmp(W.follow_names[1], "ChasedWomanNav3") == 0);
    {
        long n = 2; int i;
        for (i = 1; i < W.follows && i < 64; ++i) {
            char b[32]; n = (n + 1) % 6; NavName(b, n);
            CHECK(std::strcmp(W.follow_names[i], b) == 0);
        }
    }
    CHECK(W.clears == W.follows - 1);
    CHECK(W.timer_registered && W.timer_released && W.timer_value == 11);
    CHECK(W.scared_off && W.lines == 1 && std::strcmp(W.line_text, "TEXT_QST_072_WASP_CHASE_FEMALE_ON_SAVED_10") == 0);
    CHECK(W.morality_given == 1 && W.saved_count == 2);
    CHECK(W.moves >= 1 && W.fades == 1 && W.fade_time > 2.49f && W.fade_time < 2.51f);
    CHECK(!W.removed);

    // 2. third villager: no morality; queen already attacking removes the woman but the script carries on
    Reset();
    W.start_chase_after = 1; W.chaser_dies_after = 5; W.task_busy_frames = 100; W.escape_frames = 0; W.saved_count = 2;
    W.queen_attacks = true;
    Main();
    CHECK(W.removed);
    CHECK(W.follows == 1 && W.clears == 0);
    CHECK(W.morality_given == 0 && W.saved_count == 2);
    CHECK(W.moves == 0 && W.fades == 1);

    // 3. terminate during the chase: no conversation, timer released
    Reset();
    W.start_chase_after = 0; W.chaser_dies_after = 1000; W.task_busy_frames = 2; W.terminate_at = 12;
    Main();
    CHECK(W.timer_registered && W.timer_released);
    CHECK(W.lines == 0 && W.fades == 0);

    // 4. terminate at once: nothing happens
    Reset();
    W.terminate_at = 0;
    Main();
    CHECK(W.follows == 0 && !W.timer_registered && W.fades == 0);

    if (g_fail) { std::printf("00e10e60_TEST FAIL\n"); return 1; }
    std::printf("00e10e60_TEST PASS\n");
    return 0;
}