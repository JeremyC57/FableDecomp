// CPhysicsMesh::GenerateLineListFromBaseline(float baseline, CArray<C2DLineF>& lines, CArray<long>& triangles) const -- navigation-line layer: find the triangle centre height nearest the baseline, then cut every triangle that straddles that height with the horizontal plane and collect the 2-D cut segments and their triangle indices
// Grade: DIFFER(782v811) -- VC7.1 homes loop-2 i in frame slot +0x04 (retail reuses the dead baseline arg slot, frame 0x48 vs 0x4c), CSEs the TotalFaceIndices/TotalPoints starts and the three vertex indices that retail re-reads inside the cut block, and emits the side==-1 tests before the cut body where retail defers them after it; 12 probes (continue/goto/flag/positive-OR/function-scope i/triangle re-read)
// Leaf: ?GenerateLineListFromBaseline@CPhysicsMesh@@ABEXMAAV?$CArray@VC2DLineF@@@@AAV?$CArray@J@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A73E50, 811 bytes, thiscall `ret 0Ch`, 0x48-byte frame.
// Identity: Ego_r.pdb ?GenerateLineListFromBaseline@CPhysicsMesh@@ABEXMAAV?$CArray@VC2DLineF@@@@AAV?$CArray@J@@@Z (private
//   const, float + two CArray references): the body walks TotalFaceIndices (+0x30, 6-byte C3DTriangle2) and TotalPoints
//   (+0x3c, C3DVector) of the retail CPhysicsMesh, pushes 16-byte C2DLineF records into the second argument and longs into
//   the third, and ends with CArray<C2DLineF>::DeallocateUnusedMemory. Sole caller 0x00A7425A inside the per-level line
//   rebuild 0x00A74180 (CPhysicsMesh::CalculateLayeredNavigationLineInfo, which CalculateNavigationLineInfo 0x00A74360 calls).
//   The catalog label "Mesh_FindIntersectionPoints" is a descriptive stand-in.
// Callees: CPlane::GetIntersectionWithTriangle(const C3DVector&, const C3DVector&, const C3DVector&, C3DVector&, C3DVector&) const
//   0x00A42470 (landed), vector<long>::_M_insert_overflow(long*, const long&, const __true_type&, size_type, bool) 0x00440990,
//   std::__uninitialized_copy<C2DLineF*, C2DLineF*>(first, last, result, const __false_type&) 0x00A13490 (landed), malloc
//   0x00BFEA0E, free 0x00BFEA14, CArray<C2DLineF>::DeallocateUnusedMemory 0x00A74EA0 (landed). Float pool: 1/3 0x0129DC8C,
//   0.0f 0x0122DEDC, 1e10f immediate.
// Shape: the triangle count is a signed local ((finish - start) / 6); the first loop keeps the running best |centre - baseline|
//   and its centre in frame slots; the second loop classifies each vertex as -1 / +1 against the chosen height and cuts the
//   six mixed sign patterns; the plane is a local {0,0,1,height}; the triangle index push_back calls the POD overflow out of
//   line while the C2DLineF push_back's overflow is inlined (allocate, __uninitialized_copy, construct, free, re-point).
#include <math.h>
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

#pragma pack(push, 1)
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
#pragma pack(pop)
class C2DVector { public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { }
};
class C2DLineF { public:                                    // 16 bytes (Ego_r: Point1 +0, Point2 +8)
    C2DVector Point1;
    C2DVector Point2;
    C2DLineF() { }
    C2DLineF(const C2DVector& p1, const C2DVector& p2) : Point1(p1), Point2(p2) { }
};
class C3DTriangle2 { public:                                // 6 bytes
    unsigned short VertexIndex[3];
};
class CPlane { public:                                      // 16 bytes
    C3DVector Normal;
    float     Distance;
    CPlane(const C3DVector& normal, float distance) : Normal(normal), Distance(distance) { }
    bool GetIntersectionWithTriangle(const C3DVector& a, const C3DVector& b, const C3DVector& c, C3DVector& start, C3DVector& end) const;   // 0x00A42470
};

