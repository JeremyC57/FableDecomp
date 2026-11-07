// Behaviour test for NScript::CQ_BanditCampBossBattleScript::CCROWDBANDITS::Main (0x00D0B3E0): during the king fight a crowd
// bandit attacks a hero closer than 3.5, reacts only beyond 6.5 when its timer has run out, picks the boast set when the king lost
// health, the wave set when the hero is more than 8 from the king, else the cocky set; afterwards it turns on the hero when the
// angry-bandit counter is set or the hero hits it (a heal-life spell does not count) and decrements the counter.
#include <cstdio>

enum { NONE, ATTACK, BOAST, WAVE, COCKY };
static int React(float heroDist, long timer, bool kingHurt, float heroToKing)
{
    if (heroDist < 3.5f) return ATTACK;
    if (!(heroDist > 6.5f) || timer > 0) return NONE;
    if (kingHurt) return BOAST;
    if (heroToKing > 8.0f) return WAVE;
    return COCKY;
}
static bool TurnsHostile(long& angryNeeded, bool hitByHero, bool hitBySpecial, bool specialIsHealLife)
{
    if (!(angryNeeded > 0 || hitByHero || (hitBySpecial && !specialIsHealLife))) return false;
    if (angryNeeded != 0) --angryNeeded;
    return true;
}

int main()
{
    bool ok = React(2.0f, 5, false, 0.0f) == ATTACK && React(5.0f, 0, true, 0.0f) == NONE && React(7.0f, 1, true, 0.0f) == NONE
           && React(7.0f, 0, true, 9.0f) == BOAST && React(7.0f, 0, false, 9.0f) == WAVE && React(7.0f, 0, false, 3.0f) == COCKY;
    long angry = 0;
    ok = ok && !TurnsHostile(angry, false, true, true) && TurnsHostile(angry, false, true, false) && angry == 0;
    angry = 2;
    ok = ok && TurnsHostile(angry, false, false, false) && angry == 1;
    if (!ok) { std::printf("00d0b3e0_TEST FAIL\n"); return 1; }
    std::printf("00d0b3e0_TEST PASS\n");
    return 0;
}
