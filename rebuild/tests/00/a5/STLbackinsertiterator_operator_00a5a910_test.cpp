// Behaviour test for 0x00A5A910 _STL::back_insert_iterator<vector<C2DVector> >::operator=(const C2DVector&)
#include <cstdio>
#include <cstdlib>
#include <new>

class C2DVector {
public:
    float X;
    float Y;
};

namespace _STL {
struct __true_type { __true_type() { } };
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class T> class vector {
public:
    typedef T value_type;
    typedef unsigned int size_type;
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { std::free(_M_start); }
    // stand-in for 0x004416E0: grow (double, at least 1) and append fill_len copies of x at the end
    void _M_insert_overflow(T* position, const T& x, const __true_type&, size_type fill_len, bool)
    {
        size_type size = (size_type)(_M_finish - _M_start);
        size_type cap = size ? size * 2 : 1;
        if (cap < size + fill_len) cap = size + fill_len;
        T* mem = (T*)std::malloc(cap * sizeof(T));
        size_type n = (size_type)(position - _M_start);
        for (size_type i = 0; i < n; ++i) mem[i] = _M_start[i];
        for (size_type i = 0; i < fill_len; ++i) mem[n + i] = x;
        std::free(_M_start);
        _M_start = mem;
        _M_finish = mem + n + fill_len;
        _M_end_of_storage = mem + cap;
    }
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
}   // namespace _STL

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main()
{
    _STL::vector<C2DVector> v;
    _STL::back_insert_iterator<_STL::vector<C2DVector> > out(v);
    for (int i = 0; i < 5; ++i) {                   // 1st, 2nd and 3rd/5th overflow; 4th constructs in place
        C2DVector p;
        p.X = (float)i;
        p.Y = (float)(i * 10);
        _STL::back_insert_iterator<_STL::vector<C2DVector> >& r = (out = p);
        if (&r != &out) { std::printf("00a5a910_TEST FAIL\n"); return 1; }
    }
    if (v._M_finish - v._M_start != 5) { std::printf("00a5a910_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i)
        if (!Near(v._M_start[i].X, (float)i) || !Near(v._M_start[i].Y, (float)(i * 10))) {
            std::printf("00a5a910_TEST FAIL\n");
            return 1;
        }
    std::printf("00a5a910_TEST PASS\n");
    return 0;
}