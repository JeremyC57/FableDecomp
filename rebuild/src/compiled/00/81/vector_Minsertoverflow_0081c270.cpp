// vector::_M_insert_overflow(SRecord20* position, const SRecord20& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport non-POD growth of a vector of 20-byte records: allocate old + max(old, n), copy the head, place the n copies, copy the tail, free the old block
// Leaf: ?_M_insert_overflow@?$vector@USRecord20@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081C270, 232 bytes (+int3 pad), thiscall `ret 14h`.
// Callees: malloc @ 0x00BFEA0E (the inline allocate, `len ? malloc(len * 20) : 0`), __uninitialized_copy @ 0x0081A650
//   (landed as UninitCopy_Record20, twice), __uninitialized_fill_n @ 0x008195E0 (`ret 8`, the n != 1
//   arm), free @ 0x00BFEA14 (the inline deallocate).
// Caller: 0x0081D1E6 (the vector's insert / push_back front end).
// Label: the catalog's "std_vector_InsertRepeatFill_20Byte" is descriptive; the body is STLport `_M_insert_overflow`.
// Shape: STLport 4.x verbatim, as the landed CQuake twin 0x0069D300 but with the fill_len == 1 `_Construct` INLINE (the
//   null-tested placement copy of the 5-dword POD through ecx, then `add edi,14h`): `old = size(); len = old + (max)(old,
//   fill_len)` (max by const reference: `lea ecx,[esp+24h] ; jb ; lea ecx,[esp+10h]`), the __false_type temporaries homed
//   in the atend argument slot (`lea ecx,[esp+28h]`), _M_clear = the trivial destroy loop (folds away) + `if (_M_start)
//   free(_M_start)`, then _M_set. The two placement helpers are same-TU __declspec(noinline) bodies (declared-only
//   helpers move the tags into a frame byte -- the 0x0069D300 note).
// Element: a 20-byte POD record (the landed 0x0081A650 note: NThingMap::CMapwhoLevelInfo is 20 bytes in Ego_r.pdb but
//   nothing pins the element to it).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) throw() { return where; }

struct SRecord20 {                                          // 20-byte POD record, implicit copy constructor
    long m_0, m_4, m_8, m_c, m_10;
};

namespace std {
struct __false_type { __false_type() {} };
template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator first, _ForwardIterator last)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

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
};

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter first, _InputIter last, _ForwardIter result,
                                                                  const __false_type&)   // 0x0081A650 (same-TU body)
{
    _ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter first, _Size n, const _Tp& x,
                                                                    const __false_type&)   // 0x008195E0 (same-TU body)
{
    _ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}

template <class T> class vector : public _Vector_base<T> {
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
    void _M_set(T* s, T* f, T* e)
    {
        this->_M_start = s;
        this->_M_finish = f;
        this->_M_end_of_storage._M_data = e;
    }
    public:
    protected:
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend);
    public:
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

template void std::vector<SRecord20>::_M_insert_overflow(SRecord20*, const SRecord20&, const std::__false_type&, unsigned int, bool);