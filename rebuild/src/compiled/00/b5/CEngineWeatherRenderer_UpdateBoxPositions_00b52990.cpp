// CEngineWeatherRenderer::UpdateBoxPositions() -- unless the weather boxes are frozen, rebuild them when the camera has moved a whole box size away from the current box centre on any axis
// Leaf: UpdateBoxPositions
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B52990, length 416 bytes (body 0x196), plain `ret`.
// Callees: the box rebuild @ 0x00B51AD0 (thiscall, no arguments).
// Layout: `this` offsets read from the retail body: freeze flag +0x11a (inside Settings +0x114), box size C3DVector +0x1d4,
//   box centre long[3] +0x1f0; camera position C3DVector at +0x54 of the global @ 0x01436EA0; scale 0x0122F59C.
#pragma optimize("y", off)
// Retail-asm: six expansions, 0x00B529C1..0x00B529DB (`fld [ebp-10h] ; fsub [ebp-14h] ; fistp dword ptr [ebp-8] ; fild [ebp-8] ; fadd [ebp-18h] ; fstp [ebp-0Ch] ; mov eax,[ebp-10h] ; cmp eax,[ebp-0Ch] ; jne ; inc dword ptr [ebp-8]`) through 0x00B52AE8..0x00B52B02 -- the engine's inline-assembly floor helper expanded in place (0.5f / 1.0f stored as frame constants, a bare dword fistp into a named long, the bit compare and `jne ; inc dword ptr` in the same 10-instruction block: rasm1 family (b)). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the result is read back by C++ after the block.
class C3DVector { public: float X, Y, Z; };
struct SCamera_01436ea0 { unsigned char _pad[0x54]; C3DVector Position; };
extern SCamera_01436ea0* g_pCamera_01436ea0;       // 0x01436EA0
extern "C" double __cdecl fabs(double);
#pragma intrinsic(fabs)

// the engine's inline-assembly FPU floor helper (0x0048D940 is its standalone copy): VC7.1 expands the __forceinline body in
// place; the by-value parameter and the four locals become frame words, which is why the function carries an ebp frame.
// The whole 10-instruction block is asm (the bit compare and the memory increment included: rasm1 family (b)); the result
// is read next by C++ after the block.
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

class CEngineWeatherRenderer { public:
    unsigned char _pad000[0x11a];
    bool          FreezeBoxes;                     // +0x11a
    unsigned char _pad11b[0x1d4 - 0x11b];
    C3DVector     BoxSize;                         // +0x1d4
    unsigned char _pad1e0[0x1f0 - 0x1e0];
    long          BoxCentre[3];                    // +0x1f0
    void RebuildBoxes();                           // 0x00B51AD0
    void UpdateBoxPositions();
};

void CEngineWeatherRenderer::UpdateBoxPositions()
{
    if (FreezeBoxes)
        return;
    long sizeX = FloorToLong(BoxSize.X * 0.5f);
    long sizeY = FloorToLong(BoxSize.Y * 0.5f);
    long sizeZ = FloorToLong(BoxSize.Z * 0.5f);
    const C3DVector& camera = g_pCamera_01436ea0->Position;
    long distX = FloorToLong((float)fabs((float)BoxCentre[0] - camera.X));
    long distY = FloorToLong((float)fabs((float)BoxCentre[1] - camera.Y));
    long distZ = FloorToLong((float)fabs((float)BoxCentre[2] - camera.Z));
    if (distX >= sizeX || distY >= sizeY || distZ >= sizeZ)
        RebuildBoxes();
}