#include <stdio.h>
// Behaviour test for CEngineInternalPrimitiveGroup::SetChildPrimitive (0x00BC65D0): grows the handle vector,
// creates the child through the type's manager, releases the old child, updates and enables the new one.
class CEnginePrimitive { public: long Type; };
class CEnginePrimitiveHandle;
static int g_destroyed = 0, g_lastReleased = 0, g_updates = 0, g_layer = 0, g_resize = -1;
class CEngineInternalPrimitiveBase { public:
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    virtual void v02() {}
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle*) { ++g_lastReleased; }
    virtual void v04() {}
    virtual void Update(const CEnginePrimitive& p, unsigned long f) { ++g_updates; lastType = p.Type; lastFlags = f; }
    long RefCount;
    unsigned char _pad08[0x34 - 8];
    unsigned char Enable : 1;
    unsigned char NonPersistent : 1;
    unsigned char OverrideFadeDistance : 1;
    unsigned char PostRenderEnable : 1;
    long lastType; unsigned long lastFlags;
    void Release() { if (--RefCount == 0) Destroy(); }
};
class CIVCountedPointer {
public:
    CEngineInternalPrimitiveBase* Object;
    CEngineInternalPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    CIVCountedPointer& operator=(CEngineInternalPrimitiveBase* p)
    { if (Object) { Object->Release(); Object = 0; } Object = p; return *this; }
};
class CEnginePrimitiveHandle { public:
    virtual void v00() {}
    CIVCountedPointer Data;
    void Clear() { if (Data.IsUnique()) Data->OnLastHandleReleased(this); Data = 0; }
};
static CEnginePrimitiveHandle g_store[8];
struct CHandleVector {
    CEnginePrimitiveHandle* m_pFirst; CEnginePrimitiveHandle* m_pLast; CEnginePrimitiveHandle* m_pEnd;
    unsigned long size() const { return m_pLast - m_pFirst; }
    void Resize(long count) { g_resize = count; m_pLast = m_pFirst + count; }
    CEnginePrimitiveHandle& operator[](unsigned long index) { return m_pFirst[index]; }
};
static CEngineInternalPrimitiveBase g_child;
struct CEnginePrimitiveManagerBase {
    virtual void v00() {}
    virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type) { created = type; g_child.RefCount = 1; return &g_child; }
    long created;
};
struct CEnginePrimitiveManagerTable { unsigned char _pad[0x10]; CEnginePrimitiveManagerBase* Managers[4]; };
static CEnginePrimitiveManagerTable g_table;
CEnginePrimitiveManagerTable* g_pPrimitiveManagers = &g_table;
struct CEngineInternalPrimitiveGroup : CEngineInternalPrimitiveBase {
    unsigned char _pad38[0x50 - 0x38 - 8];
    CHandleVector Primitives;
    unsigned long PrimitiveFlags;
    void UpdateRenderLayerMask() { ++g_layer; }
    void SetChildPrimitive(unsigned long index, const CEnginePrimitive& primitive, unsigned long flags);
};
void CEngineInternalPrimitiveGroup::SetChildPrimitive(unsigned long index, const CEnginePrimitive& primitive, unsigned long flags)
{
    if (index >= Primitives.size())
        Primitives.Resize(index + 1);
    CEnginePrimitiveHandle& handle = Primitives[index];
    CEnginePrimitiveManagerBase* manager = g_pPrimitiveManagers->Managers[primitive.Type];
    bool enable = (flags & 0x20) ? false : true;
    if (manager) {
        CEngineInternalPrimitiveBase* child = manager->CreateInternalPrimitive(primitive.Type);
        handle.Clear();
        handle.Data = child;
        child->Update(primitive, flags);
        child->Enable = enable;
        UpdateRenderLayerMask();
    }
}
int main()
{
    int ok = 1;
    CEngineInternalPrimitiveGroup g;
    g.Primitives.m_pFirst = g_store; g.Primitives.m_pLast = g_store + 1; g.Primitives.m_pEnd = g_store + 8;
    CEngineInternalPrimitiveBase old; old.RefCount = 1;
    g_store[2].Data.Object = 0;
    CEnginePrimitiveManagerBase mgr; g_table.Managers[2] = &mgr; g_table.Managers[3] = 0;
    CEnginePrimitive p; p.Type = 2;
    g_store[2].Data.Object = &old;
    g.SetChildPrimitive(2, p, 0x11);
    if (g_resize != 3 || mgr.created != 2 || g_lastReleased != 1 || g_destroyed != 1) ok = 0;
    if (g_store[2].Data.Object != &g_child || g_updates != 1 || g_child.lastFlags != 0x11 || !g_child.Enable || g_layer != 1) ok = 0;
    g_resize = -1;
    g_child.RefCount = 2;
    g.SetChildPrimitive(1, p, 0x20);
    if (g_resize != -1 || g_child.Enable || g_store[1].Data.Object != &g_child || g_layer != 2) ok = 0;
    p.Type = 3;
    g.SetChildPrimitive(0, p, 0);
    if (g_layer != 2 || g_updates != 2) ok = 0;
    if (ok) { printf("00bc65d0_TEST PASS\n"); return 0; }
    printf("00bc65d0_TEST FAIL\n"); return 1;
}