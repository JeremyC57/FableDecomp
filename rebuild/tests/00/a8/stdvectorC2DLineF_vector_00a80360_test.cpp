#include <cstdio>
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }
class C2DVector {
public:
    float X;
    float Y;
    C2DVector() {}
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) {}
};
class C2DLineF {
public:
    C2DVector Point1;
    C2DVector Point2;
    C2DLineF() {}
};
namespace std {
template <class T> class allocator {
public:
    allocator() {}
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};
template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) {}
};
template <class T, class A> class _Vector_base {
public:
    _Vector_base(unsigned int n, const A& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, A> _M_end_of_storage;
};
template <class T> __forceinline T* uninitialized_fill_n(T* first, unsigned int n, const T& x)
{
    T* cur = first;
    for ( ; n > 0; --n, ++cur)
        new (cur) T(x);
    return cur;
}
template <class T, class A = allocator<T> > class vector : public _Vector_base<T, A> {
public:
    explicit vector(unsigned int n) : _Vector_base<T, A>(n, A())
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, n, T());
    }
};
template class vector<C2DLineF>;
}

extern "C" void* __cdecl malloc(unsigned int);
int main()
{
    std::vector<C2DLineF> a(0);
    bool ok = a._M_start == 0 && a._M_finish == 0 && a._M_end_of_storage._M_data == 0;
    std::vector<C2DLineF>* b = new std::vector<C2DLineF>(5);
    ok = ok && b->_M_start != 0 && b->_M_finish == b->_M_start + 5 && b->_M_end_of_storage._M_data == b->_M_start + 5;
    std::vector<C2DLineF> c(1);
    ok = ok && c._M_finish == c._M_start + 1;
    std::printf(ok ? "00a80360_TEST PASS\n" : "00a80360_TEST FAIL\n");
    return ok ? 0 : 1;
}