// _STL::vector::resize(unsigned int new_size) -- STLport vector<QuadricOptimiserInternals::COptimisedFace>::resize(n): resize the optimiser's face list to n entries, filling with a fresh face (existing, no edges, no segment)
// Leaf: ?resize@?$vector@VCOptimisedFace@QuadricOptimiserInternals@@V?$allocator@VCOptimisedFace@QuadricOptimiserInternals@@@_STL@@@_STL@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD7350, 47 bytes, thiscall on the vector, `ret 4`.
// Identity: Ego_r.pdb / FableWin.pdb ?resize@?$vector@VCOptimisedFace@QuadricOptimiserInternals@@...@@QAEXI@Z; no direct
//   call site in the image (xref: 0 calls / 0 references). Its callee 0x00AD6860 is the 72-byte two-argument resize
//   (the /72 divide; erase 0x00AD3D60 and _M_fill_insert 0x00AD54F0 are the COptimisedPrimitive::Faces COMDATs, the
//   landed fill 0x00AD04A0 names the 72-byte element COptimisedFace).
// Callees: vector<COptimisedFace>::resize(n, const COptimisedFace&) 0x00AD6860 (out of line).
// Layout: Ego_r.pdb QuadricOptimiserInternals::COptimisedFace (72 bytes): VertexIndices[3] +0, UniqueVertexIndices[3]
//   +0xC, Edges[3] +0x18, UTangent +0x24, VTangent +0x30, Exists bool +0x3C, OutputIndex +0x40, Segment +0x44;
//   FableWin.pdb lists its user default constructor.
// Shape: STLport 4.x `void resize(size_type n) { resize(n, _Tp()); }` with the two-argument resize out of line: the face
//   constructor's -1s share one register (`or eax,-1`: Segment, then the chained Edges assignment right to left), the
//   Exists byte is an immediate after the pushes; everything else stays unset.
// Label correction: catalogued as `vector<EWeaponClass>::_Umove<EWeaponClass*>` (propagated); the body is
//   vector<COptimisedFace>::resize(n).
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
};

namespace QuadricOptimiserInternals {
class COptimisedFace {                             // 72 bytes (Ego_r.pdb)
public:
    unsigned long VertexIndices[3];                // +0x00
    unsigned long UniqueVertexIndices[3];          // +0x0c
    long          Edges[3];                        // +0x18
    C3DVector     UTangent;                        // +0x24
    C3DVector     VTangent;                        // +0x30
    bool          Exists;                          // +0x3c
    unsigned long OutputIndex;                     // +0x40
    long          Segment;                         // +0x44
    COptimisedFace() : Exists(true), Segment(-1)
    {
        Edges[0] = Edges[1] = Edges[2] = -1;
    }
};
}

namespace _STL {
template <class _Tp> class allocator {
public:
    allocator() {}
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    typedef unsigned int size_type;
    _Tp* _M_start;                                                  // +0x00
    _Tp* _M_finish;                                                 // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;         // +0x08

    void resize(size_type __new_size, const _Tp& __x);              // 0x00AD6860
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};
}

template void _STL::vector<QuadricOptimiserInternals::COptimisedFace>::resize(unsigned int);