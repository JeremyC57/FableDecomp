// CInputProcessGameBase::IsEventButtonPressed(const CInputEvent& event) -- true when the event is a button press: keyboard event kind 0x13, mouse event kind 1, or a joystick button event of kind 4/7/10 or in 22..26
// Leaf: IsEventButtonPressed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00687A90, length 154 bytes.
// Callees: CInputEvent::GetDeviceType() @ 0x00A03B50 (`mov eax,[ecx+20h]`; 1 keyboard, 2 mouse, 3 joystick) and
// CInputEvent::GetEventType() @ 0x00A03B40 (`mov eax,[ecx+28h]`); both out of line, so every test re-calls them.
// The manifest label (IsEventButtonHeld on all three siblings 0x00687A90 / 0x00687B30 / 0x00687BD0) is propagated: the
// keyboard kinds 0x13/0x14/0x15 split the three into pressed / released / held.
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
    bool IsEventButtonPressed(const CInputEvent& event);
};

bool CInputProcessGameBase::IsEventButtonPressed(const CInputEvent& event)
{
    if (event.GetDeviceType() == 1)
        return event.GetEventType() == 0x13;
    if (event.GetDeviceType() == 2)
        return event.GetEventType() == 1;
    if (event.GetDeviceType() == 3)
        return event.GetEventType() == 4 || event.GetEventType() == 7 || event.GetEventType() == 10
            || (event.GetEventType() >= 22 && event.GetEventType() <= 26);
    return false;
}