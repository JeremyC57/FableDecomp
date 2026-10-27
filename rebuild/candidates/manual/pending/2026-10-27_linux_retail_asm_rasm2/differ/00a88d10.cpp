// CQuaternion::SetFromAxisAngle(const C3DVector&, float) -- set the rotation of `angle` (in turns) about the unit axis: (axis * sin(angle/2), cos(angle/2)) from the 1024-entry interpolated cosine table
// Retail-asm: 0x00A88D35 `fld dword ptr [esp + 4]` ; 0x00A88D39 `fistp dword ptr [esp]` and 0x00A88D98 `fld dword ptr [esp + 8]` ; 0x00A88D9C `fistp dword ptr [esp + 0x10]` -- the engine's inline-assembly float-to-long helper (rasm1 family (a)): the value is stored to the helper's by-value parameter slot and reloaded for a bare dword fistp into a frame word; VC7.1 C++ `(long)x` calls __ftol2, /QIfist emits `fistp qword` and /arch:SSE2 cvttss2si (rasm1 probe family (a)). Everything around it is C++.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A88D10; reconstructed from the parked reference body (rebuild/candidates/manual/pending), the C++ rounding stand-in replaced by the inline-assembly helper. Both table lookups round (position - 0.5) by the helper and read the result back through fild.
struct C3DVector { float X, Y, Z; };
extern float g_CosineTable[1025];                        // 0x013CD550
struct CQuaternion {
    float X, Y, Z, W;
    void SetFromAxisAngle(const C3DVector& axis, float angle);
};
// the engine's inline-assembly float-to-long helper (VC7.1 expands the __forceinline body and its __asm in place)
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
static __forceinline float TableCos(float position, long offset)
{
    long i = FloatToLong(position - 0.5f);
    float f = position - (float)i;
    long index = (i + offset) & 0x3ff;
    return (1.0f - f) * g_CosineTable[index] + f * g_CosineTable[index + 1];
}
void CQuaternion::SetFromAxisAngle(const C3DVector& axis, float angle)
{
    float position = angle * 0.5f * 1024.0f;
    float s = TableCos(position, -256);
    X = s * axis.X;
    Y = s * axis.Y;
    Z = s * axis.Z;
    W = TableCos(position, 0);
}
