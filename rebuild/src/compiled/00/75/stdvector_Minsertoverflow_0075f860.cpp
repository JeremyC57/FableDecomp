// std::vector::_M_insert_overflow(CKeyCamera* position, const CKeyCamera& x, const __false_type&, unsigned int fill_len, bool atend) -- grow the scripted-camera key vector and insert fill_len copies of x
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075F860, 281 bytes, `ret 0x14`.
// Label correction: catalog "Vector_InsertRange_CopyFill" -> STLport 4.x vector<CKeyCamera>::_M_insert_overflow (the
//   landed vector<CKeyCamera>::insert 0x0075FAE0 and push_back 0x0075FAA0 call it; 56-byte element: 0x92492493 /
//   `add edx,ecx ; sar edx,5`, `imul 38h`).
// Element: CKeyCamera (Ego_r.pdb, 56 bytes) with its out-of-line copy constructor 0x0075F1F0; the implicit destructor
//   is EventName's ~CCharString 0x0099EAE0 (+0x30).
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14, CKeyCamera::CKeyCamera(const CKeyCamera&) 0x0075F1F0, the tail
//   __uninitialized_copy 0x0075F3E0, ~CCharString 0x0099EAE0.
// Shape: STLport `_M_insert_overflow` with inline helpers: as in 0x00720F40 VC7.1 inlines the head
//   __uninitialized_copy and the __uninitialized_fill_n (placement copy-constructor calls behind their null tests) but
//   keeps the tail copy out of line; _M_clear's __destroy_aux loop is inlined (EventName's destructor per element),
//   then free and _M_set.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);           // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

class CCharString {
public:
    void* PStringData;                          // +0x00
    ~CCharString();                             // 0x0099EAE0
};

class C3DVector {
public:
    float X, Y, Z;
};

class CKeyCamera {                              // Ego_r.pdb, 56 bytes
public:
    float       DeltaIndex;                     // +0x00
    C3DVector   Pos;                            // +0x04
    C3DVector   LookDirection;                  // +0x10
    float       FOV;                            // +0x1c
    float       Roll;                           // +0x20
    float       ShuttleSpeed;                   // +0x24
    float       Duration;                       // +0x28
    float       PauseTime;                      // +0x2c
    CCharString EventName;                      // +0x30
    float       SpeedFraction;                  // +0x34
    CKeyCamera(const CKeyCamera& other);        // 0x0075F1F0
};

namespace std {

struct __false_type { __false_type() {} };

template<class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }

template<class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template<class T> class allocator {
public:
    allocator() {}
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p) free(p); }
};

template<class InputIter, class ForwardIter>
inline ForwardIter __fastcall __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
template<class ForwardIter, class Size, class T>
inline ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}

template<class T> inline void _Destroy(T* p) { p->~T(); }
template<class ForwardIter>
inline void __destroy_aux(ForwardIter first, ForwardIter last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
template<class ForwardIter> inline void _Destroy(ForwardIter first, ForwardIter last) { __destroy_aux(first, last, __false_type()); }

template<class T, class Alloc = allocator<T> > class vector {
public:
    T* _M_start;                                // +0x00
    T* _M_finish;                               // +0x04
    T* _M_end_of_storage;                       // +0x08
    Alloc _M_alloc() const { return Alloc(); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_clear()
    {
        _Destroy(_M_start, _M_finish);
        _M_alloc().deallocate(_M_start, (unsigned int)(_M_end_of_storage - _M_start));
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

    T* new_start = _M_alloc().allocate(len);
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

template class vector<CKeyCamera>;

}