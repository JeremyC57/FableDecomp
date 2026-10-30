// Behaviour test for list<pair<long,long> >::list(const list&) (retail 0x0053CB20): the copy has its own sentinel and
// the source's elements in order.
#include <cstdio>
#include <list>
#include <utility>
int main()
{
    std::list<std::pair<long, long> > a;
    a.push_back(std::make_pair(1L, 2L)); a.push_back(std::make_pair(3L, 4L));
    std::list<std::pair<long, long> > b(a);
    a.front().first = 9;
    bool ok = b.size() == 2 && b.front().first == 1 && b.back().second == 4;
    std::list<std::pair<long, long> > e, f(e);
    ok = ok && f.empty();
    if (!ok) { std::printf("0053cb20_TEST FAIL\n"); return 1; }
    std::printf("0053cb20_TEST PASS\n");
    return 0;
}