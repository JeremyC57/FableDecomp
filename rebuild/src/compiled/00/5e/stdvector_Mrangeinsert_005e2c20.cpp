// std::vector::_M_range_insert(iterator position, const value_type* first, const value_type* last, const forward_iterator_tag&) -- STLport range insert into the animation bank's vector<pair<unsigned long, CAnimationEntry> >: shift the tail in place when the spare capacity suffices, else reallocate to old + max(old, n) and copy head, range and tail
// Leaf: ??$_M_range_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E2C20, 480 bytes (0x1E0), thiscall `ret 10h`.
// Identity: the 20-byte element (`imul 66666667h ; sar 3`) is pair<unsigned long, CAnimationEntry> (Ego_r.pdb
//   CAnimationEntry {long BankIndex; CDefString AnimName; CDefString GroupName; CIVCountedPointer<CAnimComponentMap>
//   Components}) -- the element of the landed CArray<pair<unsigned long, CAnimationEntry> >::DeallocateMemory
//   0x005E3170, whose helpers this body shares. Callers: the vector's insert(pos, first, last) 0x005E3458 (tail
//   jump), 0x005E3734, 0x005E3990. The catalog label "std_Vector_InsertRange" is descriptive.
// Callees: __uninitialized_copy 0x005E16F0 (five sites), __copy_backward(..., const random_access_iterator_tag&, int*)
//   0x005E0E70, __copy(..., const random_access_iterator_tag&, int*) 0x005E18B0 (twice), malloc 0x00BFEA0E (inline
//   allocate), __destroy_aux 0x00454404 (a folded COMDAT; the inline _M_clear), free 0x00BFEA14 -- all helpers
//   __fastcall; same-TU noinline stand-ins here.
// Shape: STLport 4.6 `_M_range_insert(pos, first, last, const forward_iterator_tag&)` verbatim (distance = last -
//   first, the `elems_after > n` and `<= n` in-place arms, the growth arm with const-reference max). With same-TU
//   noinline bodies for the four helpers every `__false_type()` / `random_access_iterator_tag()` temporary is homed
//   in the first argument slot (although first is still read later), copy() reaching __copy through the inline
//   __copy_ptrs with the `(int*)0` distance pushed first, as retail.
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                    // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) throw() { return where; }

class CDefString { public: long ID; };
class CIVCountedPointeeBase { public: virtual ~CIVCountedPointeeBase(); long RefCount; };
class CAnimComponentMap : public CIVCountedPointeeBase { };
template <class T> class CIVCountedPointer {
public:
    T* Data;
    CIVCountedPointer(const CIVCountedPointer& o) : Data(o.Data) { if (Data) ++Data->RefCount; }
    CIVCountedPointer& operator=(const CIVCountedPointer& o);
    ~CIVCountedPointer();
};
class CAnimationEntry {
public:
    long BankIndex;                             // +0x00
    CDefString AnimName;                        // +0x04
    CDefString GroupName;                       // +0x08
    CIVCountedPointer<CAnimComponentMap> Components;   // +0x0c
};

namespace std {
struct __false_type { __false_type() {} };
struct forward_iterator_tag { forward_iterator_tag() {} };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() {} };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() {} };
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class T> inline void _Destroy(T* p) { p->~T(); }

template <class T> class allocator {
public:
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};
template <class V, class T, class A> class _STLP_alloc_proxy : public A {
public:
    V _M_data;
};
}

typedef std::pair<unsigned long, CAnimationEntry> CAnimationPair;

namespace std {
__declspec(noinline) CAnimationPair* __fastcall __uninitialized_copy(const CAnimationPair* first,
    const CAnimationPair* last, CAnimationPair* result, const __false_type&)   // 0x005E16F0
{
    CAnimationPair* cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
__declspec(noinline) CAnimationPair* __fastcall __copy_backward(CAnimationPair* first, CAnimationPair* last,
    CAnimationPair* result, const random_access_iterator_tag&, int*)   // 0x005E0E70
{
    for (int n = int(last - first); n > 0; --n)
        *--result = *--last;
    return result;
}
__declspec(noinline) CAnimationPair* __fastcall __copy(const CAnimationPair* first, const CAnimationPair* last,
    CAnimationPair* result, const random_access_iterator_tag&, int*)   // 0x005E18B0
{
    for (int n = int(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
__declspec(noinline) void __fastcall __destroy_aux(CAnimationPair* first, CAnimationPair* last,
    const __false_type&)   // 0x00454404 (folded COMDAT)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
inline void _Destroy(CAnimationPair* first, CAnimationPair* last) { __destroy_aux(first, last, __false_type()); }
inline CAnimationPair* __copy_ptrs(const CAnimationPair* first, const CAnimationPair* last, CAnimationPair* result,
                                   const __false_type&)
{
    return __copy(first, last, result, random_access_iterator_tag(), (int*)0);
}
inline CAnimationPair* __copy_backward_ptrs(CAnimationPair* first, CAnimationPair* last, CAnimationPair* result,
                                            const __false_type&)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}
inline CAnimationPair* copy(const CAnimationPair* first, const CAnimationPair* last, CAnimationPair* result)
{
    return __copy_ptrs(first, last, result, __false_type());
}

template <class T, class A> class _Vector_base {
public:
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, T, A> _M_end_of_storage;
};

template <class T, class A = allocator<T> > class vector : public _Vector_base<T, A> {
public:
    typedef T* pointer;
    typedef T* iterator;
    typedef const T* const_pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
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
    template <class _ForwardIterator>
    void _M_range_insert(iterator __position, _ForwardIterator __first, _ForwardIterator __last,
                         const forward_iterator_tag&)
    {
        if (__first != __last) {
            size_type __n = size_type(__last - __first);
            if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
                const size_type __elems_after = this->_M_finish - __position;
                pointer __old_finish = this->_M_finish;
                if (__elems_after > __n) {
                    __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __false_type());
                    this->_M_finish += __n;
                    __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                    copy(__first, __last, __position);
                }
                else {
                    _ForwardIterator __mid = __first + __elems_after;
                    __uninitialized_copy(__mid, __last, this->_M_finish, __false_type());
                    this->_M_finish += __n - __elems_after;
                    __uninitialized_copy(__position, __old_finish, this->_M_finish, __false_type());
                    this->_M_finish += __elems_after;
                    copy(__first, __mid, __position);
                }
            }
            else {
                const size_type __old_size = size();
                const size_type __len = __old_size + (max)(__old_size, __n);
                pointer __new_start = this->_M_end_of_storage.allocate(__len);
                pointer __new_finish = __new_start;
                __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
                __new_finish = __uninitialized_copy(__first, __last, __new_finish, __false_type());
                __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
                _M_clear();
                _M_set(__new_start, __new_finish, __new_start + __len);
            }
        }
    }
};
}

template void std::vector<CAnimationPair>::_M_range_insert<const CAnimationPair*>(CAnimationPair*,
    const CAnimationPair*, const CAnimationPair*, const std::forward_iterator_tag&);