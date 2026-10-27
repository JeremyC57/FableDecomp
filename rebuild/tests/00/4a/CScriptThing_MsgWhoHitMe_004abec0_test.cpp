// behaviour test for 0x004ABEC0: a handle without implementation returns a copy of itself (sharing its count), else forwards
#include <cstdio>
struct Info { long refs; };
struct H { H* data; Info* info; int answer;
    H(const H& o) : data(o.data), info(o.info), answer(o.answer) { if (info) ++info->refs; }
    H(H* d, Info* i, int a) : data(d), info(i), answer(a) { }
    virtual H Who() const { H* impl = data; if (impl == 0) return *this; return impl->Who(); } };
int main()
{
    Info i = { 1 }; H impl(0, 0, 7); H handle(&impl, &i, 1); H null(0, &i, 3);
    H a = handle.Who(); H b = null.Who();
    if (a.answer != 7 || b.answer != 3 || b.info != &i || i.refs != 2) { std::printf("004abec0_TEST FAIL\n"); return 1; }
    std::printf("004abec0_TEST PASS\n");
    return 0;
}