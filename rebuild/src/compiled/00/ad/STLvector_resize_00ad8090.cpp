// _STL::vector::resize(size_type new_size, const CUniqueVertex& x) -- shrink with erase or grow with a fill insert (28-byte mesh-optimiser unique vertices)
// Leaf: ?resize@?$vector@VCUniqueVertex@QuadricOptimiserInternals@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD8090, 105 bytes, thiscall, `ret 8`.
// Identity: vector<QuadricOptimiserInternals::CUniqueVertex>. The only caller is the one-argument resize 0x00AD8310,
//   which builds its default element with CUniqueVertex::CUniqueVertex() (landed 0x00AD4C40) and destroys it with
//   ~CUniqueVertex (landed 0x00AD4A70); the divide is by 28 (0x92492493 with the `add edx,esi` correction,
//   `sar edx,4`) = the retail 0x1C-byte record (Exists, two 12-byte STLport sets).
//   Catalog "vector<CVectorMap<unsigned long,...>>::resize" is propagated.
// Callees: erase(iterator, iterator) @ 0x00AD7C20, _M_fill_insert @ 0x00AD7E80.
// Shape: STLport 4.6 `resize(n, x)` (see 0x0052BEA0); the corrected reciprocal multiply needs the difference in esi,
//   so start goes to ebx, finish to edi, and the else arm re-reads _M_finish into ebp (an extra `push ebp`).
namespace QuadricOptimiserInternals {
class CIndexList {                            // { set<unsigned long> Indices }: 12-byte STLport tree
public:
    void* Header;
    unsigned int NodeCount;
    unsigned char Compare;
};

class CUniqueVertex {                         // 28 bytes
public:
    bool Exists;                              // +0x00
    CIndexList VertexIndices;                 // +0x04
    CIndexList FaceList;                      // +0x10
};
}

namespace _STL {
template <class _Tp> class allocator { public: allocator() { } };

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last);                     // out of line in retail
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);    // out of line in retail
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x) {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
};

template void vector<QuadricOptimiserInternals::CUniqueVertex>::resize(size_type, const QuadricOptimiserInternals::CUniqueVertex&);
}