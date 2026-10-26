// C3DMeshPartitionBuilderElement::BuildOutput(C3DMeshPartitionElement*, vector<C3DTriangle2>&) -- copy the builder tree into the output partition tree and gather leaf triangles
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC2A80, length 352 bytes.
// Callees: C3DMeshPartitionElement::BuildSubElement(long) @ 0x00AC2160, itself (recursion on SubElements[0]),
//          STLport vector<C3DTriangle2>::_M_insert_overflow @ 0x00A71550 (push_back's slow path; manifest label
//          "VertexData_BuildInterleaved" is propagated).
// Shape: the box and sphere are implicit struct assignments (C3DVector with a user copy ctor gives the
//          `lea` source/destination pointer copies), then the triangle range. An inner node builds sub
//          element 0 and recurses, then builds sub element 1 and tail-recurses (VC7.1's rotated loop); a leaf
//          push_backs every triangle (6-byte C3DTriangle2: dword + word copy, placement-new null test,
//          the __true_type tag in the dead `out` argument slot; size()/6 re-evaluated each iteration).
// Layout: PDB (struct_layouts_egor.tsv) C3DMeshPartitionBuilderElement: StartTriangle +0, TriangleCount +4,
//          BoundingBox +8, BoundingSphere +0x20, Triangles +0x30, SubElements[2] +0x3C (PDB +0x40: retail
//          STLport vectors are 12 bytes, the PDB CArray 16). C3DMeshPartitionElement as in 0x00AC27A0.
//          No engine headers exist for either class.
inline void* operator new(unsigned int, void* p) { return p; }

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
class C3DBoundingBox { public: C3DVector Min; C3DVector Max; };
class C3DBoundingSphere { public: C3DVector Center; float Radius; };
class C3DTriangle2 { public: unsigned short Index[3]; };

struct __true_type { __true_type() {} };
template <class T> inline void _Construct(T* p, const T& value) { new (p) T(value); }
template <class T> struct vector {                 // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const T& operator[](unsigned long i) const { return _M_start[i]; }
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fillLength, bool atEnd);   // 0x00A71550
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1UL, true);
    }
};

class C3DMeshPartitionElement { public:
    C3DBoundingBox           BoundingBox;      // +0x00
    long                     StartTriangle;    // +0x18
    long                     TriangleCount;    // +0x1C
    C3DMeshPartitionElement* SubElements[2];   // +0x20
    C3DBoundingSphere        BoundingSphere;   // +0x28
    C3DMeshPartitionElement* BuildSubElement(long index);   // 0x00AC2160
};

struct C3DMeshPartitionBuilderElement {
    long                            StartTriangle;   // +0x00
    long                            TriangleCount;   // +0x04
    C3DBoundingBox                  BoundingBox;     // +0x08
    C3DBoundingSphere               BoundingSphere;  // +0x20
    vector<C3DTriangle2>            Triangles;       // +0x30
    C3DMeshPartitionBuilderElement* SubElements[2];  // +0x3C
    void BuildOutput(C3DMeshPartitionElement* out, vector<C3DTriangle2>& triangles);
};

void C3DMeshPartitionBuilderElement::BuildOutput(C3DMeshPartitionElement* out, vector<C3DTriangle2>& triangles)
{
    out->BoundingBox = BoundingBox;
    out->BoundingSphere = BoundingSphere;
    out->StartTriangle = StartTriangle;
    out->TriangleCount = TriangleCount;
    if (SubElements[0]) {
        SubElements[0]->BuildOutput(out->BuildSubElement(0), triangles);
        SubElements[1]->BuildOutput(out->BuildSubElement(1), triangles);
    }
    else {
        for (unsigned long i = 0; i < Triangles.size(); ++i)
            triangles.push_back(Triangles[i]);
    }
}