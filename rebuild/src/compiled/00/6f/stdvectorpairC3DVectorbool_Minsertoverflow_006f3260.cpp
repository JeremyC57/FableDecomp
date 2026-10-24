// std::vector<pair<C3DVector,bool>>::_M_insert_overflow(pair* position, const pair& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport grow-and-insert of fill_len copies of x
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F3260, 223 bytes, `ret 0x14`.
// Label correction: catalog "Vector_InsertRepeatElements" -> STLport vector<pair<C3DVector,bool> >::_M_insert_overflow.
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14, __uninitialized_copy 0x006EFEC0 (landed separately) and
//   __uninitialized_fill_n 0x006EFF00 -- both given same-TU noinline bodies under `#pragma auto_inline(off)` so the
//   empty __false_type temporaries share argument slots as retail's do (GOTCHAS 24l).
// Layout: pair<C3DVector,bool> is a 13-byte record in retail (stride 0xd, `imul eax,eax,0Dh`), hence pack(1) on the
//   vector (the landed precedent for the byte-packed C3DVector).
// Shape: STLport _M_insert_overflow verbatim (len = size + max(size, fill_len), copy, construct or fill, copy tail
//   unless atend, _M_clear, _M_set); the pair keeps STLport's explicit copy constructor.
extern "C" void* __cdecl malloc(unsigned int size);      // MSVCR71 import thunk 0x00BFEA0E
extern "C" void __cdecl free(void* block);                // MSVCR71 import thunk 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

#pragma pack(push, 1)
class C3DVector {                                         // byte-packed record in retail (13-byte pair stride)
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};
#pragma pack(pop)

namespace std {
struct __false_type { __false_type() {} };

template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) {}   // STLport's explicit copy constructor
};

template<class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }

template<class T1, class T2> inline void _Construct(T1* pointer, const T2& value) { new (pointer) T1(value); }

#pragma auto_inline(off)
// 0x006EFEC0 (landed separately; same-TU body so the empty tag can share the live `atend` slot)
template<class InputIter, class ForwardIter>
__declspec(noinline) ForwardIter __fastcall __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
// 0x006EFF00 (separate function; same-TU body for the same reason)
template<class ForwardIter, class Size, class T>
__declspec(noinline) ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
#pragma auto_inline(on)

template<class T> class allocator { public: allocator() {} };

template<class T, class Alloc = allocator<T> > class vector {
public:
    T* _M_start;                                          // +0x00
    T* _M_finish;                                         // +0x04
    T* _M_end_of_storage;                                 // +0x08
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    void _M_clear()
    {
        if (_M_start)
            free(_M_start);
    }
    void _M_set(T* s, T* f, T* e)
    {
        _M_start = s;
        _M_finish = f;
        _M_end_of_storage = e;
    }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend);
};

template<class T, class Alloc>
void vector<T, Alloc>::_M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend)
{
    const unsigned int old_size = size();
    const unsigned int len = old_size + (max)(old_size, fill_len);
    T* new_start = len ? (T*)malloc(len * sizeof(T)) : 0;
    T* new_finish = __uninitialized_copy(_M_start, position, new_start, __false_type());
    if (fill_len == 1) {
        _Construct(new_finish, x);
        ++new_finish;
    }
    else
        new_finish = __uninitialized_fill_n(new_finish, fill_len, x, __false_type());
    if (!atend)
        new_finish = __uninitialized_copy(position, _M_finish, new_finish, __false_type());
    _M_clear();
    _M_set(new_start, new_finish, new_start + len);
}

template class vector<pair<C3DVector, bool> >;
}