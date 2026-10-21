// CGameCameraManager::IsPosCameraInsideThing(const CThing* thing, const C3DVector& pos) const -- model the thing as a capsule from its position up past its right eye dummy (1.2x the eye height) with 1.8x its collision radius, and report whether the camera position lies inside it
// Grade: DIFFER(475v475) -- one scheduling swap: retail loads the pos argument before the last normalised-axis store (fstp [esp+14h]); the inline normalise variant fixes that but then keeps 1/len on the x87 stack
// Leaf: IsPosCameraInsideThing
// Retail: Fable.exe (TLC, VC7.1) @ 0x00697490, 473 bytes, `ret 8`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / dtor 0x0099EAE0, CTCGraphicAppearance::GetDummyObject 0x004C08C0,
//   CThing::GetPosition 0x004C73D0; constants 1.2f 0x0125A690, 1.8f 0x0125C288, 1.0f 0x0122DED8 and the extern epsilon
//   0x0129BA3C (0.0001f).
// Layout: local projections; the thing keeps its physics/collision def at +0x60 (radius +0x34) and its graphic
//   appearance at +0x64.
#include <math.h>
#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
};
struct CCharStringData { CBasicStringChar Data; long NoRefs; };
#pragma pack(pop)
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector& operator*=(float s) { X *= s; Y *= s; Z *= s; return *this; }
    float Dot(const C3DVector& o) const { return X * o.X + (Y * o.Y + Z * o.Z); }
    float LengthSquared() const { return X * X + (Y * Y + Z * Z); }
    float Length() const { return (float)sqrt(X * X + (Y * Y + Z * Z)); }
};
struct CRightHandedSet { C3DVector Forward; C3DVector Up; };
struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString& name, long zero1, unsigned long zero2, C3DVector* position, CRightHandedSet* orientation, float scale);   // 0x004C08C0
};
struct CTCPhysics { unsigned char Data[0x34]; float Radius; };
struct CThing {
    unsigned char         Data[0x60];
    CTCPhysics*           Physics;                       // +0x60
    CTCGraphicAppearance* Appearance;                    // +0x64
    const C3DVector& GetPosition() const;                // 0x004C73D0
};
extern float g_CameraInsideEpsilon;                      // 0x0129BA3C (0.0001f)
static inline bool InRange(float value, float low, float high) { return value > low && value < high; }
struct CGameCameraManager {
    virtual void v00();
    bool IsPosCameraInsideThing(const CThing* thing, const C3DVector& pos) const;
};

bool CGameCameraManager::IsPosCameraInsideThing(const CThing* thing, const C3DVector& pos) const
{
    C3DVector eye;
    CRightHandedSet orientation;
    thing->Appearance->GetDummyObject("EYE_SET_R", 0, 0, &eye, &orientation, 1.0f);
    C3DVector base = thing->GetPosition();
    C3DVector top = base + (eye - base) * 1.2f;
    float radius = thing->Physics->Radius * 1.8f;
    C3DVector axis = top - base;
    float height = axis.Length();
    axis *= 1.0f / height;
    C3DVector offset = pos - base;
    double along = offset.Dot(axis);
    if (InRange(along, -g_CameraInsideEpsilon, height)) {
        C3DVector perp = offset - axis * (float)along;
        if (perp.LengthSquared() < radius * radius)
            return true;
    }
    return false;
}
