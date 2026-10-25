// for_each<C3DVector*, CRescaleBy>(C3DVector* first, C3DVector* last, CRescaleBy f) -- scale every vector in the range by the functor's factor and return the functor
// Leaf: ??$for_each@PAVC3DVector@@UCRescaleBy@@@std@@YI?AUCRescaleBy@@PAVC3DVector@@0U1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A13440, 73 bytes, `ret 8`. Only caller 0x00A1287F.
// Identity: the body MULTIPLIES each component by the functor's float, so the functor is the PDB's CRescaleBy
//   (`_For_each<C3DVector*, CRescaleBy>` in Ego_r.pdb, struct, one float), not CAddElements<C3DVector> as the catalog
//   label has it (CAddElements::operator() takes `const C3DVector&` and adds). Under STLport the algorithm is
//   `for_each` itself (no Dinkumware `_For_each` helper).
// Calling convention: free template under /Gr = __fastcall: hidden result (CRescaleBy has a user constructor, so it
//   is returned through memory) in ecx, first in edx, last and the functor by value on the stack.
// Shape: the scale is re-loaded from the functor's argument slot for each component (`fld [esp+0Ch]; fmul [X]`).
class C3DVector {
public:
    float X, Y, Z;
    C3DVector& operator*=(float f) { X *= f; Y *= f; Z *= f; return *this; }
};

struct CRescaleBy {
    float Scale;
    CRescaleBy(float scale) : Scale(scale) {}
    void operator()(C3DVector& v) const { v *= Scale; }
};

namespace std {
template <class _InputIter, class _Function>
_Function __fastcall for_each(_InputIter __first, _InputIter __last, _Function __f)
{
    for ( ; __first != __last; ++__first)
        __f(*__first);
    return __f;
}
}

template CRescaleBy __fastcall std::for_each(C3DVector*, C3DVector*, CRescaleBy);