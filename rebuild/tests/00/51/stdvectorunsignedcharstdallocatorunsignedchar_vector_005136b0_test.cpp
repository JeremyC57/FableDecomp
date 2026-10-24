// Behaviour test for 0x005136B0 std::vector<unsigned char>(n): n zeroed bytes, finish == end of storage == start + n;
// n == 0 leaves every pointer null.
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct byte_vector_model {
    unsigned char* _M_start;
    unsigned char* _M_finish;
    unsigned char* _M_end_of_storage;
    explicit byte_vector_model(unsigned int n) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_start = n != 0 ? (unsigned char*)std::malloc(n) : 0;
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
        unsigned char zero = 0;
        if (n) std::memset(_M_start, zero, n);
        _M_finish = _M_start + n;
    }
};

int main()
{
    byte_vector_model v(13);
    if (v._M_start == 0 || v._M_finish != v._M_start + 13 || v._M_end_of_storage != v._M_start + 13) { std::printf("005136b0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 13; ++i)
        if (v._M_start[i] != 0) { std::printf("005136b0_TEST FAIL\n"); return 1; }
    std::free(v._M_start);
    byte_vector_model e(0);
    if (e._M_start != 0 || e._M_finish != 0 || e._M_end_of_storage != 0) { std::printf("005136b0_TEST FAIL\n"); return 1; }
    std::printf("005136b0_TEST PASS\n");
    return 0;
}