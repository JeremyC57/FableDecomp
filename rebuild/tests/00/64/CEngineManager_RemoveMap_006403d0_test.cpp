// behaviour test for 0x006403D0: every engine in the list receives the map index
#include <cstdio>
#include <list>
struct Engine { long last; void F(long m) { last = m; } };
int main()
{
    Engine a = { 0 }, b = { 0 }; std::list<Engine*> l; l.push_back(&a); l.push_back(&b);
    for (std::list<Engine*>::iterator it = l.begin(); it != l.end(); ++it) (*it)->F(7);
    if (a.last != 7 || b.last != 7) { std::printf("006403d0_TEST FAIL\n"); return 1; }
    std::printf("006403d0_TEST PASS\n");
    return 0;
}