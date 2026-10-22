// Grade: DIFFER(951v969) -- retail rounds each axis with an inline-asm fistp helper (ebp frame, 0.5/1.0 constants stored to frame slots, fild/fadd/fstp, integer compare): asm_bake family lane gap, parked at once; the grid snap and the unrolled 9x9 fill are otherwise as written
// CEngineWeatherRenderer::BuildBoxPositionMatrix() -- snap the camera position to the weather box grid (round each axis to the nearest box multiple, Z only when not locked) and fill the 9 x 9 box-centre position table around it
// Leaf: ?BuildBoxPositionMatrix@CEngineWeatherRenderer
struct C3DVector { float X, Y, Z; };
struct CCamera { unsigned char _pad[0x54]; C3DVector Position; };
extern CCamera* g_pWeatherCamera;                              // 0x01436EA0
inline long RoundUp(float value)
{
    long n = (long)(value - 0.5f);
    float t = (float)n + 1.0f;
    if (*(long*)&value == *(long*)&t)
        ++n;
    return n;
}
inline long SnapToGrid(long value, long size)
{
    long snapped = value / size * size;
    if (value % size >= size / 2)
        snapped += size;
    return snapped;
}
struct CEngineWeatherRenderer {
    unsigned char _pad0[0x11b];
    bool ZLocked;                                              // +0x11b
    unsigned char _pad11c[0xb8];
    float BoxSizeX, BoxSizeY, BoxSizeZ;                        // +0x1d4
    unsigned char _pad1e0[0x10];
    long CentreX, CentreY, CentreZ;                            // +0x1f0
    C3DVector Boxes[9][9];                                     // +0x1fc
    void BuildBoxPositionMatrix();
};
void CEngineWeatherRenderer::BuildBoxPositionMatrix()
{
    C3DVector& camera = g_pWeatherCamera->Position;
    CentreX = RoundUp(camera.X);
    CentreX = SnapToGrid(CentreX, RoundUp(BoxSizeX));
    CentreY = RoundUp(camera.Y);
    CentreY = SnapToGrid(CentreY, RoundUp(BoxSizeY));
    if (!ZLocked) {
        CentreZ = RoundUp(camera.Z);
        CentreZ = SnapToGrid(CentreZ, RoundUp(BoxSizeZ));
    }
    static const float rows[9] = { 4.0f, 3.0f, 2.0f, 1.0f, 0.0f, -1.0f, -2.0f, -3.0f, -4.0f };
    for (long i = -4; i <= 4; ++i) {
        float x = (float)i;
        for (long j = 0; j < 9; ++j) {
            Boxes[i + 4][j].X = x * BoxSizeX + CentreX;
            Boxes[i + 4][j].Y = CentreY - BoxSizeY * rows[j];
            Boxes[i + 4][j].Z = (float)CentreZ;
        }
    }
}
