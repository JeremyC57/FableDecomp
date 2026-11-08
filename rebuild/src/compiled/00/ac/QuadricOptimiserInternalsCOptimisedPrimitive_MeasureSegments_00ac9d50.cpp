// QuadricOptimiserInternals::COptimisedPrimitive::MeasureSegments() -- size every painted segment: gather the per-segment bounding box of the faces' vertex positions, then store each segment's scaled box diagonal in SegmentSizes
// Leaf: ?MeasureSegments@COptimisedPrimitive@QuadricOptimiserInternals@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC9D50, 874 bytes, thiscall `ret`, 0x2c-byte frame.
// Identity: Ego_r.pdb ?MeasureSegments@COptimisedPrimitive@QuadricOptimiserInternals@@AAEXXZ (private, void()): the body
//   reads Faces (0x48-byte COptimisedFace, VertexIndices +0 and Segment +0x44), Vertices (0xb8-byte COptimisedVertex,
//   Position +0) and SegmentCount, and writes SegmentSizes (resize + one length per segment) scaled by Parameters.Scaling --
//   the only segment member of the PDB set without a parameter (PaintSegments 0x00AC8540 and CollapseSegments(float)
//   0x00AC85D0 are landed). Sole caller COptimisedPrimitive::Initialise 0x00ACA830 at 0x00ACAA89. The catalog label
//   "QuadricOptimiser_ComputeVertexBounds" is a descriptive stand-in.
// Callees: vector<C3DVector>::vector(size_type, const C3DVector&, const allocator<C3DVector>&) 0x00AD30C0 (the two box
//   vectors, filled with FLT_MAX / FLT_MIN), memmove 0x00BFEAE6 (the inline erase's __copy_trivial when SegmentSizes shrinks),
//   vector<float>::_M_fill_insert 0x00441020 (when it grows), free 0x00BFEA14 (the box vectors).
// Layout: retail COptimisedPrimitive (engine/COptimisedPrimitive.h flattens the containers and Parameters: restated here
//   with the real STLport members -- header debt): Vertices +0x10, Faces +0x1c, SegmentCount +0x40, SegmentSizes +0x44,
//   Parameters.Scaling +0x5c.
// Shape: the face count is hoisted into a signed local; each face's three vertices go through one inline helper (the
//   position's Y/Z homed in frame slots, X kept on the x87 stack across both compares); STLport resize(n) = erase tail /
//   _M_fill_insert; the diagonal is the difference vector's GetLength (Z*Z + Y*Y + X*X under the sqrt).
#include "fable_compiler.h"
#include <math.h>
inline void* operator new(unsigned int, void* p) { return p; }
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int n);   // 0x00BFEAE6

#pragma pack(push, 1)
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector& operator=(const C3DVector& o) { X = o.X; Y = o.Y; Z = o.Z; return *this; }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLength() const { return (float)sqrt(Z * Z + Y * Y + X * X); }
};
#pragma pack(pop)

