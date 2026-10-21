// behaviour test for 0x004AE9A0
#include <stdio.h>
class CGameEvent; static const CGameEvent* g_last; static int g_adds;
class CGameEvent;
class CGameEventPackage {
public:
    void AddEvent(const CGameEvent* event) { g_last = event; ++g_adds; }
};
class CNetworkClient {
public:
    unsigned char     _pad00[0x2010];
    CGameEventPackage LocalEventPackage;                 // +0x2010
    unsigned char     _pad2011[0x2662 - 0x2011];
    bool              LocalGame;                         // +0x2662
    void SendGameEvent(const CGameEvent* event);
};
void CNetworkClient::SendGameEvent(const CGameEvent* event)
{
    if (LocalGame)
        LocalEventPackage.AddEvent(event);
}
int main()
{
    static CNetworkClient c; int ok = 1; const CGameEvent* ev = (const CGameEvent*)0x1234;
    c.LocalGame = false; c.SendGameEvent(ev); ok &= g_adds == 0;
    c.LocalGame = true; c.SendGameEvent(ev); ok &= g_adds == 1 && g_last == ev;
    printf(ok ? "004ae9a0_TEST PASS\n" : "004ae9a0_TEST FAIL\n");
    return ok ? 0 : 1;
}