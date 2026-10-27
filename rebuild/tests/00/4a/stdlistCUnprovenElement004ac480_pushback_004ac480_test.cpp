// behaviour test for 0x004AC480 list<CUnprovenElement_004ac480*>::push_back (model: appended at the back)
#include <cstdio>
#include <list>
int main()
{
    std::list<int> l; l.push_back(1); l.push_back(2); l.push_back(3);
    int ok = l.size() == 3 && l.front() == 1 && l.back() == 3;
    if (!ok) { std::printf("004ac480_TEST FAIL\n"); return 1; }
    std::printf("004ac480_TEST PASS\n");
    return 0;
}