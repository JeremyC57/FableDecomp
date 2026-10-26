// behaviour test for 0x00ABC680 C3DMeshFileGroupChunk::~C3DMeshFileGroupChunk(): destroying the object releases VertexList's storage exactly once and then runs the base destructor (derived first, base last)
#include <cstdio>
#include <vector>
static int gOrder = 0, gBaseOrder = 0, gReleased = 0, gReleaseOrder = 0;
struct Base { virtual ~Base() { gBaseOrder = ++gOrder; } };
struct Storage {
    std::vector<int> items;
    ~Storage() { if (!items.empty()) { gReleased++; gReleaseOrder = ++gOrder; } }
};
struct Object : Base {
    Storage VertexList;
    virtual ~Object() { }                                   // the function under test (member then base)
};
int main()
{
    int fail = 0;
    Base* p = new Object;
    static_cast<Object*>(p)->VertexList.items.push_back(1);
    delete p;
    if (gReleased != 1) fail |= 1;
    if (gReleaseOrder != 1 || gBaseOrder != 2) fail |= 2;
    if (fail) { std::printf("00abc680_TEST FAIL %d\n", fail); return 1; }
    std::printf("00abc680_TEST PASS\n");
    return 0;
}