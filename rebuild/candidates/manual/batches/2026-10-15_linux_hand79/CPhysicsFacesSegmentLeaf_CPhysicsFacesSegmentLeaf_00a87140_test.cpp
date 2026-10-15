// Behaviour test for CPhysicsFacesSegmentLeaf::~CPhysicsFacesSegmentLeaf @ 0x00A87140: the triangle
// index buffer is freed exactly once, a null buffer is skipped.
#include <cstdio>

static void* g_freed[4];
static int g_freeCount = 0;
static void FreeStub(void* p) { if (g_freeCount < 4) g_freed[g_freeCount] = p; ++g_freeCount; }

template<class T>
struct CArray {
    T* m_first;
    T* m_last;
    T* m_end;
    ~CArray() { if (m_first) FreeStub(m_first); }
};

class CIPhysicsFacesSegmentBase {
public:
    virtual bool HasChildren() const = 0;
    virtual ~CIPhysicsFacesSegmentBase() {}
};

class CPhysicsFacesSegmentLeaf : public CIPhysicsFacesSegmentBase {
public:
    virtual bool HasChildren() const { return false; }
    virtual ~CPhysicsFacesSegmentLeaf() {}
    CArray<unsigned short> TriangleListIndex;
};

int main()
{
    static unsigned short indices[6];
    bool ok = true;
    {
        CPhysicsFacesSegmentLeaf leaf;
        leaf.TriangleListIndex.m_first = indices;
        leaf.TriangleListIndex.m_last = indices + 6;
        leaf.TriangleListIndex.m_end = indices + 6;
        if (leaf.HasChildren()) ok = false;
    }
    if (g_freeCount != 1 || g_freed[0] != indices) ok = false;
    {
        CIPhysicsFacesSegmentBase* p = 0;
        CPhysicsFacesSegmentLeaf* leaf = new CPhysicsFacesSegmentLeaf;
        leaf->TriangleListIndex.m_first = 0;
        leaf->TriangleListIndex.m_last = 0;
        leaf->TriangleListIndex.m_end = 0;
        p = leaf;
        delete p;                                // virtual destructor reaches the leaf
    }
    if (g_freeCount != 1) ok = false;            // null buffer: no free
    if (!ok) { std::printf("00a87140_TEST FAIL\n"); return 1; }
    std::printf("00a87140_TEST PASS\n");
    return 0;
}
