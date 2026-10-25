// std::vector::resize(size_type new_size, const COptimisedVertex& x) -- shrink with erase or grow with a fill insert (184-byte mesh-optimiser vertices)
// Leaf: ?resize@?$vector@VCOptimisedVertex@QuadricOptimiserInternals@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD87A0, 108 bytes (+ int3 padding), thiscall, `ret 8`.
// Identity: vector<QuadricOptimiserInternals::COptimisedVertex>: the insert arm calls the landed
//   vector<COptimisedVertex>::_M_fill_insert 0x00AD85D0 (stride 184); callers COptimisedPrimitive::COptimisedPrimitive
//   (0x00ACDC60) and the one-argument resize 0x00AD8810. Divide by 184 (0xB21642C9 with the `add edx,esi` correction,
//   `sar edx,7`). Catalog "vector<CVectorMap<unsigned long,...>>::resize" is propagated.
// Callees: erase(iterator, iterator) @ 0x00AD8170, _M_fill_insert @ 0x00AD85D0.
// Shape: STLport 4.6 `resize(n, x)` (twin of 0x00AD8090: start in ebx, finish in edi, `imul esi,esi,0B8h`).
namespace QuadricOptimiserInternals {
class CVec3 {
public:
    float X, Y, Z;
};

class CQuadric {
public:
    double Coefficients[11];
};

class CRetailTree {                           // 12-byte STLport set/map
public:
    void* Header;
    unsigned int NodeCount;
    unsigned char Compare;
};

class COptimisedVertex {                      // retail 184 bytes (8-aligned)
public:
    CVec3 Position;                           // +0x00
    CVec3 Normal;                             // +0x0C
    CVec3 TextureCoord;                       // +0x18
    CQuadric PositionError;                   // +0x28
    float Cost;                               // +0x80
    CRetailTree FaceList;                     // +0x84
    bool Exists;                              // +0x90
    unsigned int UniqueIndex;                 // +0x94
    unsigned int OutputIndex;                 // +0x98
    CRetailTree Groups;                       // +0x9C
    CRetailTree PairContractions;             // +0xA8
};
}

namespace std {
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

template void vector<QuadricOptimiserInternals::COptimisedVertex>::resize(size_type, const QuadricOptimiserInternals::COptimisedVertex&);
}