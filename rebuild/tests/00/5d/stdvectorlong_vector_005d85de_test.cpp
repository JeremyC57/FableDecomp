// behaviour test for 005d85de: STLport vector<long> copy constructor (sized base + memmove copy)
#include <cstdio>
#include <cstdlib>
#include <cstring>
static void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result : (char*)std::memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}
struct VectorBase {
    int* _M_start; int* _M_finish; int* _M_end_of_storage;
    VectorBase(unsigned n) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_start = n ? (int*)std::malloc(n * sizeof(int)) : 0;
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
    }
};
struct Vector : VectorBase {
    Vector(unsigned n) : VectorBase(n) {}
    unsigned size() const { return unsigned(_M_finish - _M_start); }
    Vector(const Vector& x) : VectorBase(x.size())
    {
        _M_finish = (int*)CopyTrivial(x._M_start, x._M_finish, _M_start);
    }
};
int main()
{
    int bad = 0;
    Vector a(5);
    for (int i = 0; i < 5; ++i) *a._M_finish++ = 10 * i - 3;
    Vector b(a);
    if (b.size() != 5 || b._M_end_of_storage != b._M_start + 5 || b._M_start == a._M_start) ++bad;
    for (int i = 0; i < 5 && !bad; ++i) if (b._M_start[i] != 10 * i - 3) ++bad;
    Vector e(0);
    Vector f(e);
    if (f.size() != 0 || f._M_start != 0 || f._M_finish != 0) ++bad;
    std::free(a._M_start); std::free(b._M_start);
    if (bad) { std::printf("005d85de_TEST FAIL\n"); return 1; }
    std::printf("005d85de_TEST PASS\n");
    return 0;
}