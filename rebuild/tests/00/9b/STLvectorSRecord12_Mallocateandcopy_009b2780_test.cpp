// Behaviour test for 0x009B2780 vector<pair<unsigned, {int,int}>>::_M_allocate_and_copy: n != 0 allocates n * 12 bytes
// and copy-constructs [first, last) into the front; n == 0 allocates nothing and returns null (empty range).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
static int g_mallocs = 0, g_frees = 0;
static void* my_malloc(unsigned int n) { ++g_mallocs; return std::malloc(n); }
static void my_free(void* p) { ++g_frees; std::free(p); }
struct SValue8 { int a; int b; };
template <class T1, class T2> struct pair {
    T1 first; T2 second;
    pair() : first(T1()), second(T2()) {}
    pair(const T1& a, const T2& b) : first(a), second(b) {}
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) {}
};
template <class T> struct vec {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    T* allocate(unsigned int n) { return n != 0 ? (T*)my_malloc(n * sizeof(T)) : 0; }
    T* _M_allocate_and_copy(unsigned int n, const T* first, const T* last)
    {
        T* result = allocate(n);
        T* cur = result;
        for (; first != last; ++first, ++cur) new (cur) T(*first);
        return result;
    }
    void reserve(unsigned int n)
    {
        if (capacity() < n) {
            const unsigned int old_size = size();
            T* tmp;
            if (_M_start) { tmp = _M_allocate_and_copy(n, _M_start, _M_finish); if (_M_start) my_free(_M_start); }
            else tmp = allocate(n);
            _M_start = tmp; _M_finish = tmp + old_size; _M_end_of_storage = tmp + n;
        }
    }
};

typedef pair<unsigned int, SValue8> E;
int main()
{
    E src[3];
    for (int i = 0; i < 3; ++i) { src[i].first = 0xA0u + i; src[i].second.a = i * 7; src[i].second.b = -i; }
    vec<E> v; v._M_start = v._M_finish = v._M_end_of_storage = 0;
    E* p = v._M_allocate_and_copy(5, src, src + 3);
    if (p == 0 || g_mallocs != 1) { std::printf("009b2780_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i)
        if (p[i].first != 0xA0u + i || p[i].second.a != i * 7 || p[i].second.b != -i) { std::printf("009b2780_TEST FAIL\n"); return 1; }
    std::free(p);
    E* q = v._M_allocate_and_copy(0, src, src);
    if (q != 0 || g_mallocs != 1) { std::printf("009b2780_TEST FAIL\n"); return 1; }
    if (sizeof(E) != 12) { std::printf("009b2780_TEST FAIL\n"); return 1; }
    std::printf("009b2780_TEST PASS\n");
    return 0;
}