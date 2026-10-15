// CInputProcessGameBase::IsEventButtonHeld(const CInputEvent& event) -- true when the event is a button hold: keyboard event kind 0x15, mouse event kind 3, or a joystick button event of kind 6/9/12 or in 32..36
// Leaf: IsEventButtonHeld
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00687BD0, length 156 bytes.
// Callees: CInputEvent::GetDeviceType() @ 0x00A03B50 (`mov eax,[ecx+20h]`; 1 keyboard, 2 mouse, 3 joystick) and
// CInputEvent::GetEventType() @ 0x00A03B40 (`mov eax,[ecx+28h]`); both out of line, so every test re-calls them.
// The manifest label (IsEventButtonHeld on all three siblings 0x00687A90 / 0x00687B30 / 0x00687BD0) is propagated: the
// keyboard kinds 0x15/0x14/0x15 split the three into pressed / released / held.
// Shape: each device arm returns a full-register bool (`neg/sbb/inc`, `xor eax,eax` / `mov eax,1`); only the
// fall-through for an unknown device returns through `xor al,al`. `this` is never read (thiscall, `ret 4`).
class CInputEvent {
public:
    long GetDeviceType() const;                    // 0x00A03B50
    long GetEventType() const;                     // 0x00A03B40
};
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h (members not used here)
public:
    virtual void v00();
    bool IsEventButtonHeld(const CInputEvent& event);
};

bool CInputProcessGameBase::IsEventButtonHeld(const CInputEvent& event)
{
    if (event.GetDeviceType() == 1)
        return event.GetEventType() == 0x15;
    if (event.GetDeviceType() == 2)
        return event.GetEventType() == 3;
    if (event.GetDeviceType() == 3)
        return event.GetEventType() == 6 || event.GetEventType() == 9 || event.GetEventType() == 12
            || (event.GetEventType() >= 32 && event.GetEventType() <= 36);
    return false;
}