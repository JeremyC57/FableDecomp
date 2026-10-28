// CIPhysicsFacesSegmentBase::Calculate(const CArray<C3DTriangle2>& triangles, const CArray<C3DVector>& vertices, const CArray<unsigned short>& faces, long depth, CPlane& plane, CArray<unsigned short>& positive, CArray<unsigned short>& negative) -- split a face set for the segmentation tree: past 40 faces and above depth 9, take the dividing plane and sort every face to the positive side, the negative side, or both when it straddles the plane (epsilon 0x0129BA3C)
// Grade: DIFFER(585v652, probe) -- retail inlines only the first CArray<unsigned short>::push_back (positive, all-positive arm) and calls 0x009B4A20 out of line for the other two; VC7.1 here inlines all three (+67 bytes); also the third vertex's X product loads the plane first in retail (`fld [esi] ; fmul [eax]`), ours the vertex; prologue, frame (0x30), register allocation and the six side tests otherwise identical
// Leaf: ?Calculate@CIPhysicsFacesSegmentBase@@KI_NABV?$CArray@VC3DTriangle2@@@@ABV?$CArray@VC3DVector@@@@ABV?$CArray@G@@JAAVCPlane@@AAV4@4@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A868C0, 585 bytes, protected static `__fastcall` (triangles ecx, vertices edx), `ret 14h`.
// Identity (proven): Ego_r.pdb ?Calculate@CIPhysicsFacesSegmentBase@@KI_NABV?$CArray@VC3DTriangle2@@@@ABV?$CArray@VC3DVector@@@@ABV?$CArray@G@@JAAVCPlane@@AAV4@4@Z
//   (donor 0x00C09920, right after CPhysicsFacesSegmentLeaf::Sizeof 0x00C09910 = retail 0x00A86100 and the
//   CalculateDividingPlane donor 0x00C09540); the landed Create 0x00A86CD0 already binds this spelling, the landed Branch
//   constructor 0x00A86B30 binds an invented CPhysicsFacesSegmentBranch / pointer spelling (NAME_FIXES).
// Callees: CIPhysicsFacesSegmentBase::CalculateDividingPlane 0x00A86110 (protected static __fastcall returning CPlane: hidden
//   result in ecx, vertices in edx, then triangles, faces, depth on the stack -- Ego_r ?CalculateDividingPlane@...@@KI?AVCPlane@@
//   ABV?$CArray@VC3DVector@@@@ABV?$CArray@VC3DTriangle2@@@@ABV?$CArray@G@@J@Z), CArray<unsigned short>::push_back 0x009B4A20,
//   vector<unsigned short>::_M_insert_overflow 0x005135D0 (the inline push_back's grow path).
// Globals: 0x0129BA3C, 1e-4f (`g_epsilon_0129ba3c`, the GOTCHAS spelling; no PDB name).
// Shape: CPlane needs a user constructor (non-POD), else VC pushes the hidden result pointer instead of passing it in ecx;
//   the six side flags are separate bools (all six tests run before the two && chains); `-epsilon` is CSE'd into one frame
//   float; `if (all_positive) ... else { if (!all_negative) positive...; negative...; }` gives retail's shared negative call;
//   `vertices[triangles[faces[i]].VertexIndex[k]]` re-indexed per vertex (one triangle reference gives a 0x2c frame and a
//   different register assignment).
// Tried (4 grades): POD CPlane; one triangle reference; Dot()/member DotProduct helpers; if / else if / else; the full STLport
//   _M_insert_overflow body in the TU; a loop over the three vertices; by-value vertex copies. None stops VC inlining the
//   later push_back sites.
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
    typedef const _Tp& const_reference;
    const_reference operator[](size_type __n) const { return *(this->_M_start + __n); }
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
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&,
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
    void push_back(const T& x) { std::vector<T>::push_back(x); }   // 0x009B4A20 (unsigned short; Ego_r ?push_back@?$CArray@G@@QAEXABG@Z)
};
class C3DVector { public: float X, Y, Z;
};
inline float operator*(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
class C3DTriangle2 { public: unsigned short VertexIndex[3]; };
class CPlane { public: C3DVector Normal; float Distance; CPlane() { }
    float GetDistance(const C3DVector& p) const { return p * Normal - Distance; }
};
extern const float g_epsilon_0129ba3c;
inline float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }

class CIPhysicsFacesSegmentBase {
public:
    virtual ~CIPhysicsFacesSegmentBase();
protected:
    static CPlane __fastcall CalculateDividingPlane(const CArray<C3DVector>& vertices, const CArray<C3DTriangle2>& triangles,
                                                    const CArray<unsigned short>& faces, long depth);   // 0x00A86110
    static bool __fastcall Calculate(const CArray<C3DTriangle2>& triangles, const CArray<C3DVector>& vertices,
                                     const CArray<unsigned short>& faces, long depth, CPlane& plane,
                                     CArray<unsigned short>& positive, CArray<unsigned short>& negative);
};

bool __fastcall CIPhysicsFacesSegmentBase::Calculate(const CArray<C3DTriangle2>& triangles, const CArray<C3DVector>& vertices,
                                                     const CArray<unsigned short>& faces, long depth, CPlane& plane,
                                                     CArray<unsigned short>& positive, CArray<unsigned short>& negative)
{
    if (faces.size() > 40 && depth < 9) {
        plane = CalculateDividingPlane(vertices, triangles, faces, depth);
        long no_faces = faces.size();
        for (long i = 0; i < no_faces; ++i) {
            const C3DVector& v0 = vertices[triangles[faces[i]].VertexIndex[0]];
            const C3DVector& v1 = vertices[triangles[faces[i]].VertexIndex[1]];
            const C3DVector& v2 = vertices[triangles[faces[i]].VertexIndex[2]];
            float d0 = plane.GetDistance(v0);
            float d1 = plane.GetDistance(v1);
            float d2 = plane.GetDistance(v2);
            bool p0 = d0 > -g_epsilon_0129ba3c, n0 = d0 < g_epsilon_0129ba3c;
            bool p1 = d1 > -g_epsilon_0129ba3c, n1 = d1 < g_epsilon_0129ba3c;
            bool p2 = d2 > -g_epsilon_0129ba3c, n2 = d2 < g_epsilon_0129ba3c;
            bool all_positive = p0 && p1 && p2;
            bool all_negative = n0 && n1 && n2;
            if (all_positive)
                positive.push_back(faces[i]);
            else {
                if (!all_negative)
                    positive.push_back(faces[i]);
                negative.push_back(faces[i]);
            }
        }
        return true;
    }
    return false;
}
