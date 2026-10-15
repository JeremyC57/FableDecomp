// CInputEvent::CInputEvent(const CInputEvent& other) -- copy constructor of the 0x34-byte input event
// Leaf: ??0CInputEvent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E4850, length 117 bytes. No callees.
// Layout (PDB): UInputEventData Data +0x00 (0x20-byte union: raw words, a key word, the joystick
// position + button, the mouse block at +0xc), DeviceType +0x20, DeviceNumber +0x24, Type +0x28,
// CurrentTime +0x2c, StartTime +0x30.
// Shape: retail copies word 0, then the whole union word by word, then re-copies the overlapping views
// (the 16-bit key, the joystick C2DVector + button byte, the mouse word at +0xc) and finally the
// scalars: a user-written copy constructor whose statements copy every union view in turn. The
// destination and source may alias, so VC7.1 keeps every redundant load/store.
union UInputEventData {
    unsigned long  Raw[8];                         // the whole union as words
    unsigned short Key;                            // keyboard view: the 16-bit key code
    struct {
        unsigned long JoystickPosX;                // C2DVector JoystickPos (moved as words)
        unsigned long JoystickPosY;
        unsigned char JoystickButton;              // +0x08
    } Joystick;
    struct {
        unsigned long _pad[3];
        unsigned long MouseMovementX;              // +0x0c (first word of the mouse view)
    } Mouse;
};
struct CInputEvent {
    UInputEventData Data;                          // +0x00
    long            DeviceType;                    // +0x20
    long            DeviceNumber;                  // +0x24
    long            Type;                          // +0x28
    unsigned long   CurrentTime;                   // +0x2c (float, moved as a word)
    unsigned long   StartTime;                     // +0x30 (float, moved as a word)
    CInputEvent(const CInputEvent& other);
};

CInputEvent::CInputEvent(const CInputEvent& other)
{
    Data.Raw[0] = other.Data.Raw[0];
    Data.Raw[0] = other.Data.Raw[0];
    Data.Raw[1] = other.Data.Raw[1];
    Data.Raw[2] = other.Data.Raw[2];
    Data.Raw[3] = other.Data.Raw[3];
    Data.Raw[4] = other.Data.Raw[4];
    Data.Raw[5] = other.Data.Raw[5];
    Data.Raw[6] = other.Data.Raw[6];
    Data.Raw[7] = other.Data.Raw[7];
    Data.Key = other.Data.Key;
    Data.Joystick.JoystickPosX = other.Data.Joystick.JoystickPosX;
    Data.Joystick.JoystickPosY = other.Data.Joystick.JoystickPosY;
    Data.Joystick.JoystickButton = other.Data.Joystick.JoystickButton;
    Data.Mouse.MouseMovementX = other.Data.Mouse.MouseMovementX;
    DeviceType = other.DeviceType;
    DeviceNumber = other.DeviceNumber;
    Type = other.Type;
    CurrentTime = other.CurrentTime;
    StartTime = other.StartTime;
}