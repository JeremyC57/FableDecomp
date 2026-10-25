// std::vector::resize(unsigned int new_size) -- STLport vector<KnickKnack>::resize(n): erase the surplus [begin + n, end) when shrinking, else fill-insert n - size() default (unset) knick-knack records at the end
// Leaf: ?resize@?$vector@UKnickKnack@@V?$allocator@UKnickKnack@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D9FC0, 74 bytes (+6 int3), thiscall on the vector, `ret 4`.
// Identity: the KnickKnacks vector (Ego_r.pdb CTCTavernGameSpotTheAddition::KnickKnacks, vector<KnickKnack>): sole
//   caller CTCTavernGameSpotTheAddition::FrameUpdate 0x007D8E20.
// Callees: vector<KnickKnack>::erase(first, last) 0x007D9C20, vector<KnickKnack>::_M_fill_insert(pos, n, const T&)
//   0x007D9D70 (the inline insert's body).
// Layout: Ego_r.pdb struct KnickKnack (16 bytes): Pos C3DVector +0, Thing CThingObject* +0xC; FableWin.pdb lists its
//   user default constructor ??0KnickKnack@@QAE@XZ, which stores nothing (C3DVector's own default constructor is empty).
//   The leaf names the exact one-argument symbol (the inline two-argument resize is also emitted as a COMDAT).
// Shape: STLport 4.x `void resize(size_type n) { resize(n, _Tp()); }` with the two-argument
//   `resize(n, const _Tp& x) { if (n < size()) erase(begin() + n, end()); else insert(end(), n - size(), x); }` inline:
//   the temporary is only frame space (no stores), so the count load is hoisted above `sub esp,10h`.
// Label correction: catalogued as `vector<std::pair<CCharString...>::resize` (propagated); the element is KnickKnack.
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
};

class CThingObject;

struct KnickKnack {                                                 // 16 bytes
    C3DVector     Pos;                                              // +0x00
    CThingObject* Thing;                                            // +0x0C
    KnickKnack() { }
};

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                                  // +0x00
    _Tp* _M_finish;                                                 // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;         // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last);
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
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

// 0x007D9C20 erase, 0x007D9D70 _M_fill_insert (out of line in retail)
template void std::vector<KnickKnack>::resize(unsigned int);