#include <cstdio>
struct SElement48 { unsigned char _body[0x30]; };
static SElement48* g_ef = 0; static SElement48* g_el = 0; static SElement48* g_iw = 0; static unsigned long g_ic = 99;
template<class T> class CArray { public:
    T* first; T* last; T* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    void Erase(T* f, T* l) { g_ef = f; g_el = l; }
    void Insert(T* where, unsigned long count, const T&) { g_iw = where; g_ic = count; }
    void resize(unsigned long n, const T& value) { if (n < size()) Erase(first + n, last); else Insert(last, n - size(), value); }
};
int main()
{
    SElement48 buf[8], v;
    CArray<SElement48> a; a.first = buf; a.last = buf + 5; a.end_of_storage = buf + 8;
    a.resize(2, v);
    if (g_ef != buf + 2 || g_el != buf + 5 || g_iw) { std::printf("004eda6c_TEST FAIL\n"); return 1; }
    a.resize(7, v);
    if (g_iw != buf + 5 || g_ic != 2) { std::printf("004eda6c_TEST FAIL\n"); return 1; }
    a.resize(5, v);
    if (g_ic != 0) { std::printf("004eda6c_TEST FAIL\n"); return 1; }
    std::printf("004eda6c_TEST PASS\n");
    return 0;
}