// CInputEvent::SetAsKeyboardKeyPressed(long key, double time2, double time) -- a keyboard (device 2) event of kind 2 with two separate times (+0x30 from the first double, +0x2c from the second) and the key at +0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A03C20, length 48 bytes.
// Shape: the doubles narrowed with `fst`/`fstp`, no frame.
struct C2DVector { float X, Y; };
struct C3DVector { float X, Y, Z; };
struct CInputEvent {                               // 0x34 bytes: a payload union, the device (+0x20), the kind (+0x28), two times
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
    void SetAsKeyboardKeyPressed(long key, double time2, double time);
};

void CInputEvent::SetAsKeyboardKeyPressed(long key, double time2, double time)
{
    m_time2 = (float)time2;
    m_device = 2;
    m_kind = 2;
    m_time = (float)time;
    m_key = key;
}