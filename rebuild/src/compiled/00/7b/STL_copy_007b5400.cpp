// std::__copy(const CCrimeWaitingForBribe* first, const CCrimeWaitingForBribe* last, back_insert_iterator<deque<CCrimeWaitingForBribe> > result, const random_access_iterator_tag&, int*) -- STLport random-access copy of a bribe-record array into a deque through a back inserter: n = last - first push_backs, returns the inserter
// Leaf: ??$__copy@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B5400, 105 bytes (+7 int3), `ret 10h`: /Gr __fastcall returning the 4-byte
//   back_insert_iterator through the hidden pointer in ecx (it has a user constructor), `first` in edx, then last, the
//   inserter by value, the tag reference and the distance pointer on the stack.
// Callees: deque<CCrimeWaitingForBribe>::_M_push_back_aux_v 0x007B4F70 (the inline deque::push_back's slow path).
// Identity: called by the CTCGuard transfer 0x007B3760 (call 0x007B381A: `copy(records.begin(), records.end(),
//   back_inserter(PendingCrimes))` after the vector was loaded) and twice from 0x007B5546 / 0x007B5576. Catalog label
//   "Vector_InsertRange_8byte_StdVector" is descriptive.
// Shape: STLport `for (_Distance __n = __last - __first; __n > 0; --__n) { *__result = *__first; ++__first;
//   ++__result; } return __result;` with the inserter's operator= (deque::push_back) inline: the container stays in
//   esi, the count in ebx, the hidden result pointer in ecx (spilled to the `push ecx` word around the call); the
//   empty range stores the inserter straight from its argument slot.
inline void* operator new(unsigned int, void* p) { return p; }

class CCrimeWaitingForBribe {                               // 8 bytes
public:
    long DeedHandle;                                        // +0x00
    long Type;                                              // +0x04 EOpinionDeedType
};

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class T> struct _Deque_iterator {
    T*  _M_cur;                                             // +0x00
    T*  _M_first;                                           // +0x04
    T*  _M_last;                                            // +0x08
    T** _M_node;                                            // +0x0c
};

template <class T> class deque {
public:
    typedef T value_type;
    _Deque_iterator<T> _M_start;                            // +0x00
    _Deque_iterator<T> _M_finish;                           // +0x10
    T**                _M_map;                              // +0x20
    unsigned int       _M_map_size;                         // +0x24
    void _M_push_back_aux_v(const value_type& __t);         // 0x007B4F70
    void push_back(const value_type& __t)
    {
        if (this->_M_finish._M_cur != this->_M_finish._M_last - 1) {
            _Construct(this->_M_finish._M_cur, __t);
            ++this->_M_finish._M_cur;
        }
        else
            _M_push_back_aux_v(__t);
    }
};

template <class _Container> class back_insert_iterator {
protected:
    _Container* container;
public:
    explicit back_insert_iterator(_Container& __x) : container(&__x) { }
    back_insert_iterator<_Container>& operator=(const typename _Container::value_type& __value)
    {
        container->push_back(__value);
        return *this;
    }
    back_insert_iterator<_Container>& operator*() { return *this; }
    back_insert_iterator<_Container>& operator++() { return *this; }
};

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template back_insert_iterator<deque<CCrimeWaitingForBribe> > __fastcall
__copy<const CCrimeWaitingForBribe*, back_insert_iterator<deque<CCrimeWaitingForBribe> >, int>(
    const CCrimeWaitingForBribe*, const CCrimeWaitingForBribe*, back_insert_iterator<deque<CCrimeWaitingForBribe> >,
    const random_access_iterator_tag&, int*);
}