namespace std {
template <class _Tp> inline const _Tp& max(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
struct __true_type { __true_type() { } };
template <class _Tp> class allocator { public:
    allocator() { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
inline void* __copy_trivial(const void* __first, const void* __last, void* __result) {
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) + ((const char*)__last - (const char*)__first);
}
template <class _ForwardIter, class _Size, class _Tp>
FABLE_FORCEINLINE _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        new (&*__cur) _Tp(__x);
    return __cur;
}
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Vector_base(unsigned int __n, const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> { public:
    typedef _Tp value_type;
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    vector(size_type __n, const _Tp& __value, const allocator_type& __a = allocator_type());   // 0x00AD30C0 (C3DVector), same-TU body below
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
    iterator erase(iterator __first, iterator __last) {
        pointer __i = (pointer)__copy_trivial(__last, this->_M_finish, __first);
        this->_M_finish = __i;
        return __first;
    }
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);   // 0x00441020 (float)
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x) {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};
// Same-TU body of vector<C3DVector>::vector(size_type, const C3DVector&, const allocator_type&) 0x00AD30C0 (landed): VC7.1
// uses its register and memory facts at the two calls (the vector locals' start pointers stay loop-invariant).
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
vector<_Tp, _Alloc>::vector(size_type __n, const _Tp& __value, const allocator_type& __a)
    : _Vector_base<_Tp, _Alloc>(__n, __a)
{
    this->_M_finish = __uninitialized_fill_n(this->_M_start, __n, __value);
}
#pragma auto_inline(on)
} // namespace std
template <class T> class CArray : public std::vector<T, std::allocator<T> > { public:
    typedef typename std::vector<T, std::allocator<T> >::size_type size_type;
    CArray() { }
    CArray(size_type n, const T& value) : std::vector<T, std::allocator<T> >(n, value) { }
};

namespace QuadricOptimiserInternals {
class CUniqueVertex;
class COptimisedEdge;
class CVertexPairContraction;
class COptimisedVertex { public:                            // retail 0xb8 bytes (Ego_r 0xc0: two Dinkumware maps)
    C3DVector     Position;                                 // +0x00
    C3DVector     Normal;                                   // +0x0c
    C3DVector     TextureCoord;                             // +0x18
    unsigned char _rest[0xb8 - 0x24];                       // +0x24 PositionError .. PairContractions
};
class COptimisedFace { public:                              // 0x48 bytes
    unsigned long VertexIndices[3];                         // +0x00
    unsigned long UniqueVertexIndices[3];                   // +0x0c
    long          Edges[3];                                 // +0x18
    C3DVector     UTangent;                                 // +0x24
    C3DVector     VTangent;                                 // +0x30
    bool          Exists;                                   // +0x3c
    unsigned long OutputIndex;                              // +0x40
    long          Segment;                                  // +0x44
};
class COptimisationParameters { public:                     // 0x28 bytes
    float         Scaling;                                  // +0x00
    float         EdgeWeight;                               // +0x04
    float         EdgeCost;                                 // +0x08
    float         GroupWeight;                              // +0x0c
    float         TextureWeight;                            // +0x10
    float         TexturePenalty;                           // +0x14
    float         UnsplitTextureWeight;                     // +0x18
    float         UnsplitNormalWeight;                      // +0x1c
    float         HighPenalty;                              // +0x20
    unsigned long MaximumBonesPerVertex;                    // +0x24
};
class COptimisedPrimitive { public:                         // Layout: engine/COptimisedPrimitive.h (retail 0x84)
    long                          MaterialIndex;            // +0x00
    CArray<CUniqueVertex>         UniqueVertices;           // +0x04
    CArray<COptimisedVertex>      Vertices;                 // +0x10
    CArray<COptimisedFace>        Faces;                    // +0x1c
    CArray<COptimisedEdge>        Edges;                    // +0x28
    unsigned char                 VertexPairContractions[0xc];   // +0x34 multimap<float, CCountedPointer<CVertexPairContraction>>
    long                          SegmentCount;             // +0x40
    CArray<float>                 SegmentSizes;             // +0x44
    unsigned char                 SubChunksToKeep[0xc];     // +0x50 CArray<CCountedPointer<CChunkedFileChunk>>
    COptimisationParameters       Parameters;               // +0x5c
private:
    void MeasureSegments();
    void MeasureVertex(C3DVector& p, const COptimisedFace& face, long vertex, CArray<C3DVector>& mins, CArray<C3DVector>& maxs);
};

FABLE_FORCEINLINE void COptimisedPrimitive::MeasureVertex(C3DVector& p, const COptimisedFace& face, long vertex, CArray<C3DVector>& mins, CArray<C3DVector>& maxs)
{
    p = Vertices[face.VertexIndices[vertex]].Position;
    C3DVector& mn = mins[face.Segment];
    if (mn.X > p.X) mn.X = p.X;
    if (mn.Y > p.Y) mn.Y = p.Y;
    if (mn.Z > p.Z) mn.Z = p.Z;
    C3DVector& mx = maxs[face.Segment];
    if (mx.X < p.X) mx.X = p.X;
    if (mx.Y < p.Y) mx.Y = p.Y;
    if (mx.Z < p.Z) mx.Z = p.Z;
}

void COptimisedPrimitive::MeasureSegments()
{
    long faceCount = Faces.size();
    CArray<C3DVector> mins(SegmentCount, C3DVector(3.402823466e+38F, 3.402823466e+38F, 3.402823466e+38F));
    CArray<C3DVector> maxs(SegmentCount, C3DVector(1.175494351e-38F, 1.175494351e-38F, 1.175494351e-38F));
    for (long i = 0; i < faceCount; ++i)
    {
        COptimisedFace& face = Faces[i];
        C3DVector p;
        MeasureVertex(p, face, 0, mins, maxs);
        MeasureVertex(p, face, 1, mins, maxs);
        MeasureVertex(p, face, 2, mins, maxs);
    }
    SegmentSizes.resize(SegmentCount);
    for (long i = 0; i < SegmentCount; ++i)
        SegmentSizes[i] = (maxs[i] - mins[i]).GetLength() * Parameters.Scaling;
}
}