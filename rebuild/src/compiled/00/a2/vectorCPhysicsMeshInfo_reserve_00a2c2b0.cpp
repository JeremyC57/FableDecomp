// vector<CPhysicsMeshInfo>::reserve(unsigned int n) -- grow the mesh bank's physics-mesh info array to n entries, copying the existing infos into a fresh block
// Leaf: ?reserve@?$vector@VCPhysicsMeshInfo@@V?$allocator@VCPhysicsMeshInfo@@@_STL@@@_STL@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A2C2B0, 163 bytes, `ret 4`. Only caller 0x00A2670E (CMeshDataBank sizing its
//   +0x398 physics-mesh info array). The catalog label "vector_reserve_generic_0x10" is descriptive.
// Callees: malloc 0x00BFEA0E (inline allocator), STLport `__uninitialized_copy(first, last, result, const
//   __false_type&)` 0x00A2AE80 (__fastcall; the catalog's "_Uninit_copy<...CScriptThing...>" is an ICF-propagated
//   name), vector<CPhysicsMeshInfo>::_M_clear 0x00A2C030 (out of line: releases each counted MeshData, frees).
// Shape: STLport 4.6 `reserve` with `_M_allocate_and_copy` inlined; as at 0x00A13650 the copy's same-TU noinline body
//   is what homes the new block and the tag on the `n` argument slot.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

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
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
};

class CPhysicsMeshInfo : public CMeshDataBankEntry {   // vtable 0x0129CD50, 0x10 bytes
public:
    class CMeshData;
    virtual void v00();
    CIVCountedPointer<CMeshData> MeshData;              // +0x0C
};

namespace _STL {
struct __false_type { __false_type() {} };
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
#pragma auto_inline(on)
template <class _Tp> class allocator {
public:
    allocator() {}
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
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
    pointer _M_allocate_and_copy(size_type __n, const_pointer __first, const_pointer __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
    void _M_clear();                                    // 0x00A2C030 (destroy + deallocate)
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

template void _STL::vector<CPhysicsMeshInfo>::reserve(unsigned int);