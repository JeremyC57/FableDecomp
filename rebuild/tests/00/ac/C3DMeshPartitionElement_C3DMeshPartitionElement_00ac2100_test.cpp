#include <cstdio>

static int g_destroyed = 0;

class C3DVector { public: float X, Y, Z; };
class C3DBoundingBox { public: C3DVector Min, Max; };
class C3DBoundingSphere { public: C3DVector Center; float Radius; };

class C3DMeshPartitionElement { public:
    C3DBoundingBox BoundingBox; long StartTriangle; long TriangleCount; C3DMeshPartitionElement* SubElements[2];
    C3DBoundingSphere BoundingSphere;
    C3DMeshPartitionElement() : StartTriangle(5), TriangleCount(6) { SubElements[0] = 0; SubElements[1] = 0; }
    ~C3DMeshPartitionElement()
    {
        ++g_destroyed;
        delete SubElements[0];
        delete SubElements[1];
        StartTriangle = 0;
        TriangleCount = 0;
    }
};

int main()
{
    C3DMeshPartitionElement* root = new C3DMeshPartitionElement;
    root->SubElements[0] = new C3DMeshPartitionElement;
    root->SubElements[1] = new C3DMeshPartitionElement;
    root->SubElements[0]->SubElements[1] = new C3DMeshPartitionElement;
    root->~C3DMeshPartitionElement();               // the plain destructor: frees the whole subtree, clears the range
    if (g_destroyed != 4 || root->StartTriangle != 0 || root->TriangleCount != 0) { std::printf("00ac2100_TEST FAIL\n"); return 1; }
    operator delete(root);
    std::printf("00ac2100_TEST PASS\n");
    return 0;
}