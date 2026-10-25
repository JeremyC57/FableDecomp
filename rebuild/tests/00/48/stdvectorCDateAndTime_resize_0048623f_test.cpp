// Behaviour test for std::vector<CDateAndTime>::resize(size_type) (0x0048623F): constructs one default CDateAndTime and
// hands it, with new_size, to resize(size_type, const CDateAndTime&).
#include <cstdio>
#include <cstring>

static char g_seq[32] = "";
static unsigned int g_size = 99;
static long g_year = -1;

class CDateAndTime {
public:
    long          Year;
    unsigned long Month, Day, Hour, Minute, Second, Millisecond;
    CDateAndTime();
};
CDateAndTime::CDateAndTime() : Year(0), Month(0), Day(0), Hour(0), Minute(0), Second(0), Millisecond(0)
{
    std::strcat(g_seq, "C");
}

namespace std {
template <class _Tp> class allocator;
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void resize(unsigned int __new_size, const _Tp& __x);
    void resize(unsigned int __new_size);
};
template <> void vector<CDateAndTime>::resize(unsigned int __new_size, const CDateAndTime& __x)
{
    std::strcat(g_seq, "R");
    g_size = __new_size;
    g_year = __x.Year;
}
}

int main()
{
    std::vector<CDateAndTime> v;
    v._M_start = v._M_finish = v._M_end_of_storage = 0;
    v.resize(5);
    if (std::strcmp(g_seq, "CR") != 0 || g_size != 5 || g_year != 0) { std::printf("0048623f_TEST FAIL\n"); return 1; }
    std::printf("0048623f_TEST PASS\n");
    return 0;
}