namespace std {
struct __false_type { __false_type() { } };
struct __true_type { __true_type() { } };
template <class _Tp> inline const _Tp& max(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> class allocator { public:
    allocator() { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
    _Tp* allocate(unsigned int __n) { return _MaybeReboundAlloc::allocate(__n); }
};
#pragma auto_inline(off)
// 0x00A13490 (landed): the real body as a same-TU noinline function; it never touches the tag.
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
#pragma auto_inline(on)
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _Tp> struct _IsPODTypeOf { typedef __false_type _Type; };
template <> struct _IsPODTypeOf<long> { typedef __true_type _Type; };
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
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
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
    const _Tp& operator[](size_type __n) const { return *(this->_M_start + __n); }
    typedef typename _IsPODTypeOf<_Tp>::_Type _IsPODType;
    void _M_clear() { _M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start); }
    void _M_set(pointer __s, pointer __f, pointer __e) { this->_M_start = __s; this->_M_finish = __f; this->_M_end_of_storage._M_data = __e; }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len, bool __atend);   // 0x00440990 (long)
    // STLport 4.6 vector.h: the class-element overload, defined in the class (inlined here at the one lines site)
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend = false)
    {
        const size_type __old_size = size();
        const size_type __len = __old_size + (max)(__old_size, __fill_len);
        pointer __new_start = this->_M_end_of_storage.allocate(__len);
        pointer __new_finish = __new_start;
        __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
        if (__fill_len == 1) {
            _Construct(__new_finish, __x);
            ++__new_finish;
        }
        else
            __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
        if (!__atend)
            __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
        _M_clear();
        _M_set(__new_start, __new_finish, __new_start + __len);
    }
    void push_back(const _Tp& __x) {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, _IsPODType(), 1UL, true);
    }
};
} // namespace std
template <class T> class CArray : public std::vector<T, std::allocator<T> > { public:
    void DeallocateUnusedMemory();                          // 0x00A74EA0 (C2DLineF, landed)
};

class CPhysicsMesh { public:                                // Layout: retail CPhysicsMesh (landed 0x00A74360 model): TotalFaceIndices +0x30, TotalPoints +0x3c
    unsigned char          _pad_0x00[0x30];                 // +0x00 vtable .. navigation-line state
    CArray<C3DTriangle2>   TotalFaceIndices;                // +0x30
    CArray<C3DVector>      TotalPoints;                     // +0x3c
private:
    void GenerateLineListFromBaseline(float baseline, CArray<C2DLineF>& lines, CArray<long>& triangles) const;
};

void CPhysicsMesh::GenerateLineListFromBaseline(float baseline, CArray<C2DLineF>& lines, CArray<long>& triangles) const
{
    long triangleCount = TotalFaceIndices.size();
    float closestDistance = 1e10f;
    float closestZ = 0.0f;
    for (long i = 0; i < triangleCount; ++i) {
        const C3DTriangle2& triangle = TotalFaceIndices[i];
        float z = (TotalPoints[triangle.VertexIndex[2]].Z + TotalPoints[triangle.VertexIndex[1]].Z + TotalPoints[triangle.VertexIndex[0]].Z) * (1.0f / 3.0f);
        float distance = (float)fabs(z - baseline);
        if (distance < closestDistance) {
            closestDistance = distance;
            closestZ = z;
        }
    }
    for (long i = 0; i < triangleCount; ++i) {
        long side0 = TotalPoints[TotalFaceIndices[i].VertexIndex[0]].Z - closestZ < 0.0f ? -1 : 1;
        long side1 = TotalPoints[TotalFaceIndices[i].VertexIndex[1]].Z - closestZ < 0.0f ? -1 : 1;
        long side2 = TotalPoints[TotalFaceIndices[i].VertexIndex[2]].Z - closestZ < 0.0f ? -1 : 1;
        if ((side0 == 1 && ((side1 == 1 && side2 == -1) || (side1 == -1 && side2 == 1) || (side1 == -1 && side2 == -1))) ||
            (side0 == -1 && ((side1 == 1 && side2 == 1) || (side1 == 1 && side2 == -1) || (side1 == -1 && side2 == 1))))
        {
            CPlane plane(C3DVector(0.0f, 0.0f, 1.0f), closestZ);
            C3DVector start;
            C3DVector end;
            if (plane.GetIntersectionWithTriangle(TotalPoints[TotalFaceIndices[i].VertexIndex[0]], TotalPoints[TotalFaceIndices[i].VertexIndex[1]], TotalPoints[TotalFaceIndices[i].VertexIndex[2]], start, end)) {
                triangles.push_back(i);
                C2DLineF line(C2DVector(start.X, start.Y), C2DVector(end.X, end.Y));
                lines.push_back(line);
            }
        }
    }
    lines.DeallocateUnusedMemory();
}
