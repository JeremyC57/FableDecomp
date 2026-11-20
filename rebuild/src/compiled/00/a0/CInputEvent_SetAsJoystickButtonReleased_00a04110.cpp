// CInputEvent::SetAsJoystickButtonReleased(long button, float pressure, const C2DVector& position, double time) -- a joystick (device 1) button-released event (kind 21): the button byte at +8, the stick position copied into +0, both times from one double, the pressure at +0xc
// Leaf: ?SetAsJoystickButtonReleased@CInputEvent@@QAEXJMABVC2DVector@@N@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A04110, length 55 bytes.
// Shape: the doubles narrowed with `fst`/`fstp`, no frame.
// Naming (NF160, 2026-11-20, q11726): Ego_r.pdb 0x00B6EC30 + FableWin.pdb 0x0317DC80 ?SetAsJoystickButtonReleased@CInputEvent@@QAEXJMABVC2DVector@@N@Z,
//   the third twin of the landed 0x00A04090 (Pressed, kind 19) / 0x00A040D0 (Held, kind 20), storing kind 21; respelled in place on the
//   row's existing CInputEvent declaration as 0x00A04090 was (was the made-up SetAsJoystickButton3(bool, long, ...): the first argument
//   is the button stored as a byte, the second the float pressure). Its only landed caller 0x00AB6E40 already binds this spelling.
class C2DVector { public: float X, Y; };
#include "engine/C3DVector.h"
class CInputEvent { public:                               // 0x34 bytes: a payload union, the device (+0x20), the kind (+0x28), two times
    long      m_key;                               // +0x00 (also a C2DVector at +0)
    long      m_4;                                 // +0x04
    unsigned char m_8;                             // +0x08 (the button, a byte)
    unsigned char _pad09[3];
    C3DVector m_movement;                          // +0x0c
    C2DVector m_position;                          // +0x18
    long      m_device;                            // +0x20 (1 joystick, 2 keyboard, 3 mouse)
    long      m_24;
    long      m_kind;                              // +0x28
    float     m_time;                              // +0x2c
    float     m_time2;                             // +0x30
    void SetAsJoystickButtonReleased(long button, float pressure, const C2DVector& position, double time);
};

void CInputEvent::SetAsJoystickButtonReleased(long button, float pressure, const C2DVector& position, double time)
{
    m_8 = (unsigned char)button;
    m_kind = 21;
    m_device = 1;
    *(C2DVector*)&m_key = position;
    m_time2 = (float)time;
    m_time = (float)time;
    m_movement.X = pressure;
}