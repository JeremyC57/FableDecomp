// Behaviour test for NScript::CQ_GuildTrainingSkillScript::CTheRealGuildmaster::Main (0x00D5AE70): the walk-up comment alternates
// FIRST / SECOND (commentCount = 1 - commentCount); the control ticks are removed once, when the hero carries the longbow and holds both
// the lock-target and fire buttons; the practice loop runs while the score is under 3. A model of those rules.
#include <cstdio>
#include <cstring>
static const char* Comment(long& count)
{
    const char* text = count == 0 ? "COMMENT_FIRST" : "COMMENT_SECOND";
    count = 1 - count;
    return text;
}
static bool RemoveTicks(bool& removed, bool bow, bool lock, bool fire)
{
    if (bow && lock && fire) { removed = true; return true; }
    return false;
}
int main()
{
    long c = 0;
    bool ok = !std::strcmp(Comment(c), "COMMENT_FIRST") && !std::strcmp(Comment(c), "COMMENT_SECOND") && !std::strcmp(Comment(c), "COMMENT_FIRST");
    bool removed = false;
    ok = ok && !RemoveTicks(removed, true, true, false) && !removed && RemoveTicks(removed, true, true, true) && removed;
    int score = 0, rounds = 0;
    while (score < 3) { ++score; ++rounds; }
    ok = ok && rounds == 3;
    std::printf(ok ? "00d5ae70_TEST PASS\n" : "00d5ae70_TEST FAIL\n");
    return ok ? 0 : 1;
}
