// behaviour test for 0x0064E952 NPlayerGui::CDrawExperience::CDrawExperience: the base is built first from (gui, list), then the handle vector is resized to 3
#include <cstdio>
static int gOrder[4]; static int gCount = 0; static unsigned int gSize = 0;
struct Gui { int id; };
struct Handles { unsigned int size; void resize(unsigned int n) { gOrder[gCount++] = 2; size = n; gSize = n; } };
struct Base {
    Gui* gui; void* list; Handles PrimitiveHandles;
    Base(Gui& g, void* l) : gui(&g), list(l) { gOrder[gCount++] = 1; PrimitiveHandles.size = 0; }
};
struct Derived : Base {
    Derived(Gui& g, void* l) : Base(g, l) { PrimitiveHandles.resize(3); }      // the function under test
};
int main()
{
    int fail = 0;
    Gui g; g.id = 7; int list = 0;
    Derived d(g, &list);
    if (gCount != 2 || gOrder[0] != 1 || gOrder[1] != 2) fail |= 1;
    if (gSize != 3 || d.PrimitiveHandles.size != 3 || d.gui != &g || d.list != &list) fail |= 2;
    if (fail) { std::printf("0064e952_TEST FAIL %d\n", fail); return 1; }
    std::printf("0064e952_TEST PASS\n");
    return 0;
}