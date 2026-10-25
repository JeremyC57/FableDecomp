// _STL::vector::resize(size_type new_size) -- forward to resize(new_size, value_type()) for the (EExpressionType, CExpressionSet::CEntry) pair vector
// Leaf: ?resize@?$vector@U?$pair@W4EExpressionType@@VCEntry@CExpressionSet@@@_STL@@V?$allocator@U?$pair@W4EExpressionType@@VCEntry@CExpressionSet@@@_STL@@@2@@_STL@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008701A0, 41 bytes, thiscall, `ret 4`.
// Identity: the storage vector of CExpressionSet::Expressions, a CVectorMap<EExpressionType, CExpressionSet::CEntry>
//   (Ego_r.pdb _Pair_base is 12 bytes: the enum + CEntry {Type, AnimIndex}). The callee 0x00870040 is the two-argument
//   resize; the map's binary transfer-in calls it at 0x0086F6F9 with the same inlined default pair. No direct reference
//   to this body (xref 0). CEntry has no constructor in FableWin.pdb (POD). Catalog
//   "vector<EWeaponClass>::_Umove<EWeaponClass*>" is propagated.
// Callees: vector::resize(size_type, const value_type&) @ 0x00870040.
// Shape: STLport 4.6 `resize(n) { resize(n, _Tp()); }` over `pair() : first(_T1()), second(_T2())`: the POD CEntry()
//   zero temporary is copied into `second` (two eax stores before the pushes) and first's enum zero is an immediate
//   stored after them.
enum EExpressionType { EXPRESSION_NONE };
class CExpressionSet {
public:
    class CEntry {
    public:
        EExpressionType Type;
        long AnimIndex;
    };
};
namespace _STL {
template <class _T1, class _T2>
struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};

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
    typedef unsigned int size_type;
    void resize(size_type __new_size, const _Tp& __x);                    // out of line (0x00870040)
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};

template void vector<_STL::pair<EExpressionType, CExpressionSet::CEntry> >::resize(size_type);
}