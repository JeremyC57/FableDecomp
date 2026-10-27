// behaviour test for 0x009C6A10: the range is inserted before pos in order
#include <cstdio>
#include <list>
int main()
{
    int a, b, c; std::list<int*> src; src.push_back(&a); src.push_back(&b);
    std::list<int*> dst; dst.push_back(&c);
    dst.insert(dst.begin(), src.begin(), src.end());
    std::list<int*>::iterator it = dst.begin();
    if (dst.size() != 3 || *it != &a || *++it != &b || *++it != &c) { std::printf("009c6a10_TEST FAIL\n"); return 1; }
    std::printf("009c6a10_TEST PASS\n");
    return 0;
}