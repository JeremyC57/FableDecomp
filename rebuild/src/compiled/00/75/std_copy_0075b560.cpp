// std::__copy(first, last, result, const random_access_iterator_tag&, int*) -- (const C3DVector, C3DVector) STLport random-access forward copy of 12-byte records: n = last - first, then `*result = *first` n times; returns the advanced result
// Leaf: ??$__copy@PBVC3DVector@@PAV1@H@std@@YIPAVC3DVector@@PBV1@0PAV1@ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075B560, 74 bytes (+ int3 padding), __fastcall (first ecx, last edx; result, the
//   tag reference and the null distance pointer on the stack), `ret 0Ch`.
// Identity (proven): std::__copy<const C3DVector*, C3DVector*, int>; evidence: its callers are the landed vector<C3DVector>::operator= 0x0075B8D0 (twice; reloc_bindings names this callee ??$__copy@PBVC3DVector@@PAV1@H@std@@...) and the unreferenced __copy_ptrs 0x0075B860; 0x0075B8D0 assigns CRigidBodyConfig::BoundingVertices (+0xA8, Ego_r.pdb vector<C3DVector>) in CRigidBodyConfig::operator= 0x0075BA30 right after AngularVelocity (+0x9C).
// Label: the manifest's "_Copy_impl<CTCSmokeGenerator::CGenerationPos*,CTCSmokeGenerator::CGenerationPos*>" is propagated (family J112: five byte-identical 12-byte __copy bodies; retail has no
//   identical-code folding).
// Shape: STLport 4.6 _algobase.h `__copy(first, last, result, const random_access_iterator_tag&, _Distance*)` verbatim (the
//   landed 0x0062FC20 body; the landed __copy_backward 0x00ADAB20 spelling): the signed count by the reciprocal divide, one
//   3-dword record copy per iteration (the implicit assignment); `__fastcall` on the template definition is the TU's /Gr.
class C3DVector {                                        // Ego_r.pdb 12 bytes; the landed 0x0075B8D0 model
public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }   // memberwise user copy (no user operator=)
};

namespace std {

struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                              _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template C3DVector* __fastcall __copy<const C3DVector*, C3DVector*, int>(const C3DVector*, const C3DVector*, C3DVector*, const random_access_iterator_tag&, int*);
}   // namespace std