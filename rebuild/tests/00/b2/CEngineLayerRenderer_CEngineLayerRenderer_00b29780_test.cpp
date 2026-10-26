// behaviour test for 0x00B29780 CEngineLayerRenderer::~CEngineLayerRenderer(): destroying the object releases Components's storage exactly once and then runs the base destructor (derived first, base last)
#include <cstdio>
#include <vector>
static int gOrder = 0, gBaseOrder = 0, gReleased = 0, gReleaseOrder = 0;
struct Base { virtual ~Base() { gBaseOrder = ++gOrder; } };
struct Storage {
    std::vector<int> items;
    ~Storage() { if (!items.empty()) { gReleased++; gReleaseOrder = ++gOrder; } }
};
struct Object : Base {
    Storage Components;
    virtual ~Object() { }                                   // the function under test (member then base)
};
int main()
{
    int fail = 0;
    Base* p = new Object;
    static_cast<Object*>(p)->Components.items.push_back(1);
    delete p;
    if (gReleased != 1) fail |= 1;
    if (gReleaseOrder != 1 || gBaseOrder != 2) fail |= 2;
    if (fail) { std::printf("00b29780_TEST FAIL %d\n", fail); return 1; }
    std::printf("00b29780_TEST PASS\n");
    return 0;
}