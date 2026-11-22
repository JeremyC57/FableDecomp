// CInputProcessGameBase::IsEventButtonReleased(const CInputEvent& event) -- true when the event is a button release: one of the Ego_r.pdb EInputEvent IE_*_RELEASED codes 0x15 (joystick button), 3 (key), 6/9/12 (L/M/R mouse button) or 32..36 (MB4..MB8)
// Leaf: ?IsEventButtonReleased@CInputProcessGameBase@@IBE_NABVCInputEvent@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00687BD0, length 156 bytes.
// Callees: CInputEvent::GetDeviceType() @ 0x00A03B50 (`mov eax,[ecx+20h]`; 1 keyboard, 2 mouse, 3 joystick) and
// CInputEvent::GetEventType() @ 0x00A03B40 (`mov eax,[ecx+28h]`); both out of line, so every test re-calls them.
// The manifest label (IsEventButtonHeld on all three siblings 0x00687A90 / 0x00687B30 / 0x00687BD0) is propagated: the
// codes split the three into pressed / held / released (EInputEvent; landed swapped until 2026-11-22, Codex pass 27 + chunk 829).
// Shape: each device arm returns a full-register bool (`neg/sbb/inc`, `xor eax,eax` / `mov eax,1`); only the
// fall-through for an unknown device returns through `xor al,al`. `this` is never read (thiscall, `ret 4`).
// Naming (NF160, 2026-11-20, q4300): the local CInputEvent model replaced by the shared engine/CInputEvent.h (Ego_r.pdb / FableWin.pdb
//   spellings): 0x00A03B40 GetEventType -> GetType; GetDeviceType 0x00A03B50 unchanged.
#define FABLE_RETAIL_CINPUTEVENT_DEVICETYPE
#include "engine/CInputEvent.h"
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h (members not used here)
public:
    virtual void v00();
protected:   // Ego_r.pdb / FableWin.pdb: protected const (?IsEventButtonReleased@CInputProcessGameBase@@IBE_NABVCInputEvent@@@Z; NF70)
    bool IsEventButtonReleased(const CInputEvent& event) const;
public:
};

bool CInputProcessGameBase::IsEventButtonReleased(const CInputEvent& event) const
{
    if (event.GetDeviceType() == 1)
        return event.GetType() == 0x15;
    if (event.GetDeviceType() == 2)
        return event.GetType() == 3;
    if (event.GetDeviceType() == 3)
        return event.GetType() == 6 || event.GetType() == 9 || event.GetType() == 12
            || (event.GetType() >= 32 && event.GetType() <= 36);
    return false;
}