// std::vector<CAbilityTrackLevelDef>::erase(iterator first, iterator last) -- STLport range erase: assign the tail down over [first, last), destroy the surplus, move finish, return first
// Leaf: ?erase@?$vector@VCAbilityTrackLevelDef@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00478C2D, 74 bytes, thiscall, `ret 8`, returns first; size-optimised
//   frame-pointer def TU (unaligned entry, `push ebp ; mov ebp,esp`, `push 48h ; pop ecx` constants) -- the pragma
//   optimize "s" below.
// Identity: the manifest's "std::vector::erase" is right but unparameterised. The element is 0x48 bytes (`idiv` by 0x48)
//   and the assignment 0x00478876 is CAbilityTrackLevelDef::operator=: CAbilityTrackLevelDef::Copy 0x00478871 (slot 19 of
//   the vtable 0x01236504, COL .?AVCAbilityTrackLevelDef@@) is a bare `jmp 0x00478876`, while CBalverineBattleDef's
//   Copy 0x004E4857 (slot 19 of 0x01242F74, .?AVCBalverineBattleDef@@) jumps to its own byte-identical operator=
//   0x004E485C (both classes are CParentDefClassBase + three dwords at retail +0x3C/+0x40/+0x44, 76 bytes in Ego_r.pdb
//   with the 0x40 parent). Ego_r.pdb has vector<CAbilityTrackLevelDef> (CAbilityTrackDef::Abilities, +68) and no
//   vector<CBalverineBattleDef>. The only caller 0x00478C06 is this vector's resize(n, x) (erase(begin()+n, end()) or
//   _M_fill_insert 0x00478C79). (Earlier landed as vector<CBalverineBattleDef>::erase; same bytes.)
// Callees: CAbilityTrackLevelDef::operator=(const CAbilityTrackLevelDef&) 0x00478876, _Destroy(first, last) 0x00478751
//   (`__fastcall`, ecx/edx, `ret`; the virtual destructor on each 0x48-byte element).
// Shape: STLport 4.6 `pointer i = __copy_ptrs(last, finish, first, __false_type()); _Destroy(i, finish); finish = i;
//   return first;` with the counted random-access __copy forced inline (the count `(finish - last) / 0x48` homed in the
//   dead `last` slot; with the smaller element VC7.1 keeps the source offset in ebx instead of [ebp-4]) and _Destroy out
//   of line as in the 0x88 sibling 0x00477E36.
#pragma optimize("s", on)

class CAbilityTrackLevelDef {                       // retail 0x48 bytes (only the size and the assignment matter here)
public:
    virtual ~CAbilityTrackLevelDef();
    unsigned char _body[0x44];
    CAbilityTrackLevelDef& operator=(const CAbilityTrackLevelDef& other);   // 0x00478876
};

namespace std {

struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _ForwardIterator>
void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last);   // 0x00478751

template <class _RandomAccessIter, class _OutputIter, class _Distance>
__forceinline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                 const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template <class _InputIter, class _OutputIter>
__forceinline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef _Tp* pointer;
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};

template vector<CAbilityTrackLevelDef>::iterator vector<CAbilityTrackLevelDef>::erase(iterator, iterator);

} // namespace std