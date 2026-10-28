// CTCUserActionPointBase::IsActionPointIndexBlocked(long index) -- whether the action-point index is in the BlockedActionPointIndexes list
// Leaf: ?IsActionPointIndexBlocked@CTCUserActionPointBase@@IAE_NJ@Z
// Retail: Fable.exe @ 0x007FEC30, 33 bytes, thiscall `ret 4`.
// Identity (proven): FableWin.pdb ?IsActionPointIndexBlocked@CTCUserActionPointBase@@IAE_NJ@Z (protected, non-const);
//   the caller 0x007FF263 is in CTCNamedUserActionPoints::GetFreeNamedActionPoint 0x007FF190 (CTCNamedUserActionPoints
//   derives from CTCUserActionPointBase, Fable.exe RTTI), and the body searches Ego_r.pdb
//   CTCUserActionPointBase::BlockedActionPointIndexes (vector<long>, Ego_r +0x1c, retail +0x18 after the 12-byte STLport
//   FreeActionPointIndexes). The non-const method makes find run over long* (__find 0x0050E540,
//   ??$__find@PAJJ@std@@YIPAJPAJ0ABJABUrandom_access_iterator_tag@0@@Z as bound). The catalog's
//   "CHitParameters::IsDivineWrathStrike" is a propagated label.
namespace std {
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template <class _Tp> inline random_access_iterator_tag iterator_category(const _Tp*) { return random_access_iterator_tag(); }
// Same-TU body of the STLport 4.6 random-access __find (the landed CRegion::IsMapIncluded 0x006BBFA0 model) under auto_inline(off):
// it never touches the tag, so VC7.1 homes the empty tag temporary in the live argument slot as retail does.
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
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator begin() const { return this->_M_start; }
    const_iterator end() const { return this->_M_finish; }
};
}

class CTCUserActionPointBase {
public:
    unsigned char _pad00[0x0c];
    std::vector<long> FreeActionPointIndexes;            // +0x0c
    std::vector<long> BlockedActionPointIndexes;         // +0x18
protected:
    bool IsActionPointIndexBlocked(long index);
};
bool CTCUserActionPointBase::IsActionPointIndexBlocked(long index)
{
    if (std::find(BlockedActionPointIndexes.begin(), BlockedActionPointIndexes.end(), index) != BlockedActionPointIndexes.end())
        return true;
    return false;
}