// CInputProcessGameBase::IsEventButtonPressed(const CInputEvent& event) const (protected) -- true when the event is a button press: keyboard event kind 0x13, mouse event kind 1, or a joystick button event of kind 4/7/10 or in 22..26
// Leaf: IsEventButtonPressed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00687A90, length 154 bytes.
// Callees: CInputEvent::GetDeviceType() @ 0x00A03B50 (`mov eax,[ecx+20h]`; 1 keyboard, 2 mouse, 3 joystick) and
// CInputEvent::GetEventType() @ 0x00A03B40 (`mov eax,[ecx+28h]`); both out of line, so every test re-calls them.
// The manifest label (IsEventButtonHeld on all three siblings 0x00687A90 / 0x00687B30 / 0x00687BD0) is propagated: the
// keyboard kinds 0x13/0x14/0x15 split the three into pressed / released / held.
// Shape: each device arm returns a full-register bool (`neg/sbb/inc`, `xor eax,eax` / `mov eax,1`); only the
// fall-through for an unknown device returns through `xor al,al`. `this` is never read (thiscall, `ret 4`).
// Naming (NF160, 2026-11-20, q4300): the local CInputEvent model replaced by the shared engine/CInputEvent.h (Ego_r.pdb / FableWin.pdb
//   spellings): 0x00A03B40 GetEventType -> GetType; GetDeviceType 0x00A03B50 unchanged.
#define FABLE_RETAIL_CINPUTEVENT_DEVICETYPE
#include "engine/CInputEvent.h"
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h (members not used here)
public:
    virtual void v00();
protected:
    bool IsEventButtonPressed(const CInputEvent& event) const;
public:
};

bool CInputProcessGameBase::IsEventButtonPressed(const CInputEvent& event) const
{
    if (event.GetDeviceType() == 1)
        return event.GetType() == 0x13;
    if (event.GetDeviceType() == 2)
        return event.GetType() == 1;
    if (event.GetDeviceType() == 3)
        return event.GetType() == 4 || event.GetType() == 7 || event.GetType() == 10
            || (event.GetType() >= 22 && event.GetType() <= 26);
    return false;
}