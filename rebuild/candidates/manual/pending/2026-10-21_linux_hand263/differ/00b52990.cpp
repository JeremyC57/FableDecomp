// Grade: DIFFER(387v406) -- lane gap: inline asm. Retail inlines an asm fistp floor helper (ebp frame, 0.5/1.0 spilled to locals, fistp/fild per axis); compiler-emitted C++ goes through __ftol2, so the body cannot be reproduced byte-pure
// CEngineWeatherRenderer::UpdateBoxPositions() -- unless the weather boxes are frozen, rebuild them when the camera has moved a whole box size away from the current box centre on any axis
// Leaf: UpdateBoxPositions
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B52990, length 416 bytes (body 0x196), plain `ret`.
// Callees: the box rebuild @ 0x00B51AD0 (thiscall, no arguments).
// Layout: `this` offsets read from the retail body: freeze flag +0x11a (inside Settings +0x114), box size C3DVector +0x1d4,
//   box centre long[3] +0x1f0; camera position C3DVector at +0x54 of the global @ 0x01436EA0; scale 0x0122F59C.
struct C3DVector { float X, Y, Z; };
struct SCamera_01436ea0 { unsigned char _pad[0x54]; C3DVector Position; };
extern SCamera_01436ea0* g_pCamera_01436ea0;       // 0x01436EA0
extern "C" double __cdecl fabs(double);
#pragma intrinsic(fabs)

inline long FloorToInt(float value)
{
    long result = (long)(value - 0.5f);
    if (value == (float)result + 1.0f)
        ++result;
    return result;
}

struct CEngineWeatherRenderer {
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
    long sizeX = FloorToInt(BoxSize.X * 0.5f);
    long sizeY = FloorToInt(BoxSize.Y * 0.5f);
    long sizeZ = FloorToInt(BoxSize.Z * 0.5f);
    const C3DVector& camera = g_pCamera_01436ea0->Position;
    long distX = FloorToInt((float)fabs((float)BoxCentre[0] - camera.X));
    long distY = FloorToInt((float)fabs((float)BoxCentre[1] - camera.Y));
    long distZ = FloorToInt((float)fabs((float)BoxCentre[2] - camera.Z));
    if (distX >= sizeX || distY >= sizeY || distZ >= sizeZ)
        RebuildBoxes();
}
