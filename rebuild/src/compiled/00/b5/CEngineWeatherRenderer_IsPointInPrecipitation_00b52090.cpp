// CEngineWeatherRenderer::IsPointInPrecipitation(const C3DVector& point) -- true when the point lies within any of the three weather masks' bounding spheres grown by the weather box's bounding-sphere radius
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B52090, 142 bytes (+ int3 padding).
// Callees: CEngineWeatherRenderer::GetBoxBoundingSphereRadiusSquared() @ 0x00B50E50 (float), the `fsqrt` intrinsic.
// Layout: engine/CEngineWeatherRenderer.h keeps WeatherMaskList (CWeatherMask[3], 0x5c bytes each) at +0x66c as opaque bytes;
//   a CWeatherMask starts with its C3DBoundingSphere BSphere { C3DVector Centre; float Radius; } (Ego_r.pdb), so the
//   class is modelled locally with the masks typed.
// Shape: the box radius is a named float local (stored once to the frame); the loop walks the masks by pointer (stride
//   0x5c) with an int counter; the sphere centre comes through a BY-VALUE getter (`GetCentre()`, a memberwise-copied C3DVector
//   temporary: x/y stay on the FPU, z is copied as a dword into the frame) and `(point - centre).Length()` builds the
//   difference through C3DVector's value constructor (dx spilled to the frame), VC summing z*z + y*y before x*x; the 0x1c
//   frame is the box radius plus the two vector temporaries. `Length() < boxRadius + Radius` is `fcompp ; test ah,41h ;
//   je`; the result is an int 0/1. (Reading `sphere.Centre` directly folds the subtraction into memory operands.)
extern "C" double __cdecl sqrt(double value);
#pragma intrinsic(sqrt)

struct C3DVector {
    float x, y, z;
    C3DVector(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
    C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(x - o.x, y - o.y, z - o.z); }
    float Length() const { return (float)sqrt(x * x + y * y + z * z); }
};

struct C3DBoundingSphere {
    C3DVector Centre;                            // +0x00
    float     Radius;                            // +0x0c
    C3DVector GetCentre() const { return Centre; }
};

struct CWeatherMask {                            // 0x5c bytes
    C3DBoundingSphere BSphere;                   // +0x00
    unsigned char     _rest[0x5c - 0x10];        // BBox, InvTransform, Scale
};

struct CEngineWeatherRenderer {
    unsigned char _pad0[0x66c];
    CWeatherMask  WeatherMaskList[3];            // +0x66c
    float GetBoxBoundingSphereRadiusSquared();   // 0x00B50E50
    long IsPointInPrecipitation(const C3DVector& point);
};

long CEngineWeatherRenderer::IsPointInPrecipitation(const C3DVector& point)
{
    float boxRadius = (float)sqrt(GetBoxBoundingSphereRadiusSquared());
    for (long i = 0; i < 3; ++i) {
        const C3DBoundingSphere& sphere = WeatherMaskList[i].BSphere;
        if ((point - sphere.GetCentre()).Length() < boxRadius + sphere.Radius)
            return 1;
    }
    return 0;
}