// std::vector::_M_insert_overflow(pointer position, const value_type& x, const __false_type&, size_type fill_len, bool atend) -- (vector<CCountedPointer<CShaderResource>>) STLport growth insert for an 8-byte counted-pointer element: allocate size + max(size, fill_len) elements, copy-construct the head, x (once, or fill_len times) and the tail unless atend, destroy and free the old block, adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@V?$CCountedPointer@VCShaderResource@@@@V?$allocator@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098DC30, 218 bytes (+ int3 padding), thiscall, `ret 14h` (five arguments).
// Identity: element `CCountedPointer<CShaderResource>`; evidence: the landed CShaderRenderManager::CreateResource 0x0098B860 calls it for ResourceManagers (CArray<CCountedPointer<CShaderResource> >); the Ego_r name set instantiates that vector.
//   Family F011 (17 byte-identical bodies, one COMDAT per element type): the manifest label "std_vector_Insert_Range_Uninitialized" is
//   propagated and behavioural. STLport 4.6 vector.c `_M_insert_overflow(pos, x, const __false_type&, fill_len, atend)`
//   (no EH: _STLP_TRY/_STLP_UNWIND are empty) with the three range helpers out of line.
// Callees: malloc 0x00BFEA0E (allocator::allocate), __uninitialized_copy 0x0098BFF0 (head and tail), __uninitialized_fill_n
//   0x0098C030 (fill_len != 1), __destroy_aux 0x0098CEE0 (_M_clear's _Destroy), free 0x00BFEA14 (deallocate behind its null
//   test). The helpers are this element type's own COMDATs next to this body and are `__fastcall` in retail (first two
//   arguments in ecx/edx, the value and the tag on the stack, `ret 8` / `ret 4`).
// Shape: `(max)` through its references (`lea ecx,[esp+24h]` / `jb` / `lea ecx,[esp+10h]`), len in ebp, new_start in ebx,
//   the fill_len == 1 placement copy inline (both words, Info->RefCount bumped when set). Every __false_type temporary
//   lands on the atend argument slot (even while atend is still live: the tag is empty and never written); that needs
//   the same-TU noinline helper bodies below -- with bare declarations the first tag gets its own frame byte.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

class CCPPointerInfo {
public:
    unsigned long RefCount;                                 // +0x00
    void (__fastcall* DeleteFunc)(void* data);              // +0x04
    void* Data;                                             // +0x08
};

template <class T> class CCountedPointer {
public:
    T* Data;                                                // +0x00
    CCPPointerInfo* Info;                                   // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer();
};

class CShaderResource;

namespace std {

struct __false_type { __false_type() { } };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                                                  const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
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
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
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
template void vector<CCountedPointer<CShaderResource> >::_M_insert_overflow(CCountedPointer<CShaderResource>*, const CCountedPointer<CShaderResource>&, const __false_type&, unsigned int, bool);
} // namespace std