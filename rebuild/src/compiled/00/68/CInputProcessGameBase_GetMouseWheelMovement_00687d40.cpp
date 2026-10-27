// CInputProcessGameBase::GetMouseWheelMovement(const CInputEvent&) const -- the mouse-wheel movement of an input event, through the game player interface
// Leaf: ?GetMouseWheelMovement@CInputProcessGameBase@@QBEMABVCInputEvent@@@Z
// Retail: Fable.exe @ 0x00687D40, 16 bytes: `mov eax,[esp+4] ; mov ecx,[ecx+24h] ; push eax ; call 0x00445920 ; ret 4`.
// Identity (proven): Ego_r.pdb names ?GetMouseWheelMovement@CInputProcessGameBase@@QBEMABVCInputEvent@@@Z and
// ?GetMouseWheelMovement@CGamePlayerInterface@@QBEMABVCInputEvent@@@Z; the callee 0x00445920 is
// `mov ecx,[esp+4] ; call 0x00A03BB0 ; ret 4`, i.e. CGamePlayerInterface's forwarder to the event's
// CInputEvent::GetMouseWheelMovement() const (0x00A03BB0, `fld [ecx+14h]`). The only caller,
// CInputProcessSpecialAbilities::ProcessInput 0x00690030, uses the float (fabs against 1e-4f) and landed this call as a
// descriptive "GetEventScrollAmount". The manifest label "operator_new" and the landed 0x00445920 label (_Cons_val) are
// propagated.
// Layout: engine/CInputProcessGameBase.h (PGamePlayerInterface +0x24).
// Shape: the result goes through a named float local, which keeps retail's `call ; ret 4` (returning the call directly
// makes VC7.1 tail-jump).
#define CInputProcessGameBase DonorCInputProcessGameBase
#include "engine/CInputProcessGameBase.h"
#undef CInputProcessGameBase
class CInputEvent;
class CGamePlayerInterface { public:
    float GetMouseWheelMovement(const CInputEvent& event) const;        // 0x00445920
};
class CInputProcessGameBase : public DonorCInputProcessGameBase { public:   // engine header: PGamePlayerInterface +0x24
    float GetMouseWheelMovement(const CInputEvent& event) const;
};

float CInputProcessGameBase::GetMouseWheelMovement(const CInputEvent& event) const
{
    float movement = PGamePlayerInterface->GetMouseWheelMovement(event);
    return movement;
}