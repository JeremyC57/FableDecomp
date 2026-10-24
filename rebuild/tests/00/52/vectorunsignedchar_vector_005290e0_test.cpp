// Behaviour test for 0x005290E0 vector<unsigned char> copy constructor: the copy gets its own exactly-sized block with
// the same bytes (finish == end_of_storage == start + size); an empty source gives three null pointers.
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct __true_type { __true_type() {} };

inline void* __copy_trivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
                           : ((char*)std::memmove(result, first, ((const char*)last - (const char*)first)))
                               + ((const char*)last - (const char*)first);
}
template <class _Tp> inline _Tp* __uninitialized_copy(const _Tp* first, const _Tp* last, _Tp* result, const __true_type&)
{
    return (_Tp*)__copy_trivial(first, last, result);
}

template <class T> class allocator {
public:
    allocator() {}
    allocator(const allocator<T>&) {}
    T* allocate(unsigned int n) { return n != 0 ? (T*)std::malloc(n * sizeof(T)) : 0; }
};
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p) : allocator<T>(a), _M_data(p) {}
};
template <class T, class A> class _Vector_base {
public:
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T> _M_end_of_storage;
    _Vector_base(unsigned int n, const A& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
};
template <class T, class A = allocator<T> > class vector : public _Vector_base<T, A> {
public:
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    A get_allocator() const { return A(); }
    vector(const vector<T, A>& x) : _Vector_base<T, A>(x.size(), x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const T*)x._M_start, (const T*)x._M_finish, this->_M_start, __true_type());
    }
};


int main()
{
    static unsigned char bytes[7] = { 1, 0, 255, 7, 7, 42, 9 };
    char raw[sizeof(vector<unsigned char>)];
    vector<unsigned char>* src = (vector<unsigned char>*)raw;
    src->_M_start = bytes; src->_M_finish = bytes + 7; src->_M_end_of_storage._M_data = bytes + 7;
    vector<unsigned char> c(*src);
    int ok = c._M_start != 0 && c._M_start != bytes && c._M_finish == c._M_start + 7
          && c._M_end_of_storage._M_data == c._M_start + 7 && std::memcmp(c._M_start, bytes, 7) == 0;
    std::free(c._M_start);
    src->_M_start = 0; src->_M_finish = 0; src->_M_end_of_storage._M_data = 0;
    vector<unsigned char> e(*src);
    ok &= e._M_start == 0 && e._M_finish == 0 && e._M_end_of_storage._M_data == 0;
    if (!ok) { std::printf("005290e0_TEST FAIL\n"); return 1; }
    std::printf("005290e0_TEST PASS\n");
    return 0;
}