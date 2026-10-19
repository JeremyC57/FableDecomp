// CGamePlayerInterface::GetButtonPressureForEvent(const CInputEvent& event) const -- analogue pad buttons report their
// pressure fraction, digital key/button/mouse events count as fully pressed (1), anything else 0
// Leaf: ?GetButtonPressureForEvent@CGamePlayerInterface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00445800, length 240 bytes, `ret 4` (`this` unused).
// Callees: CInputEvent::GetType @ 0x00A03B40 (returns +0x28; called afresh for every test, another TU),
// CInputEvent::GetJoystickButtonPressureFraction @ 0x00A03BA0. Float pool: 0x0122DEDC = 0.0f, 0x0122DED8 = 1.0f.
// Shape: one `||` chain per result; the two ranges (0x16..0x1A, 0x1B..0x1F) are `a >= lo && a <= hi` terms. Retail
// lays the default 0.0f exit first, then the 1.0f exit, then the pressure call.

enum EInputEventType {
    INPUT_EVENT_TYPE_KEYBOARD_KEY_PRESSED = 1,
    INPUT_EVENT_TYPE_KEYBOARD_KEY_HELD = 2,
    INPUT_EVENT_TYPE_LEFT_MOUSE_BUTTON_PRESSED = 4,
    INPUT_EVENT_TYPE_LEFT_MOUSE_BUTTON_HELD = 5,
    INPUT_EVENT_TYPE_MIDDLE_MOUSE_BUTTON_PRESSED = 7,
    INPUT_EVENT_TYPE_MIDDLE_MOUSE_BUTTON_HELD = 8,
    INPUT_EVENT_TYPE_RIGHT_MOUSE_BUTTON_PRESSED = 10,
    INPUT_EVENT_TYPE_RIGHT_MOUSE_BUTTON_HELD = 11,
    INPUT_EVENT_TYPE_JOYSTICK_BUTTON_PRESSED = 0x13,
    INPUT_EVENT_TYPE_JOYSTICK_BUTTON_HELD = 0x14,
    INPUT_EVENT_TYPE_JOYSTICK_BUTTON_RELEASED = 0x15,
    INPUT_EVENT_TYPE_FIRST_RANGE_A = 0x16,
    INPUT_EVENT_TYPE_LAST_RANGE_A = 0x1A,
    INPUT_EVENT_TYPE_FIRST_RANGE_B = 0x1B,
    INPUT_EVENT_TYPE_LAST_RANGE_B = 0x1F
};

class CInputEvent {
public:
    EInputEventType GetType() const;                        // 0x00A03B40
    float GetJoystickButtonPressureFraction() const;        // 0x00A03BA0
};

class CGamePlayerInterface {
public:
    float GetButtonPressureForEvent(const CInputEvent& event) const;
};

float CGamePlayerInterface::GetButtonPressureForEvent(const CInputEvent& event) const
{
    if (event.GetType() == INPUT_EVENT_TYPE_JOYSTICK_BUTTON_PRESSED ||
        event.GetType() == INPUT_EVENT_TYPE_JOYSTICK_BUTTON_HELD ||
        event.GetType() == INPUT_EVENT_TYPE_JOYSTICK_BUTTON_RELEASED)
        return event.GetJoystickButtonPressureFraction();

    if (event.GetType() == INPUT_EVENT_TYPE_KEYBOARD_KEY_PRESSED ||
        event.GetType() == INPUT_EVENT_TYPE_KEYBOARD_KEY_HELD ||
        event.GetType() == INPUT_EVENT_TYPE_LEFT_MOUSE_BUTTON_PRESSED ||
        event.GetType() == INPUT_EVENT_TYPE_LEFT_MOUSE_BUTTON_HELD ||
        event.GetType() == INPUT_EVENT_TYPE_MIDDLE_MOUSE_BUTTON_PRESSED ||
        event.GetType() == INPUT_EVENT_TYPE_MIDDLE_MOUSE_BUTTON_HELD ||
        event.GetType() == INPUT_EVENT_TYPE_RIGHT_MOUSE_BUTTON_PRESSED ||
        event.GetType() == INPUT_EVENT_TYPE_RIGHT_MOUSE_BUTTON_HELD ||
        (event.GetType() >= INPUT_EVENT_TYPE_FIRST_RANGE_A && event.GetType() <= INPUT_EVENT_TYPE_LAST_RANGE_A) ||
        (event.GetType() >= INPUT_EVENT_TYPE_FIRST_RANGE_B && event.GetType() <= INPUT_EVENT_TYPE_LAST_RANGE_B))
        return 1.0f;

    return 0.0f;
}
