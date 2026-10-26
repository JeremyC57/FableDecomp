// CInputProcessGameBase::IsGameActionEventInQueue(long action, long buttonEventType) -- forward to the player interface (+0x24) with this player's number (MyPlayer +0x18) as the third argument
// Leaf: IsGameActionEventInQueue
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00687D50, length 34 bytes (+ int3 padding).
// Callees: CPlayer::GetPlayerNumber @ 0x00487510 (the label "CWorld::DrawGetEnvironment" is propagated, see the landed
// sibling 0x00687D80 (the four-argument IsGameActionEventInQueue, landed)), CGamePlayerInterface::IsGameActionEventInQueue(long, EInputButtonEventType, long) @ 0x00446B10.
// The catalog label "CPersistContext::Transfer<CEngineGraphic>" is propagated: the body is a CInputProcessGameBase
// forwarder like its landed neighbours 0x00687C90 / 0x00687D80.
// Shape: the arguments are evaluated right to left -- the player-number call first (ecx = MyPlayer), then the two
// words reloaded from their slots; the bool rides back through al with no tail jump (esi restored); `ret 8`.
// Layout: engine/CInputProcessGameBase.h (PDB): MyPlayer +0x18, PGamePlayerInterface +0x24.
#include "engine/CInputProcessGameBase.h"

class CPlayer { public:
public:
    long GetJoystickDeviceNumber() const;   // 0x00487510
};
struct CGamePlayerInterfaceCalls {
    bool IsGameActionEventInQueue(long action, long buttonEventType, long playerNumber);   // 0x00446B10
};

struct CInputProcessGameBase_Methods : CInputProcessGameBase {
    bool IsGameActionEventInQueue(long action, long buttonEventType);
};

bool CInputProcessGameBase_Methods::IsGameActionEventInQueue(long action, long buttonEventType)
{
    return reinterpret_cast<CGamePlayerInterfaceCalls*>(PGamePlayerInterface)->IsGameActionEventInQueue(
        action, buttonEventType, reinterpret_cast<CPlayer*>(MyPlayer)->GetJoystickDeviceNumber());
}