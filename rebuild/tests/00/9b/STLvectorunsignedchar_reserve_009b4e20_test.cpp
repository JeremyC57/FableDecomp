// Behaviour test for 0x009B4E20 vector<unsigned char>::reserve: growing past capacity moves the elements into a fresh
// n-element block (allocate + copy, then free the old block), keeps size, sets capacity n; an empty vector just allocates;
// a request at or below capacity changes nothing.
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

typedef unsigned char E;
int main()
{
    vec<E> v; v._M_start = v._M_finish = v._M_end_of_storage = 0;
    v.reserve(4);                                    // empty: allocate only
    if (v.capacity() != 4 || v.size() != 0 || g_mallocs != 1 || g_frees != 0) { std::printf("009b4e20_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i) { *v._M_finish = (unsigned char)(0xF0 + i); ++v._M_finish; }
    E* old = v._M_start;
    v.reserve(2);                                    // below capacity: no-op
    v.reserve(4);                                    // equal: no-op
    if (v._M_start != old || g_mallocs != 1) { std::printf("009b4e20_TEST FAIL\n"); return 1; }
    v.reserve(10);                                   // grow: copy + free
    if (v.capacity() != 10 || v.size() != 3 || g_mallocs != 2 || g_frees != 1) { std::printf("009b4e20_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i) { if (v._M_start[i] != (unsigned char)(0xF0 + i)) { std::printf("009b4e20_TEST FAIL\n"); return 1; } }
    my_free(v._M_start);
    std::printf("009b4e20_TEST PASS\n");
    return 0;
}