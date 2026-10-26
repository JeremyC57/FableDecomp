// std::vector<pair<long,CDefPointer<const CHeroAbilityDef> > >::operator=(const vector& x) -- STLport vector assignment of {key, def pointer} pairs: reallocate when x outgrows the capacity, else assign over the live elements and destroy the surplus or copy-construct the tail; returns *this
// Leaf: ??4?$vector@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005FA1BA, 177 bytes, `ret 4`; caller CTCInventoryAbilities::Copy
//   0x005F3DC6 (landed; it assigns the Abilities member, retail +0x160).
// Callees: _M_allocate_and_copy(n, first, last) 0x005F9A3C, _M_clear() 0x005FA018 (landed), `__copy(first, last, result,
//   const random_access_iterator_tag&, int*)` 0x005F8C63 (8-byte pairs; reached through the inline `__copy_ptrs(..,
//   __false_type)`), `__destroy_aux(first, last, const __false_type&)` 0x005F9A7D (landed; through the inline
//   `_Destroy(first, last)`), `__uninitialized_copy(first, last, result, const __false_type&)` 0x005F9132 (landed; through the
//   pair copy constructor 0x005F8578).
// Label correction: the catalog `std_vector_resize_grow` is behavioural (byte-shape twin of 0x005F9F5A with an 8-byte
//   stride); the body is STLport 4.x `vector::operator=`
//   verbatim (the landed vector<CRGBColour> 0x0045BAAA template, here with the element copy out of line).
// Shape: frame pointer kept in a size TU (`#pragma optimize("y", off)` + `("s", on)`); x stays in ebx and its slot holds
//   x._M_finish; the empty tags are homed in the high byte of that slot (`lea [ebp+0Bh]`); the `push 0` Distance* shared
//   by both copy arms is hoisted above the size test.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CDefPointeeBase { public:                             // counted def object: vptr, reference count at +4
    virtual void v00();
    virtual void Destroy() const;                    // slot 1
    mutable long m_refCount;                         // +0x04
};

class CHeroAbilityDef : public CDefPointeeBase { public: };

template <class T> class CDefPointer {
public:
    T* Object;                                       // +0x00
};

typedef CDefPointer<const CHeroAbilityDef> CAbilityDefPointer;

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __false_type { __false_type() { } };

template <class RandomAccessIter, class OutputIter, class Distance>
OutputIter __fastcall __copy(RandomAccessIter first, RandomAccessIter last, OutputIter result,
                             const random_access_iterator_tag&, Distance*);                         // 0x005F8C63
template <class ForwardIter>
void __fastcall __destroy_aux(ForwardIter first, ForwardIter last, const __false_type&);           // 0x005F9A7D
template <class InputIter, class ForwardIter>
ForwardIter __fastcall __uninitialized_copy(InputIter first, InputIter last, ForwardIter result,
                                            const __false_type&);                                   // 0x005F9132

template <class InputIter, class OutputIter>
__forceinline OutputIter __copy_ptrs(InputIter first, InputIter last, OutputIter result, const __false_type&)
{
    return __copy(first, last, result, random_access_iterator_tag(), (int*)0);
}

template <class ForwardIter>
__forceinline void _Destroy(ForwardIter first, ForwardIter last)
{
    __destroy_aux(first, last, __false_type());
}

template <class T> class vector {
public:
    typedef T* pointer;
    typedef const T* const_pointer;
    pointer _M_start;
    pointer _M_finish;
    pointer _M_end_of_storage;

    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    pointer _M_allocate_and_copy(unsigned int n, const_pointer first, const_pointer last);   // 0x005F9A3C
    void _M_clear();                                                                        // 0x005FA018
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
            pointer i = __copy_ptrs((const_pointer)x._M_start + 0, (const_pointer)x._M_finish + 0, (pointer)_M_start,
                                    __false_type());
            _Destroy(i, _M_finish);
        }
        else {
            __copy_ptrs((const_pointer)x._M_start, (const_pointer)x._M_start + size(), (pointer)_M_start, __false_type());
            __uninitialized_copy((const_pointer)x._M_start + size(), (const_pointer)x._M_finish + 0, _M_finish,
                                 __false_type());
        }
        _M_finish = _M_start + xlen;
    }
    return *this;
}

template class vector<pair<long, CAbilityDefPointer> >;
}