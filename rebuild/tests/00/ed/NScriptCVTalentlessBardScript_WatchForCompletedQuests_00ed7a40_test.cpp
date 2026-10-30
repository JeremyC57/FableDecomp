// Behaviour test for NScript::CV_TalentlessBardScript::WatchForCompletedQuests (0x00ED7A40): the four pre-completed quests set
// their song flags, each quest-completed message sets the quest's song flag, QS_ScytheInfo clears song 3, Q_DragonBossFight
// sets it only when the hero did not wear the mask, and the thread stops when terminating.
#include <cstdio>
#include <cstring>

struct Str {
    const char* p;                                      // 0 = the null string
    Str() : p(0) { }
    bool operator==(const char* t) const { return std::strcmp(p ? p : "", t) == 0; }
};

struct Game {
    const char* completed[4];
    int ncompleted;
    const char* msgs[8];
    int nmsgs, next, frames;
    int pendingFrames;                                  // frames to wait before the next message is delivered
    bool IsQuestCompleted(const Str& s) const
    {
        for (int i = 0; i < ncompleted; ++i)
            if (s.p && std::strcmp(completed[i], s.p) == 0)
                return true;
        return false;
    }
    bool MsgOnAnyQuestCompleted(Str& out)
    {
        if (pendingFrames > 0 || next >= nmsgs)
            return false;
        out.p = msgs[next++];
        pendingFrames = 1;
        return true;
    }
    void NewScriptFrame() { ++frames; if (pendingFrames > 0) --pendingFrames; }
};

struct Bard {
    Game* game;
    bool heroWoreMask;
    Str questName[12];
    bool song[12];
    int termBudget;                                     // IsActiveThreadTerminating returns true once this runs out
    bool IsActiveThreadTerminating() { if (termBudget <= 0) return true; --termBudget; return false; }
    long GetQuestIndex(Str name)
    {
        long i;
        for (i = 0; i < 12; ++i)
            if (questName[i].p && name.p && std::strcmp(questName[i].p, name.p) == 0)
                break;
        if (i == 12) {
            if (IsActiveThreadTerminating())
                return -1;
            i = -1;
        }
        return i;
    }
    void Run()
    {
        Str q;
        if (game->IsQuestCompleted(questName[9])) { if (IsActiveThreadTerminating()) return; song[9] = true; }
        if (game->IsQuestCompleted(questName[5])) { if (IsActiveThreadTerminating()) return; song[5] = true; }
        if (game->IsQuestCompleted(questName[6])) { if (IsActiveThreadTerminating()) return; song[6] = true; }
        if (game->IsQuestCompleted(questName[10])) { if (IsActiveThreadTerminating()) return; song[10] = true; }
        while (!IsActiveThreadTerminating()) {
            while (!game->MsgOnAnyQuestCompleted(q)) {
                game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            long index = GetQuestIndex(q);
            if (index != -1) {
                if (IsActiveThreadTerminating())
                    return;
                song[index] = true;
            }
            if (q == "QS_ScytheInfo") {
                if (IsActiveThreadTerminating())
                    return;
                song[3] = false;
            } else if (q == "Q_DragonBossFight") {
                if (!heroWoreMask) {
                    if (IsActiveThreadTerminating())
                        return;
                    song[3] = true;
                }
            }
            game->NewScriptFrame();
        }
    }
};

static const char* kNames[12] = { "Q0", "Q1", "Q2", "Q3", "Q4", "Q5", "Q6", "Q7", "Q8", "Q9", "Q10", "Q11" };

static void Setup(Bard& b, Game& g, bool mask)
{
    for (int i = 0; i < 12; ++i) { b.questName[i].p = kNames[i]; b.song[i] = false; }
    b.game = &g; b.heroWoreMask = mask; b.termBudget = 200;
    g.ncompleted = 0; g.nmsgs = 0; g.next = 0; g.frames = 0; g.pendingFrames = 0;
}

int main()
{
    Game g; Bard b;
    // pre-completed: Q5 and Q10 (Q1 is completed too but is not one of the four checked songs)
    Setup(b, g, false);
    g.completed[0] = "Q5"; g.completed[1] = "Q10"; g.completed[2] = "Q1"; g.ncompleted = 3;
    g.msgs[0] = "Q2"; g.msgs[1] = "Q_DragonBossFight"; g.msgs[2] = "QS_ScytheInfo"; g.nmsgs = 3;
    b.Run();
    if (!b.song[5] || !b.song[10] || b.song[9] || b.song[6] || b.song[1] || !b.song[2] || b.song[3] || g.next != 3) {
        std::printf("00ed7a40_TEST FAIL\n"); return 1;
    }
    // Dragon boss fight without the mask leaves song 3 set; with the mask it stays clear
    Setup(b, g, false);
    g.msgs[0] = "Q_DragonBossFight"; g.nmsgs = 1;
    b.Run();
    if (!b.song[3]) { std::printf("00ed7a40_TEST FAIL\n"); return 1; }
    Setup(b, g, true);
    g.msgs[0] = "Q_DragonBossFight"; g.nmsgs = 1;
    b.Run();
    if (b.song[3]) { std::printf("00ed7a40_TEST FAIL\n"); return 1; }
    // an immediately terminating thread marks nothing even when a quest is already complete
    Setup(b, g, false);
    g.completed[0] = "Q9"; g.ncompleted = 1; b.termBudget = 0;
    b.Run();
    if (b.song[9]) { std::printf("00ed7a40_TEST FAIL\n"); return 1; }
    std::printf("00ed7a40_TEST PASS\n");
    return 0;
}