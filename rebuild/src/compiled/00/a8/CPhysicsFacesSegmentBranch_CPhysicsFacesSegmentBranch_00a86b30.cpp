// CPhysicsFacesSegmentBranch::CPhysicsFacesSegmentBranch(const CArray<C3DTriangle2>& triangles, const CArray<C3DVector>& vertices, const CPlane& plane, const CArray<unsigned short>& positive, const CArray<unsigned short>& negative, long depth) -- stores the dividing plane, then builds each child: a further branch when the face set splits again, otherwise a leaf holding the faces
// Leaf: ??0CPhysicsFacesSegmentBranch@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A86B30, 409 bytes, `ret 18h`; Ego_r.pdb
//   ??0CPhysicsFacesSegmentBranch@@QAE@ABV?$CArray@VC3DTriangle2@@@@ABV?$CArray@VC3DVector@@@@ABVCPlane@@ABV?$CArray@G@@3J@Z.
// Callees: CPhysicsFacesSegmentBranch::Calculate 0x00A868C0 (static `__fastcall`: triangles ecx, vertices edx), operator new
//   0x00BFEA1A (0x1C branch / 0x10 leaf), this constructor recursively, the CArray<unsigned short> copy 0x00A87090 (the
//   leaf's member; manifest "Vector_Copy"), free 0x00BFEA14 (the two child face arrays). Vtables: branch 0x0129DE88,
//   leaf 0x0129DEA4.
// Shape: the plane is copied by CPlane's own member-wise copy constructor (a plain struct copy addresses the member
//   through `lea eax,[this+4]` and reshuffles every register); `depth + 1` is written at each use (VC computes it once);
//   the child arrays are cleared (finish = start) between the two children.
// Layout: engine/CPhysicsFacesSegmentBranch.h is a flat POD view (void* vftable, the plane as bytes); this needs the
//   polymorphic class to construct, so the model is local with the same offsets (plane +4, children +0x14/+0x18).
extern "C" void __cdecl free(void* block);         // 0x00BFEA14
void* operator new(unsigned int size);              // 0x00BFEA1A

template <class T> class CArray { public:                  // STLport vector<T> with the malloc allocator: start, finish, end of storage
    T* m_pStart;
    T* m_pFinish;
    T* m_pEndOfStorage;
    CArray() : m_pStart(0), m_pFinish(0), m_pEndOfStorage(0) { }
    CArray(const CArray& other);                    // 0x00A87090
    ~CArray()
    {
        if (m_pStart)
            free(m_pStart);
    }
    void clear() { m_pFinish = m_pStart; }
};
class C3DVector { public: float X, Y, Z; };
class C3DTriangle2 { public: unsigned short Index[3]; };
class CPlane { public: float A, B, C, D; CPlane() { } CPlane(const CPlane& o) : A(o.A), B(o.B), C(o.C), D(o.D) { } };

class CIPhysicsFacesSegmentBase { public:
    virtual ~CIPhysicsFacesSegmentBase();
};
struct CPhysicsFacesSegmentLeaf : CIPhysicsFacesSegmentBase {   // vtable 0x0129DEA4, 0x10 bytes
    CArray<unsigned short> TriangleListIndex;       // +0x04
    CPhysicsFacesSegmentLeaf(const CArray<unsigned short>& faces) : TriangleListIndex(faces) { }
};
struct CPhysicsFacesSegmentBranch : CIPhysicsFacesSegmentBase {   // vtable 0x0129DE88, 0x1C bytes
    CPlane                     ChildDividingPlane;  // +0x04
    CIPhysicsFacesSegmentBase* PositiveChild;       // +0x14
    CIPhysicsFacesSegmentBase* NegativeChild;       // +0x18
    static bool __fastcall Calculate(const CArray<C3DTriangle2>& triangles, const CArray<C3DVector>& vertices,
                                     const CArray<unsigned short>& faces, long depth, CPlane* plane,
                                     CArray<unsigned short>* positive, CArray<unsigned short>* negative);   // 0x00A868C0
    CPhysicsFacesSegmentBranch(const CArray<C3DTriangle2>& triangles, const CArray<C3DVector>& vertices, const CPlane& plane,
                               const CArray<unsigned short>& positive, const CArray<unsigned short>& negative, long depth);
};

CPhysicsFacesSegmentBranch::CPhysicsFacesSegmentBranch(const CArray<C3DTriangle2>& triangles, const CArray<C3DVector>& vertices, const CPlane& plane,
                                                       const CArray<unsigned short>& positive, const CArray<unsigned short>& negative, long depth)
    : ChildDividingPlane(plane)
{
    CArray<unsigned short> childPositive;
    CArray<unsigned short> childNegative;
    CPlane childPlane;
    if (Calculate(triangles, vertices, positive, depth + 1, &childPlane, &childPositive, &childNegative))
        PositiveChild = new CPhysicsFacesSegmentBranch(triangles, vertices, childPlane, childPositive, childNegative, depth + 1);
    else
        PositiveChild = new CPhysicsFacesSegmentLeaf(positive);
    childPositive.clear();
    childNegative.clear();
    if (Calculate(triangles, vertices, negative, depth + 1, &childPlane, &childPositive, &childNegative))
        NegativeChild = new CPhysicsFacesSegmentBranch(triangles, vertices, childPlane, childPositive, childNegative, depth + 1);
    else
        NegativeChild = new CPhysicsFacesSegmentLeaf(negative);
}