// vector<C3DMeshInfo>::reserve(unsigned int n) -- grow the mesh bank's mesh info array to n entries, copying the existing infos into a fresh block
// Leaf: ?reserve@?$vector@VC3DMeshInfo@@V?$allocator@VC3DMeshInfo@@@_STL@@@_STL@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A2C210, 145 bytes, `ret 4`. Only caller 0x00A26702 (CMeshDataBank sizing its
//   three info arrays: +0x38C C3DMeshInfo, +0x398 CPhysicsMeshInfo, +0x3A4 C3DAnimationInfo). The catalog label
//   "vector_reserve_generic" is descriptive; the element is the 0x60-byte C3DMeshInfo (Ego_r.pdb, /96 reciprocal).
// Callees: vector<C3DMeshInfo>::_M_allocate_and_copy 0x00A2B030 and _M_clear 0x00A2BFF0 (both out of line here,
//   thiscall), malloc 0x00BFEA0E (inline allocator for the empty arm).
// Shape: STLport 4.6 `reserve` verbatim; size/capacity through the 0x2AAAAAAB / sar 4 division by 96.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E

class C3DVector {
public:
    float X, Y, Z;
};

class C3DBoundingBox {                                  // 0x18 bytes
public:
    C3DVector MinCoord;
    C3DVector MaxCoord;
};

class C3DBoundingSphere {                               // 0x10 bytes
public:
    C3DVector Center;
    float     Radius;
};

class CMeshDataBank;

class CMeshDataBankEntry {                              // vtable 0x0129CD28
public:
    virtual void v00();
    CMeshDataBank* Bank;                                // +0x04
    unsigned long  Index;                               // +0x08
};

class CIVCountedPointeeBase {
public:
    virtual void Destroy(int flags);
    virtual void Delete();
    unsigned long IVRefCount;                           // +0x04
};

template <class T> class CIVCountedPointer {
public:
    CIVCountedPointeeBase* Object;
};

class C3DMeshInfo : public CMeshDataBankEntry {        // 0x60 bytes (Ego_r.pdb)
public:
    class CMeshData;
    virtual void v00();
    CIVCountedPointer<CMeshData> MeshData;              // +0x0C
    C3DBoundingBox    BoundingBox;                      // +0x10
    C3DBoundingSphere BoundingSphere;                   // +0x28
    float             SafeBoundingRadius;               // +0x38
    long              PhysicsMeshIndex;                 // +0x3C
    float             LODErrors[4];                     // +0x40
    unsigned long     LODDataSizes[4];                  // +0x50
};

namespace _STL {
template <class _Tp> class allocator {
public:
    allocator() {}
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last);   // 0x00A2B030
    void _M_clear();                                    // 0x00A2BFF0 (destroy + deallocate)
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void reserve(size_type __n);
};
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::reserve(size_type __n)
{
    if (capacity() < __n) {
        const size_type __old_size = size();
        pointer __tmp;
        if (this->_M_start) {
            __tmp = _M_allocate_and_copy(__n, this->_M_start, this->_M_finish);
            _M_clear();
        } else {
            __tmp = this->_M_end_of_storage.allocate(__n);
        }
        _M_set(__tmp, __tmp + __old_size, __tmp + __n);
    }
}
}

template void _STL::vector<C3DMeshInfo>::reserve(unsigned int);