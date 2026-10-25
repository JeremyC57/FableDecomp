// CAmbientCube::GetLevel() const -- read the ambient cube's ramped level parameter (member at +0xE0, after the eight apexes): its current value, wrapped once into [-pi, pi) when the parameter is an angle
// Leaf: ?GetLevel@CAmbientCube@@QBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C45940, 56 bytes (+ int3 padding), __fastcall, float result on the x87 stack.
// Library: libpk audio (no PDB/RTTI): descriptive names. The 0x34-byte ramped parameter is the landed CRampedParam
//   (0x00C0B510 SetImmediate: +0x00 current value, +0x30 angle flag) embedded at +0xE0, so the flag reads at +0x110;
//   byte twin of chunk 438's 0x00C38510 (the same parameter at +0x0C). Callers 0x00C44DE0 / 0x00C61DC9 / 0x00C61F86,
//   all with `this` = owner +0x4C: the CAmbientCube of UpdateDistances 0x00C45810 / SetCentre 0x00C458D0 (eight
//   0x1C-byte apexes = 0xE0 bytes, then this parameter). The manifest's "CTCWeapon_GetDamageValue" is propagated.
// Constants: pi @ 0x0128F604, 2*pi @ 0x0128F608, -pi @ 0x012B1EB8.
// Shape: the parameter's inline GetValue() with early returns in register form (`fcom`, no reload; each return its own
//   `ret`); `>= pi` is `test ah,1 ; jne`, `< -pi` is `test ah,5 ; jp`.
struct CRampedParam {                           // 0x34 bytes
    float m_value;                              // +0x00 current value
    float m_target;                             // +0x04 target value
    float m_rates[10];                          // +0x08 rates, time, duration, coefficients
    bool  m_bAngle;                             // +0x30 wrap the value into [-pi, pi)
    float GetValue() const
    {
        float value = m_value;
        if (m_bAngle) {
            if (value >= 3.14159265f)
                return value - 6.28318531f;
            if (value < -3.14159265f)
                return value + 6.28318531f;
        }
        return value;
    }
};

struct SAmbientApex {                           // 0x1C bytes: offset, position (two C3DVectors), minimum distance
    float m_offset[3];                          // +0x00
    float m_position[3];                        // +0x0c
    float m_minDistance;                        // +0x18
};

class CAmbientCube {
public:
    SAmbientApex m_apexes[8];                   // +0x00
    CRampedParam m_level;                       // +0xe0
    float GetLevel() const;
};

float CAmbientCube::GetLevel() const
{
    return m_level.GetValue();
}