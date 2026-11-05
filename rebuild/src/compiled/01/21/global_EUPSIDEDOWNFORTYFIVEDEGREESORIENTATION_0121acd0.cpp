// _global::$E_InventoryOrientationRHSet5() -- dynamic initializer of the anonymous-namespace const CRightHandedSet UPSIDE_DOWN_FORTY_FIVE_DEGREES_ORIENTATION @ 0x013B8BE4: Up = (-sqrt(0.5), sqrt(0.5), 0), Forward = (-sqrt(0.5), -sqrt(0.5), 0)
// Leaf: _$E1
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121ACD0, 70 bytes (+ padding), plain `ret`, reached from the .CRT$XCU table; catalog "$E2" is the compiler's generic name.
// Identity: CTCInventoryItem::GetRHSetFromOrientation(EInventoryObjectOrientation) @ 0x005D8EF0 (Ego_r.pdb, private
//   static __fastcall, returns CRightHandedSet by value) switches over 12 orientations and copies one of twelve 24-byte
//   CRightHandedSet globals through the copy constructor 0x0049CF80; case 5 is this object. The object's real
//   identifier is the FableWin.pdb anonymous-namespace constant UPSIDE_DOWN_FORTY_FIVE_DEGREES_ORIENTATION of the CTCInventoryItem TU (chunk c675:
//   dynamic-initializer order); naming wave 17 respelled the descriptive g_InventoryOrientationRHSet5 (name-only).
// Shape: VC7.1 folds every constant store of the inlined constructors into the object's static image (the zero Z words, so the object lives in .bss)
//   and keeps only the sqrt-derived words dynamic; C3DVector needs its memberwise user copy constructor for that (with
//   the implicit bitwise copy the constants go through the temporaries and nothing is folded). sqrt is the float
//   overload `sqrt(0.5f)` from <math.h> (inline `(float)sqrt((double)x)` over the x87 intrinsic on the 0.5 double at
//   0x012316F0): the fsqrt is shared, but each component's negation is its own inline expansion, so the negative words
//   CSE only per position (two `fchs`, as retail); the double `-sqrt(0.5)` spelling CSEs all of them into one `fchs`.

#include <math.h>
#pragma intrinsic(sqrt)

#include "engine/C3DVector.h"

class CRightHandedSet {                            // Ego_r.pdb CRightHandedSet {Up +0, Forward +0xc}, 24 bytes
public:
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) { }   // Ego_r 0x00418440 (inline here)
    CRightHandedSet(const CRightHandedSet& other);                                                  // 0x0049CF80
};

namespace {
const CRightHandedSet UPSIDE_DOWN_FORTY_FIVE_DEGREES_ORIENTATION(C3DVector(-sqrt(0.5f), sqrt(0.5f), 0.0f), C3DVector(-sqrt(0.5f), -sqrt(0.5f), 0.0f));
}
