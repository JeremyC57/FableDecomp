// behaviour test for 0x004DEBD4: a new component starts with an empty member vector
#include <cstdio>
#include <vector>
struct Base { void* thing; Base(void* t) : thing(t) { } };
struct C : Base { std::vector<int> v; C(void* t) : Base(t) { } };
int main()
{
    int thing = 0; C c(&thing);
    if (c.thing != &thing || !c.v.empty()) { std::printf("004debd4_TEST FAIL\n"); return 1; }
    std::printf("004debd4_TEST PASS\n");
    return 0;
}