// CInputTypeMouseButtonEvent::IsEventOfThisType(const CInputEvent& event) const -- true when a mouse-device event (device type 3) is one of this binding's button events: event types 4-6 for button 1, 7-9 for button 3, 10-12 for button 2, and 22-26 / 27-31 / 32-36 for buttons 8-12
// Leaf: ?IsEventOfThisType@
// Retail: Fable.exe @ 0x0066EEE0, 376 bytes, `ret 4`.
// Body: CInputEvent::GetDeviceType (0x00A03B50) == 3, then CInputEvent::GetType (0x00A03B40) re-read for every test:
// {4,5,6} -> Button == 1; {7,8,9} -> Button == 3; {10,11,12} -> Button == 2; [22,26] -> Button == type - 14;
// [27,31] -> Button == type - 19; [32,36] -> Button == type - 24; else false.
// Layout: +0x04 the mouse button (as the landed GetEventTypeFromGeneralButtonType 0x0066F060 / IsButton 0x00447A90).
// Opt: /O2 (16-aligned entry).
class CInputEvent {
public:
    long GetType() const;                                // 0x00A03B40
    long GetDeviceType() const;                          // 0x00A03B50
};
class CInputTypeMouseButtonEvent {
public:
    virtual void v000();
    long Button;                                         // +0x04
    bool IsEventOfThisType(const CInputEvent& event) const;
};
bool CInputTypeMouseButtonEvent::IsEventOfThisType(const CInputEvent& event) const
{
    if (event.GetDeviceType() == 3) {
        if (event.GetType() == 4 || event.GetType() == 5 || event.GetType() == 6)
            return Button == 1;
        if (event.GetType() == 7 || event.GetType() == 8 || event.GetType() == 9)
            return Button == 3;
        if (event.GetType() == 10 || event.GetType() == 11 || event.GetType() == 12)
            return Button == 2;
        if (event.GetType() >= 22 && event.GetType() <= 26)
            return Button == event.GetType() - 14;
        if (event.GetType() >= 27 && event.GetType() <= 31)
            return Button == event.GetType() - 19;
        if (event.GetType() >= 32 && event.GetType() <= 36)
            return Button == event.GetType() - 24;
    }
    return false;
}