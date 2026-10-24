// CCardinalSpline::CCardinalSpline(const C3DVector& start, const C3DVector& end, float tension, bool endPointsConnected) -- two-point cardinal spline: empty control-point array, the tension and the end-points-connected flag, then both points appended
// Grade: DIFFER(167v167) -- register swap only: the first inline push_back copies through edx/ecx (retail ecx/edx) and the overflow call pushes the tag from ecx (retail edx); the second push_back and everything else match
// Leaf: ??0CCardinalSpline@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A56AA0, 167 bytes, `ret 10h` (PDB ??0CCardinalSpline@@QAE@ABVC3DVector@@0M_N@Z).
// Callees: vector<C3DVector>::_M_insert_overflow(pos, x, const __false_type&, n, atend) @ 0x0066E370 (twice).
// Label: the manifest's `CPSCSplineBase_Initialize` is descriptive (the particle spline component holds a CCardinalSpline).
// Layout: Ego_r.pdb CCardinalSpline: ControlPoints (CArray<C3DVector>, a 12-byte STLport vector in retail) +0x00,
//   Tension +0x0c, EndPointsConnected +0x10 (PDB +0x10 / +0x14 with the 16-byte Dinkumware container). No engine header.
// Shape: the CArray constructor is STLport's `_Vector_base(const allocator&)` (`_M_start(0), _M_finish(0)` and the
//   end-of-storage pointer in an `_STLP_alloc_proxy` over an `allocator_type()` temporary); Tension / EndPointsConnected
//   come from the mem-initialiser list; the two inline push_backs (placement-new null test, three dword copies,
//   `add [esi+4],0Ch`, else _M_insert_overflow with the tag homed in the dead bool argument slot).
inline void* operator new(unsigned int, void* p) { return p; }

class C3DVector {
public:
    float X, Y, Z;
};

namespace _STL {

struct __false_type { __false_type() { } };

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class T> class allocator {
public:
    allocator() { }
};

template <class V, class A> class _STLP_alloc_proxy : public A {
public:
    V _M_data;
    _STLP_alloc_proxy(const A& a, V v) : A(a), _M_data(v) { }
};

template <class T, class Alloc> class _Vector_base {
public:
    T* _M_start;                                        // +0x00
    T* _M_finish;                                       // +0x04
    _STLP_alloc_proxy<T*, Alloc> _M_end_of_storage;     // +0x08
    _Vector_base(const Alloc& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) { }
};

template <class T, class Alloc = allocator<T> > class vector : public _Vector_base<T, Alloc> {
public:
    typedef Alloc allocator_type;
    typedef unsigned int size_type;
    explicit vector(const allocator_type& a = allocator_type()) : _Vector_base<T, Alloc>(a) { }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, size_type fill_len, bool atend);   // 0x0066E370
    void push_back(const T& x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, x, __false_type(), 1UL, true);
    }
};

}   // namespace _STL

template <class T> class CArray : public _STL::vector<T> {
public:
    CArray() { }
};

class CCardinalSpline {
public:
    CArray<C3DVector> ControlPoints;                    // +0x00
    float             Tension;                          // +0x0c
    bool              EndPointsConnected;               // +0x10
    CCardinalSpline(const C3DVector& start, const C3DVector& end, float tension, bool endPointsConnected);
};

CCardinalSpline::CCardinalSpline(const C3DVector& start, const C3DVector& end, float tension, bool endPointsConnected)
    : Tension(tension), EndPointsConnected(endPointsConnected)
{
    ControlPoints.push_back(start);
    ControlPoints.push_back(end);
}
