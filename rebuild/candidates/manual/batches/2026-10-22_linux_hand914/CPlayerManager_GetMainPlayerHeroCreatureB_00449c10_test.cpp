// Behaviour test for 00449c10 CPlayerManager::GetMainPlayerHeroCreatureB (hero creature of the main player)
#include <cstdio>
struct CThing { long id; };
struct CPlayer {
    CThing* hero;
    CThing* GetHeroCreature() { return hero; }
};
struct CPlayerManager {
    CPlayer** PlayersFirst;
    long MainPlayer;
    CPlayer* GetPlayer(long number) { return PlayersFirst[number]; }
    CThing* GetMainPlayerHeroCreatureB(long unused);
};
CThing* CPlayerManager::GetMainPlayerHeroCreatureB(long unused)
{
    return GetPlayer(MainPlayer)->GetHeroCreature();
}
int main()
{
    CThing h0 = { 10 }, h1 = { 11 };
    CPlayer p0 = { &h0 }, p1 = { &h1 };
    CPlayer* players[2] = { &p0, &p1 };
    CPlayerManager m; m.PlayersFirst = players;
    m.MainPlayer = 1;
    if (m.GetMainPlayerHeroCreatureB(0) != &h1) { std::printf("00449c10_TEST FAIL\n"); return 1; }
    m.MainPlayer = 0;
    if (m.GetMainPlayerHeroCreatureB(5) != &h0) { std::printf("00449c10_TEST FAIL\n"); return 1; }
    std::printf("00449c10_TEST PASS\n");
    return 0;
}
