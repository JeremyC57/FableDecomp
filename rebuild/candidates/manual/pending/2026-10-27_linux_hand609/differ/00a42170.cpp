// CPlane::Initialise(const C3DVector& a, const C3DVector& b, const C3DVector& c) -- the plane through three points: normal = normalised (b - a) x (c - a) (both edges normalised first), distance = normal . a; always succeeds
// Grade: DIFFER(269v269) -- the two cross-product temporary copy loads (mov ecx/edx,[temp]) come one x87 instruction earlier than retail; chunk 609 tried: operator^ through a named result (adds an operand swap, worse). Earlier: class C3DVector leaf fix, forceinline copy ctor (same), user operator= (225, worse).
// History: Grade: DIFFER(269v269) -- the parked Leaf ?Initialise@CPlane@@QAE_NABVC3DVector@@00@Z bound no symbol (C3DVector was a
// Leaf: ?Initialise@CPlane@@QAE_NABVC3DVector@@00@Z
//   struct: U); C3DVector is now a class (V) so the leaf binds; residue unchanged: the two cross-temporary copy loads
//   (mov ecx/edx,[temp]) come one x87 instruction earlier than retail; tried a forceinline memberwise copy ctor (same)
//   and a user operator= (225, worse)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A42170, 269 bytes, `ret 0Ch`. PDB: ?Initialise@CPlane@@QAE_NABVC3DVector@@00@Z.
// Callees: C3DVector::Normalise 0x00A14440 (catalogued "GetScaled"; GOTCHAS chunk 160 names it).
// Layout: Ego_r.pdb CPlane: Normal +0, Distance +0x0C.
// Shape: the edges are default-constructed locals ASSIGNED the operator results (copy-initialisation elides the temporary;
//   retail builds each in a frame temporary and copies it with integer moves, edge2 reusing edge1's temporary slot); the
//   cross product is assigned to the member the same way; the dot product is the
//   inline `operator*`.
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator^(const C3DVector& o) const { return C3DVector(Y * o.Z - Z * o.Y, Z * o.X - X * o.Z, X * o.Y - Y * o.X); }
    float operator*(const C3DVector& o) const { return X * o.X + Y * o.Y + Z * o.Z; }
    void Normalise();                                   // 0x00A14440
};

class CPlane {
public:
    C3DVector Normal;                                   // +0x00
    float     Distance;                                 // +0x0c
    bool Initialise(const C3DVector& a, const C3DVector& b, const C3DVector& c);
};

bool CPlane::Initialise(const C3DVector& a, const C3DVector& b, const C3DVector& c)
{
    C3DVector edge1;
    edge1 = b - a;
    C3DVector edge2;
    edge2 = c - a;
    edge1.Normalise();
    edge2.Normalise();
    Normal = edge1 ^ edge2;
    Normal.Normalise();
    Distance = Normal * a;
    return true;
}
