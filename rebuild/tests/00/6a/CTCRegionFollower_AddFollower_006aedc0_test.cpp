#include <cstdio>
#include <list>
// Behaviour model: each call appends one pointer to the thing at the back of the list, in call order.
struct Thing { int id; };
struct Comp { std::list<Thing*> l; void Add(Thing& t) { l.push_back(&t); } };
int main()
{
    Thing a = { 1 }, b = { 2 };
    Comp c;
    c.Add(a); c.Add(b); c.Add(a);
    if (c.l.size() != 3 || c.l.front() != &a || *(++c.l.begin()) != &b || c.l.back() != &a) { std::printf("006aedc0_TEST FAIL\n"); return 1; }
    std::printf("006aedc0_TEST PASS\n");
    return 0;
}