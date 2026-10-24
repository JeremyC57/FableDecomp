// _STL::__copy(_Deque_iterator<CCrimeWaitingForBribe> first, _Deque_iterator<CCrimeWaitingForBribe> last, back_insert_iterator<vector<CCrimeWaitingForBribe> > result, const random_access_iterator_tag&, int*) -- STLport random-access copy of a deque range into a vector through a back inserter: n = last - first push_backs walking the deque nodes, returns the inserter
// Leaf: ??$__copy@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B5030, 170 bytes (+2 int3), `ret 28h`: /Gr __fastcall returning the 4-byte
//   back_insert_iterator through the hidden pointer in ecx; the tag reference is the first register-eligible argument
//   (edx, unused); first and last (16-byte deque iterators), the inserter and the distance pointer on the stack.
// Callees: vector<CCrimeWaitingForBribe>::_M_insert_overflow(pointer, const T&, const __false_type&, size_type, bool)
//   0x007B4BE0 (the inline vector::push_back's growth path).
// Identity: called by __copy_aux 0x007B51B0 (copy 0x007B52B0 -> __copy_aux -> __copy), the CTCGuard save path copying
//   deque PendingCrimes into a vector<CCrimeWaitingForBribe>. Catalog label "Vector_InsertRange_8byte_Deque" is
//   descriptive.
// Shape: STLport `for (_Distance __n = __last - __first; __n > 0; --__n) { *__result = *__first; ++__first;
//   ++__result; } return __result;` with the deque iterator's `_M_subtract` (16 * (node gap - 1) + the two partial
//   runs), `_M_increment` (cur in esi, last in ebp, node in ebx; `lea ebp,[esi+80h]` after the node step) and the
//   inserter's vector::push_back inline. The inserter's container is copied to edi and the count is spilled into the
//   inserter's dead argument slot; the empty __false_type tag of the growth call is homed on that same live slot
//   (`lea ecx,[esp+40h]`), which VC7.1 does only with a same-TU noinline _M_insert_overflow that reads the value and
//   never touches the (unnamed) tag. The hidden result pointer sits in the `push ecx` word across the call.
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

class CCrimeWaitingForBribe {                               // 8 bytes
public:
    long DeedHandle;                                        // +0x00
    long Type;                                              // +0x04 EOpinionDeedType
};

namespace _STL {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class T> class vector {
public:
    typedef T value_type;
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    T* _M_end_of_storage;                                   // +0x08
    __declspec(noinline) void _M_insert_overflow(T* __position, const T& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x007B4BE0
    void push_back(const T& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};

// Same-TU stand-in for the out-of-line growth path (only the target row is graded): it reads the value through
// its reference and never touches the empty tag.
template <class T>
void vector<T>::_M_insert_overflow(T* __position, const T& __x, const __false_type&, unsigned int __fill_len, bool __atend)
{
    if (__x.DeedHandle == 0 && __atend)
        this->_M_finish = __position + __fill_len;
}

template <class T> struct _Deque_iterator {
    T*  _M_cur;                                             // +0x00
    T*  _M_first;                                           // +0x04
    T*  _M_last;                                            // +0x08
    T** _M_node;                                            // +0x0c
    typedef _Deque_iterator<T> _Self;
    _Deque_iterator(const _Self& __x) : _M_cur(__x._M_cur), _M_first(__x._M_first), _M_last(__x._M_last), _M_node(__x._M_node) { }
    static unsigned int _S_buffer_size() { return 128 / sizeof(T); }
    int operator-(const _Self& __x) const
    {
        unsigned int __buffer_size = _S_buffer_size();
        return int(__buffer_size) * (_M_node - __x._M_node - 1) + (_M_cur - _M_first) + (__x._M_last - __x._M_cur);
    }
    void _M_set_node(T** __new_node)
    {
        _M_last = (_M_first = *(_M_node = __new_node)) + int(_S_buffer_size());
    }
    _Self& operator++()
    {
        if (++_M_cur == _M_last) {
            _M_set_node(_M_node + 1);
            _M_cur = _M_first;
        }
        return *this;
    }
    T& operator*() const { return *_M_cur; }
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

template back_insert_iterator<vector<CCrimeWaitingForBribe> > __fastcall
__copy<_Deque_iterator<CCrimeWaitingForBribe>, back_insert_iterator<vector<CCrimeWaitingForBribe> >, int>(
    _Deque_iterator<CCrimeWaitingForBribe>, _Deque_iterator<CCrimeWaitingForBribe>,
    back_insert_iterator<vector<CCrimeWaitingForBribe> >, const random_access_iterator_tag&, int*);
}