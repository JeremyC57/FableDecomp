// CTCHeroReceiveItems::HasPickedUpItem(long item) const -- whether the item id is in the component's PickedUpItems list
// Leaf: ?HasPickedUpItem@CTCHeroReceiveItems@@QBE_NJ@Z
// Retail: Fable.exe @ 0x007CA800, 33 bytes, thiscall `ret 4`.
// Identity (proven): FableWin.pdb ?HasPickedUpItem@CTCHeroReceiveItems@@QBE_NJ@Z; both callers (0x006ACCDC, 0x006ACEAD)
//   look up interface 0xcb first, and 0xcb is CTCHeroReceiveItems (vtable 0x0126EBFC slot 21 returns 0xcb); the body
//   searches Ego_r.pdb CTCHeroReceiveItems::PickedUpItems (vector<long>, Ego_r +0x18, retail +0x10 after the 4-byte
//   STLport Items list). The catalog's "CHitParameters::IsDivineWrathStrike" is a propagated label.
// Shape: STLport find via iterator_category(first) with tag classes without user constructors and a same-TU noinline
//   __find body (the landed 0x006BBFA0 shape); a declaration-only __find gives the tag its own frame word.
// Tail: retail's `cmp eax,esi ; setne al` comes from the if/return form; `return find(...) != end();` gives
//   `xor ecx,ecx ; setne cl ; mov al,cl` (the twin 0x00778AB0 form).
// Body: std::find(PickedUpItems.begin(), PickedUpItems.end(), item) != end (STLport find -> __fastcall __find 0x006BCC10
//   over const long*, ??$__find@PBJJ@std@@YIPBJPBJ0ABJABUrandom_access_iterator_tag@0@@Z as bound; the item and the tag share the argument slot).
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

class CTCHeroReceiveItems {
public:
    unsigned char _pad00[0x10];
    std::vector<long> PickedUpItems;                     // +0x10
    bool HasPickedUpItem(long item) const;
};
bool CTCHeroReceiveItems::HasPickedUpItem(long item) const
{
    if (std::find(PickedUpItems.begin(), PickedUpItems.end(), item) != PickedUpItems.end())
        return true;
    return false;
}