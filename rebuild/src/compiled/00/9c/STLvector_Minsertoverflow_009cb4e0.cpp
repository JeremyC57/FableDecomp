// std::vector::_M_insert_overflow(NGameText::CDataBankTextEntry* position, const NGameText::CDataBankTextEntry& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport reallocating insert of the text bank's 8-byte text entries: allocate old_size + max(old_size, n), copy the head, construct the n new entries, copy the tail unless appending, destroy and free the old block
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CB4E0, 223 bytes (+ int3 padding), thiscall `ret 14h` (five arguments).
// Identity: called by push_back @ 0x009CB740 (this chunk) of NGameText::CDataBank's TextEntryBuffer (retail +0x1EC,
//   Ego_r.pdb CArray<NGameText::CDataBankTextEntry>; filled by the CDataBank virtual at 0x009C8C00). Byte twin (up to
//   relocations) of the landed 0x00A43B20, the same instantiation in another TU; this source is that one with this TU's
//   callees. The catalog label "vector_reserve_8byte_fill" is descriptive.
// Callees: malloc @ 0x00BFEA0E / free @ 0x00BFEA14, __uninitialized_copy @ 0x009CA4C0 (landed), __uninitialized_fill_n
//   @ 0x009CA500 (landed), __destroy_aux @ 0x009CABC0 (the _M_clear destroy).
// Shape: STLport 4.x `_M_insert_overflow` (non-POD path) verbatim. Every `__false_type()` tag temporary is homed on the
//   bool `__atend` argument slot (`lea ecx,[esp+28h]`), which VC7.1 does only when the tag-taking callees are visible
//   and never touch the tag: same-TU `auto_inline(off)` stand-ins for the three COMDATs. `max` by reference with
//   old_size in the `push ecx` frame word; the n == 1 arm inlines the entry's implicit copy (CIVCountedPointer's user
//   copy ctor add-refs at +4 of the pointee).
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)
extern "C" void* __cdecl malloc(unsigned int);      // 0x00BFEA0E
extern "C" void __cdecl free(void*);                // 0x00BFEA14

namespace std {
struct __false_type { __false_type() { } };          // user constructor: no zero-initialised tag temporary
}

namespace NGameText {
class CTextEntry { public:                                  // intrusively counted pointee
    void* __vftable;                                 // +0x00
    long  RefCount;                                  // +0x04
};
}

template <class T>
class CIVCountedPointer { public:                           // 4 bytes
    T* P;                                            // +0x00
    CIVCountedPointer(const CIVCountedPointer& x) : P(x.P) { if (P) ++P->RefCount; }
    ~CIVCountedPointer();
};

namespace NGameText {
class CDataBankTextEntry { public:                          // 8 bytes
    CIVCountedPointer<CTextEntry> PText;             // +0x00
    long                          NarratorIndex;     // +0x04
};
}

namespace std {
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp>
inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

#pragma auto_inline(off)
// Same-TU stand-ins for the STLport COMDATs 0x009CA4C0 / 0x009CA500 / 0x009CABC0 (they never touch the tag).
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    for ( ; __first != __last; ++__first, ++__result)
        _Construct(&*__result, *__first);
    return __result;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
#pragma auto_inline(on)

template <class _Tp>
inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }

template <class _Tp>
inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

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
    _Tp* _M_start;                                   // +0x00
    _Tp* _M_finish;                                  // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    unsigned int size() const { return unsigned int(this->_M_finish - this->_M_start); }
    protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
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
void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&,
                                             unsigned int __fill_len, bool __atend)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (max)(__old_size, __fill_len);

    _Tp* __new_start = this->_M_end_of_storage.allocate(__len);
    _Tp* __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    }
    else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template class vector<NGameText::CDataBankTextEntry>;
}