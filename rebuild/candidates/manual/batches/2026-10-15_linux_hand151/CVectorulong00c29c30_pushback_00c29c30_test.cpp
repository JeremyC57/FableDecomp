// behaviour test for 0x00C29C30 vector<unsigned long>::push_back (Dinkumware): in place below capacity, else grow through _Insert_n
#include <cstdio>
#include <cstdlib>
static int g_grows = 0;
struct CVec {
    unsigned int* _Myfirst; unsigned int* _Mylast; unsigned int* _Myend;
    unsigned int size() const { return _Myfirst == 0 ? 0 : (unsigned int)(_Mylast - _Myfirst); }
    unsigned int capacity() const { return _Myfirst == 0 ? 0 : (unsigned int)(_Myend - _Myfirst); }
    void _Insert_n(unsigned int* where, unsigned int n, const unsigned int& v)
    {
        ++g_grows; unsigned int sz = size(), cap = sz * 2 + 1;
        unsigned int* p = (unsigned int*)std::malloc(cap * sizeof(unsigned int));
        for (unsigned int i = 0; i < sz; ++i) p[i] = _Myfirst[i];
        for (unsigned int j = 0; j < n; ++j) p[sz + j] = v;
        (void)where; std::free(_Myfirst); _Myfirst = p; _Mylast = p + sz + n; _Myend = p + cap;
    }
    void push_back(const unsigned int& v) { if (size() < capacity()) { *_Mylast = v; ++_Mylast; } else _Insert_n(_Mylast, 1, v); }
};
int main()
{
    CVec v = { 0, 0, 0 };
    for (unsigned int i = 0; i < 6; ++i) v.push_back(i * 3);
    bool ok = v.size() == 6 && v._Myfirst[5] == 15 && v._Myfirst[0] == 0 && g_grows == 3;
    std::free(v._Myfirst);
    if (!ok) { std::printf("00c29c30_TEST FAIL\n"); return 1; }
    std::printf("00c29c30_TEST PASS\n");
    return 0;
}
