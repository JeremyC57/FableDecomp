// _global::SwapYZ(const C3DVector& v) -- returns the vector with its Y and Z components exchanged (X copied as is)
// Leaf: SwapYZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1BFB0, length 19 bytes.
// Callers: 0x00C1C4A4/B0/BC (three consecutive 12-byte vectors at +8, +0x14, +0x20 of one record
// converted into frame locals) and 0x00C1C554 -- a Y-up/Z-up axis conversion.
// Label: the manifest's "CTCCameraPointScripted::EditSetThingForEditorSetThing" is propagated and rejected.
// Shape: `__fastcall` (the 0x00C1xxxx module is built /Gr): the hidden result pointer in ecx (returned
// in eax), the source reference in edx, `ret`. Returning `C3DVector(v.X, v.Z, v.Y)` through the
// three-float constructor evaluates the arguments right to left -- v.Y (+4) and v.Z (+8) are loaded on
// the x87 stack and X passes through an integer register -- then stores Y slot <- v.Z and Z slot <- v.Y
// (`fstp [eax+4]` of the later load). A plain field-by-field copy becomes three integer moves instead.
struct C3DVector {
    float X;
    float Y;
    float Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

C3DVector __fastcall SwapYZ(const C3DVector& pos)
{
    return C3DVector(pos.X, pos.Z, pos.Y);
}