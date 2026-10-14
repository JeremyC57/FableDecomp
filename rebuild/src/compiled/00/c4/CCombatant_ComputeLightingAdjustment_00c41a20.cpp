// CCombatant::ComputeLightingAdjustment() const -- 1 - (facing . light) * intensity * global scale, clamped at zero
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C41A20, length 80 bytes.
// No callees. Reads two C3DVectors at +0x108 and +0x120, the float at +0x18c, the global float @ 0x0143BBF0 and
// the pool constants 1.0f @ 0x0122DED8 / 0.0f @ 0x0122DEDC. No PDB layout exists for CCombatant (local model).
// Shape: the dot product is emitted z, y (faddp), x (faddp) with each product `fld [v1] ; fmul [v0]`, i.e.
// written `v0.Z * v1.Z + v0.Y * v1.Y + v0.X * v1.X` (VC evaluates the left operand of each `+` first here and
// `a * b` as `fld b ; fmul a`); then `fmul [18Ch] ; fmul [global] ; fsubr [1.0]`; the clamp is `fcom [0.0] ;
// test ah,5 ; jp` returning st(0), else `fstp ; fld [0.0]`.
struct C3DVector {
    float X;
    float Y;
    float Z;
};

extern float g_combatLightingScale;             // 0x0143BBF0

struct CCombatant {
    unsigned char _pad[0x108];
    C3DVector m_lightDirection;                 // +0x108
    unsigned char _pad114[0x120 - 0x114];       // +0x114
    C3DVector m_facing;                         // +0x120
    unsigned char _pad12c[0x18c - 0x12c];       // +0x12c
    float m_lightingIntensity;                  // +0x18c
    float ComputeLightingAdjustment() const;
};

float CCombatant::ComputeLightingAdjustment() const
{
    float dot = m_lightDirection.Z * m_facing.Z + m_lightDirection.Y * m_facing.Y + m_lightDirection.X * m_facing.X;
    float adjustment = 1.0f - dot * m_lightingIntensity * g_combatLightingScale;
    if (adjustment < 0.0f)
        adjustment = 0.0f;
    return adjustment;
}