// CNetworkClient::Uninitialise() -- chains to the protected CInitBaseClass::Uninitialise (VC7.1 tail-calls it: `jmp 0x0099A360`)
// Leaf: ?Uninitialise@CNetworkClient@@QAEXXZ
// Identity: Ego_r.pdb ?Uninitialise@CNetworkClient@@QAEXXZ (0x004B43D0, directly before SendGameEvent 0x004B43E0) and FableWin.pdb
//   (0x01B32F10); retail 0x004AE990 sits directly before SendGameEvent 0x004AE9A0 (engine/CNetworkClient.h), and its callers
//   CMainGameComponent::CreatePlayers 0x004166A8 / Shutdown 0x004175E5 call it on the CNetworkClient Client member (+0x13AB8).
//   NF132 (q4019): was the free extern "C" forwarder `_Forward` -> `_ForwardTarget_0099a360`.
#define FABLE_RETAIL_CNETWORKCLIENT_UPDATE
#define FABLE_RETAIL_CNETWORKCLIENT_UNINITIALISE
#define FABLE_RETAIL_CINITBASECLASS_UNINITIALISE
#include "engine/CNetworkClient.h"

void CNetworkClient::Uninitialise()
{
    CInitBaseClass::Uninitialise();
}
