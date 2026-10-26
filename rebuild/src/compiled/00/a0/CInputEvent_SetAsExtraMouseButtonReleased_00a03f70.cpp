// CInputEvent::SetAsExtraMouseButtonReleased(const C2DVector& position, double time2, double time, long button) -- a mouse (device 3) event of kind 0x20 + button: the position at +0x18 and the two times (+0x30 from the first double, +0x2c from the second)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A03F70, length 64 bytes.
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
    void SetAsExtraMouseButtonReleased(const C2DVector& position, double time2, double time, long button);
};

void CInputEvent::SetAsExtraMouseButtonReleased(const C2DVector& position, double time2, double time, long button)
{
    m_kind = button + 0x20;
    m_device = 3;
    m_position = position;
    m_time2 = (float)time2;
    m_time = (float)time;
}