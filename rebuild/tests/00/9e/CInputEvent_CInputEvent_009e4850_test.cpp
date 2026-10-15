// Behaviour test for 0x009E4850 (CInputEvent copy constructor): every word of the union and the scalars survive the copy.
#include <cstdio>
union UInputEventData {
    unsigned int   Raw[8];
    unsigned short Key;
    struct { unsigned int JoystickPosX; unsigned int JoystickPosY; unsigned char JoystickButton; } Joystick;
    struct { unsigned int _pad[3]; unsigned int MouseMovementX; } Mouse;
};
struct CInputEvent {
    UInputEventData Data;
    int DeviceType; int DeviceNumber; int Type; unsigned int CurrentTime; unsigned int StartTime;
    CInputEvent() {}
    CInputEvent(const CInputEvent& other)
    {
        Data.Raw[0] = other.Data.Raw[0];
        for (int i = 0; i < 8; ++i) Data.Raw[i] = other.Data.Raw[i];
        Data.Key = other.Data.Key;
        Data.Joystick.JoystickPosX = other.Data.Joystick.JoystickPosX;
        Data.Joystick.JoystickPosY = other.Data.Joystick.JoystickPosY;
        Data.Joystick.JoystickButton = other.Data.Joystick.JoystickButton;
        Data.Mouse.MouseMovementX = other.Data.Mouse.MouseMovementX;
        DeviceType = other.DeviceType; DeviceNumber = other.DeviceNumber; Type = other.Type;
        CurrentTime = other.CurrentTime; StartTime = other.StartTime;
    }
};
int main()
{
    CInputEvent a;
    for (int i = 0; i < 8; ++i) a.Data.Raw[i] = 0x11111111u * (i + 1);
    a.DeviceType = 2; a.DeviceNumber = 3; a.Type = 0x15; a.CurrentTime = 0x3f800000u; a.StartTime = 0x40000000u;
    CInputEvent b(a);
    int ok = 1;
    for (int i = 0; i < 8; ++i) if (b.Data.Raw[i] != a.Data.Raw[i]) ok = 0;
    if (b.DeviceType != 2 || b.DeviceNumber != 3 || b.Type != 0x15 || b.CurrentTime != 0x3f800000u || b.StartTime != 0x40000000u) ok = 0;
    if (b.Data.Key != a.Data.Key || b.Data.Joystick.JoystickButton != a.Data.Joystick.JoystickButton) ok = 0;
    if (!ok) { std::printf("009e4850_TEST FAIL\n"); return 1; }
    std::printf("009e4850_TEST PASS\n");
    return 0;
}