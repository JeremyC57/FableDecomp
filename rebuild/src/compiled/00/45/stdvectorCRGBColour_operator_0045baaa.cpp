// std::vector<CRGBColour>::operator=(const vector<CRGBColour>& x) -- STLport vector assignment of 4-byte colours: reallocate when x outgrows the capacity, else assign over the live elements and copy-construct the tail; returns *this
// Leaf: ??4?$vector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045BAAA, length 229 bytes, `ret 4`.
// Callees: _M_allocate_and_copy @ 0x0045BBC5 and std::_Construct<CRGBColour, CRGBColour> @ 0x0045BB8F (`__fastcall`), both
// in this chunk; _M_clear @ 0x0045BBB7 (free the old block).
// Label: the catalog "CActiveFile_AssignFrom" is behavioural; eight callers (def operator= bodies around 0x0046DC27 and
// 0x0082CB53, and 0x0045A431) assign colour vectors through it.
// Model: STLport 4.x `vector::operator=` verbatim, as the landed CBrainBehaviour (0x00462B4E) / CMusicEntry (0x00460D75) rows:
// `__xlen > capacity()` reallocates, `size() >= __xlen` copies with the counted random-access `__copy` (no destroy: the
// colour's destructor is trivial), else copies size() elements and constructs the rest through `__uninitialized_copy`
// with the out-of-line `_Construct`. CRGBColour has a user copy ctor (so it is not POD for the type traits) and the
// implicit assignment (one dword move).
// Shape: frame pointer, locals homed in the dead argument slot = size TU with /Oy off: #pragma optimize("y", off) + ("s", on).
#pragma optimize("y", off)
#pragma optimize("s", on)

class CRGBColour { public:                                // D3DCOLOR byte order, 4 bytes
    unsigned char B, G, R, A;
    CRGBColour(const CRGBColour& other);
};

namespace std {
struct random_access_iterator_tag { };
struct __false_type { };

template <class T1, class T2> void __fastcall _Construct(T1* p, const T2& value);   // 0x0045BB8F

template <class RandomAccessIter, class OutputIter, class Distance>
__forceinline OutputIter __copy(RandomAccessIter first, RandomAccessIter last, OutputIter result,
                                const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

template <class InputIter, class ForwardIter>
__forceinline ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
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
    pointer _M_allocate_and_copy(unsigned int n, const_pointer first, const_pointer last);   // 0x0045BBC5
    void _M_clear();                                                                        // 0x0045BBB7
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
            __copy((const_pointer)x._M_start + 0, (const_pointer)x._M_finish + 0, _M_start, random_access_iterator_tag(), (int*)0);
        }
        else {
            __copy((const_pointer)x._M_start, (const_pointer)x._M_start + size(), _M_start, random_access_iterator_tag(), (int*)0);
            __uninitialized_copy((const_pointer)x._M_start + size(), (const_pointer)x._M_finish + 0, _M_finish, __false_type());
        }
        _M_finish = _M_start + xlen;
    }
    return *this;
}

template class vector<CRGBColour>;
}