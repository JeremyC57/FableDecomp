// Behaviour test for 00449bf0 CPlayerManager::GetMainPlayerHeroCreature (hero creature of the main player)
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
    CThing* GetMainPlayerHeroCreature(long unused);
};
CThing* CPlayerManager::GetMainPlayerHeroCreature(long unused)
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
    if (m.GetMainPlayerHeroCreature(0) != &h1) { std::printf("00449bf0_TEST FAIL\n"); return 1; }
    m.MainPlayer = 0;
    if (m.GetMainPlayerHeroCreature(5) != &h0) { std::printf("00449bf0_TEST FAIL\n"); return 1; }
    std::printf("00449bf0_TEST PASS\n");
    return 0;
}
