// std::vector::_M_insert_overflow(NFire::CFirePoint* pos, const NFire::CFirePoint& x, const __false_type&, size_type fill_len, bool atend) -- STLport growth of CTCFlammable's fire-point list: move into a size + max(size, n) block with x inserted fill_len times at pos
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00781A80, length 272 bytes (thiscall, `ret 14h`).
// Identity: the catalog label "Vector_InsertRange_Copy" is descriptive. The element is Ego_r.pdb NFire::CFirePoint (44
//   bytes, `imul 2E8BA2E9h ; sar edx,3`): the copy constructor is the landed 0x007813D0 and the destroy loop the landed
//   __destroy_aux 0x007817B0 (CTCFlammable::FirePoints). Callers: the landed push_back 0x00781D30 and CTCFlammable's
//   fire-point setup 0x007808B0 (two inlined push_backs).
// Callees: malloc 0x00BFEA0E (allocator::allocate), CFirePoint::CFirePoint(const CFirePoint&) 0x007813D0 (prefix,
//   insert and fill copies), __uninitialized_copy 0x007816C0 (the suffix copy only), __destroy_aux 0x007817B0 (the
//   inlined _M_clear's destroy), free 0x00BFEA14.
// Shape: STLport 4.x `_M_insert_overflow(.., const __false_type&, ..)` verbatim (the landed 0x00569E30 recipe); VC7.1's
//   inliner expands the prefix __uninitialized_copy and the __uninitialized_fill_n loop (null-tested placement copies)
//   but keeps the suffix copy an out-of-line call (as 0x00765B70); `_M_clear()` inlines to STLport's
//   `_Destroy -> __destroy -> __destroy_aux(first, last, __false_type())` (a same-TU noinline `__fastcall` body that
//   never reads the tag, so the tag is homed on the dead `atend` slot) plus `if (_M_start) free(_M_start)`.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14

inline void* operator new(unsigned int, void* p) { return p; }

class CCharString {
public:
    void* PStringData;
    ~CCharString();                                         // 0x0099EAE0
};

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    void* PData;                                            // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };

class CThing;

namespace NFire {
class CFirePoint {                                          // Ego_r.pdb, 44 bytes
public:
    CCharString                 HelperName;                 // +0x00
    long                        HelperIndex;                // +0x04
    long                        EffectIndex;                // +0x08
    long                        Resistance;                 // +0x0C
    bool                        Burning;                    // +0x10
    long                        FireDamageAfterFrame;       // +0x14
    CThing*                     PParentThing;               // +0x18
    CIntelligentPointer<CThing> PEffect;                    // +0x1C
    CIntelligentPointer<CThing> PSingeEffect;               // +0x24
    CFirePoint(const CFirePoint& other);                    // 0x007813D0
};
}

namespace std {

struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _T1, class _T2> inline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                                    const __false_type&)
{                                                                                              // 0x007816C0
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                      const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{                                                                                              // 0x007817B0
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _ForwardIterator, class _Tp>
inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy(__first, __last, __first);
}

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
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
};

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

template void vector<NFire::CFirePoint>::_M_insert_overflow(NFire::CFirePoint*, const NFire::CFirePoint&,
                                                            const __false_type&, unsigned int, bool);

}