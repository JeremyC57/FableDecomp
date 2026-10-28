// std::vector::_M_insert_overflow(SKeyRecord32* position, const SKeyRecord32& x, const __false_type&, unsigned int fill_len, bool atend) -- grow a vector of 32-byte key records: allocate old size + max(old size, fill_len), copy the head, construct fill_len copies of x, copy the tail unless inserting at the end, free the old block
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A96B30, 231 bytes (+ int3 padding), thiscall `ret 14h`.
// Identity: callers 0x00A97166 (0x00A97040, the same vector's assign/insert path) and 0x00ABEDC6
//   (CScriptModuleTransitionBetweenPoints::OnInitLeaf, an inlined push_back) -- the 0x20-byte record vector whose
//   __uninitialized_copy 0x00A95A10 and __uninitialized_fill_n 0x00A94950 are its callees. The catalog label
//   "Vector_AssignCopy_AnimationKeyFrames" is descriptive; the record type is not recoverable (descriptive).
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14, __uninitialized_copy 0x00A95A10 (x2), __uninitialized_fill_n 0x00A94950.
// Shape: STLport 4.x `_M_insert_overflow` for a non-POD element (`__false_type`): the reference-returning `(max)` over
//   the old size homed in the `push ecx` frame word, `allocate(len)` = `len ? malloc(len * 32) : 0`, the tag temporaries
//   homed in the dead `atend` slot, `fill_len == 1` builds one copy inline (memberwise copy constructor through
//   placement new), the trivially destructible old range needs no _Destroy, then deallocate + `_M_set`.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);            // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (null-tested)

struct SRecord12 {                                 // 3-long record with its own user copy constructor
    long m_a, m_b, m_c;
    SRecord12(const SRecord12& o) : m_a(o.m_a), m_b(o.m_b), m_c(o.m_c) { }
};

struct SKeyRecord32 {                              // 0x20 bytes (as the landed __uninitialized_copy 0x00A95A10)
    long      m_0, m_4, m_8;                       // +0x00 .. +0x08
    SRecord12 m_record;                            // +0x0c
    long      m_18, m_1c;                          // +0x18, +0x1c
    SKeyRecord32(const SKeyRecord32& o) : m_0(o.m_0), m_4(o.m_4), m_8(o.m_8), m_record(o.m_record), m_18(o.m_18), m_1c(o.m_1c) { }
};

namespace std {
struct __false_type { __false_type() { } };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

// Same-TU stand-ins (retail compiled these templates in this TU; VC7.1 homes the empty tag temporaries over the LIVE
// `atend` slot only when it can see that the callee never reads the tag): bodies as the landed 0x00A95A10 / 0x00A94950.
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)   // 0x00A95A10
{
    for (; __first != __last; ++__first, ++__result)
        _Construct(&*__result, *__first);
    return __result;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)   // 0x00A94950
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
#pragma auto_inline(on)

template <class _Tp>
class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                 // +0x00
    _Tp* _M_finish;                                // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    protected:
    void _M_clear()
    {
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    public:
    protected:
    void _M_set(_Tp* __s, _Tp* __f, _Tp* __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    public:
    protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);
    public:
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (max)(__old_size, __fill_len);

    _Tp* __new_start = this->_M_end_of_storage.allocate(__len);
    _Tp* __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template class vector<SKeyRecord32>;
}