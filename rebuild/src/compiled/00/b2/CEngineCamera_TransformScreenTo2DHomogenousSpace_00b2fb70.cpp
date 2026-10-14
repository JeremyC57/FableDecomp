// CEngineCamera::TransformScreenTo2DHomogenousSpace(C2DVector&) const -- map a screen position into the -1..1 homogeneous square: (p - origin) / half-size * 2 - 1, with Y flipped
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B2FB70, length 53 bytes.
// Callees: none. Constant 1.0 (0x0122DED8). Shape: pure x87 per component -- `fsub origin ; fdiv size ; fadd st,st(0)` (the `* 2` is a self-add) `; fsub 1.0`;
// the Y result is negated with `fchs` before its store. PDB CEngineCamera: CameraDesc at +0x14 (0x6c bytes) holds the screen origin (+0x14/+0x18) and the
// viewport scale lives at +0xb0/+0xb4 (beyond the PDB layout in the shipped build), so the members are modelled locally.
struct C2DVector {
    float X, Y;
};

struct CEngineCamera {
    unsigned char _pad[0x14];
    float m_screenOriginX;                         // +0x14
    float m_screenOriginY;                         // +0x18
    unsigned char _pad1c[0xb0 - 0x1c];
    float m_screenScaleX;                          // +0xb0
    float m_screenScaleY;                          // +0xb4
    void TransformScreenTo2DHomogenousSpace(C2DVector& position) const;
};

void CEngineCamera::TransformScreenTo2DHomogenousSpace(C2DVector& position) const
{
    position.X = (position.X - m_screenOriginX) / m_screenScaleX * 2.0f - 1.0f;
    position.Y = -((position.Y - m_screenOriginY) / m_screenScaleY * 2.0f - 1.0f);
}