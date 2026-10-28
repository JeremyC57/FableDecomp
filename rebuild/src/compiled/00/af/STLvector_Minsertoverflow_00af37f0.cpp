// std::vector::_M_insert_overflow(T* pos, const T& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport vector growth for the 16-byte registered-PSC entries pair<CCharString, NParticleEngine::CRegisteredPSCInfo>: move into a size + max(size, fill_len) block with x inserted fill_len times at pos
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF37F0, 215 bytes, __fastcall `ret 14h`.
// Callers: push_back 0x00AF39C0 (`_M_insert_overflow(_M_finish, x, __false_type(), 1, true)`).
// Callees: malloc 0x00BFEA0E (allocator::allocate), __uninitialized_copy 0x00AF2BA0 (landed, x2), __uninitialized_fill_n
//   0x00AF2C00 (landed), CCharString copy ctor 0x0099EC30 (x2: the fill_len == 1 copy construct), _M_clear 0x00AF36C0
//   (destroy the old entries, free the block; manifest "CCombatWheel::ResetRings" is propagated).
// Element: CPSCManager's RegisteredPSCs entry: key CCharString +0, CRegisteredPSCInfo {AllocFunc, ComponentType, GUIName} +4
//   (STLport pair: explicit copy constructor, the second's implicit memberwise copy).
// Label: catalog "CCombatWheel_ResizeRings" is propagated over the byte-identical instantiation skeleton (0x00AE81F0 is the
//   same shape for another 16-byte element).
// Shape: STLport 4.6 _vector.c verbatim -- the shape of the landed vector<string>::_M_insert_overflow 0x00569E30: (max)
//   through references (the `lea ecx` pick between the spilled size and the fill_len slot), allocate, copy the prefix,
//   _Copy_Construct when fill_len == 1 else fill_n, copy the suffix unless atend, _M_clear(), _M_set(). The tag-taking
//   callees carry same-TU noinline bodies that never touch the tag, so every `__false_type()` temporary is homed on the
//   atend slot.
extern "C" void* __cdecl malloc(unsigned int size);                // 0x00BFEA0E

inline void* operator new(unsigned int, void* p) { return p; }

class CPSCBase;

class CCharString {
public:
    void* PStringData;                                      // the shared block
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);       // 0x0099EFB0
};

enum EPSCType { PSC_TYPE_NONE = 0 };

namespace NParticleEngine {
class CRegisteredPSCInfo { public:
    CPSCBase* (__cdecl* AllocFunc)();                       // +0x0
    EPSCType ComponentType;                                 // +0x4
    CCharString GUIName;                                    // +0x8
};
}

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

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
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _T1, class _T2> inline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                              // 0x00AF2BA0
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{                                                                                              // 0x00AF2C00
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
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
    protected:
    void _M_clear();                                                                           // 0x00AF36C0
    public:
    protected:
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    public:
    protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
    public:
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
}

typedef std::pair<CCharString, NParticleEngine::CRegisteredPSCInfo> CPSCPair;

template void std::vector<CPSCPair>::_M_insert_overflow(CPSCPair*, const CPSCPair&, const std::__false_type&, unsigned int, bool);