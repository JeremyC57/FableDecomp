// std::vector<CGameMapCell>::_M_insert_overflow(CGameMapCell* position, const CGameMapCell& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport vector growth: reallocate, copy the head, insert fill_len copies of x, copy the tail, free the old block
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B7F70, 219 bytes (+ int3 padding), `ret 14h` (thiscall).
// Callers: one site, 0x006B817E (vector<CGameMapCell>::_M_fill_insert, the CGameMap::Clear / resize path).
// Callees: malloc @ 0x00BFEA0E, free @ 0x00BFEA14 (the STLport malloc allocator),
//   __uninitialized_copy<const CGameMapCell*, CGameMapCell*> @ 0x006B7BB0 (landed),
//   __uninitialized_fill_n<CGameMapCell*, unsigned, CGameMapCell> @ 0x006B7C10.
// Shape: STLport 4.5 `_M_insert_overflow(__position, __x, const __false_type&, __fill_len, __atend)`:
//   `__len = __old_size + max(__old_size, __fill_len)` (max by const reference: `lea ecx,[fill_len] ; jb ; lea
//   ecx,[old_size]` with old_size in the `push ecx` frame word), `allocate(__len)` = `len ? malloc(len * 7) : 0`, the head
//   copy, `if (__fill_len == 1) { _Construct(__new_finish, __x); ++__new_finish; } else __uninitialized_fill_n(..)`, the
//   tail copy unless `__atend`, `_M_clear()` (trivial destroy + `if (start) free(start)`) and `_M_set(..)`. The empty
//   `__false_type()` tag temporaries take the address of the `atend` argument slot (`lea ecx,[esp+28h]`) only when the
//   two tag-taking callees have same-TU `auto_inline(off)` bodies (GOTCHAS 0x4ADB50); declared-only callees add a second
//   frame word (`sub esp,8`).
// Label correction: the catalog `Vector_InsertN` is descriptive.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14

inline void* operator new(unsigned int, void* p) { return p; }

struct __false_type { __false_type() { } };

class CGameMapCell { public:                                   // 7 bytes (Ego_r.pdb)
    unsigned char AtmosThemes[3];                       // +0
    unsigned char AtmosBlends[2];                       // +3
    signed char   SoundTheme;                           // +5
    unsigned char Music;                                // +6
    CGameMapCell() { }
};

namespace std {
template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class T> inline void _Destroy(T*, T*) { }

#pragma auto_inline(off)
template <class InputIter, class ForwardIter>
ForwardIter __fastcall __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)   // 0x006B7BB0
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}

template <class ForwardIter, class Size, class T>
ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)              // 0x006B7C10
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(cur, x);
    return cur;
}
#pragma auto_inline(on)

template <class T> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
};

template <class T, class A> class _Vector_base {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<T*, A> _M_end_of_storage;          // +0x08
};

template <class T, class A = allocator<T> > class vector : protected _Vector_base<T, A> {
public:
    typedef T* pointer;
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer s, pointer f, pointer e)
    {
        this->_M_start = s;
        this->_M_finish = f;
        this->_M_end_of_storage._M_data = e;
    }
    void _M_insert_overflow(pointer position, const T& x, const __false_type&, unsigned int fill_len, bool atend);
};

template <class T, class A>
void vector<T, A>::_M_insert_overflow(pointer position, const T& x, const __false_type&, unsigned int fill_len, bool atend)
{
    const unsigned int old_size = size();
    const unsigned int len = old_size + (max)(old_size, fill_len);

    pointer new_start = this->_M_end_of_storage.allocate(len);
    pointer new_finish = new_start;
    new_finish = __uninitialized_copy((const T*)this->_M_start, (const T*)position, new_start, __false_type());
    if (fill_len == 1) {
        _Construct(new_finish, x);
        ++new_finish;
    } else
        new_finish = __uninitialized_fill_n(new_finish, fill_len, x, __false_type());
    if (!atend)
        new_finish = __uninitialized_copy((const T*)position, (const T*)this->_M_finish, new_finish, __false_type());
    _M_clear();
    _M_set(new_start, new_finish, new_start + len);
}

template void vector<CGameMapCell, allocator<CGameMapCell> >::_M_insert_overflow(CGameMapCell*, const CGameMapCell&, const __false_type&, unsigned int, bool);
}