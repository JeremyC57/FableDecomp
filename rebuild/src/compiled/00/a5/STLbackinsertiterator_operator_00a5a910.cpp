// _STL::back_insert_iterator::operator=(const C2DVector& value) -- push the 2D vector onto the back of the target vector<C2DVector> (in place when there is capacity, else through _M_insert_overflow) and return the inserter
// Leaf: ??4?$back_insert_iterator@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5A910, 69 bytes, `ret 4` (thiscall: the inserter in ecx).
// Callees: vector<C2DVector>::_M_insert_overflow(C2DVector* pos, const C2DVector& x, const __true_type&, size_type n,
//   bool atend) @ 0x004416E0 (catalogued "VectorReallocateDouble"). Out-of-line COMDAT of the inline inserter used by the
//   transform at 0x00A5A9D0 (the plane-projection of a C3DVector range into a vector<C2DVector>).
// Label: the manifest's `Vector_Pair_Append` is descriptive; the body is STLport 4.6's
//   `back_insert_iterator<vector<C2DVector> >::operator=`.
// Shape: `container->push_back(value); return *this;` with push_back inline: `_M_finish != _M_end_of_storage` ->
//   placement-new null test + two dword copies + `add [ecx+4],8`; else `_M_insert_overflow(_M_finish, x, __true_type(),
//   1, true)` (C2DVector is POD, so the tag is __true_type).
inline void* operator new(unsigned int, void* p) { return p; }

class C2DVector {
public:
    float X;                                            // +0x00
    float Y;                                            // +0x04
};

namespace _STL {

struct __true_type { __true_type() { } };

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class T> class allocator { };

template <class T, class Alloc = allocator<T> > class vector {
public:
    typedef T value_type;
    typedef unsigned int size_type;
    T* _M_start;                                        // +0x00
    T* _M_finish;                                       // +0x04
    T* _M_end_of_storage;                               // +0x08
    void _M_insert_overflow(T* position, const T& x, const __true_type&, size_type fill_len, bool atend);   // 0x004416E0
    void push_back(const T& x)
    {
        if (this->_M_finish != this->_M_end_of_storage) {
            _Construct(this->_M_finish, x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, x, __true_type(), 1UL, true);
    }
};

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
};

template class back_insert_iterator<vector<C2DVector> >;

}   // namespace _STL