// behaviour test for 0x00655ECE: push_front puts the new element first
#include <cstdio>
#include <list>
int main()
{
    std::list<long> l; l.push_back(2); l.push_front(1); l.push_front(0);
    if (l.size() != 3 || l.front() != 0 || *(++l.begin()) != 1 || l.back() != 2) { std::printf("00655ece_TEST FAIL\n"); return 1; }
    std::printf("00655ece_TEST PASS\n");
    return 0;
}