// CNetworkClient::Update @ 0x004ae9d0  (__fastcall, ret 0xc)
// Leaf: ?CNetworkClient_Update@@YIXPAUCNetworkClient@@PAXHHH@Z

#include "engine/CNetworkClient.h"  // retyped onto the PDB layout; byte parity re-verified
void __fastcall CNetworkClient_Update(CNetworkClient *client, void * /*edx*/, int a, int b, int c)
{
    if (client->LocalGame) {
        client->LocalFrame = a;
        client->Checksum1 = b;
        client->Checksum2 = c;
    }
}