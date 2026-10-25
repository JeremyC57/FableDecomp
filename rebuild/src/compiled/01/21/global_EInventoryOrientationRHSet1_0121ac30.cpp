// _global::$E_InventoryOrientationRHSet1() -- dynamic initializer of CRightHandedSet g_InventoryOrientationRHSet1 @ 0x013B8BCC: Up = (sqrt(0.5), -sqrt(0.5), 0), Forward = (-sqrt(0.5), -sqrt(0.5), 0)
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121AC30, 64 bytes (+ padding), plain `ret`, reached from the .CRT$XCU table; catalog "$E2" is the compiler's generic name.
// Identity: CTCInventoryItem::GetRHSetFromOrientation(EInventoryObjectOrientation) @ 0x005D8EF0 (Ego_r.pdb, private
//   static __fastcall, returns CRightHandedSet by value) switches over 12 orientations and copies one of twelve 24-byte
//   CRightHandedSet globals through the copy constructor 0x0049CF80; case 1 is this object. The object's real
//   identifier is not in the PDB name set: the name is descriptive.
// Shape: VC7.1 folds every constant store of the inlined constructors into the object's static image (the zero Z words, so the object lives in .bss)
//   and keeps only the sqrt-derived words dynamic; C3DVector needs its memberwise user copy constructor for that (with
//   the implicit bitwise copy the constants go through the temporaries and nothing is folded). sqrt is the float
//   overload `sqrt(0.5f)` from <math.h> (inline `(float)sqrt((double)x)` over the x87 intrinsic on the 0.5 double at
//   0x012316F0): the fsqrt is shared, but each component's negation is its own inline expansion, so the negative words
//   CSE only per position (two `fchs`, as retail); the double `-sqrt(0.5)` spelling CSEs all of them into one `fchs`.

#include <math.h>
#pragma intrinsic(sqrt)

class C3DVector {                                  // Ego_r.pdb C3DVector {X, Y, Z}
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

class CRightHandedSet {                            // Ego_r.pdb CRightHandedSet {Up +0, Forward +0xc}, 24 bytes
public:
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) { }   // Ego_r 0x00418440 (inline here)
    CRightHandedSet(const CRightHandedSet& other);                                                  // 0x0049CF80
};

CRightHandedSet g_InventoryOrientationRHSet1(C3DVector(sqrt(0.5f), -sqrt(0.5f), 0.0f), C3DVector(-sqrt(0.5f), -sqrt(0.5f), 0.0f));