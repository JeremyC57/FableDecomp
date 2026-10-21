// test for 00416392 PeekWorldChecksum
#include <stdio.h>
#include <string.h>
class CWorld {
public:
    unsigned long GetChecksum() const;                   // 0x0049E200
};
class CInputManager {
public:
    unsigned char _pad00[0x38];
    bool          Loading;                               // +0x38
    bool          Saving;                                // +0x39
};
extern CInputManager* g_pInputManager;                   // 0x013B8388
class CNetworkClient {
public:
    unsigned char _pad00[0x2662];
    bool          LocalGame;                             // +0x2662
};
class CMainGameComponent {
public:
    unsigned long PeekWorldChecksum() const;
    unsigned char  _pad00[0x24];
    CWorld*        PWorld;                               // +0x24 scoped_ptr<CWorld>
    unsigned char  _pad28[0x13ab8 - 0x28];
    CNetworkClient Client;                               // +0x13ab8
};
unsigned long CMainGameComponent::PeekWorldChecksum() const
{
    if (Client.LocalGame && !g_pInputManager->Loading && !g_pInputManager->Saving)
        return 1;
    return PWorld->GetChecksum();
}

CInputManager* g_pInputManager;
unsigned long CWorld::GetChecksum() const { return 0xC0FFEE; }
int main()
{
    static CInputManager im; memset(&im, 0, sizeof(im)); g_pInputManager = &im;
    CMainGameComponent* c = new CMainGameComponent; memset(c, 0, sizeof(*c));
    CWorld w; c->PWorld = &w;
    int ok = c->PeekWorldChecksum() == 0xC0FFEE;
    c->Client.LocalGame = true;  ok &= c->PeekWorldChecksum() == 1;
    im.Loading = true;           ok &= c->PeekWorldChecksum() == 0xC0FFEE;
    im.Loading = false; im.Saving = true; ok &= c->PeekWorldChecksum() == 0xC0FFEE;
    ok &= (char*)&c->Client.LocalGame - (char*)c == 0x1611a;
    printf(ok ? "00416392_TEST PASS\n" : "00416392_TEST FAIL\n");
    return ok ? 0 : 1;
}