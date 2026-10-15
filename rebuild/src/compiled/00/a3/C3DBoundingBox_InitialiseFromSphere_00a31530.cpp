// C3DBoundingBox::InitialiseFromSphere(const C3DVector& centre, const C3DVector& extent) -- MinCoord = centre - extent, MaxCoord = centre + extent
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A31530, 120 bytes, no calls (`ret 8`).
// Layout from the PDB (C3DBoundingBox: MinCoord +0, MaxCoord +0xC). Each side is a by-value C3DVector
// temporary built on the frame by the inline operator (user constructor) and then assigned word by word
// through integer registers.
struct C3DVector {
    float X;    // +0x00
    float Y;    // +0x04
    float Z;    // +0x08
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};

inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
inline C3DVector operator+(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }

struct C3DBoundingBox {
    C3DVector MinCoord;     // +0x00
    C3DVector MaxCoord;     // +0x0C
    void InitialiseFromSphere(const C3DVector& centre, const C3DVector& extent);
};

void C3DBoundingBox::InitialiseFromSphere(const C3DVector& centre, const C3DVector& extent)
{
    MinCoord = centre - extent;
    MaxCoord = centre + extent;
}