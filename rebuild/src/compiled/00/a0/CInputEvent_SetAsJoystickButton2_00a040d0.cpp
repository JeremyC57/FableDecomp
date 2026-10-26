// CInputEvent::SetAsJoystickButton2(bool pressed, long button, const C2DVector& position, double time) -- a joystick (device 1) event of kind 20: the pressed flag at +8, the stick position copied into +0, both times from one double, the button at +0xc
// Leaf: SetAsJoystickButton2
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A040D0, length 64 bytes.
// Shape: the doubles narrowed with `fst`/`fstp`, no frame.
class C2DVector { public: float X, Y; };
class C3DVector { public: float X, Y, Z; };
class CInputEvent { public:                               // 0x34 bytes: a payload union, the device (+0x20), the kind (+0x28), two times
    long      m_key;                               // +0x00 (also a C2DVector at +0)
    long      m_4;                                 // +0x04
    bool      m_8;                                 // +0x08
    unsigned char _pad09[3];
    C3DVector m_movement;                          // +0x0c
    C2DVector m_position;                          // +0x18
    long      m_device;                            // +0x20 (1 joystick, 2 keyboard, 3 mouse)
    long      m_24;
    long      m_kind;                              // +0x28
    float     m_time;                              // +0x2c
    float     m_time2;                             // +0x30
    void SetAsJoystickButton2(bool pressed, long button, const C2DVector& position, double time);
};

void CInputEvent::SetAsJoystickButton2(bool pressed, long button, const C2DVector& position, double time)
{
    m_8 = pressed;
    m_kind = 20;
    m_device = 1;
    *(C2DVector*)&m_key = position;
    m_time2 = (float)time;
    m_time = (float)time;
    *(long*)&m_movement.X = button;
}