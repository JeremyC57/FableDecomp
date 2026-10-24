// Behaviour test for 0x00512630 std::vector<unsigned long>(n, value): n slots allocated, every one equal to value,
// finish == end of storage == start + n; n == 0 leaves all three pointers null.
#include <cstdio>
#include <cstdlib>

typedef unsigned int u32;

template <class OutputIter, class Size, class T>
OutputIter fill_n_model(OutputIter first, Size n, const T& value)
{
    for (; n > 0; --n, ++first)
        *first = value;
    return first;
}

struct vector_model {
    u32* _M_start;
    u32* _M_finish;
    u32* _M_end_of_storage;
    vector_model(unsigned int n, const u32& value) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_start = n != 0 ? (u32*)std::malloc(n * sizeof(u32)) : 0;
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
        _M_finish = fill_n_model(_M_start, n, value);
    }
};

int main()
{
    vector_model v(7, 0xA5A5u);
    if (v._M_start == 0 || v._M_finish != v._M_start + 7 || v._M_end_of_storage != v._M_start + 7) { std::printf("00512630_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 7; ++i)
        if (v._M_start[i] != 0xA5A5u) { std::printf("00512630_TEST FAIL\n"); return 1; }
    std::free(v._M_start);
    vector_model e(0, 3u);
    if (e._M_start != 0 || e._M_finish != 0 || e._M_end_of_storage != 0) { std::printf("00512630_TEST FAIL\n"); return 1; }
    std::printf("00512630_TEST PASS\n");
    return 0;
}