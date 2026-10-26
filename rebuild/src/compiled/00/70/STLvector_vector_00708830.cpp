// std::vector::vector(const vector& x) -- STLport copy constructor of vector<CTCHeroAttachableAppearanceModifiers::CModifierEntry> (an appearance-modifier list): allocate x.size() entries, then placement-copy x's range
// Leaf: ??0?$vector@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00708830, length 118 bytes (+ int3 padding), `ret 4`.
// Callers: 7 call sites -- 0x00706DBB in the landed CTCHeroAttachableAppearanceModifiers constructor 0x00706D10 (its
//   `vector(3)` fills the three modifier lists from an empty default through this copy), 0x00708C95, 0x00709047, 0x00709077,
//   0x00709297, 0x007092E9, 0x0070941B (the vector<vector<CModifierEntry> > fill/copy helpers).
// Callees: malloc @ 0x00BFEA0E (allocator<T>::allocate: `n != 0 ? malloc(n * 8) : 0`).
// Label: catalog "std::vector_InitFromRange" is descriptive; the body is STLport's vector copy constructor.
// Element: CTCHeroAttachableAppearanceModifiers::CModifierEntry {long DefIndex; bool Rendered;} (Ego_r.pdb, 8 bytes; the
//   landed erase 0x00708600 models the same element), copied as two dwords by the implicit copy constructor.
// Shape: the same STLport 4.6 copy constructor as the landed-in-chunk 0x00673E10 (`_Vector_base(__x.size(), a)` then
//   `_M_finish = __uninitialized_copy(x._M_start, x._M_finish, _M_start, __false_type())`; STLport's default type traits
//   call a user class non-POD, so the copy is the null-tested placement loop, not memmove); `__forceinline` on
//   __uninitialized_copy keeps the loop inline.
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class CTCHeroAttachableAppearanceModifiers {
public:
    class CModifierEntry { public:                               // Ego_r.pdb, 8 bytes (implicit copy)
        long DefIndex;                                    // +0x00 global def index
        bool Rendered;                                    // +0x04
    };
};

namespace std {
struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    typedef _Alloc allocator_type;
    _Tp* _M_start;                                        // +0x00
    _Tp* _M_finish;                                       // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef vector<_Tp, _Alloc> _Self;
    typedef const _Tp* const_pointer;
    typedef _Alloc allocator_type;
    unsigned int size() const { return unsigned int(this->_M_finish - this->_M_start); }
    allocator_type get_allocator() const { return this->_M_end_of_storage; }
    vector(const _Self& __x)
        : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const_pointer)__x._M_start, (const_pointer)__x._M_finish, this->_M_start, __false_type());
    }
};
}

typedef CTCHeroAttachableAppearanceModifiers::CModifierEntry TModifierEntry;
template std::vector<TModifierEntry>::vector(const std::vector<TModifierEntry>&);