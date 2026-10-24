// Behaviour test for 0x0050FA40 std::_Vector_base<void*>::_Vector_base(n, a): start = malloc(n * 4) (null for n == 0),
// finish = start, end of storage = start + n.
#include <cstdio>
#include <cstdlib>

template <class T> class allocator_model {
public:
    allocator_model() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)std::malloc(n * sizeof(T)) : 0; }
};

template <class V, class A> struct alloc_proxy_model : public A {
    V _M_data;
    alloc_proxy_model(const A& a, V p) : A(a), _M_data(p) { }
};

template <class T, class A> class vector_base_model {
public:
    vector_base_model(unsigned int n, const A& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
    T* _M_start;
    T* _M_finish;
    alloc_proxy_model<T*, A> _M_end_of_storage;
};

int main()
{
    allocator_model<void*> a;
    vector_base_model<void*, allocator_model<void*> > v(5, a);
    if (v._M_start == 0 || v._M_finish != v._M_start || v._M_end_of_storage._M_data != v._M_start + 5) { std::printf("0050fa40_TEST FAIL\n"); return 1; }
    std::free(v._M_start);
    vector_base_model<void*, allocator_model<void*> > e(0, a);
    if (e._M_start != 0 || e._M_finish != 0 || e._M_end_of_storage._M_data != 0) { std::printf("0050fa40_TEST FAIL\n"); return 1; }
    std::printf("0050fa40_TEST PASS\n");
    return 0;
}