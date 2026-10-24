// _STL::transform(const C3DVector* first, const C3DVector* last, back_insert_iterator<vector<C2DVector> > result, CProjectOntoPlane op) -- project every 3D point of [first, last) onto a plane basis ((p - origin).axisX, (p - origin).axisY) and push the 2D results through the back inserter; returns the inserter
// Grade: DIFFER(211v203) -- the X dot product goes to the named local r and is copied into the push_back temporary by an integer move (+8 bytes, frame 0x14 v 0x0c); retail stores it straight into the temporary
// Leaf: ??$transform@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5A9D0, 203 bytes (+5 int3), `ret 14h` (link-time __fastcall: the
//   4-byte back_insert_iterator result through the hidden pointer in ecx (it has a user constructor), `first` in edx,
//   then last, the inserter by value and the 12-byte functor by value on the stack).
// Callees: vector<C2DVector>::_M_insert_overflow(pos, x, const __true_type&, n, atend) @ 0x004416E0.
// Callers: 0x00A58DBC and 0x00A59781 (the functor is built in the argument area from three C3DVector addresses).
// Label: the manifest's `Vector_InsertMany_PairFloat` is descriptive; the body is STLport 4.6's `transform` into a
//   back inserter (its operator= is the sibling row 0x00A5A910).
// Shape: `for (; first != last; ++first, ++result) *result = op(*first); return result;` -- op() is an inline
//   `C3DVector d = p - *Origin; return C2DVector(d * *AxisX, d * *AxisY);` (dot products, x first); push_back inline with
//   the __true_type tag homed in a dead argument slot.
inline void* operator new(unsigned int, void* p) { return p; }

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float operator*(const C3DVector& v) const { return X * v.X + Y * v.Y + Z * v.Z; }
};

class C2DVector {
public:
    float X;                                            // +0x00
    float Y;                                            // +0x04
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) { }
};

// descriptive: the plane-projection functor (three borrowed C3DVector pointers, passed by value)
struct CProjectOntoPlane {
    const C3DVector* Origin;                            // +0x00
    const C3DVector* AxisX;                             // +0x04
    const C3DVector* AxisY;                             // +0x08
    C2DVector operator()(const C3DVector& p) const
    {
        C3DVector d = p - *Origin;
        C2DVector r;
        r.X = d * *AxisX;
        r.Y = d * *AxisY;
        return r;
    }
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
    back_insert_iterator<Container>& operator*() { return *this; }
    back_insert_iterator<Container>& operator++() { return *this; }
    back_insert_iterator<Container> operator++(int) { return *this; }
};

template <class InputIter, class OutputIter, class UnaryOperation>
OutputIter transform(InputIter first, InputIter last, OutputIter result, UnaryOperation opr)
{
    for (; first != last; ++first, ++result)
        *result = opr(*first);
    return result;
}

template back_insert_iterator<vector<C2DVector> >
transform(const C3DVector*, const C3DVector*, back_insert_iterator<vector<C2DVector> >, CProjectOntoPlane);

}   // namespace _STL
