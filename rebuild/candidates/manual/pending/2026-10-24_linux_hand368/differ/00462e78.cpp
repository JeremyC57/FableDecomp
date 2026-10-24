// std::vector<bool>::operator=(const vector<bool>& x) -- STLport vector assignment of 1-byte bools: reallocate when x outgrows the capacity, else memmove over the live elements and copy the tail through the POD __uninitialized_copy; returns *this
// Grade: DIFFER(169v167) -- the tail-copy first argument x._M_start + size() is formed as `mov ecx,[x] ; sub ecx,[esi] ; push edx ; add ecx,edx` (x.start first, finish kept in edx) where retail keeps finish in ecx and forms `push ecx ; sub ecx,[esi] ; add ecx,[x]`; everything else matches
// Leaf: ??4?$vector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00462E78, length 167 bytes, `ret 4`.
// Callees: _M_allocate_and_copy @ 0x00462F50, _M_clear @ 0x00462F42, memmove @ 0x00BFEAE6, the POD
// __uninitialized_copy(first, last, result, const __true_type&) @ 0x00462F1F (`__fastcall`: first ecx, last edx).
// Label: the catalog "CSimpleMemoryPool_AssignFrom" is propagated. CPlayerInventoryDef::operator= (landed 0x00462E2D) names
// this body as IsUsable's vector<bool>::operator=; retail STLport keeps vector<bool> a plain byte vector.
// Model: STLport 4.x `vector::operator=` verbatim for a POD element: `copy` reaches `__copy_trivial` (the
// `(last == first) ? result : memmove(...) + n` form, inlined), the tail goes to the out-of-line POD
// __uninitialized_copy with a POD tag returned BY VALUE from an inline `_Is_POD<T>::_Answer()`: the tag type has no
// user constructor, so its value-initialisation is the retail `xor eax,eax ; lea edi,[ebp-1] ; stosb` and the return
// copy is the `mov al,[ebp-1] ; mov [ebp-1],al` into the (shared) argument temporary.
// Shape: frame pointer = size TU with /Oy off: #pragma optimize("y", off) + ("s", on).
#pragma optimize("y", off)
#pragma optimize("s", on)

extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int n);   // 0x00BFEAE6

namespace std {
struct __true_type { };

__forceinline void* __copy_trivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result :
        ((char*)memmove(result, first, ((const char*)last - (const char*)first))) + ((const char*)last - (const char*)first);
}

template <class T> __forceinline T* copy(const T* first, const T* last, T* result)
{
    return (T*)__copy_trivial(first, last, result);
}

template <class T> struct _Is_POD {                // the POD answer handed back by value
    static __forceinline __true_type _Answer() { return __true_type(); }
};

template <class InputIter, class ForwardIter>
ForwardIter __fastcall __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __true_type&);   // 0x00462F1F

template <class T> class vector {
public:
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef __true_type _IsPODType;
    pointer _M_start;
    pointer _M_finish;
    pointer _M_end_of_storage;

    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    pointer _M_allocate_and_copy(unsigned int n, const_pointer first, const_pointer last);   // 0x00462F50
    void _M_clear();                                                                        // 0x00462F42
    vector& operator=(const vector& x);
};

template <class T> vector<T>& vector<T>::operator=(const vector<T>& x)
{
    if (&x != this) {
        const unsigned int xlen = x.size();
        if (xlen > capacity()) {
            pointer tmp = _M_allocate_and_copy(xlen, (const_pointer)x._M_start + 0, (const_pointer)x._M_finish + 0);
            _M_clear();
            _M_start = tmp;
            _M_end_of_storage = _M_start + xlen;
        }
        else if (size() >= xlen) {
            copy((const_pointer)x._M_start + 0, (const_pointer)x._M_finish + 0, _M_start);
        }
        else {
            copy((const_pointer)x._M_start, (const_pointer)x._M_start + size(), _M_start);
            __uninitialized_copy((const_pointer)x._M_start + size(), (const_pointer)x._M_finish + 0, _M_finish, _Is_POD<T>::_Answer());
        }
        _M_finish = _M_start + xlen;
    }
    return *this;
}

template class vector<bool>;
}
