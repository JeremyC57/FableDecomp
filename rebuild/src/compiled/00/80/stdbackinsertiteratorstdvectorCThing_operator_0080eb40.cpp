// std::back_insert_iterator<std::vector<CThing*> >::operator=(CThing* const& value) -- STLport back inserter assignment: push the thing pointer onto the target vector, returning the inserter
// Leaf: ??4?$back_insert_iterator@
// Retail: Fable.exe @ 0x0080EB40, 61 bytes, `ret 4`: __fastcall (this = the inserter -> esi), the value reference on the
//   stack; returns this.
// No callers (an unreferenced COMDAT of the CTCDExplosion TU's transform into back_inserter(vector<CThing*>)).
// Callee: vector<CThing*>::_M_insert_overflow(pointer, const T&, const __true_type&, size_type, bool) 0x00436AC0
//   (stand-in body below; landed callers declare it for vector<CThing*>).
// Label: catalogued `Vector_PushBack_U32` (descriptive).
// Shape: STLport `container->push_back(value); return *this;` with push_back inlined (placement-new null test,
//   `add [ecx+4],4`; else the overflow with the empty __true_type temporary homed on the value parameter's slot, which
//   needs the same-TU auto_inline(off)/noinline overflow body that never touches its tag -- GOTCHA 1276).
extern "C" void* __cdecl malloc(unsigned int size);          // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

class CThing;

namespace std {
struct __true_type { __true_type() { } };
template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }

template <class T> class vector {
public:
    typedef T value_type;
    T* _M_start;                                             // +0x00
    T* _M_finish;                                            // +0x04
    T* _M_end_of_storage;                                    // +0x08
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned int fill_len, bool atend);
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type(), 1UL, true);
    }
};

// Same-TU stand-in for the out-of-line STLport growth path (GOTCHAS: it reads x only through its reference and never
// touches the tag, so the caller homes the tag temporary on a dead argument slot as retail does).
#pragma auto_inline(off)
template <class T>
__declspec(noinline) void vector<T>::_M_insert_overflow(T* position, const T& x, const __true_type&,
                                                        unsigned int fill_len, bool atend)
{
    const unsigned int old_size = size();
    const unsigned int len = old_size + (max)(old_size, fill_len);
    T* new_start = (T*)malloc(len * sizeof(T));
    T* new_finish = new_start;
    for (T* p = _M_start; p != position; ++p, ++new_finish)
        *new_finish = *p;
    for (unsigned int n = fill_len; n > 0; --n, ++new_finish)
        *new_finish = x;
    if (!atend)
        for (T* q = position; q != _M_finish; ++q, ++new_finish)
            *new_finish = *q;
    if (_M_start)
        free(_M_start);
    _M_start = new_start;
    _M_finish = new_finish;
    _M_end_of_storage = new_start + len;
}
#pragma auto_inline()

template <class Container> class back_insert_iterator {
protected:
    Container* container;
public:
    explicit back_insert_iterator(Container& x) : container(&x) { }
    back_insert_iterator<Container>& operator=(const typename Container::value_type& value)
    {
        container->push_back(value);
        return *this;
    }
    back_insert_iterator<Container>& operator*() { return *this; }
    back_insert_iterator<Container>& operator++() { return *this; }
};
}

template class std::back_insert_iterator<std::vector<CThing*> >;