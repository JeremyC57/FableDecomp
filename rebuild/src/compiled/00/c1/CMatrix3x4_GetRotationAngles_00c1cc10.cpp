// CMatrix3x4::GetRotationAngles(float& first, float& second, float& third) const -- decompose the rotation into three angles by peeling them off a working copy
// Leaf: GetRotationAngles
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1CC10, 133 bytes (`ret 0Ch`). Callees: the __stdcall Atan2(float, float)
// 0x00C1CAB0 (fpatan with quadrant fix-ups), the two in-place rotations CMatrix3x4::Rotate 0x00C1C990 and
// 0x00C1C900 (thiscall, one float angle).
// The catalog name "C3DMeshFileBoneChunk::SetLocalizationMatrix" is a propagated label: `this` is a 48-byte
// matrix copied whole (`rep movsd`, 12 dwords) into a local, and the three float references are written.
// Rows are {Right, Up, Forward, Position}: first = Atan2(Forward.Z, -Forward.X), the copy is rotated by -first,
// second = Atan2(Forward.Z, Forward.Y), rotated by -second, third = Atan2(Right.X, -Right.Y). Each angle stays
// on the FPU stack for the store and the negated rotate argument.
float __stdcall Atan2(float x, float y);         // 0x00C1CAB0

class C3DVector { public: float X, Y, Z; };

class CMatrix3x4 { public:
    C3DVector Right;        // +0x00
    C3DVector Up;           // +0x0C
    C3DVector Forward;      // +0x18
    C3DVector Position;     // +0x24
    void RotateA(float angle);                   // 0x00C1C990
    void RotateB(float angle);                   // 0x00C1C900
    void GetRotationAngles(float& first, float& second, float& third) const;
};

void CMatrix3x4::GetRotationAngles(float& first, float& second, float& third) const
{
    CMatrix3x4 m = *this;
    first = Atan2(m.Forward.Z, -m.Forward.X);
    m.RotateA(-first);
    second = Atan2(m.Forward.Z, m.Forward.Y);
    m.RotateB(-second);
    third = Atan2(m.Right.X, -m.Right.Y);
}