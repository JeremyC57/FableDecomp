// behaviour test for CTCEnemy::IsEnemyOfFaction (0x0076C7D0): null faction or no own faction -> false without asking;
// otherwise the own faction's answer for *faction
#include <cstdio>
static const void* gAskedThis = 0;
static const void* gAskedArg = 0;
class CFaction {
public:
    bool Enemy;
    bool IsEnemyOfFaction(const CFaction& faction) const { gAskedThis = this; gAskedArg = &faction; return Enemy; }
};
class CTCEnemy {
public:
    virtual ~CTCEnemy() { }
    unsigned char _base04[8];
    bool FriendsWithEverythingFlag;
    bool EnableFollowersEnemyProxy;
    void* EnemyThings;
    void* AllyThings;
    CFaction* PFaction;
    bool IsEnemyOfFaction(const CFaction* faction) const;
};
bool CTCEnemy::IsEnemyOfFaction(const CFaction* faction) const
{
    if (faction && PFaction)
        return PFaction->IsEnemyOfFaction(*faction);
    return false;
}
int main()
{
    CFaction mine, other;
    CTCEnemy e;
    e.PFaction = 0;
    bool ok = !e.IsEnemyOfFaction(&other) && gAskedThis == 0;
    e.PFaction = &mine;
    ok = ok && !e.IsEnemyOfFaction(0) && gAskedThis == 0;
    mine.Enemy = true;
    ok = ok && e.IsEnemyOfFaction(&other) && gAskedThis == &mine && gAskedArg == &other;
    mine.Enemy = false;
    ok = ok && !e.IsEnemyOfFaction(&other);
    if (!ok) { std::printf("0076c7d0_TEST FAIL\n"); return 1; }
    std::printf("0076c7d0_TEST PASS\n");
    return 0;
}
