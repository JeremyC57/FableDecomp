// std::for_each(C3DVector* first, C3DVector* last, CAddElements<C3DVector> f) -- add every vector of [first, last) into the functor's running sum; returns the functor
// Leaf: ??$for_each@
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6ECE0, 64 bytes, /Gr `__fastcall` (hidden CAddElements result in ecx,
//   first in edx, last and the 4-byte functor by value on the stack, `ret 8`). No callees.
// Caller: 0x00A6C8B1, CPhysicsMesh::CalculateCentreOfGravity 0x00A6C890 (landed; sums TotalPoints into
//   CentreOfGravity through `CAddElements<C3DVector>(centre)`).
// Identity: Ego_r.pdb names the Dinkumware `std::_For_each<C3DVector*, CAddElements<C3DVector> >`; retail STL is
//   STLport, whose front end is `for_each(first, last, f) { for (; first != last; ++first) f(*first); return f; }`.
// Shape: the functor has a user constructor, so the /Gr result travels through a hidden pointer in ecx (kept in eax)
//   and is written once at the end (`mov [eax],esi`), or straight from the argument slot for an empty range;
//   CAddElements::operator() is `Sum->x += v.x; ...` (each `fld v ; fadd [sum] ; fstp [sum]`).
#include "engine/C3DVector.h"

template <class T> class CAddElements {
public:
    T* Sum;                                             // +0x00
    CAddElements(T& sum) : Sum(&sum) { }
    void operator()(const T& value)
    {
        Sum->X += value.X;
        Sum->Y += value.Y;
        Sum->Z += value.Z;
    }
};

namespace std {

template <class _InputIter, class _Function>
_Function __fastcall for_each(_InputIter __first, _InputIter __last, _Function __f)
{
    for (; __first != __last; ++__first)
        __f(*__first);
    return __f;
}

template CAddElements<C3DVector> __fastcall for_each<C3DVector*, CAddElements<C3DVector> >(C3DVector*, C3DVector*, CAddElements<C3DVector>);

}