// CRegion::IsMapIncluded(long map) const -- whether the map number is one of this region's maps
// Leaf: ?IsMapIncluded@CRegion@@QBE_NJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006BBFA0, 36 bytes, thiscall, `ret 4`.
// Identity (proven): the only caller, CWorldMap::GetRegionNumberMapIsIn 0x004FC190, walks CWorldMap::Regions (0x58-byte
//   elements: Ego_r.pdb CRegion is 100 bytes with three 16-byte Dinkumware containers, 12 bytes each in retail) and calls
//   this on each region with the map number; Ego_r.pdb CRegion::Maps (vector<long>) is the first member and
//   ?IsMapIncluded@CRegion@@QBE_NJ@Z the only bool(long) const of CRegion. The catalog label
//   "CHitParameters::IsDivineWrathStrike" is propagated from the byte twin 0x00407330 (which calls 0x0040E170).
// Callees: std::__find<const long*, long>(first, last, val, const random_access_iterator_tag&) 0x006BCC10 (__fastcall,
//   out of line; the non-const twin is landed 0x0040E170). The landed 0x006BCC10 source names it as an invented global
//   `Find_006bcc10` (see NOTES "Review at landing").
// Shape: STLport `find(begin(), end(), map) != end()`; the iterator-category temporary and &map share the argument slot
//   only with the same-TU __find body (declaration-only, VC7.1 gives the tag its own frame word).
namespace std {
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template <class _Tp> inline random_access_iterator_tag iterator_category(const _Tp*) { return random_access_iterator_tag(); }
// Same-TU body of the STLport 4.6 random-access __find (0x006BCC10 for <const long*, long>) under auto_inline(off):
// it never touches the tag, so VC7.1 homes the empty tag temporary in the live `map` argument slot as retail does.
#pragma auto_inline(off)
template <class _RandomAccessIter, class _Tp>
__declspec(noinline) _RandomAccessIter __fastcall __find(_RandomAccessIter __first, _RandomAccessIter __last, const _Tp& __val,
                                    const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;
    for (; __trip_count > 0; --__trip_count) {
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
    }
    switch (__last - __first) {
    case 3:
        if (*__first == __val) return __first;
        ++__first;
    case 2:
        if (*__first == __val) return __first;
        ++__first;
    case 1:
        if (*__first == __val) return __first;
    case 0:
    default:
        return __last;
    }
}
#pragma auto_inline(on)
template <class _InputIter, class _Tp>
inline _InputIter find(_InputIter __first, _InputIter __last, const _Tp& __val)
{
    return __find(__first, __last, __val, iterator_category(__first));
}
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef const _Tp* const_iterator;
    const_iterator begin() const { return this->_M_start; }
    const_iterator end() const { return this->_M_finish; }
};
}

class CRegion {
public:
    std::vector<long> Maps;                          // +0x00
    std::vector<long> VisMaps;                       // +0x0C
    bool IsMapIncluded(long map) const;
};

bool CRegion::IsMapIncluded(long map) const
{
    return std::find(Maps.begin(), Maps.end(), map) != Maps.end();
}