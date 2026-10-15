// CTCRotate::SetRotationInfo(const C3DVector&, float, float) -- sets the rotation axis (+0xc), the rotation speed (+0x18) and the duration, converted from seconds to game frames (+0x24)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072A530, length 55 bytes (+9 int3 padding).
// Callee: __ftol2 @ 0x00BFEA70. Global: the game frames-per-second long @ 0x01375550 (`fild`, value 15).
// Layout matches the CTCRotate constructor (0x004D2B2A): axis +0xc, speed +0x18, total frames +0x24.
// Shape: the axis is a 12-byte POD assigned whole (`lea eax,[esi+0Ch]` then three dword moves); the speed is copied as a
// dword; `seconds * g_GameFramesPerSecond` loads the extern first (`fild [g] ; fmul [arg]`).
struct C3DVector {
    float x, y, z;
};

extern long g_GameFramesPerSecond;           // 0x01375550

struct CTCRotate {
    unsigned char _pad[0xc];
    C3DVector     m_axis;                    // +0x0c
    float         m_speed;                   // +0x18
    long          m_unknown1c;               // +0x1c
    long          m_framesElapsed;           // +0x20
    long          m_framesTotal;             // +0x24
    void SetRotationInfo(const C3DVector& axis, float speed, float seconds);
};

void CTCRotate::SetRotationInfo(const C3DVector& axis, float speed, float seconds)
{
    m_axis = axis;
    m_speed = speed;
    m_framesTotal = (long)(seconds * g_GameFramesPerSecond);
}
