// CNetworkClient::SendGameEvent(const CGameEvent*) -- in a local game, queues the event in the local event package
// Leaf: SendGameEvent@CNetworkClient
// Retail: Fable.exe @ 0x004AE9A0, 24 bytes, `ret 4`. Catalog label CTCTrapBase::ManualReset is wrong. Only caller is
// CMainGameComponent::AddGameEvent 0x004164C4 (`lea ecx,[esi+13AB8h]`, the component's CNetworkClient). Ego_r.pdb
// ?SendGameEvent@CNetworkClient@@QAEXPBVCGameEvent@@@Z; CNetworkClient::LocalEventPackage (CGameEventPackage) +0x2010 and
// LocalGame +0x2662 match the bytes exactly; callee ?AddEvent@CGameEventPackage@@QAEXPBVCGameEvent@@@Z 0x009F1650 (tail jump).
class CGameEvent;
class CGameEventPackage {
public:
    void AddEvent(const CGameEvent* event);              // 0x009F1650
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