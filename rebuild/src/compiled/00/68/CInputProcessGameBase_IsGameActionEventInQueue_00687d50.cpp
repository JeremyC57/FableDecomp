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
// NF171 (2026-11-20, q11704): the callee 0x00446B10 through engine/CGamePlayerInterface.h (retail path, FABLE_RETAIL_CGAMEPLAYERINTERFACE):
//   Ego_r.pdb + FableWin.pdb ?IsGameActionEventInQueue@CGamePlayerInterface@@QBE_NW4EGameAction@@W4EInputButtonEventType@@J@Z (const, enum
//   parameters; was the stand-in ?IsGameActionEventInQueue@CGamePlayerInterfaceCalls@@QAE_NJJJ@Z reached by a reinterpret_cast of
//   PGamePlayerInterface). The row's own (long, long) parameters are its existing stand-in signature (Ego_r / FableWin spell the protected
//   ?IsGameActionEventInQueue@CInputProcessGameBase@@IBE_NW4EGameAction@@W4EInputButtonEventType@@@Z; its six landed referencing rows bind
//   four other spellings, so the self leaf is left for a family pass), hence the two enum conversions at the call.
#include "engine/CInputProcessGameBase.h"
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE
#include "engine/CGamePlayerInterface.h"

class CPlayer { public:
public:
    long GetJoystickDeviceNumber() const;   // 0x00487510
};

struct CInputProcessGameBase_Methods : CInputProcessGameBase {
    bool IsGameActionEventInQueue(long action, long buttonEventType);
};

bool CInputProcessGameBase_Methods::IsGameActionEventInQueue(long action, long buttonEventType)
{
    return PGamePlayerInterface->IsGameActionEventInQueue(
        (EGameAction)action, (EInputButtonEventType)buttonEventType, reinterpret_cast<CPlayer*>(MyPlayer)->GetJoystickDeviceNumber());
}