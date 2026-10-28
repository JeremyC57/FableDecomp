// std::vector::_M_insert_overflow(Coin* position, const Coin& x, const __false_type&, unsigned int fill_len, bool atend) -- (vector<Coin>) STLport regrowth of the tavern coin vector: allocate size + max(size, fill_len) coins, copy the prefix, construct the fill, copy the suffix unless appending, destroy (virtually) and free the old block
// Leaf: ?_M_insert_overflow@?$vector@VCoin@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F0450, 288 bytes incl. int3 padding, thiscall, `ret 14h`.
// Identity: the catalog's "Vector_Reallocate" is descriptive. Callers: the landed push_back 0x008F0570 and
//   CTCTavernGameCoinBase code (0x008EFA76 in the landed AddNewCoin 0x008EFA00, 0x008EFD07). The 0x3C-byte polymorphic
//   element copied through 0x008EFF40 is Ego_r.pdb Coin (landed copy constructor Coin::Coin(const Coin&) 0x008EFF40,
//   virtual destructor inherited from Mass).
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (STLport malloc allocator), Coin::Coin(const Coin&) 0x008EFF40 (thiscall)
//   and the out-of-line __uninitialized_copy(first, last, result, const __false_type&) 0x008F0280 (landed, /Gr
//   `__fastcall`); the old coins are destroyed through vtable slot 0 (`push 0 ; call [edx]`, the virtual destructor).
// Shape: STLport 4.6 `_M_insert_overflow` verbatim (as vector<C3DSkeletalBoneMorph> 0x0088B4F0): the `(max)(__old_size,
//   __fill_len)` reference select, the new block parked in fill_len's dead slot; VC7.1 inlines the first
//   __uninitialized_copy and the __uninitialized_fill_n (placement-copy loops) but leaves the remainder copy out of line,
//   whose empty __false_type temporary is homed on atend's slot.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

class Mass {                                                // vtable 0x01288230, 0x24 bytes
public:
    float m_state[8];                                       // +0x04 Pos, Vel, IMass, APos, AVel, AIMass
    virtual ~Mass() { }
};

class Coin : public Mass {                                  // vtable 0x01288238, 0x3C bytes (Ego_r.pdb)
public:
    bool          Owned;                                    // +0x24
    unsigned char Thing[0x8];                               // +0x25 CIntelligentPointer<CThing> (pack(1))
    float         Radius;                                   // +0x30
    int           Owner;                                    // +0x34
    unsigned long Sound;                                    // +0x38
    Coin(const Coin& other);                                // 0x008EFF40
};

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _T1, class _T2>
inline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
inline _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _Tp>
inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIter>
inline void __destroy_aux(_ForwardIter __first, _ForwardIter __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp>
inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }

template <class _Tp>
inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _Tp> class allocator {
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
    _Tp* _M_start;                                                  // +0x00
    _Tp* _M_finish;                                                 // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;         // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;

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
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend);
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

template class vector<Coin>;

} // namespace std