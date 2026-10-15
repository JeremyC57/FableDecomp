// CMatrix3x4::Set(const CRightHandedSet& orientation, const C3DVector& position) -- build a 3x4 transform from an up/forward orientation (right = forward x up) and a translation
// Leaf: Set
// Grade: DIFFER(127v127) -- one store: retail writes the spilled right.z after right.x (`fstp [ecx] ; mov [ecx+8],edx ; fstp [ecx+4]`), ours before both FPU stores; everything else (cross product via an inline operator^, plain row copies) matches
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007767A0, length 127 bytes.
// The catalog calls this CRightHandedSet::GetRight: GetRight (PDB ?GetRight@CRightHandedSet@@QBE?AVC3DVector@@XZ,
// out of line at 0x004A67D0) is only INLINED here. `this` (ecx) is a 48-byte four-row matrix: the routine writes
// the right vector, then Forward, Up and the position, returns nothing (eax ends as the second argument) and takes
// two reference arguments (`ret 8`). No direct callers (reached through a pointer), so the owning name is a model.
// PDB: CRightHandedSet (24 bytes) = C3DVector Up (+0x00), C3DVector Forward (+0x0C).
// Shape: GetRight returns a C3DVector by value; VC keeps x/y of that temporary on the FPU and spills z through the
// frame (`fstp [esp+8] ; mov edx,[esp+8]`), the by-value getter result shape (GOTCHAS wave 25 chunk 138).
struct C3DVector {
    C3DVector() {}
    C3DVector(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    C3DVector& operator=(const C3DVector& v) { x = v.x; y = v.y; z = v.z; return *this; }
    // cross product
    C3DVector operator^(const C3DVector& v) const
    {
        return C3DVector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
    float x, y, z;
};

struct CRightHandedSet {
    C3DVector Up;        // +0x00
    C3DVector Forward;   // +0x0C

    C3DVector GetRight() const { return Forward ^ Up; }
};

struct CMatrix3x4 {
    C3DVector Right;     // +0x00
    C3DVector Forward;   // +0x0C
    C3DVector Up;        // +0x18
    C3DVector Position;  // +0x24

    void Set(const CRightHandedSet& orientation, const C3DVector& position);
};

void CMatrix3x4::Set(const CRightHandedSet& orientation, const C3DVector& position)
{
    Right = orientation.GetRight();
    Forward = orientation.Forward;
    Up = orientation.Up;
    Position = position;
}
