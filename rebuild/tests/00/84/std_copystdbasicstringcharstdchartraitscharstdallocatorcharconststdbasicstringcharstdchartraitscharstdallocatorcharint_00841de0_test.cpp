#include <cstdio>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif

struct random_access_iterator_tag { };
struct __false_type { };
static int g_dispatch = 0;
struct CRetailString {
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;
    CRetailString() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    CRetailString& _M_assign_dispatch(const char* first, const char* last, const __false_type&)
    {
        ++g_dispatch;
        _M_start = (char*)first;
        _M_finish = (char*)last;
        return *this;
    }
    CRetailString& operator=(const CRetailString& other)
    {
        if (&other != this)
            _M_assign_dispatch(other._M_start, other._M_finish, __false_type());
        return *this;
    }
};
static char g_chars[3][8] = { "one", "two", "three" };

typedef CRetailString T_;
enum { N = 3 };

static T_* Copy(const T_* first, const T_* last, T_* result, const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

int main()
{
    bool ok = true;
    static T_ src[N];
    static T_ dst[N];
    for (int i = 0; i < N; ++i) {
        T_* s = &src[i];
        T_* d = &dst[i];
        s->_M_start = g_chars[i]; s->_M_finish = g_chars[i] + 3; d->_M_end_of_storage = g_chars[0];
    }
    random_access_iterator_tag tag;
    T_* r = Copy(src, src + N, dst, tag, (int*)0);
    if (r != dst + N) ok = false;
    for (int i = 0; i < N; ++i) {
        const T_* s = &src[i];
        const T_* d = &dst[i];
        if (d->_M_start != g_chars[i] || d->_M_finish != g_chars[i] + 3 || d->_M_end_of_storage != g_chars[0]) ok = false;
    }
    if (g_dispatch != 3) ok = false;
    Copy(src, src + N, src, tag, (int*)0);              // self-assignment of every element: no dispatch
    if (g_dispatch != 3) ok = false;
    if (Copy(src, src, dst + 1, tag, (int*)0) != dst + 1) ok = false;
    if (!ok) { std::printf("00841de0_TEST FAIL\n"); return 1; }
    std::printf("00841de0_TEST PASS\n");
    return 0;
}