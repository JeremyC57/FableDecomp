// Test for 0x004C7A10 CThing::PeekPlayer(): the const overload of CPlayerManager::GetPlayer with the thing's PlayerNumber.
#include <stdio.h>
class CPlayer { public: int Id; };
static CPlayer g_players[4];
static int g_constCalls, g_mutCalls;
class CPlayerManager {
public:
    CPlayer&       GetPlayer(long number);
    const CPlayer& GetPlayer(long number) const;
};
CPlayer& CPlayerManager::GetPlayer(long number) { ++g_mutCalls; return g_players[number]; }
const CPlayer& CPlayerManager::GetPlayer(long number) const { ++g_constCalls; return g_players[number]; }
class CThingManager {
public:
    unsigned char   _pad00[0x30];
    CPlayerManager* PlayerManager;
};
CThingManager* g_pThingManager;
class CThing {
public:
    unsigned char _pad00[0x90];
    signed char   PlayerNumber;
    const CPlayer& PeekPlayer() const;
    CPlayer&       GetPlayer();
};
int main()
{
    CPlayerManager pm; CThingManager tm; tm.PlayerManager = &pm; g_pThingManager = &tm;
    if ((char*)&tm.PlayerManager - (char*)&tm != 0x30) { printf("004c7a10_TEST FAIL layout\n"); return 1; }
    CThing t; t.PlayerNumber = 2;
    if ((char*)&t.PlayerNumber - (char*)&t != 0x90) { printf("004c7a10_TEST FAIL thing layout\n"); return 1; }
    const CThing& r = t;
    if (&r.PeekPlayer() != &g_players[2]) { printf("004c7a10_TEST FAIL index\n"); return 1; }
    t.PlayerNumber = 1;
    if (&r.PeekPlayer() != &g_players[1]) { printf("004c7a10_TEST FAIL reread\n"); return 1; }
    if (g_constCalls != 2 || g_mutCalls != 0) { printf("004c7a10_TEST FAIL overload\n"); return 1; }
    printf("004c7a10_TEST PASS\n");
    return 0;
}
