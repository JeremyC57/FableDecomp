// _STL::__copy_aux(_Deque_iterator<CCrimeWaitingForBribe> first, _Deque_iterator<CCrimeWaitingForBribe> last, back_insert_iterator<vector<CCrimeWaitingForBribe> > result, const __false_type&) -- STLport copy dispatch for non-pointer iterators: forwards to the random-access __copy with a fresh category tag and a null distance pointer
// Leaf: ??$__copy_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B51B0, 91 bytes (+5 int3), `ret 24h`: /Gr __fastcall returning the inserter
//   through the hidden pointer in ecx (passed straight on to __copy), the __false_type tag reference in edx (unused),
//   first, last (16-byte deque iterators with a user copy constructor) and the inserter on the stack.
// Callees: __copy<_Deque_iterator<...>, back_insert_iterator<vector<...> >, int> 0x007B5030.
// Identity: called by copy(first, last, result) 0x007B52B0 (`return __copy_aux(first, last, result, _BothPtrType<>
//   ::_Ret())`), the CTCGuard save path that copies deque PendingCrimes into a vector<CCrimeWaitingForBribe>. Catalog
//   label "Fractal_InitDensityMap" is wrong.
// Shape: STLport 4.5 `return __copy(__first, __last, __result, _STLP_ITERATOR_CATEGORY(__first, _InputIter),
//   _STLP_DISTANCE_TYPE(__first, _InputIter));` -- the null distance pointer and the inserter pushed first, then the
//   two iterators copied into the argument area (last, then first: right to left), the random_access_iterator_tag
//   temporary homed on the inserter's dead argument slot (`lea edx,[esp+50h]`).
namespace _STL {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
}

class CCrimeWaitingForBribe {                               // 8 bytes
public:
    long DeedHandle;                                        // +0x00
    long Type;                                              // +0x04 EOpinionDeedType
};

namespace _STL {
template <class T> class vector {
public:
    typedef T value_type;
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    T* _M_end_of_storage;                                   // +0x08
};

template <class T> struct _Deque_iterator {
    T*  _M_cur;                                             // +0x00
    T*  _M_first;                                           // +0x04
    T*  _M_last;                                            // +0x08
    T** _M_node;                                            // +0x0c
    typedef _Deque_iterator<T> _Self;
    _Deque_iterator(const _Self& __x) : _M_cur(__x._M_cur), _M_first(__x._M_first), _M_last(__x._M_last), _M_node(__x._M_node) { }
};

template <class T> inline random_access_iterator_tag __ITERATOR_CATEGORY(const _Deque_iterator<T>&)
{
    return random_access_iterator_tag();
}
template <class T> inline int* __DISTANCE_TYPE(const _Deque_iterator<T>&) { return (int*)0; }

template <class _Container> class back_insert_iterator {
protected:
    _Container* container;
public:
    explicit back_insert_iterator(_Container& __x) : container(&__x) { }
};

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);   // 0x007B5030

template <class _InputIter, class _OutputIter>
_OutputIter __fastcall __copy_aux(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, __ITERATOR_CATEGORY(__first), __DISTANCE_TYPE(__first));
}

template back_insert_iterator<vector<CCrimeWaitingForBribe> > __fastcall
__copy_aux<_Deque_iterator<CCrimeWaitingForBribe>, back_insert_iterator<vector<CCrimeWaitingForBribe> > >(
    _Deque_iterator<CCrimeWaitingForBribe>, _Deque_iterator<CCrimeWaitingForBribe>,
    back_insert_iterator<vector<CCrimeWaitingForBribe> >, const __false_type&);
}