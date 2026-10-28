// std::vector<CEngineGraphic>::_M_insert_overflow(CEngineGraphic* pos, const CEngineGraphic& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport vector growth: move into a size + max(size, fill_len) block with x inserted fill_len times at pos
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EA5A1, 244 bytes, thiscall `ret 14h`, ebp frame. Caller: vector<CEngineGraphic>::
//   _M_fill_insert 0x004EA446 (this chunk; reached from resize(n) 0x004EA398 -> resize(n, x) 0x004EA3C3). The catalog label
//   "Vector_ReallocateInsert_0xe" is descriptive.
// Callees: malloc @ 0x00BFEA0E, free @ 0x00BFEA14 (every copy is inline).
// Layout: CEngineGraphic (Ego_r.pdb, 14 bytes = `push 0Eh ; pop ecx ; idiv`): BankIndex +0, AnimStep +4, RenderSizeX +8,
//   AdditiveAlpha +0xC, Type +0xD -- byte-packed (the resize default value is stored at ebp-0Eh..-1), so pack(1) states
//   the real layout; the implicit copy is the `movsd x3 ; movsw` block move.
// Shape: STLport 4.6 `_M_insert_overflow(.., const __false_type&, ..)` verbatim in a size-optimised TU (the defs TU
//   0x004E9xxx..0x004FAxxx: ebp frames, `push N ; pop reg ; idiv`, unaligned entry points): `#pragma optimize("s", on)`.
//   There VC7.1 inlines nothing on its own, but retail inlined every helper, so _Construct / _Copy_Construct /
//   __uninitialized_copy / __uninitialized_fill_n are `__forceinline` (inline-only templates give calls).
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);            // 0x00BFEA14

inline void* operator new(unsigned int, void* p) { return p; }

#pragma pack(push, 1)
class CEngineGraphic {                                // 14 bytes (Ego_r.pdb)
public:
    unsigned long BankIndex;                          // +0x00
    float         AnimStep;                           // +0x04
    float         RenderSizeX;                        // +0x08
    bool          AdditiveAlpha;                      // +0x0C
    unsigned char Type;                               // +0x0D
};
#pragma pack(pop)

namespace std {
struct __false_type { };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _T1, class _T2> __forceinline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                             const __false_type&)
{                                                                                 // 0x0049AD70
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{                                                                                 // 0x0049ADA0
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
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
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
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

template class vector<CEngineGraphic>;
}