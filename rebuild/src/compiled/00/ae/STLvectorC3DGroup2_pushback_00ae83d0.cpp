// std::vector<C3DGroup2>::push_back(const C3DGroup2& x) -- append one 16-byte skinning group (CArray<C3DGroup2>): copy-construct it at the finish pointer, or grow into a size + max(size, 1) block (the inlined _M_insert_overflow) when the storage is full
// Leaf: ?push_back@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE83D0, 187 bytes, __fastcall `ret 4`.
// Callers: CParticleProgram::AddGroup 0x00AE37D0 (landed), CArray<C3DGroup2>::AddDefault 0x00AE8490 (landed), the program
//   loader 0x00AE39BF.
// Callees: CArray<C3DVertexBlend2> copy ctor 0x00AE5D80 (the group's VertexBlends), malloc 0x00BFEA0E (allocator::allocate),
//   __uninitialized_copy 0x00AE7480 (landed), _M_clear 0x00AE7FA0 (destroy the old groups, free the block).
// Layout: retail CArray<T> is an STLport vector {_M_start, _M_finish, _M_end_of_storage}; C3DGroup2 is {long BoneIndex,
//   CArray<C3DVertexBlend2> VertexBlends} = 16 bytes with the implicit copy constructor (the word, then the CArray copy).
// Label: catalog "Vector_PushBackExpand" is descriptive.
// Shape: STLport 4.6 push_back verbatim (`if (_M_finish != _M_end_of_storage._M_data) { _Construct(_M_finish, x);
//   ++_M_finish; } else _M_insert_overflow(_M_finish, x, _IsPODType(), 1UL, true);`) with the out-of-line-capable
//   _M_insert_overflow (0x00AE81F0 is its own instantiation) auto-inlined under /O2: fill_len == 1 and atend fold, but
//   (max)(size, 1) still picks through two frame words. The tag-taking __uninitialized_copy is a same-TU noinline
//   body that never touches the tag, so the `__false_type()` temporary is homed on the x argument slot.
extern "C" void* __cdecl malloc(unsigned int size);                // 0x00BFEA0E

inline void* operator new(unsigned int, void* p) { return p; }

namespace std {
struct __false_type { __false_type() { } };

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
{                                                                                              // 0x00AE7480
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&);   // 0x00AE74C0

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
    void _M_clear();                                                                           // 0x00AE7FA0
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
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
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

class C3DVertexBlend2 { public: long Vertex; float Weight; };   // 8 bytes

template <class T> class CArray : public std::vector<T> {   // retail CArray<T>: an STLport vector (12 bytes)
public:
    CArray(const CArray& other);                         // 0x00AE5D80 for CArray<C3DVertexBlend2>
};

class C3DGroup2 { public:                                       // 16 bytes in retail
    long                     BoneIndex;                  // +0x0
    CArray<C3DVertexBlend2>  VertexBlends;               // +0x4
};

template void std::vector<C3DGroup2>::push_back(const C3DGroup2&);