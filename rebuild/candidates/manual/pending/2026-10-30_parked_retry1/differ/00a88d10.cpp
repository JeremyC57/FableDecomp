// CQuaternion::SetFromAxisAngle(const C3DVector&, float) -- set the rotation of `angle` (in turns) about the unit axis: (axis * sin(angle/2), cos(angle/2)) from the 1024-entry interpolated cosine table
// Grade: DIFFER(182v196) -- the rounding is the landed FloatToLong retail_asm helper and the half angle is a named local (two fmuls as retail); residue: retail keeps TableCos's position parameter as a separate frame word (re-copied for the second call, position-0.5 recomputed), the lane merges it with the local and reuses position-0.5
// Retail-asm: 0x00A88D35 fld dword ptr [esp+4] ; fistp dword ptr [esp] -- the engine FloatToLong helper, expanded twice
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A88D10, length 196 bytes, `ret 8`.
// Constants: 0.5f 0x0122F59C, 1024.0f 0x01230010, 1.0f 0x0122DED8; the cosine table 0x013CD550 (1025 floats: entry i
// and i+1 are interpolated). Sine is the cosine a quarter turn earlier (index - 256, masked to 1023).
struct C3DVector { float X, Y, Z; };
extern float g_CosineTable[1025];                        // 0x013CD550
struct CQuaternion {
    float X, Y, Z, W;
    void SetFromAxisAngle(const C3DVector& axis, float angle);
};
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
static inline float TableCos(float position, long offset)
{
    long i = FloatToLong(position - 0.5f);
    float f = position - (float)i;
    long index = (i + offset) & 0x3ff;
    return (1.0f - f) * g_CosineTable[index] + f * g_CosineTable[index + 1];
}
void CQuaternion::SetFromAxisAngle(const C3DVector& axis, float angle)
{
    float halfAngle = angle * 0.5f;
    float position = halfAngle * 1024.0f;
    float s = TableCos(position, -256);
    X = s * axis.X;
    Y = s * axis.Y;
    Z = s * axis.Z;
    W = TableCos(position, 0);
}
