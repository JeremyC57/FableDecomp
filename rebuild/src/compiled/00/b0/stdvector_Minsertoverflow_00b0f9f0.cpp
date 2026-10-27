// std::vector::_M_insert_overflow(pair<unsigned long,CParticleUpdateParamDesc<unsigned long,unsigned long> >* position, const pair<..>& x, const __false_type&, unsigned int fill_len, bool atend) -- grow a particle update-parameter map's vector into a size + max(size, n) block with x inserted fill_len times at position
// Leaf: ?_M_insert_overflow@?$vector@U?$pair@KV?$CParticleUpdateParamDesc@KK@NParticleEngine@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B0F9F0, 254 bytes, thiscall `ret 14h`.
// Identity: the 16-byte element (`sar eax,4`) is std::pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc
//   <unsigned long, unsigned long> > -- the inline copy stores vtable 0x0125FEE4 (RTTI
//   .?AV?$CParticleUpdateParamDesc@KK@NParticleEngine@@) at +4 -- the storage of Ego_r.pdb's CVectorMap<unsigned long,
//   CParticleUpdateParamDesc<unsigned long, unsigned long>, ...> (CParticleUpdateParameters). Callers: the
//   CPSC*::CollectUpdateParameters push_backs (0x00B0F291, 0x00B15925, 0x00B17498, 0x00B1B3FC, ...). Its copy helpers
//   are the landed 0x00B0F670 / 0x00B0F6B0 (descriptive names there). The catalog's "Vector_ReallocateWithNew" is
//   descriptive.
// Callees: malloc 0x00BFEA0E, __uninitialized_copy<pair*, pair*> 0x00B0F670 (prefix and remainder),
//   __uninitialized_fill_n 0x00B0F6B0, CBaseClass::~CBaseClass 0x0099A2E0 (the desc's compiler-generated destructor
//   chained straight to its base, per element), free 0x00BFEA14.
// Shape: STLport 4.6 `_M_insert_overflow(.., const __false_type&, ..)` verbatim (this chunk's 0x00AD83D0) with the
//   copy/fill helpers as same-TU noinline bodies (tags homed on the `atend` slot, the new block on the dead fill_len
//   slot) and _M_clear's destroy loop inline; CParticleUpdateParamDesc declares no destructor (no vptr reset before
//   the base call).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                // 0x00BFEA14

inline void* operator new(unsigned int, void* p) { return p; }

class CBaseClass {                                         // vtable 0x01231710
public:
    CBaseClass();                                         // 0x0099A2D0
    virtual ~CBaseClass();                                // 0x0099A2E0
};

namespace NParticleEngine {
template <class T1, class T2> class CParticleUpdateParamCallback;
template <class T1, class T2> class CParticleUpdateParamDesc : public CBaseClass {   // vtable 0x0125FEE4 for <K,K>
public:
    CParticleUpdateParamCallback<T1, T2>* Callback;       // +0x04
    unsigned long Index;                                  // +0x08
};
}

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

// The helpers' real bodies (landed 0x00B0F670 / 0x00B0F6B0) sit in this TU as in retail; out of line (auto_inline off
// + noinline) they let VC7.1 home each __false_type() tag on the dead `atend` slot. Not graded here.
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{                                                                                              // 0x00B0F670
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{                                                                                              // 0x00B0F6B0
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
#pragma auto_inline(on)

template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
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
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        // copy remainder
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

}   // namespace std

typedef std::pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> > TParamDescPair;
template void std::vector<TParamDescPair>::_M_insert_overflow(TParamDescPair*, const TParamDescPair&, const std::__false_type&,
                                                            unsigned int, bool);