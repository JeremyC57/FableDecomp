// Grade: DIFFER(514v517) -- frame size 0x38 vs retail 0x44: retail keeps the operator+/- C3DVector results of the bounding-sphere section as separate 12-byte frame temporaries; the box section matches
// CPhysicsMesh::CalculateBoundingInfo() -- fit the mesh's bounds to its points: the axis-aligned box (per-axis min/max of every point), the bounding sphere around the box centre (radius = half the box diagonal) and the XY bounding circle (radius = half the XY diagonal)
// Leaf: CalculateBoundingInfo
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A6C640, 517 bytes (+ int3 padding), plain `ret`.
// Callees: none (the 0.5f constant is 0x0122F59C).
// Layout: Ego_r.pdb CPhysicsMesh (0xdc) with retail 12-byte containers (PDB 16/20) ahead of the bounds: TotalPoints
//   (CArray<C3DVector>) +0x3c, BoundingSphere +0x74 (Center, Radius), BoundingBox +0x84 (MinCoord, MaxCoord),
//   BoundingCircle +0x9c (CCircle: Centre, Radius). engine headers carry the PDB offsets, so the class is modelled locally.
extern "C" float __cdecl sqrtf(float);
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
    C3DVector operator+(const C3DVector& other) const { return C3DVector(X + other.X, Y + other.Y, Z + other.Z); }
    C3DVector operator-(const C3DVector& other) const { return C3DVector(X - other.X, Y - other.Y, Z - other.Z); }
    C3DVector operator*(float scale) const { return C3DVector(X * scale, Y * scale, Z * scale); }
};
struct C3DBoundingSphere {
    C3DVector Center;
    float     Radius;
    void Set(const C3DVector& centre, const C3DVector& half)
    {
        Center = centre;
        Radius = (float)sqrt(half.X * half.X + half.Y * half.Y + half.Z * half.Z);
    }
};
struct C3DBoundingBox {
    C3DVector MinCoord;
    C3DVector MaxCoord;
    void Set(const C3DVector& minimum, const C3DVector& maximum) { MinCoord = minimum; MaxCoord = maximum; }
};
struct CCircle {
    C2DVector Centre;
    float     Radius;
};
template <class T> struct CArray {                     // retail 12 bytes
    T* m_first;
    T* m_last;
    T* m_end;
    unsigned long size() const { return m_last - m_first; }
    const T& operator[](unsigned long i) const { return m_first[i]; }
};
class CPhysicsMesh {
public:
    unsigned char     _head00[0x3c];                   // +0x00 vptr, refcount, helper/dummy maps, face indices
    CArray<C3DVector> TotalPoints;                     // +0x3c
    unsigned char     _nav48[0x2c];                    // +0x48 navigation lines and volumes, containment volumes
    C3DBoundingSphere BoundingSphere;                  // +0x74
    C3DBoundingBox    BoundingBox;                     // +0x84
    CCircle           BoundingCircle;                  // +0x9c
    void CalculateBoundingInfo();
};

void CPhysicsMesh::CalculateBoundingInfo()
{
    C3DVector minimum = TotalPoints[0];
    C3DVector maximum = TotalPoints[0];
    for (unsigned long i = 0; i < TotalPoints.size(); ++i) {
        const C3DVector& point = TotalPoints[i];
        if (point.X < minimum.X)
            minimum.X = point.X;
        if (point.Y < minimum.Y)
            minimum.Y = point.Y;
        if (point.Z < minimum.Z)
            minimum.Z = point.Z;
        if (point.X > maximum.X)
            maximum.X = point.X;
        if (point.Y > maximum.Y)
            maximum.Y = point.Y;
        if (point.Z > maximum.Z)
            maximum.Z = point.Z;
    }
    BoundingBox.Set(minimum, maximum);
    C3DVector centre = (maximum + minimum) * 0.5f;
    C3DVector half = (maximum - minimum) * 0.5f;
    BoundingSphere.Set(centre, half);
    BoundingCircle.Centre = C2DVector(centre.X, centre.Y);
    BoundingCircle.Radius = (float)sqrt(half.X * half.X + half.Y * half.Y);
}
