#include <cstdio>
// Behaviour test: SaySpeech sets the talker and pending group and speaks only when CanSaySpeech allows it.
struct CThingCreatureBase { int id; };
struct CPtr {
    CThingCreatureBase* p;
    CPtr() : p(0) { }
    CPtr(CThingCreatureBase* q) : p(q) { }
};
static int g_attempts = 0;
struct CombatDialogueManager {
    CPtr PCurrentTalker;
    unsigned int CurrentTalkerTextGroupIndex;
    bool allow;
    bool CanSaySpeech(CThingCreatureBase&, unsigned int) const { return allow; }
    void AttemptToSpeak() { ++g_attempts; }
    bool SaySpeech(CThingCreatureBase& creature, unsigned int textGroup)
    {
        if (CanSaySpeech(creature, textGroup))
        {
            PCurrentTalker = CPtr(&creature);
            CurrentTalkerTextGroupIndex = textGroup;
            AttemptToSpeak();
            return true;
        }
        return false;
    }
};
int main()
{
    CThingCreatureBase c; c.id = 1;
    CombatDialogueManager m; m.CurrentTalkerTextGroupIndex = 0; m.allow = false;
    if (m.SaySpeech(c, 7) || m.PCurrentTalker.p != 0 || m.CurrentTalkerTextGroupIndex != 0 || g_attempts != 0)
    { std::printf("008afc40_TEST FAIL\n"); return 1; }
    m.allow = true;
    if (!m.SaySpeech(c, 7) || m.PCurrentTalker.p != &c || m.CurrentTalkerTextGroupIndex != 7 || g_attempts != 1)
    { std::printf("008afc40_TEST FAIL\n"); return 1; }
    std::printf("008afc40_TEST PASS\n");
    return 0;
}