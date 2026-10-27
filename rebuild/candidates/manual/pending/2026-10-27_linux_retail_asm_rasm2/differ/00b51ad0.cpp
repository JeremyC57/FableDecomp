// CEngineWeatherRenderer::BuildBoxPositionMatrix() -- snap the camera position to the weather box grid (round each axis to the nearest box multiple, Z only when not locked) and fill the 9 x 9 box-centre position table around it
// Leaf: ?BuildBoxPositionMatrix@CEngineWeatherRenderer
// Retail-asm: 0x00B51AF6..0x00B51B16 `fld [ebp - 0x10] ; fsub [ebp - 0xc] ; fistp [ebp - 4] ; fild [ebp - 4] ; fadd [ebp - 8] ; fstp [ebp - 0x14] ; mov eax, [ebp - 0x10] ; cmp eax, [ebp - 0x14] ; jne 0xb51b13 ; inc [ebp - 4] ; mov edx, [ebp - 4] ; mov eax, [ecx + 0x1d4]` and 0x00B51B33..0x00B51B56 `fld [ebp - 0xc] ; fsub [ebp - 0x10] ; fistp [ebp - 4] ; fild [ebp - 4] ; fadd [ebp - 0x14] ; fstp [ebp - 8] ; mov eax, [ebp - 0xc] ; cmp eax, [ebp - 8] ; jne 0xb51b50 ; inc [ebp - 4] ; mov eax, [ecx + 0x1f0] ; mov edi, [ebp - 4]` and 0x00B51B94..0x00B51BB4 `fld [ebp - 0xc] ; fsub [ebp - 0x10] ; fistp [ebp - 4] ; fild [ebp - 4] ; fadd [ebp - 0x14] ; fstp [ebp - 8] ; mov eax, [ebp - 0xc] ; cmp eax, [ebp - 8] ; jne 0xb51bb1 ; inc [ebp - 4] ; mov eax, [ebp - 4] ; mov edx, [ecx + 0x1d8]` and 0x00B51BD1..0x00B51BF4 `fld [ebp - 0xc] ; fsub [ebp - 0x10] ; fistp [ebp - 4] ; fild [ebp - 4] ; fadd [ebp - 0x14] ; fstp [ebp - 8] ; mov eax, [ebp - 0xc] ; cmp eax, [ebp - 8] ; jne 0xb51bee ; inc [ebp - 4] ; mov eax, [ecx + 0x1f4] ; mov edi, [ebp - 4]` -- the engine's inline-assembly floor helper expanded in place (0.5f / 1.0f stored as frame constants, a bare dword fistp into a named long, the bit compare and `jne ; inc dword ptr` in the same 10-instruction block: rasm1 family (b)). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the result is read back by C++ after the block.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B51AD0; reconstructed from the parked reference body (rebuild/candidates/manual/pending), the C++ rounding stand-in replaced by the inline-assembly helper. Six floor blocks (the three axes, each twice).
#pragma optimize("y", off)
struct C3DVector { float X, Y, Z; };
struct CCamera { unsigned char _pad[0x54]; C3DVector Position; };
extern CCamera* g_pWeatherCamera;                              // 0x01436EA0
// the engine's inline-assembly FPU floor helper (0x0048D940 is its standalone copy): VC7.1 expands the __forceinline body in
// place; the by-value parameter and the four locals become frame words, which is why the function carries an ebp frame.
__forceinline long RoundUp(float value)
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
