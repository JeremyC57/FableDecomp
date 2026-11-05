// CPhysicsMesh::CalculateCentreOfGravity() -- zero the centre of gravity, sum every mesh point into it with for_each(CAddElements), then scale it by 1 / point count
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6C890, length 110 bytes (+ int3 padding).
// Leaf: CalculateCentreOfGravity
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Callees: for_each<C3DVector*, CAddElements<C3DVector>> @ 0x00A6ECE0 (`__fastcall`: hidden result in ecx -- the
// `push ecx` frame word --, first in edx, last and the functor by value on the stack).
// Constants: 4294967296.0f @ 0x0122DCB4 (unsigned-to-float fix-up), 1.0f @ 0x0122DED8.
// Layout: Ego_r.pdb CPhysicsMesh with the retail 12-byte CArray: TotalPoints +0x3c/+0x40 (12-byte C3DVector elements),
// CentreOfGravity +0xa8 (PDB +0xc4).
// Shape: the centre is a reference local (`lea esi,[edi+0A8h]` before the stores; naming the member folds the offsets);
// the zero stores run z, y, x (`x = y = z = 0` through one register); the count is the unsigned element count
// (`/12` by the 0x2AAAAAAB multiply) converted with the 2^32 fix-up; `1.0f / count` is kept on the x87 stack and
// multiplied into x, y, z in place; `ret`.
#include "engine/C3DVector.h"

template <class T>
class CAddElements { public:
    T* Sum;
    CAddElements(T& sum) : Sum(&sum) { }
    void operator()(const T& value) { Sum->X += value.X; Sum->Y += value.Y; Sum->Z += value.Z; }
};
template <class It, class F> F __fastcall for_each(It first, It last, F f);   // 0x00A6ECE0 for (C3DVector*, CAddElements<C3DVector>)

#include "engine/CArray.h"

class CPhysicsMesh { public:
    unsigned char      _pad00[0x3c];
    CArray<C3DVector>  TotalPoints;                // +0x3c
    unsigned char      _pad48[0xa8 - 0x48];
    C3DVector          CentreOfGravity;            // +0xa8
    void CalculateCentreOfGravity();
};

void CPhysicsMesh::CalculateCentreOfGravity()
{
    C3DVector& centre = CentreOfGravity;
    centre.X = centre.Y = centre.Z = 0.0f;
    for_each(TotalPoints.begin(), TotalPoints.end(), CAddElements<C3DVector>(centre));
    centre *= 1.0f / TotalPoints.size();
}