// Behaviour test for std::vector<unsigned short>::vector(size_type n) @ 0x00AADD80
#include <cstdio>
#include <cstdlib>

struct CUShortVector {
    unsigned short* _M_start;
    unsigned short* _M_finish;
    unsigned short* _M_end_of_storage;
    explicit CUShortVector(unsigned int n) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_start = n != 0 ? (unsigned short*)std::malloc(n * sizeof(unsigned short)) : 0;
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
        unsigned short* p = _M_start;
        unsigned short value = 0;
        for (unsigned int i = n; i > 0; --i, ++p)
            *p = value;
        _M_finish = p;
    }
};

int main()
{
    CUShortVector a(7);
    int ok = a._M_start != 0 && a._M_finish == a._M_start + 7 && a._M_end_of_storage == a._M_start + 7;
    for (int i = 0; i < 7 && ok; ++i)
        if (a._M_start[i] != 0) ok = 0;
    CUShortVector b(0);
    if (b._M_start != 0 || b._M_finish != 0 || b._M_end_of_storage != 0) ok = 0;
    std::free(a._M_start);
    if (!ok) { std::printf("00aadd80_TEST FAIL\n"); return 1; }
    std::printf("00aadd80_TEST PASS\n");
    return 0;
}