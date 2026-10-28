// std::vector::_M_insert_overflow(pair* pos, const pair& x, const __false_type&, unsigned int fill_len, bool atend) -- (vector<pair<unsigned long, CDefinitionManager::CDefClassInfo> >) STLport grow path: allocate size + max(size, fill_len), copy the prefix, construct fill_len copies of x, copy the suffix unless atend, destroy and free the old block, adopt the new one
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B8DD0, 288 bytes, thiscall `ret 14h` (pos, &x, the tag reference, fill_len,
//   atend). Called by the landed vector::push_back 0x009B9170 (`_M_insert_overflow(_M_finish, x, _IsPODType(), 1, true)`)
//   and referenced from three unwind/descriptor sites. The catalog label "Vector_InsertRange_24byte_CSpeechFrame" is a
//   propagated guess: the element is the definition manager's class-info pair (the landed callees below say so).
// Callees: __uninitialized_copy(first, last, result, const __false_type&) @ 0x009B7320 (landed, `__fastcall`, `ret 8`),
//   __uninitialized_fill_n(first, n, x, const __false_type&) @ 0x009B73E0 (landed), __destroy_aux(first, last,
//   const __false_type&) @ 0x009B8070 (`ret 4`; _M_clear's _Destroy), CCharString(const CCharString&) @ 0x0099EC30 and
//   vector<unsigned long>(const vector&) @ 0x00535470 (the fill_len == 1 copy construction, inline here), malloc
//   @ 0x00BFEA0E, free @ 0x00BFEA14.
// Layout: pair<unsigned long, CDefClassInfo> = 24 bytes (`0x2AAAAAAB ; sar 2`, `lea [n+n*2] ; shl 3`): key +0x00,
//   CDefClassInfo Name +0x04, AllocFunc +0x08, GlobalIndices CArray<unsigned long> +0x0C (the landed push_back model).
// Shape: STLport 4.6 `_M_insert_overflow(pos, x, const __false_type&, fill_len, atend)` verbatim: max() takes the
//   address of the fill_len argument, placement new keeps its null test, _M_clear = _Destroy + deallocate, _M_set.
//   The helpers are same-TU noinline bodies that never touch their tag, so VC7.1 homes every __false_type temporary on
//   the atend argument slot (it is only read as a byte), as retail does.
extern "C" void* __cdecl malloc(unsigned int size);                                  // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                           // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dest, const void* source, unsigned int count);   // 0x00BFEAE6
inline void* operator new(unsigned int, void* where) { return where; }

class CCharString {
public:
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
private:
    void* PStringData;
};

namespace std {
struct __true_type { __true_type() {} };
struct __false_type { __false_type() {} };

template <class _Tp> class allocator {                  // STLport allocator over malloc / free (empty)
public:
    allocator() {}
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _Alloc& __a, _Value __p) : _Alloc(__a), _M_data(__p) {}
};

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
        : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) + ((const char*)__last - (const char*)__first);
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    unsigned int size() const { return unsigned int(this->_M_finish - this->_M_start); }
    _Alloc get_allocator() const { return _Alloc(); }
    vector(const vector<_Tp, _Alloc>& __x)
        : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = (_Tp*)__copy_trivial(__x._M_start, __x._M_finish, this->_M_start);
    }
    ~vector() {}
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);
    protected:
    void _M_clear();
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

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                 // 0x009B7320
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{                                                                                 // 0x009B73E0
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{                                                                                 // 0x009B8070
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
#pragma auto_inline(on)

template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}
}

template <class T> class CArray : public std::vector<T> {      // retail CArray: the 12-byte STLport vector
};

class CDefClassBase;
class CDefinitionManager {
public:
    class CDefClassInfo {                               // Ego_r.pdb (24 bytes; retail 20: CArray 16 -> 12)
    public:
        CCharString            Name;                    // +0x00
        CDefClassBase* (__cdecl* AllocFunc)();          // +0x04
        CArray<unsigned long>  GlobalIndices;           // +0x08
    };
};
typedef std::pair<unsigned long, CDefinitionManager::CDefClassInfo> CDefClassInfoPair;

namespace std {
template <class _Tp, class _Alloc> void vector<_Tp, _Alloc>::_M_clear()
{
    _Destroy(this->_M_start, this->_M_finish);
    this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
}

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
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

template void vector<CDefClassInfoPair>::_M_insert_overflow(CDefClassInfoPair*, const CDefClassInfoPair&,
                                                            const __false_type&, unsigned int, bool);
}