// Behaviour test for vector<long>::vector(size_type n) (0x00D38F30): allocate n longs (none for n == 0) and
// value-initialise them all to zero; finish and end-of-storage both land at start + n.
#include <cstdio>
#include <cstdlib>

struct CLongVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    explicit CLongVector(unsigned int n)
    {
        _M_start = 0; _M_finish = 0; _M_end_of_storage = 0;
        _M_start = n ? (long*)std::malloc(n * sizeof(long)) : 0;
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
        long* p = _M_start;
        for (unsigned int k = n; k > 0; --k, ++p) *p = 0;
        _M_finish = _M_start + n;
    }
};

int main()
{
    CLongVector v(7);
    bool ok = v._M_start != 0 && v._M_finish == v._M_start + 7 && v._M_end_of_storage == v._M_start + 7;
    for (int i = 0; ok && i < 7; ++i) ok = v._M_start[i] == 0;
    CLongVector e(0);
    ok = ok && e._M_start == 0 && e._M_finish == 0 && e._M_end_of_storage == 0;
    std::free(v._M_start);
    if (!ok) {
        std::printf("00d38f30_TEST FAIL\n");
        return 1;
    }
    std::printf("00d38f30_TEST PASS\n");
    return 0;
}