// COptimisedFace::COptimisedFace(const COptimisedFace&) -- the compiler-generated memberwise copy constructor of a 72-byte optimised mesh face (three index triples, two tangents, flags)
// Leaf: ??0COptimisedFace
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD0100, 131 bytes (`ret 4`, `this` returned in eax).
// Identity: the manifest label "CThingCompare_BestForTargeting2::CThingCompare_BestForTargeting2" is propagated (that
//   class is 52 bytes). Every caller (0x00AD05F5 _Copy_impl, 0x00AD19A7 / 0x00AD1A17 _Uninit_copy, 0x00AD4177 /
//   0x00AD419A / 0x00AD41B7 "InsertFillN_Element48", 0x00AD552F) is a vector copy loop over 0x48-byte elements, and the
//   copied fields fit Ego_r.pdb COptimisedFace (72 bytes) exactly: VertexIndices[3] +0x00, UniqueVertexIndices[3] +0x0c,
//   Edges[3] +0x18, UTangent +0x24, VTangent +0x30, Exists +0x3c, OutputIndex +0x40, Segment +0x44. Mesh / rendering
//   data (C3DMeshFile export side).
// Shape: C3DVector has a user copy constructor, so COptimisedFace's implicit copy ctor is non-trivial and is emitted
//   memberwise out of line: each array copied as one block through `lea` source/destination pointers, the vectors
//   through their (inline) copy ctors, then the scalars. A placement-new copy in this TU makes VC7.1 emit it.
inline void* operator new(unsigned int, void* p) { return p; }
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
struct COptimisedFace {
    unsigned long VertexIndices[3];
    unsigned long UniqueVertexIndices[3];
    long Edges[3];
    C3DVector UTangent;
    C3DVector VTangent;
    bool Exists;
    unsigned long OutputIndex;
    long Segment;
};
// the use that makes the implicit copy constructor appear (its own symbol is graded)
COptimisedFace* CopyFace(void* p, const COptimisedFace& f) { return new (p) COptimisedFace(f); }
