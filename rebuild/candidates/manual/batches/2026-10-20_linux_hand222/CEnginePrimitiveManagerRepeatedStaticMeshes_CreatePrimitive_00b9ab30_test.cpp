// Behaviour test for 0x00B9AB30 CEnginePrimitiveManagerRepeatedStaticMeshes::CreatePrimitive (model copy).
#include <cstdio>

static int g_destroyed = 0, g_lastHandle = 0, g_added = 0;
struct CPrim {
    long IVRefCount; unsigned char NonPersistent; float sphere[4];
    CPrim() : IVRefCount(0), NonPersistent(0) { for (int i = 0; i < 4; ++i) sphere[i] = 0.0f; }
    void Destroy() { ++g_destroyed; }
    void OnLastHandleReleased() { ++g_lastHandle; }
};
struct CHandle {
    CPrim* Object;
    CHandle(CPrim* p) : Object(p) {}
    CHandle(const CHandle& o) : Object(o.Object) { if (Object) ++Object->IVRefCount; }
    ~CHandle()
    {
        if (Object && Object->IVRefCount == 1) Object->OnLastHandleReleased();
        if (Object) { if (--Object->IVRefCount == 0) Object->Destroy(); }
        Object = 0;
    }
};
struct CDesc { bool Persistent; };
struct CScene { void AddPrimitive(CPrim*, unsigned long f) { if (f == 1) ++g_added; } };
struct CMgr {
    CScene* scene;
    void Update(const CHandle& h, const CDesc&) { ++h.Object->IVRefCount; }   // the update takes the first reference
    CHandle CreatePrimitive(const CDesc& desc)
    {
        CPrim* primitive = new CPrim;
        CHandle handle(primitive);
        Update(handle, desc);
        primitive->NonPersistent = !desc.Persistent;
        scene->AddPrimitive(primitive, 1);
        return handle;
    }
};

int main()
{
    CScene scene; CMgr mgr; mgr.scene = &scene;
    CDesc d; d.Persistent = false;
    CPrim* p;
    {
        CHandle h = mgr.CreatePrimitive(d);
        p = h.Object;
        if (!p || p->NonPersistent != 1 || p->IVRefCount != 1 || g_added != 1 || g_destroyed != 0)
        { std::printf("00b9ab30_TEST FAIL\n"); return 1; }
    }
    if (g_destroyed != 1 || g_lastHandle < 1) { std::printf("00b9ab30_TEST FAIL\n"); return 1; }
    delete p;
    d.Persistent = true;
    { CHandle h = mgr.CreatePrimitive(d); p = h.Object; if (p->NonPersistent != 0) { std::printf("00b9ab30_TEST FAIL\n"); return 1; } }
    delete p;
    std::printf("00b9ab30_TEST PASS\n");
    return 0;
}
