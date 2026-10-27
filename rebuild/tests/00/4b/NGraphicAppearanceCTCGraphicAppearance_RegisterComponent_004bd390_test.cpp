// behaviour test for 0x004BD390: registering appends the pointers to the list in order
#include <cstdio>
#include <list>
struct C { std::list<int*> l; void Add(int& x) { l.push_back(&x); } };
int main()
{
    int a = 1, b = 2; C c; c.Add(a); c.Add(b);
    int ok = c.l.size() == 2 && c.l.front() == &a && c.l.back() == &b;
    if (!ok) { std::printf("004bd390_TEST FAIL\n"); return 1; }
    std::printf("004bd390_TEST PASS\n");
    return 0;
}