// Behaviour test for 0x008FCA40 _STL::transform<..., CGetMeshObject>: one mesh object per thing, appended in order.
#include <cstdio>

struct Mesh { int RefCount; };
struct MeshObject { Mesh* Mesh_; float Scale; };
struct Physics { Mesh* mesh; float scale; MeshObject GetPhysicsMeshObject() const { MeshObject m; m.Mesh_ = mesh; m.Scale = scale; return m; } };
struct Thing { Physics* PhysicsTC; };

struct MeshList {
    MeshObject items[8];
    int count;
    void push_back(const MeshObject& m) { items[count] = m; if (m.Mesh_) ++m.Mesh_->RefCount; ++count; }
};

struct BackInserter {
    MeshList* container;
    BackInserter& operator=(const MeshObject& v) { container->push_back(v); return *this; }
    BackInserter& operator*() { return *this; }
    BackInserter& operator++() { return *this; }
};

struct CGetMeshObject { MeshObject operator()(const Thing* t) const { return t->PhysicsTC->GetPhysicsMeshObject(); } };

static BackInserter transform(const Thing* const* first, const Thing* const* last, BackInserter result, CGetMeshObject op)
{
    for ( ; first != last; ++first, ++result)
        *result = op(*first);
    return result;
}

int main()
{
    Mesh m1 = { 1 }, m2 = { 5 };
    Physics p1 = { &m1, 1.0f }, p2 = { &m2, 2.0f }, p3 = { 0, 3.0f };
    Thing a = { &p1 }, b = { &p2 }, c = { &p3 };
    const Thing* things[3] = { &a, &b, &c };
    MeshList list; list.count = 0;
    BackInserter it; it.container = &list;
    int fail = 0;
    BackInserter r = transform(things, things + 3, it, CGetMeshObject());
    if (r.container != &list || list.count != 3) fail = 1;
    if (list.items[0].Mesh_ != &m1 || list.items[1].Mesh_ != &m2 || list.items[2].Mesh_ != 0) fail = 1;
    if (m1.RefCount != 2 || m2.RefCount != 6) fail = 1;
    if (transform(things, things, it, CGetMeshObject()).container != &list || list.count != 3) fail = 1;
    if (fail) { std::printf("008fca40_TEST FAIL\n"); return 1; }
    std::printf("008fca40_TEST PASS\n");
    return 0;
}