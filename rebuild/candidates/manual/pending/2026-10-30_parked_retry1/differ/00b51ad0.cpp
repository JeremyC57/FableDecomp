// Grade: DIFFER(965v969) -- the floor rounding is the landed FloorToLong retail_asm helper and the snap is in place; residue is the outer loop counter: retail counts 9 down in edx with the column in a frame word (pointer base +0x20C), ours compares the column to 4
// CEngineWeatherRenderer::BuildBoxPositionMatrix() -- snap the camera position to the weather box grid (round each axis to the nearest box multiple, Z only when not locked) and fill the 9 x 9 box-centre position table around it
// Retail-asm: 0x00B51AF6 fld dword ptr [ebp-10h] ; fsub dword ptr [ebp-0Ch] ; fistp dword ptr [ebp-4] ; inc dword ptr [ebp-4] -- the landed FloorToLong helper
// Leaf: ?BuildBoxPositionMatrix@CEngineWeatherRenderer
struct C3DVector { float X, Y, Z; };
struct CCamera { unsigned char _pad[0x54]; C3DVector Position; };
extern CCamera* g_pWeatherCamera;                              // 0x01436EA0
// the engine's inline-assembly floor helper (the landed CThingPhysical::ModifyHealth 0x006A68F0 FloorToLong, verbatim)
__forceinline long FloorToLong(float value)
{
    long  result;
    float one;
    float half;
    float next;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fsub  half
        fistp result
        fild  result
        fadd  one
        fstp  next
        mov   eax, value
        cmp   eax, next
        jne   done
        inc   result
    done:
    }
    return result;
}
inline void SnapToGrid(long& value, long size)
{
    long remainder = value % size;
    value = value / size * size;
    if (remainder >= size / 2)
        value += size;
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
    CentreX = FloorToLong(camera.X);
    SnapToGrid(CentreX, FloorToLong(BoxSizeX));
    CentreY = FloorToLong(camera.Y);
    SnapToGrid(CentreY, FloorToLong(BoxSizeY));
    if (!ZLocked) {
        CentreZ = FloorToLong(camera.Z);
        SnapToGrid(CentreZ, FloorToLong(BoxSizeZ));
    }
    for (long i = -4; i <= 4; ++i) {
        float x = (float)i;
        for (long j = -4; j <= 4; ++j) {
            Boxes[i + 4][j + 4].X = x * BoxSizeX + CentreX;
            Boxes[i + 4][j + 4].Y = (float)j * BoxSizeY + CentreY;
            Boxes[i + 4][j + 4].Z = (float)CentreZ;
        }
    }
}
