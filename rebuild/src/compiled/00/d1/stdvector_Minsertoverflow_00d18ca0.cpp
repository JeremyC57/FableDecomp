// std::vector::_M_insert_overflow(TeleportData* pos, const TeleportData& x, const __false_type&, unsigned int fill_len, bool atend) -- (vector<NScript::CQ_BountyHuntScript::TeleportData>) STLport grow path: allocate size + max(size, fill_len) elements, copy the prefix, construct fill_len copies of x, copy the suffix unless atend, clear the old block and adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@UTeleportData@CQ_BountyHuntScript@NScript@@V?$allocator@UTeleportData@CQ_BountyHuntScript@NScript@@@std@@@std@@IAEXPAUTeleportData@CQ_BountyHuntScript@NScript@@ABU345@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D18CA0, length 280 bytes, thiscall `ret 14h` (pos, &x, the tag
//   reference, fill_len, atend).
// Name: its only caller is the landed vector<TeleportData>::push_back 0x00D18C00 (`_M_insert_overflow(_M_finish, x,
//   __false_type(), 1, true)` at 0x00D18C39); the 24-byte element (`0x2AAAAAAB ; sar edx,2`, `lea [n+n*2] ; shl 3`) and
//   the landed callees (copy ctor 0x00D18C50, _M_clear 0x00D18DC0) are NScript::CQ_BountyHuntScript::TeleportData's. The
//   symbol is STLport 4.6's (`protected:` in stl/_vector.h, so IAE; the bound QAE spelling is wrong -- NAME_FIXES.tsv).
//   (Catalog label TeleportData_OnReadFinished is propagated.)
// Callees: malloc 0x00BFEA0E (allocator::allocate behind its zero test), TeleportData::TeleportData(const TeleportData&)
//   0x00D18C50 (out of line, inside the inlined __uninitialized_copy / _Construct / __uninitialized_fill_n),
//   vector::_M_clear 0x00D18DC0 (out of line).
// Shape: STLport 4.6 `_M_insert_overflow(pos, x, const __false_type&, fill_len, atend)` verbatim (no exceptions: the
//   _STLP_TRY / _STLP_UNWIND pair is empty); (max) returns a reference, so VC7.1 picks the address of fill_len or of the
//   old-size local; placement new keeps its null test; __uninitialized_copy is __forceinline (plain inline leaves the second copy, the
//   suffix, out of line), __uninitialized_fill_n plain inline.
extern "C" void* __cdecl malloc(unsigned int size);                                  // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
};

namespace NScript {
class CQ_BountyHuntScript {
public:
    struct TeleportData {                                       // 24 bytes (Ego_r.pdb; FableWin.pdb mangles it `U`)
        CCharString   defName;                                  // +0x00
        unsigned long appearanceSeed;                           // +0x04
        float         health;                                   // +0x08
        CCharString   scriptName;                               // +0x0C
        CCharString   teleportTo;                               // +0x10
        CCharString   teleportMkr;                              // +0x14
        TeleportData(const TeleportData& other);                // 0x00D18C50 (the compiler-generated copy, out of line)
    };
};
}

namespace std {
struct __false_type { };

template <class _Tp> class allocator {                          // STLport allocator over malloc (empty)
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
    void _M_clear();                                            // 0x00D18DC0
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

template class vector<NScript::CQ_BountyHuntScript::TeleportData>;
}