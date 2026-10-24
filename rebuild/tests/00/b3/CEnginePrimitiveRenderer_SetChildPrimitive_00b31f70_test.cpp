#include <cstdio>

class CEnginePrimitive { public: int tag; };
class CEnginePrimitiveAnimation { public: int tag; };
class CEnginePrimitivePosition { public: int tag; };
class CEnginePrimitiveHandle;

static int g_calls = 0;
static int g_lastSlot = -1;
static unsigned long g_lastId = 0, g_lastFlags = 0;
static const void* g_lastArg = 0;
static const void* g_lastArg2 = 0;
static int g_updates = 0;
static unsigned long g_updateFlags = 0;

class CEnginePersistentPrimitiveBase {
public:
    CEnginePersistentPrimitiveBase() : RefCount(1), Type(0) {}
    virtual ~CEnginePersistentPrimitiveBase() {}
    long RefCount;
    long Type;
};

class CEngineInternalPrimitiveBase : public CEnginePersistentPrimitiveBase {
public:
    CEngineInternalPrimitiveBase() : EnginePrimitiveFlags(0), GridPos_Level(-1) {}
    virtual unsigned long AddChildPrimitive(const CEnginePrimitive& primitive, unsigned long flags)
    { ++g_calls; g_lastSlot = 6; g_lastArg = &primitive; g_lastFlags = flags; return 0x1234u + flags; }
    virtual void SetChildPrimitive(unsigned long id, const CEnginePrimitive& primitive, unsigned long flags)
    { ++g_calls; g_lastSlot = 8; g_lastId = id; g_lastArg = &primitive; g_lastFlags = flags; }
    virtual void SetChildPrimitive(unsigned long id, CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, unsigned long flags)
    { ++g_calls; g_lastSlot = 7; g_lastId = id; g_lastArg = &primitive; g_lastArg2 = &handle; g_lastFlags = flags; }
    virtual void GetChildPrimitiveHandle(unsigned long id, CEnginePrimitiveHandle& handle)
    { ++g_calls; g_lastSlot = 9; g_lastId = id; g_lastArg2 = &handle; }
    virtual void ClearChildPrimitive(unsigned long id)
    { ++g_calls; g_lastSlot = 10; g_lastId = id; }
    virtual void SetAnimation(const CEnginePrimitiveAnimation& animation)
    { ++g_calls; g_lastSlot = 23; g_lastArg = &animation; }
    virtual void SetPosition(const CEnginePrimitivePosition& position)
    { ++g_calls; g_lastSlot = 24; g_lastArg = &position; }
    unsigned long EnginePrimitiveFlags;
    signed char   GridPos_Level;
    bool IsInGrid() const { return GridPos_Level >= 0; }
};

struct CEngineScene {
    static void UpdatePosition(CEngineInternalPrimitiveBase& primitive, unsigned long flags)
    { ++g_updates; g_updateFlags = flags; (void)primitive; }
};

class CEnginePrimitiveHandle {
public:
    CEnginePrimitiveHandle() : Data(0) {}
    virtual ~CEnginePrimitiveHandle() {}
    CEnginePersistentPrimitiveBase* Data;
    CEngineInternalPrimitiveBase* GetInternalPrimitive() const { return static_cast<CEngineInternalPrimitiveBase*>(Data); }
};

static void Reset() { g_calls = 0; g_lastSlot = -1; g_lastId = 0; g_lastFlags = 0; g_lastArg = 0; g_lastArg2 = 0; g_updates = 0; g_updateFlags = 0; }

struct CRenderer {
    void SetChildPrimitive(CEnginePrimitiveHandle& handle, unsigned long id, const CEnginePrimitive& primitive, unsigned long flags)
    {
        CEngineInternalPrimitiveBase* internal = handle.GetInternalPrimitive();
        if (internal)
            internal->SetChildPrimitive(id, primitive, flags);
    }
};
int main()
{
    CRenderer r; CEnginePrimitiveHandle h; CEnginePrimitive p; CEngineInternalPrimitiveBase prim;
    Reset(); r.SetChildPrimitive(h, 6, p, 2);
    if (g_calls != 0) { std::printf("00b31f70_TEST FAIL\n"); return 1; }
    h.Data = &prim;
    Reset(); r.SetChildPrimitive(h, 6, p, 2);
    if (g_calls != 1 || g_lastSlot != 8 || g_lastId != 6 || g_lastArg != &p || g_lastFlags != 2) { std::printf("00b31f70_TEST FAIL\n"); return 1; }
    std::printf("00b31f70_TEST PASS\n");
    return 0;
}