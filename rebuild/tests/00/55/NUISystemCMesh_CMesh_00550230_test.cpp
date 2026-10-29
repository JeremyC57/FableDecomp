// Behaviour test for NUISystem::CMesh::~CMesh @ 0x00550230: the mesh clears its engine primitive handle (the primitive is told it
// was released when this handle held the last reference, the reference is dropped and the primitive deleted at zero; a shared
// primitive only loses one reference), then the CChangingStateComponent base destructor runs.
#include <stdio.h>
static int g_notified = 0, g_deleted = 0, g_baseDtor = 0;
class CBaseClass { public: virtual ~CBaseClass() { } };
class CEnginePrimitiveHandle;
class CIVCountedPointeeBase { public:
    virtual ~CIVCountedPointeeBase() { }
    virtual void Delete() const { g_deleted++; }
    mutable unsigned int IVRefCount;
};
class CEnginePersistentPrimitiveBase : public CIVCountedPointeeBase { public:
    virtual void NotifyReleased(const CEnginePrimitiveHandle&) { g_notified++; }
};
template <class T> class CIVCountedPointer { public:
    CIVCountedPointeeBase* Object;
    T* operator->() const { return static_cast<T*>(Object); }
    unsigned int GetRefCount() const { return Object ? Object->IVRefCount : 0; }
    void Release()
    {
        CIVCountedPointeeBase* object = Object;
        if (object) {
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    void Reset(T* object) { Release(); Object = object; if (object) ++object->IVRefCount; }
    ~CIVCountedPointer() { Release(); }
};
class CEnginePrimitiveHandle : public CBaseClass { public:
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;
    unsigned int GetReferenceCount() const { return Data.GetRefCount(); }
    void Clear()
    {
        if (GetReferenceCount() == 1)
            Data->NotifyReleased(*this);
        Data.Reset(0);
    }
    virtual ~CEnginePrimitiveHandle() { Clear(); }
};
namespace NUISystem {
class CChangingStateComponent { public: virtual ~CChangingStateComponent() { g_baseDtor++; } };
class CMesh : public CChangingStateComponent { public:
    unsigned int           MeshIndex;
    CEnginePrimitiveHandle Handle;
    virtual ~CMesh();
};
CMesh::~CMesh()
{
    Handle.Clear();
}
}
static int fail(int line) { printf("00550230_TEST FAIL\nline %d\n", line); return 1; }
int main()
{
    CEnginePersistentPrimitiveBase prim; prim.IVRefCount = 0;
    NUISystem::CMesh* mesh = new NUISystem::CMesh;
    mesh->Handle.Data.Object = 0; mesh->Handle.Data.Reset(&prim);
    NUISystem::CChangingStateComponent* base = mesh;
    delete base;
    if (g_notified != 1 || g_deleted != 1 || prim.IVRefCount != 0 || g_baseDtor != 1) return fail(__LINE__);
    prim.IVRefCount = 1;                                    // shared: another handle still holds it
    NUISystem::CMesh* mesh2 = new NUISystem::CMesh;
    mesh2->Handle.Data.Object = 0; mesh2->Handle.Data.Reset(&prim);
    delete mesh2;
    if (g_notified != 1 || g_deleted != 1 || prim.IVRefCount != 1 || g_baseDtor != 2) return fail(__LINE__);
    NUISystem::CMesh* mesh3 = new NUISystem::CMesh;
    mesh3->Handle.Data.Object = 0;                          // empty handle
    delete mesh3;
    if (g_notified != 1 || g_deleted != 1 || g_baseDtor != 3) return fail(__LINE__);
    printf("00550230_TEST PASS\n");
    return 0;
}