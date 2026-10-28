// std::vector::_M_insert_overflow(pointer position, const value_type& x, const __false_type&, size_type fill_len, bool atend) -- (vector<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry>) STLport growth insert of 48-byte theme priority-stack entries: allocate size + max(size, fill_len) entries, copy the head, x once (or fill_len times), the tail unless atend, _M_clear the old block, adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@VCEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00889B20, 269 bytes, thiscall, `ret 14h` (five arguments).
// Identity (proven): the 48-byte element (`imul 2AAAAAABh ; sar edx,3`) is copied member by member with its Themes array
//   (+8) through 0x006B58F0, the landed vector<CEnvironmentThemePriorityStackEntry::CThemeWeight> copy constructor; the
//   same-TU out-of-line __uninitialized_copy 0x00889300 / __uninitialized_fill_n 0x00889230 and _M_clear 0x00889A30 are
//   landed over NEnvironmentInternal::CEnvironmentThemePriorityStackEntry (Ego_r.pdb: BaseWeight +0, BaseWeightLocked +4,
//   Themes +8, ThemeTransitionTargetID +0x14, ThemeTransitionInitialWeight +0x18, ThemeTransitionActive +0x1C,
//   ThemeTransitionStartTime +0x20, ThemeTransitionDuration +0x28 at retail offsets). Only caller: 0x00889EC9 inside the
//   landed fill insert 0x00889DA0. The catalog label "_Allocate_impl" is behavioural.
// Callees: malloc 0x00BFEA0E (allocator::allocate), __uninitialized_copy 0x00889300 (prefix and suffix, `__fastcall`),
//   vector<CThemeWeight>::vector(const vector&) 0x006B58F0 (inside the inlined single copy), __uninitialized_fill_n
//   0x00889230 (`__fastcall`), _M_clear 0x00889A30.
// Model: the landed shape twin 0x00CB6FD0 (both copies and the fill out of line, the single construct inline), written as
//   STLport 4.6 vector.c; the element's implicit copy constructor moves the floats through integer registers and the
//   double through the FPU.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
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
    typedef _Tp* pointer;
    vector(const vector& __x);                              // 0x006B58F0 (vector<CThemeWeight>)
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
    void _M_clear();                                        // 0x00889A30
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
};
}

template <class T> class CArray : public std::vector<T, std::allocator<T> > { };

namespace NEnvironmentInternal {
class CEnvironmentThemePriorityStackEntry {                 // retail 0x30 bytes (Ego_r.pdb names)
public:
    struct CThemeWeight {
        long  ThemeID;
        float Weight;
        bool  LockWeight;
    };
    float                 BaseWeight;                       // +0x00
    bool                  BaseWeightLocked;                 // +0x04
    CArray<CThemeWeight>  Themes;                           // +0x08
    long                  ThemeTransitionTargetID;          // +0x14
    float                 ThemeTransitionInitialWeight;     // +0x18
    bool                  ThemeTransitionActive;            // +0x1C
    double                ThemeTransitionStartTime;         // +0x20
    float                 ThemeTransitionDuration;          // +0x28
};
}

namespace std {

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

// Same-TU bodies (retail's TU holds them; VC7.1 kept them out of line): with them visible as noinline the tag
// temporaries reuse the dead atend slot, as in retail. Not graded here (landed separately).
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                              // 0x00889300
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{                                                                                              // 0x00889230
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
#pragma auto_inline(on)

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    // handle insertion
    if (__fill_len == 1) {
        _Copy_Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        // copy remainder
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template void vector<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry>::_M_insert_overflow(
    NEnvironmentInternal::CEnvironmentThemePriorityStackEntry*, const NEnvironmentInternal::CEnvironmentThemePriorityStackEntry&,
    const __false_type&, unsigned int, bool);

}