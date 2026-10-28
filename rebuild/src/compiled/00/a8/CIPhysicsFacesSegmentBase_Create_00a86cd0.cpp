// CIPhysicsFacesSegmentBase::Create(const CArray<C3DTriangle2>& triangles, const CArray<C3DVector>& vertices) -- builds the root of a mesh's face segmentation tree: index every triangle, then either split the set by a dividing plane into a branch or keep it whole in a leaf
// Leaf: ?Create@CIPhysicsFacesSegmentBase@@SIPBV1@ABV?$CArray@VC3DTriangle2@@@@ABV?$CArray@VC3DVector@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A86CD0, 331 bytes, static `__fastcall` (triangles in ecx, vertices in edx),
//   `ret`, returns the new segment.
// Identity (proven): Ego_r.pdb ?Create@CIPhysicsFacesSegmentBase@@SIPBV1@ABV?$CArray@VC3DTriangle2@@@@ABV?$CArray@VC3DVector@@@@@Z
//   (the only static of the class with that signature; the donor order is Calculate, ..., the two constructors, Create).
//   Callers: CPhysicsMesh::CalculateSegmentation 0x00A6C850 and the CPhysicsMesh constructor 0x00A6DA40 (RootSegment).
//   Its callees are CIPhysicsFacesSegmentBase::Calculate 0x00A868C0, the CPhysicsFacesSegmentBranch constructor
//   0x00A86B30 (landed, PDB spelling) and the inlined CPhysicsFacesSegmentLeaf constructor (vtable 0x0129DEA4,
//   .?AVCPhysicsFacesSegmentLeaf@@). The catalog's bare "Create" had no class.
// Callees: vector<unsigned short>::_M_insert_overflow(pos, x, const __true_type&, 1, true) 0x005135D0 (push_back),
//   Calculate 0x00A868C0 (protected static __fastcall, Ego_r.pdb spelling KI... with references; the landed binding is an
//   invented CPhysicsFacesSegmentBranch / pointer spelling -- NAME_FIXES), operator new 0x00BFEA1A, the branch
//   constructor 0x00A86B30, vector<unsigned short>::vector(const vector&) 0x00A87090 (the leaf's member copy), free 0x00BFEA14.
// Shape: STLport push_back inline in the index loop (finish cached in ebx, the converted index temporary in a frame word;
//   same-TU noinline bodies of _M_insert_overflow, the vector copy constructor and a Calculate STAND-IN keep the face
//   array from escaping, else VC reloads finish after the element store);
//   one result variable so the three CArray destructors (inline frees) are shared by both arms.
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int n);   // 0x00BFEAE6
void* __cdecl operator new(unsigned int size);             // 0x00BFEA1A
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

namespace std {
struct __true_type { __true_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}
template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __value)
{
    for (; __n > 0; --__n, ++__first)
        *__first = __value;
    return __first;
}
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Vector_base(unsigned int __n, const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    typedef __true_type _IsPODType;
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
    _Alloc get_allocator() const { return _Alloc(); }
    // 0x00A87090 (unsigned short): same-TU body of the retail COMDAT (STLport 4.6 copy constructor)
    __declspec(noinline) vector(const vector<_Tp, _Alloc>& __x)
        : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = (pointer)__copy_trivial(__x._M_start, __x._M_finish, this->_M_start);
    }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, _IsPODType(), 1UL, true);
    }
protected:
    void _M_clear()
    {
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    // 0x005135D0 (unsigned short): same-TU stand-in of the retail COMDAT, so VC sees the local vector's address
    // does not escape through it (finish stays in a register across the push_back loop).
    __declspec(noinline) void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&,
                                                 size_type __fill_len, bool __atend = false)
    {
        const size_type __old_size = size();
        const size_type __len = __old_size + (max)(__old_size, __fill_len);
        pointer __new_start = this->_M_end_of_storage.allocate(__len);
        pointer __new_finish = (pointer)__copy_trivial(this->_M_start, __position, __new_start);
        __new_finish = fill_n(__new_finish, __fill_len, __x);
        if (!__atend)
            __new_finish = (pointer)__copy_trivial(__position, this->_M_finish, __new_finish);
        _M_clear();
        _M_set(__new_start, __new_finish, __new_start + __len);
    }
};
}   // namespace std

template <class T> class CArray : public std::vector<T> {
public:
};

class C3DVector { public: float X, Y, Z; };
class C3DTriangle2 { public: unsigned short VertexIndex[3]; };
class CPlane {
public:
    C3DVector Normal;                                      // +0x00
    float Distance;                                        // +0x0C
};

class CIPhysicsFacesSegmentBase {
public:
    virtual ~CIPhysicsFacesSegmentBase();
    static const CIPhysicsFacesSegmentBase* __fastcall Create(const CArray<C3DTriangle2>& triangles,
                                                             const CArray<C3DVector>& vertices);
protected:
    static inline __declspec(noinline) bool __fastcall Calculate(const CArray<C3DTriangle2>& triangles, const CArray<C3DVector>& vertices,
                                     const CArray<unsigned short>& faces, long depth, CPlane& plane,
                                     CArray<unsigned short>& positive, CArray<unsigned short>& negative)   // 0x00A868C0
    {   // same-TU stand-in (NOT the retail body): lets VC see that the face array's address does not escape
        return faces.size() > (unsigned int)depth;
    }
};
class CPhysicsFacesSegmentLeaf : public CIPhysicsFacesSegmentBase {      // vtable 0x0129DEA4, 0x10 bytes
public:
    CPhysicsFacesSegmentLeaf(const CArray<unsigned short>& faces) : TriangleListIndex(faces) { }
protected:
    virtual ~CPhysicsFacesSegmentLeaf();
    CArray<unsigned short> TriangleListIndex;              // +0x04
};
class CPhysicsFacesSegmentBranch : public CIPhysicsFacesSegmentBase {    // vtable 0x0129DE88, 0x1C bytes
public:
    CPhysicsFacesSegmentBranch(const CArray<C3DTriangle2>& triangles, const CArray<C3DVector>& vertices,
                               const CPlane& plane, const CArray<unsigned short>& positive,
                               const CArray<unsigned short>& negative, long depth);   // 0x00A86B30
protected:
    virtual ~CPhysicsFacesSegmentBranch();
    CPlane ChildDividingPlane;                             // +0x04
    CIPhysicsFacesSegmentBase* PositiveChild;              // +0x14
    CIPhysicsFacesSegmentBase* NegativeChild;              // +0x18
};

const CIPhysicsFacesSegmentBase* __fastcall CIPhysicsFacesSegmentBase::Create(const CArray<C3DTriangle2>& triangles,
                                                                            const CArray<C3DVector>& vertices)
{
    CArray<unsigned short> faces;
    for (unsigned long i = 0; i < triangles.size(); ++i)
        faces.push_back((unsigned short)i);
    CArray<unsigned short> positive;
    CArray<unsigned short> negative;
    CPlane plane;
    const CIPhysicsFacesSegmentBase* result;
    if (Calculate(triangles, vertices, faces, 0, plane, positive, negative))
        result = new CPhysicsFacesSegmentBranch(triangles, vertices, plane, positive, negative, 0);
    else
        result = new CPhysicsFacesSegmentLeaf(faces);
    return result;
}