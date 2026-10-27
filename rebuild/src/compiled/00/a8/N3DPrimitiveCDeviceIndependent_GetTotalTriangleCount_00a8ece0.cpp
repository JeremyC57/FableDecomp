// N3DPrimitive::CDeviceIndependent::GetTotalTriangleCount() -- sum of the triangle counts of every triangle list of the primitive
// Leaf: ?GetTotalTriangleCount@CDeviceIndependent@N3DPrimitive@@QBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A8ECE0, 80 bytes. Ego_r.pdb `?GetTotalTriangleCount@CDeviceIndependent@N3DPrimitive@@QBEKXZ`
//   (donor 0x00BD1520, directly before GetTotalIndexCount 0x00BD1560 -- retail 0x00A8ECE0 / 0x00A8ED30). Its caller
//   0x00A92218 (the "CLTH" cloth-primitive loader 0x00A91640) stores the result and then calls GetTotalIndexCount.
// Layout: Ego_r.pdb N3DPrimitive::CDeviceIndependent with retail 12-byte CArrays: vptr +0, Vertices +4, TriangleLists +0x10
//   (CArray<CIVCountedPointer<CTriangleList> >); CTriangleList: CIVCountedPointee vptr/count, Triangles +8
//   (CArray<C3DTriangle2>, 6-byte triangles: the `imul 2AAAAAABh` /6), StripIndices +0x14 (CArray<unsigned short>).
// Shape: a plain indexed loop over size(); VC hoists the bound into ebx and strength-reduces the element pointer (esi);
//   `add eax,8` is the dead `&list->Triangles` of the inlined size().
template <class T> class CIVCountedPointer {            // retail 4 bytes
public:
    T* Object;                                          // +0x00
    T* operator->() const { return Object; }
};

namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef const _Tp* const_iterator;
    typedef const _Tp& const_reference;
    typedef unsigned int size_type;
    const_iterator begin() const { return this->_M_start; }
    const_iterator end() const { return this->_M_finish; }
    size_type size() const { return size_type(end() - begin()); }
    const_reference operator[](size_type __n) const { return *(begin() + __n); }
};
}

template <class T> class CArray : public std::vector<T> { };

class C3DTriangle2 { public: unsigned short Index[3]; };   // 6 bytes
class C3DVertex2;
class C3DGroup2;

namespace N3DPrimitive {

class CTriangleList {                                   // CIVCountedPointee: vptr +0, count +4
public:
    virtual ~CTriangleList();
    unsigned long             RefCount;                 // +0x04
    CArray<C3DTriangle2>      Triangles;                // +0x08
    CArray<unsigned short>    StripIndices;             // +0x14
};

class CDeviceIndependent {
public:
    virtual ~CDeviceIndependent();
    CArray<C3DVertex2>                          Vertices;        // +0x04
    CArray<CIVCountedPointer<CTriangleList> >   TriangleLists;   // +0x10
    unsigned long GetTotalTriangleCount() const;
    unsigned long GetTotalIndexCount() const;
};

}   // namespace N3DPrimitive

unsigned long N3DPrimitive::CDeviceIndependent::GetTotalTriangleCount() const
{
    unsigned long total = 0;
    for (unsigned long i = 0; i < TriangleLists.size(); ++i)
        total += TriangleLists[i]->Triangles.size();
    return total;
}