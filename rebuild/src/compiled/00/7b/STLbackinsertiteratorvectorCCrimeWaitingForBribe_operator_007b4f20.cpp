// _STL::back_insert_iterator<vector<CCrimeWaitingForBribe> >::operator=(const CCrimeWaitingForBribe& value) -- append one bribe record through the iterator's vector (STLport push_back inlined: construct in place, or grow through _M_insert_overflow) and return the iterator
// Leaf: ??4?$back_insert_iterator@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B4F20, 69 bytes (+11 int3), thiscall `ret 4`, returns this.
// Callees: vector<CCrimeWaitingForBribe>::_M_insert_overflow(pointer, const T&, const __false_type&, size_type, bool)
//   0x007B4BE0 (`ret 14h`; element loops, no memmove: STLport's __false_type path).
// Identity: no direct callers (an emitted COMDAT of the CTCGuard TU, whose save path copies the guard's
//   deque<CCrimeWaitingForBribe> PendingCrimes into a vector through back_inserter: copy 0x007B52B0 -> __copy_aux
//   0x007B51B0 -> __copy 0x007B5030). Catalog label "std_pair_InsertAt" is descriptive.
// Shape: STLport `container->push_back(value); return *this;` with push_back = `if (_M_finish != _M_end_of_storage)
//   { _Construct(_M_finish, x); ++_M_finish; } else _M_insert_overflow(_M_finish, x, __false_type(), 1, true);` -- the
//   placement-new null test, the 8-byte record copied through edi/edx, `add [ecx+4],8`. The empty tag temporary is
//   homed on the value's argument slot (`lea edx,[esp+10h]` pushed before the value itself): VC7.1 does that only when
//   _M_insert_overflow has a same-TU noinline body that reads the value and never touches the (unnamed) tag.
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

class CCrimeWaitingForBribe {                               // 8 bytes
public:
    long DeedHandle;                                        // +0x00
    long Type;                                              // +0x04 EOpinionDeedType
};

namespace _STL {
struct __false_type { __false_type() { } };
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
};

template class back_insert_iterator<vector<CCrimeWaitingForBribe> >;
}