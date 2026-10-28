// Behaviour test for 0x00A6D550 CPhysicsMesh::~CPhysicsMesh: every member is destroyed in reverse order, the root
// segment's counted pointer released, the helper / dummy / containment-volume maps' keys destroyed
#include <cstdio>
#include <cstdlib>

static int g_strings = 0, g_deleted = 0, g_order = 0, g_rootOrder = 0, g_mapOrder = 0;
struct CharString { CharString() { ++g_strings; } ~CharString() { --g_strings; } };
struct Info { unsigned int RefCount; void (*DeleteFunc)(void*); void* Data; };
static void DeleteSegment(void*) { ++g_deleted; }
struct Counted {
    void* Data; Info* Info_;
    ~Counted() { g_rootOrder = ++g_order; if (Info_ && --Info_->RefCount == 0) { Info_->DeleteFunc(Info_->Data); delete Info_; } }
};
struct HelperPoint { CharString Key; float X, Y, Z; long Bone; };
struct Map {                                     // CVectorMap<CCharString, ...>: a vector of pairs
    HelperPoint* s; HelperPoint* f;
    Map(int n) { s = new HelperPoint[n]; f = s + n; }
    ~Map() { g_mapOrder = ++g_order; delete [] s; }
};
struct Base { virtual ~Base() { } long IVRefCount; };
struct PhysicsMesh : Base {                      // the members the destructor visits, in declaration order
    unsigned long ReferenceCount;
    Map HelperPoints;
    Map DummyObjects;
    Counted RootSegment;
    PhysicsMesh(Info* info) : HelperPoints(3), DummyObjects(2) { RootSegment.Data = 0; RootSegment.Info_ = info; }
    virtual ~PhysicsMesh() { }                   // the row: an empty user destructor
};

int main()
{
    int ok = 1;
    Info* shared = new Info; shared->RefCount = 2; shared->DeleteFunc = DeleteSegment; shared->Data = 0;
    {
        PhysicsMesh a(shared);
        ok &= g_strings == 5;
    }
    ok &= g_strings == 0 && g_deleted == 0 && shared->RefCount == 1;
    ok &= g_rootOrder < g_mapOrder;                                    // reverse declaration order
    Base* b = new PhysicsMesh(shared);
    delete b;                                                          // virtual: the deleting destructor reaches it
    ok &= g_deleted == 1 && g_strings == 0;
    if (ok) std::printf("00a6d550_TEST PASS\n"); else std::printf("00a6d550_TEST FAIL\n");
    return ok ? 0 : 1;
}