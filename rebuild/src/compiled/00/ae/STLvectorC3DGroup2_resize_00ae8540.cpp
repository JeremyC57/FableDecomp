// std::vector<C3DGroup2>::resize(unsigned int new_size) -- resize the skinning-group array (CArray<C3DGroup2>) to new_size: erase the tail, or append default groups (BoneIndex -1, no blends) copied from one temporary
// Leaf: ?resize@?$vector@UC3DGroup2@@V?$allocator@UC3DGroup2@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE8540, 101 bytes, __fastcall `ret 4`.
// Callers: the program loader 0x00AE3ABF.
// Callees: C3DGroup2::C3DGroup2() 0x00AC40C0 (landed; the default value, its returned `this` pushed as x), erase(first,
//   last) 0x00AE7F30, insert(pos, n, x) / _M_fill_insert 0x00AE82C0, free 0x00BFEA14 (the temporary's VertexBlends
//   release: the CArray's inline STLport destructor).
// Layout: retail CArray<T> is an STLport vector {_M_start, _M_finish, _M_end_of_storage}; C3DGroup2 is {long BoneIndex,
//   CArray<C3DVertexBlend2> VertexBlends} = 16 bytes.
// Label: catalog "Vector_Resize_Pair128Bit" is descriptive.
// Shape: STLport 4.6 verbatim -- `resize(n) { resize(n, T()); }` over `resize(n, x) { if (n < size()) erase(begin() + n,
//   end()); else insert(end(), n - size(), x); }`, both inline; the temporary dies after the call (`if (p) free(p)`).
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
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
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last);                                         // 0x00AE7F30
    void insert(iterator __pos, size_type __n, const _Tp& __x);                                // 0x00AE82C0
    void resize(size_type __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};
}

struct C3DVertexBlend2 { long Vertex; float Weight; };   // 8 bytes

template <class T> class CArray : public std::vector<T> {   // retail CArray<T>: an STLport vector (12 bytes)
};

struct C3DGroup2 {                                       // 16 bytes in retail
    long                     BoneIndex;                  // +0x0
    CArray<C3DVertexBlend2>  VertexBlends;               // +0x4
    C3DGroup2();                                         // 0x00AC40C0 (BoneIndex -1, empty VertexBlends)
};

template void std::vector<C3DGroup2>::resize(unsigned int);