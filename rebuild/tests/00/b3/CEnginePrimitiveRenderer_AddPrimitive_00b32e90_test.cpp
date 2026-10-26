// Behaviour test for 0x00B32E90 (see src/00b32e90.cpp): AddPrimitive with a recording SetPersistentPrimitive (the temporary handle takes a reference, then is dropped).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("00b32e90_TEST FAIL\n"); return 1; } } while (0)

static int g_lastReleased = 0, g_destroyed = 0, g_baseCtor = 0, g_baseDtor = 0;
class CEnginePrimitiveHandle;
class CEnginePersistentPrimitiveBase {
public:
    int RefCount;
    CEnginePersistentPrimitiveBase() : RefCount(0) { }
    virtual ~CEnginePersistentPrimitiveBase() { }
    virtual void Destroy() { ++g_destroyed; }
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle*) { ++g_lastReleased; }
    void DecRef() { --RefCount; }
    int GetRefCount() const { return RefCount; }
    void Release() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
class CIVCountedPointer {
public:
    CEnginePersistentPrimitiveBase* Object;
    CIVCountedPointer() : Object(0) { }
    CEnginePersistentPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    ~CIVCountedPointer() { if (Object) Object->Release(); Object = 0; }
};
class CBaseClass { public: CBaseClass() { ++g_baseCtor; } virtual ~CBaseClass() { ++g_baseDtor; } };
class CEnginePrimitiveHandle : public CBaseClass {
public:
    CIVCountedPointer Data;
    CEnginePrimitiveHandle() { }
    ~CEnginePrimitiveHandle() { if (Data.IsUnique()) Data->OnLastHandleReleased(this); }
};

class CEnginePrimitive { public: int type; };
static CEnginePersistentPrimitiveBase g_persistent;
static unsigned int g_flags = 0; static bool g_transient = false; static const CEnginePrimitive* g_prim = 0;
class CEnginePrimitiveRenderer { public:
    void SetPersistentPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, unsigned int flags, bool transient)
    {
        handle.Data.Object = &g_persistent; ++g_persistent.RefCount; g_flags = flags; g_transient = transient; g_prim = &primitive;
    }
    void AddPrimitive(const CEnginePrimitive& primitive)
    {
        CEnginePrimitiveHandle handle;
        SetPersistentPrimitive(handle, primitive, 4, true);
    }
};
int main()
{
    CEnginePrimitiveRenderer r; CEnginePrimitive p = { 3 };
    r.AddPrimitive(p);
    CHECK(g_flags == 4 && g_transient && g_prim == &p && g_baseCtor == 1 && g_baseDtor == 1);
    CHECK(g_lastReleased == 1 && g_destroyed == 1 && g_persistent.RefCount == 0);
    g_persistent.RefCount = 1;                     // someone else still holds it: no last-handle callback, no destroy
    r.AddPrimitive(p);
    CHECK(g_lastReleased == 1 && g_destroyed == 1 && g_persistent.RefCount == 1);
    std::printf("00b32e90_TEST PASS\n");
    return 0;
}