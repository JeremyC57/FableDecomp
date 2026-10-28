// std::vector::_M_insert_overflow(pointer position, const value_type& x, const __false_type&, size_type fill_len, bool atend) -- (vector<CFileInstaller::CBufferChunk>) STLport growth insert of 8-byte buffer chunks: allocate size + max(size, fill_len) chunks, copy the head, x once (or fill_len times), the tail unless atend, destroy the old chunks (out-of-line __destroy_aux), free the old block, adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@VCBufferChunk@CFileInstaller@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C73E0, 274 bytes, thiscall, `ret 14h` (five arguments).
// Identity (proven): the 8-byte element is destroyed through 0x009C6D60, the landed __destroy_aux over
//   CFileInstaller::CBufferChunk (Ego_r.pdb: Mem +0, Used +4, IsInput +5; its inlined ~CBufferChunk hands Mem back to the
//   global allocator), whose nine callers are the CFileInstaller TU's vector<CBufferChunk> members; the only caller of
//   this body (0x009C76A6) passes `(pos, x, __false_type(), 1, false)`-style arguments from the same TU (insert/push_back).
//   The catalog label "vector_reserve_pair8byte" is behavioural.
// Callees: malloc 0x00BFEA0E (allocator::allocate), __destroy_aux 0x009C6D60 (`__fastcall`, first in ecx, last in edx, the
//   __false_type tag homed in the dead atend slot), free 0x00BFEA14 (deallocate behind its null test).
// Shape: __false_type needs its user constructor, or VC zero-stores the out-of-line call's tag temporary.
// Model: the landed byte twin 0x00F380E0 (vector<CScratchPool>::_M_insert_overflow, STLport 4.6 vector.c with every copy
//   inlined; the element's implicit copy is two dword moves); here the element has a destructor, so _M_clear runs
//   `_Destroy(_M_start, _M_finish)` first, which reaches the out-of-line __destroy_aux.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

class CFileInstaller {
public:
    class CBufferChunk {                                    // Ego_r.pdb, 8 bytes
    public:
        unsigned char* Mem;                                 // +0x00
        bool           Used;                                // +0x04
        bool           IsInput;                             // +0x05
        ~CBufferChunk();                                    // inlined into __destroy_aux 0x009C6D60
    };
};

namespace std {

struct __true_type { };
struct __false_type { __false_type() { } };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x009C6D60

template <class _ForwardIterator, class _Tp>
inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy(__first, __last, &*__first);
}

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
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

template void vector<CFileInstaller::CBufferChunk>::_M_insert_overflow(CFileInstaller::CBufferChunk*,
    const CFileInstaller::CBufferChunk&, const __false_type&, unsigned int, bool);

} // namespace std