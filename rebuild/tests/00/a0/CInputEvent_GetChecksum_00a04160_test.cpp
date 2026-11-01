// Behaviour test for CInputEvent::GetChecksum (0x00A04160): the unsigned sum of the key word, device type, device number and
// event type, then each float view of the event-data union (held mouse movement, mouse movement, mouse position, joystick
// position) added through an unsigned float round trip, truncating after each view.
#include <cstdio>
class C2DVector { public: float X, Y; };
class C3DVector { public: float X, Y, Z; };
struct InputEventModel {
    union {
        unsigned int Key;
        C3DVector     HeldMouseMovement;
        C2DVector     JoystickPos;
    };
    C3DVector     MouseMovement;                  // +0x0C
    C2DVector     MousePos;                       // +0x18
    unsigned int DeviceType;                     // +0x20
    long          DeviceNumber;
    unsigned int Type;
    float         CurrentTime, StartTime;
    unsigned int GetChecksum() const
    {
        unsigned int checksum = Key + DeviceType + DeviceNumber + Type;
        checksum += HeldMouseMovement.X + HeldMouseMovement.Y + HeldMouseMovement.Z;
        checksum += MouseMovement.X + MouseMovement.Y + MouseMovement.Z;
        checksum += MousePos.X + MousePos.Y;
        checksum += JoystickPos.X + JoystickPos.Y;
        return checksum;
    }
};
int main()
{
    InputEventModel e;
    e.HeldMouseMovement.X = 0.0f; e.HeldMouseMovement.Y = 1.5f; e.HeldMouseMovement.Z = 2.5f;   // Key word = 0
    e.MouseMovement.X = 1.0f; e.MouseMovement.Y = 2.0f; e.MouseMovement.Z = 3.0f;
    e.MousePos.X = 4.0f; e.MousePos.Y = 5.0f;
    e.DeviceType = 2; e.DeviceNumber = 3; e.Type = 4;
    // 9 -> 13 -> 19 -> 28 -> 29.5 truncated to 29
    if (e.GetChecksum() != 29) { std::printf("00a04160_TEST FAIL\n"); return 1; }
    InputEventModel k;
    k.Key = 7; k.HeldMouseMovement.Y = 0.0f; k.HeldMouseMovement.Z = 0.0f;
    k.MouseMovement.X = k.MouseMovement.Y = k.MouseMovement.Z = 0.0f; k.MousePos.X = k.MousePos.Y = 0.0f;
    k.DeviceType = 1; k.DeviceNumber = 0; k.Type = 2;
    // the key word 7 is also a tiny denormal float view: the float folds add nothing -> 10
    if (k.GetChecksum() != 10) { std::printf("00a04160_TEST FAIL\n"); return 1; }
    std::printf("00a04160_TEST PASS\n");
    return 0;
}