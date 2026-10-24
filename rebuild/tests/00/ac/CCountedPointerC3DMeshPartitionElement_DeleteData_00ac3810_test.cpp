// Behaviour test for CCountedPointer<C3DMeshPartitionElement>::DeleteData(void*) @ 0x00AC3810
// Deletes the element; its destructor deletes both sub-elements (recursively) and clears the triangle range.
#include <cstdio>

static int gDestroyed = 0;

struct CPartitionElementModel {
    int StartTriangle;
    int TriangleCount;
    CPartitionElementModel* SubElements[2];
    CPartitionElementModel(CPartitionElementModel* a, CPartitionElementModel* b) : StartTriangle(4), TriangleCount(9)
    {
        SubElements[0] = a;
        SubElements[1] = b;
    }
    ~CPartitionElementModel()
    {
        delete SubElements[0];
        delete SubElements[1];
        StartTriangle = 0;
        TriangleCount = 0;
        ++gDestroyed;
    }
};

static void DeleteData(void* data)
{
    delete (CPartitionElementModel*)data;
}

int main()
{
    CPartitionElementModel* leafA = new CPartitionElementModel(0, 0);
    CPartitionElementModel* leafB = new CPartitionElementModel(0, 0);
    CPartitionElementModel* inner = new CPartitionElementModel(leafA, 0);
    CPartitionElementModel* root = new CPartitionElementModel(inner, leafB);
    DeleteData(root);
    int ok = gDestroyed == 4;
    DeleteData(0);                     // deleting null is a no-op
    if (gDestroyed != 4) ok = 0;
    if (!ok) { std::printf("00ac3810_TEST FAIL\n"); return 1; }
    std::printf("00ac3810_TEST PASS\n");
    return 0;
}