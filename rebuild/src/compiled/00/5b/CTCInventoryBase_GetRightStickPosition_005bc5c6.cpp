// NInventory::CTCInventoryBase::GetRightStickPosition(C2DVector&) -- stores the right analogue stick vector of the main player's pad when a stick event is queued, else (0, 0)
// Leaf: GetRightStickPosition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BC5C6, 114 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`), `ret 4`: a void member with an out reference (no
// `mov eax` before the return).
// Callees: CInputEvent::CInputEvent() @ 0x00A04410 / ~CInputEvent() @ 0x00A03B30 (0x34-byte frame object),
//          CTCBase::GetPlayerManager() @ 0x00686D40, CPlayerManager::GetMainPlayer() @ 0x00449970,
//          CPlayer::GetPlayerNumber() @ 0x00487510, CInputManager::IsJoystickEventInQueue(long type, long device,
//          CInputEvent*, long player) @ 0x009F4C60 on the input manager global @ 0x013B8388 (event type 0x12),
//          CInputEvent::GetAnalogueVector() @ 0x00A043A0 (C2DVector by hidden pointer).
// Shape: the player number is computed before the manager load and the pushes (a call result in the argument
// list); `out = event.GetAnalogueVector()` comes back into a frame temporary ([ebp-8]) copied as two dwords; the
// fallback stores the two zero members directly (`fldz ; fstp` twice, the size-TU zero form; a
// `C2DVector(0, 0)` temporary adds a frame copy); one shared dtor call.
#pragma optimize("y", off)
#pragma optimize("s", on)

class C2DVector { public:
    float x, y;
    C2DVector(float x_, float y_) : x(x_), y(y_) {}
};

class CInputEvent {
public:
    unsigned char _data[0x34];
    CInputEvent();                                   // 0x00A04410
    ~CInputEvent();                                  // 0x00A03B30
    C2DVector GetAnalogueVector() const;             // 0x00A043A0
};

class CInputManager { public:
    bool IsJoystickEventInQueue(long type, long device, CInputEvent* event, long player);  // 0x009F4C60
};

extern CInputManager* g_pInputManager;               // 0x013B8388

class CPlayer { public:
    long GetPlayerNumber() const;                    // 0x00487510
};

// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"

class CTCBase { protected:
    CPlayerManager& GetPlayerManager();   // 0x00686D40 (Ego_r.pdb CTCBase, protected)
};

namespace NInventory {

class CTCInventoryBase : public ::CTCBase { public:
    void* __vftable;                                 // +0x00
    void* ParentThing;                               // +0x04 CTCBase

    void GetRightStickPosition(C2DVector& out);      // Ego_r.pdb / FableWin.pdb: non-const
};

void CTCInventoryBase::GetRightStickPosition(C2DVector& out)
{
    CInputEvent event;
    if (g_pInputManager->IsJoystickEventInQueue(0x12, 0, &event, GetPlayerManager().GetMainPlayer().GetPlayerNumber()))
        out = event.GetAnalogueVector();
    else {
        out.x = 0.0f;
        out.y = 0.0f;
    }
}

}