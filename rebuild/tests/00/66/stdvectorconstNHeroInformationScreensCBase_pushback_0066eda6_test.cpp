#include <cstdio>
#include <new>
namespace NHeroInformationScreens { class CBase; }
namespace std {
struct __true_type {};
template <class T> class vector {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    T* _M_end_of_storage;                                // +0x08
    void _M_insert_overflow(T* pos, const T& x, const __true_type&, unsigned int fill_len, bool atend);  // 0x0066ED1D
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else {
            __true_type is_pod;
            _M_insert_overflow(_M_finish, x, is_pod, 1, true);
        }
    }
};
}
template class std::vector<const NHeroInformationScreens::CBase*>;

namespace NHeroInformationScreens { class CBase { public: int v; }; }
typedef const NHeroInformationScreens::CBase* P;
static int g_over; static P* g_pos; static P g_x; static unsigned g_fill; static bool g_atend;
template <> void std::vector<P>::_M_insert_overflow(P* pos, const P& x, const std::__true_type&, unsigned int fill_len, bool atend)
{ ++g_over; g_pos = pos; g_x = x; g_fill = fill_len; g_atend = atend; }
int main()
{
    NHeroInformationScreens::CBase a, b;
    P buf[2] = { 0, 0 };
    std::vector<P> v; v._M_start = buf; v._M_finish = buf; v._M_end_of_storage = buf + 2;
    v.push_back(&a);
    bool ok = v._M_finish == buf + 1 && buf[0] == &a && g_over == 0;
    v.push_back(&b);
    ok = ok && v._M_finish == buf + 2 && buf[1] == &b && g_over == 0;
    v.push_back(&a);
    ok = ok && g_over == 1 && g_pos == buf + 2 && g_x == &a && g_fill == 1 && g_atend && v._M_finish == buf + 2;
    std::printf(ok ? "0066eda6_TEST PASS\n" : "FAIL\n");
    return 0;
}