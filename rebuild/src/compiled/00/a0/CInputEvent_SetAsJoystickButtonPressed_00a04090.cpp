// CInputEvent::SetAsJoystickButtonPressed(long button, float pressure, const C2DVector& position, double time) -- a joystick (device 1) event of kind 19: the button (stored as a byte) at +8, the pressure at +0xc, the stick position copied into +0, both times from one double
// Leaf: ?SetAsJoystickButtonPressed@CInputEvent@@QAEXJMABVC2DVector@@N@Z
// Respell (NF133, q6293 / q6298): Ego_r.pdb 0x00B6EBB0 / FableWin.pdb 0x0317DBA0 ?SetAsJoystickButtonPressed@CInputEvent@@QAEXJMABVC2DVector@@N@Z
//   (twins Held 0x00A040D0 / Released 0x00A04110 store kinds 20 / 21); the landed caller CJoystickDX::UpdateEvents 0x00AB6E40 binds it and
//   pushes (button, 0 pressure, &position, time). The PDB parameter order makes the first dword the button (its low byte stored at +8:
//   `mov al,[esp+4]`) and the second the float pressure (copied whole to +0xc: `mov edx,[esp+8]`); the old (bool pressed, long button)
//   reading named the same bytes the other way round. In-place respelling of the row's existing local model (m_8 now a byte).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A04090, length 64 bytes.
// Shape: the doubles narrowed with `fst`/`fstp`, no frame.
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
    void SetAsJoystickButtonPressed(long button, float pressure, const C2DVector& position, double time);
};

void CInputEvent::SetAsJoystickButtonPressed(long button, float pressure, const C2DVector& position, double time)
{
    m_8 = (unsigned char)button;
    m_kind = 19;
    m_device = 1;
    *(C2DVector*)&m_key = position;
    m_time2 = (float)time;
    m_time = (float)time;
    m_movement.X = pressure;
}