// Behaviour test for CGate1GuardInner::Main (0x00D02B80): talk movie, then attack when the hero hits the guard
// (but not with special ability 14), or immediately once the outer guards were attacked.
#include <cstdio>
#include <cstring>

static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }

struct World {
    int frame;
    int stopFrame;      // thread terminates from this frame on
    int talkFrame;      // frame at which the hero talks
    int hitFrame;       // frame at which the hero hits
    int hitAbility;     // -1 plain hit, else special ability id
    float health;
    int speakFrames;
};
static World w;

struct CQ_BanditCampScript { bool AttackedOuterGateGuards; bool Gate1Open; };
struct Guard {
    int busy;
    bool IsScripting() const { return false; }
    void Release() { Log("release"); }
    void Speak(const char* text) { Log(text); busy = w.speakFrames; }
    bool IsPerformingScriptTask() { return busy-- > 0; }
};
struct CGate1GuardInner {
    CQ_BanditCampScript* ParentClass;
    void NewScriptFrame() { ++w.frame; }
    bool IsActiveThreadTerminating() const { return w.frame >= w.stopFrame; }
    bool MsgIsTalkedToBy() const { return w.frame == w.talkFrame; }
    bool MsgIsHitBy() const { return w.frame == w.hitFrame && w.hitAbility < 0; }
    bool MsgIsHitByAnySpecialAbilityFrom() const { return w.frame == w.hitFrame && w.hitAbility >= 0; }
    bool MsgIsHitBySpecialAbilityFrom(int a) const { return w.frame == w.hitFrame && w.hitAbility == a; }
    void Pause(bool on) { Log(on ? "pause" : "unpause"); }
    void Attack() { Log("attack"); }
    void Main();
};
void CGate1GuardInner::Main()
{
    NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    Guard guard; guard.busy = 0;
    for (;;) {
        if (!ParentClass->Gate1Open) {
            if (IsActiveThreadTerminating()) return;
        } else {
            if (IsActiveThreadTerminating()) return;
            if (MsgIsTalkedToBy()) {
                Pause(true);
                if (w.health > 0.0f) {
                    guard.Speak("ASIDE");
                    while (guard.IsPerformingScriptTask()) {
                        NewScriptFrame();
                        if (IsActiveThreadTerminating()) { Pause(false); return; }
                    }
                }
                Pause(false);
            }
            if (ParentClass->AttackedOuterGateGuards) {
                Attack();
                return;
            }
            if (!ParentClass->AttackedOuterGateGuards
                && (MsgIsHitBy() || (MsgIsHitByAnySpecialAbilityFrom() && !MsgIsHitBySpecialAbilityFrom(14)))) {
                Pause(true);
                if (w.health > 0.0f) {
                    guard.Speak("ATTACKED");
                    while (guard.IsPerformingScriptTask()) {
                        NewScriptFrame();
                        if (IsActiveThreadTerminating()) { Pause(false); return; }
                    }
                }
                Pause(false);
                ParentClass->AttackedOuterGateGuards = true;
                Attack();
                return;
            }
        }
        NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
}
static bool Run(bool open, bool attacked, int talk, int hit, int ability, const char* expect)
{
    CQ_BanditCampScript p; p.Gate1Open = open; p.AttackedOuterGateGuards = attacked;
    CGate1GuardInner s; s.ParentClass = &p;
    w.frame = 0; w.stopFrame = 40; w.talkFrame = talk; w.hitFrame = hit; w.hitAbility = ability;
    w.health = 1.0f; w.speakFrames = 2;
    g_log[0] = 0;
    s.Main();
    return std::strcmp(g_log, expect) == 0;
}
int main()
{
    if (!Run(false, false, 3, 5, -1, "")
        || !Run(true, false, 3, -1, -1, "pause;ASIDE;unpause;")
        || !Run(true, false, -1, 5, -1, "pause;ATTACKED;unpause;attack;")
        || !Run(true, false, -1, 5, 14, "")
        || !Run(true, false, -1, 5, 3, "pause;ATTACKED;unpause;attack;")
        || !Run(true, true, -1, -1, -1, "attack;")) {
        std::printf("00d02b80_TEST FAIL\n");
        return 1;
    }
    std::printf("00d02b80_TEST PASS\n");
    return 0;
}