// CPlane::Initialise(const C3DVector& a, const C3DVector& b, const C3DVector& c) -- the plane through three points: normal = normalised (b - a) x (c - a) (both edges normalised first), distance = normal . a; always succeeds
// Grade: DIFFER(269v269) -- scheduling: the cross product's integer copy loads come right after each fstp in ours, one x87 instruction later in retail
// Leaf: ?Initialise@CPlane@@QAE_NABVC3DVector@@00@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A42170, 269 bytes, `ret 0Ch`. PDB: ?Initialise@CPlane@@QAE_NABVC3DVector@@00@Z.
// Callees: C3DVector::AccurateNormalise 0x00A14440 (bound spelling ?AccurateNormalise@C3DVector@@QAEAAV1@XZ).
// Layout: Ego_r.pdb CPlane: Normal +0, Distance +0x0C.
// Shape: the edges are default-constructed locals ASSIGNED the operator results (copy-initialisation elides the temporary;
//   retail builds each in a frame temporary and copies it with integer moves, edge2 reusing edge1's temporary slot); the
//   cross product is assigned to the member the same way; the dot product is the
//   inline C3DVector::Dot; the shared retail C3DVector (operator^ = the FableWin ??TC3DVector cross product).
#define FABLE_RETAIL_C3DVECTOR
#include "engine/C3DVector.h"

class CPlane {
public:
    bool Initialise(const C3DVector& a, const C3DVector& b, const C3DVector& c);
    C3DVector Normal;                                   // +0x00
    float     Distance;                                 // +0x0c
};

bool CPlane::Initialise(const C3DVector& a, const C3DVector& b, const C3DVector& c)
{
    C3DVector edge1;
    edge1 = b - a;
    C3DVector edge2;
    edge2 = c - a;
    edge1.AccurateNormalise();
    edge2.AccurateNormalise();
    Normal = edge1 ^ edge2;
    Normal.AccurateNormalise();
    Distance = Normal.Dot(a);
    return true;
}
