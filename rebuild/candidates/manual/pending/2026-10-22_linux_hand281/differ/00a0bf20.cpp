// Grade: DIFFER(517v517) -- one x87 scheduling swap: retail emits the second Normalise's 'mov esi,0x17c' before 'fld [esp+0x20]' (the up.Y load); every other instruction matches. Forms tried: init-list MakeRHSet, RHSet ctor, body stores in 7 orders, 6 up-vector spellings.
// CCamera::CCamera(const C3DVector& position, const C3DVector& lookAt, float fov, bool flag) -- place the camera at
//   position looking at lookAt: forward is the fast-normalised direction, up is world Z made orthogonal to it
//   (Gram-Schmidt) and fast-normalised; both FOV fields start at fov, the vertical FOV at 0, nothing locked, and
//   the FOV flags 4 when flag is set, else 1
// Leaf: CCamera
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0BF20, 517 bytes (528 with padding); `ret 10h`.
// Callees: none. Globals: the reciprocal-square-root mantissa table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C,
//   0.0f 0x0122DEDC, 1.0f 0x0122DED8.
// Layout: Ego_r.pdb CCamera (56 bytes): WorldPosition +0, RHSet {Up +0xc, Forward +0x18}, HeightLocked +0x24,
//   ViewVecZLocked +0x25, FOVFlags +0x28, HorizontalFOV +0x2c, VerticalFOV +0x30, Zoom +0x34.
// Shape: the fast inverse square root as its own __forceinline helper (r spilled to memory, as retail); C3DVector with
//   NO user copy ctor (integer copies) and operator* returning a named local (the extra temp copy); RHSet set through an
//   inline Set(); scalar stores HorizontalFOV, VerticalFOV, Zoom, FOVFlags, bools (the retail store schedule).
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558

__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    r = (3.0f - r * r * lengthSquared) * r;
    return r * 0.5f;
}

struct C3DVector {
    float X;
    float Y;
    float Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { C3DVector r(X * s, Y * s, Z * s); return r; }
    float Dot(const C3DVector& o) const { return X * o.X + Y * o.Y + Z * o.Z; }
    __forceinline void Normalise()
    {
        float lengthSquared = Z * Z;
        lengthSquared += Y * Y;
        lengthSquared += X * X;
        float r = FastInvSqrt(lengthSquared);
        X *= r;
        Y *= r;
        Z *= r;
    }
};
struct CRightHandedSet {
    C3DVector Up;           // +0x00
    C3DVector Forward;      // +0x0c
    CRightHandedSet() {}
    __forceinline void Set(const C3DVector& up, const C3DVector& forward) { Up = up; Forward = forward; }
};
class CCamera {
public:
    C3DVector       WorldPosition;      // +0x00
    CRightHandedSet RHSet;              // +0x0c
    bool            HeightLocked;       // +0x24
    bool            ViewVecZLocked;     // +0x25
    unsigned long   FOVFlags;           // +0x28
    float           HorizontalFOV;      // +0x2c
    float           VerticalFOV;        // +0x30
    float           Zoom;               // +0x34
    CCamera(const C3DVector& position, const C3DVector& lookAt, float fov, bool flag);
};

CCamera::CCamera(const C3DVector& position, const C3DVector& lookAt, float fov, bool flag)
    : WorldPosition(position)
{
    C3DVector forward = lookAt - WorldPosition;
    forward.Normalise();
    C3DVector worldUp(0.0f, 0.0f, 1.0f);
    C3DVector up = worldUp - forward * forward.Dot(worldUp);
    up.Normalise();
    RHSet.Set(up, forward);
    HorizontalFOV = fov;
    VerticalFOV = 0.0f;
    Zoom = fov;
    FOVFlags = flag ? 4 : 1;
    HeightLocked = false;
    ViewVecZLocked = false;
}
