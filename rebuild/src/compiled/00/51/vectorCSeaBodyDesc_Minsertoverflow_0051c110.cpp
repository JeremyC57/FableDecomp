// vector<CSeaBodyDesc>::_M_insert_overflow(CSeaBodyDesc* position, const CSeaBodyDesc& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport non-POD vector growth for sea body records: allocate old + max(old, n), copy the head, construct the n new copies, copy the tail, destroy and free the old block
// Leaf: ?_M_insert_overflow@?$vector@UCSeaBodyDesc@CStaticMapGenerationVisibiilityInfo@@
//
// Retail 0x0051C110, 224 bytes, thiscall `ret 14h`.  Callees: 0x00BFEA0E malloc, 0x00518C00 __uninitialized_copy (twice),
//   0x0099EC30 CCharString copy ctor + 0x00514160 vector<long>::vector(const vector&) (the fill_len == 1 _Construct),
//   0x00518C90 __uninitialized_fill_n, 0x00519EB0 __destroy_aux (all three this chunk), 0x00BFEA14 free.
// Caller: the vector's push_back / insert 0x0051C800.
// Element: CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc (see 0x00519EB0).
// Shape: STLport `old = size(); len = old + (max)(old, fill_len); new_start = allocate(len); new_finish =
//   __uninitialized_copy(_M_start, position, new_start, __false_type()); if (fill_len == 1) { _Construct(new_finish,
//   x); ++new_finish; } else new_finish = __uninitialized_fill_n(new_finish, fill_len, x, __false_type()); if (!atend)
//   new_finish = __uninitialized_copy(position, _M_finish, new_finish, __false_type()); _M_clear(); _M_set(new_start,
//   new_finish, new_start + len);` -- const-reference max (`lea ecx,[esp+24h] / jb / lea ecx,[esp+10h]`), allocate =
//   `len ? malloc(len * 16) : 0`, _M_clear = _Destroy (__destroy_aux) + `if (_M_start) free(_M_start)`; no /EHsc.
// Lever: every `__false_type()` temporary is homed in the atend argument slot (`lea ecx,[esp+28h]`) although atend is
//   still read later -- the empty tag is never written.  That needs the three receiving templates as same-TU
//   __declspec(noinline) bodies with the tag parameter unnamed (the 0x00A84470 recipe); vector<long>'s copy
//   constructor is only declared so the single-element _Construct calls it out of line, as retail.
// Label: catalog "Vector_InsertRangeGeneric_CCharString" (propagated).
extern "C" void* __cdecl malloc(unsigned int size);
extern "C" void __cdecl free(void* p);
inline void* __cdecl operator new(unsigned int, void* where) throw() { return where; }

class CCharString {
public:
    const char* PString;
    CCharString(const CCharString& other);     // 0x0099EC30
    ~CCharString();                            // 0x0099EAE0
};

namespace std {
struct __false_type { __false_type() {} };
template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class T> inline void _Destroy(T* p) { p->~T(); }

template <class T> class allocator {
public:
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
};
template <class T> class _Vector_base {
public:
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T> _M_end_of_storage;
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter first, _InputIter last, _ForwardIter result,
                                                                  const __false_type&)   // 0x00518C00 (same-TU body)
{
    _ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter first, _Size n, const _Tp& x,
                                                                    const __false_type&)   // 0x00518C90 (same-TU body)
{
    _ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator first, _ForwardIterator last,
                                                   const __false_type&)   // 0x00519EB0 (same-TU body)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator first, _ForwardIterator last)
{
    __destroy_aux(first, last, __false_type());
}

template <class T> class vector : public _Vector_base<T> {
public:
    vector(const vector<T>& x);                // 0x00514160 for T = long (out of line here)
    unsigned int size() const { return unsigned int(this->_M_finish - this->_M_start); }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(T* s, T* f, T* e)
    {
        this->_M_start = s;
        this->_M_finish = f;
        this->_M_end_of_storage._M_data = e;
    }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend);
};

template <class T>
void vector<T>::_M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend)
{
    const unsigned int old_size = size();
    const unsigned int len = old_size + (max)(old_size, fill_len);
    T* new_start = this->_M_end_of_storage.allocate(len);
    T* new_finish = new_start;
    new_finish = __uninitialized_copy(this->_M_start, position, new_start, __false_type());
    if (fill_len == 1) {
        _Construct(new_finish, x);
        ++new_finish;
    }
    else
        new_finish = __uninitialized_fill_n(new_finish, fill_len, x, __false_type());
    if (!atend)
        new_finish = __uninitialized_copy(position, this->_M_finish, new_finish, __false_type());
    _M_clear();
    _M_set(new_start, new_finish, new_start + len);
}
}

template <class T> class CArray : public std::vector<T> {};

class CStaticMapGenerationVisibiilityInfo {
public:
    struct CSeaBodyDesc {
        CCharString  Identifier;               // +0x0
        CArray<long> Maps;                     // +0x4
    };
};
typedef CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc CSeaBodyDesc;

template void std::vector<CSeaBodyDesc>::_M_insert_overflow(CSeaBodyDesc*, const CSeaBodyDesc&, const std::__false_type&,
                                                           unsigned int, bool);