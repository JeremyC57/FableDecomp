// CGameCameraBase::IsLandscapeCollision() -- march from the camera position towards the point it looks at in quarter-unit steps, sampling the ground height; if the ground rises above the camera anywhere on the way, lift the camera to two units above the highest sample and report a collision
// Leaf: IsLandscapeCollision
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FD6C0, 498 bytes, `ret`.
// Callees: the world's map accessor (virtual slot 12), C3DVector::Normalise 0x00A14440 (catalog label GetScaled),
//   CWorldMap::GetGroundSizeZAt 0x0063A300; constants 0.25f 0x0123078C, 1.0f 0x0122DED8, 2.0f 0x0122DCDC.
// Layout: local projection -- the generated CGameCameraBase header flattens the camera position and PointingAt into
//   scalar floats, which cannot carry the C3DVector arithmetic; offsets match it (position +0x04, PointingAt +0x80,
//   World +0x8c).
#include <math.h>
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
    float Length() const { return (float)sqrt(X * X + (Y * Y + Z * Z)); }
    void Normalise();                                    // 0x00A14440
};
struct CWorldMap { float GetGroundSizeZAt(const C3DVector& pos) const; };   // 0x0063A300
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CWorldMap* GetWorldMap() const;              // slot 12
};
struct CGameCameraBase {
    void*         VTable;
    C3DVector     Position;                              // +0x04 (InternalCam's position)
    unsigned char InternalCam[0x2c];
    unsigned char LastInternalCam[0x38];                 // +0x3c
    C3DVector     LastPointingAt;                        // +0x74
    C3DVector     PointingAt;                            // +0x80
    CWorld*       World;                                 // +0x8c
    bool IsLandscapeCollision();
};

bool CGameCameraBase::IsLandscapeCollision()
{
    CWorldMap* map = World->GetWorldMap();
    C3DVector start = Position;
    C3DVector target = PointingAt;
    C3DVector delta = target - start;
    C3DVector dir = delta;
    dir.Normalise();
    C3DVector step = dir * 0.25f;
    C3DVector pos = start;
    C3DVector highest = start;
    if (delta.Length() > 1.0f) {
        do {
            float height = map->GetGroundSizeZAt(pos);
            if (height > start.Z) {
                highest = pos;
                highest.Z = height;
            }
            pos += step;
        } while ((target - pos).Length() > 1.0f);
    }
    if (highest.Z > start.Z) {
        Position = C3DVector(highest.X, highest.Y, highest.Z + 2.0f);
        return true;
    }
    return false;
}
