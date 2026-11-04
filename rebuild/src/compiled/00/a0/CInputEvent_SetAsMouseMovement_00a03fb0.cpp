// CInputEvent::SetAsMouseMovement(const C2DVector& position, const C3DVector& movement, double time) -- a mouse (device 3) movement event (kind 13): the movement at +0xc, the position at +0x18, both times from one double
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A03FB0, length 80 bytes.
// Shape: the doubles narrowed with `fst`/`fstp`, no frame.
class C2DVector { public: float X, Y; };
#include "engine/C3DVector.h"
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
    void SetAsMouseMovement(const C2DVector& position, const C3DVector& movement, double time);
};

void CInputEvent::SetAsMouseMovement(const C2DVector& position, const C3DVector& movement, double time)
{
    m_device = 3;
    m_kind = 13;
    m_movement = movement;
    m_position = position;
    m_time2 = (float)time;
    m_time = (float)time;
}