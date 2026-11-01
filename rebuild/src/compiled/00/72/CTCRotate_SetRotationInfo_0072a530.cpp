// CTCRotate::SetRotationInfo(const C3DVector&, float, float) -- sets the rotation axis (+0xc), the rotation speed (+0x18) and the duration, converted from seconds to game frames (+0x24)
// Naming wave 18: the global stand-in g_GameFramesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072A530, length 55 bytes (+9 int3 padding).
// Callee: __ftol2 @ 0x00BFEA70. Global: the game frames-per-second long @ 0x01375550 (`fild`, value 15).
// Layout matches the CTCRotate constructor (0x004D2B2A): axis +0xc, speed +0x18, total frames +0x24.
// Shape: the axis is a 12-byte POD assigned whole (`lea eax,[esi+0Ch]` then three dword moves); the speed is copied as a
// dword; `seconds * CMainGameComponent::GetConstantFPS()` loads the extern first (`fild [g] ; fmul [arg]`).
class C3DVector { public:
    float x, y, z;
};

class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

class CTCRotate { public:
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
    m_framesTotal = (long)(seconds * CMainGameComponent::GetConstantFPS());
}