#include <cstdio>
class CEnginePrimitiveHandle;
static int g_last = 0, g_destroy = 0, g_update = 0; static unsigned int g_updateFlags = 0;
class CPrim {
public:
    long RefCount; unsigned int RenderLayerMask;
    CPrim() : RefCount(0), RenderLayerMask(0) { }
    virtual ~CPrim() { }
    virtual void Destroy() { ++g_destroy; }
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle*) { ++g_last; }
    void Release() { if (--RefCount == 0) Destroy(); }
};
class CIVCountedPointer {
public:
    CPrim* Object;
    CIVCountedPointer() : Object(0) { }
    CPrim* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    CIVCountedPointer& operator=(int) { if (Object) { Object->Release(); Object = 0; } Object = 0; return *this; }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        CPrim* old = Object;
        if (old != other.Object) {
            if (old) { old->Release(); Object = 0; }
            Object = other.Object;
            if (Object) ++Object->RefCount;
        }
        return *this;
    }
};
class CEnginePrimitiveHandle {
public:
    CIVCountedPointer Data;
    void Clear() { if (Data.IsUnique()) Data->OnLastHandleReleased(this); Data = 0; }
};
struct CDesc { long ActiveIndex; long InitialSize; bool PreloadAll; CIVCountedPointer MeshEffect; };
struct CChanger : CPrim {
    CEnginePrimitiveHandle* m_pFirst; CEnginePrimitiveHandle* m_pLast; unsigned long m_reserved;
    CIVCountedPointer MeshEffect; long m_currentIndex; bool PreloadAll; unsigned int m_flags;
    unsigned long GetCount() const { return (unsigned long)(m_pLast - m_pFirst); }
    unsigned int ComputeRenderLayerMask()
    {
        unsigned int mask = 0;
        long index = m_currentIndex;
        if (index != -1 && (unsigned long)index < GetCount()) {
            CPrim* child = m_pFirst[index].Data.Object;
            if (child) mask = child->RenderLayerMask;
        }
        return mask;
    }
    void UpdateRenderLayerMask()
    {
        unsigned int mask = ComputeRenderLayerMask();
        if (RenderLayerMask != mask) { RenderLayerMask = mask; ++g_update; g_updateFlags = m_flags; }
    }
    void ClearChildPrimitive(unsigned long index)
    {
        if (index < GetCount()) m_pFirst[index].Clear();
        UpdateRenderLayerMask();
    }
    void Update(const CDesc& desc, unsigned int flags)
    {
        m_reserved = desc.InitialSize;
        MeshEffect = desc.MeshEffect;
        m_currentIndex = desc.ActiveIndex;
        PreloadAll = desc.PreloadAll;
        m_flags = flags;
        UpdateRenderLayerMask();
    }
};
int main()
{
    CPrim child, effect, oldEffect; child.RenderLayerMask = 0x10; effect.RefCount = 1; oldEffect.RefCount = 1;
    CEnginePrimitiveHandle h[2]; h[1].Data.Object = &child;
    CChanger c; c.m_pFirst = h; c.m_pLast = h + 2; c.m_currentIndex = -1; c.RenderLayerMask = 0; c.m_flags = 0;
    c.MeshEffect.Object = &oldEffect;
    CDesc d; d.ActiveIndex = 1; d.InitialSize = 5; d.PreloadAll = true; d.MeshEffect.Object = &effect;
    c.Update(d, 3);
    if (c.m_reserved != 5 || c.MeshEffect.Object != &effect || effect.RefCount != 2 || g_destroy != 1
        || c.m_currentIndex != 1 || !c.PreloadAll || c.m_flags != 3 || c.RenderLayerMask != 0x10 || g_update != 1 || g_updateFlags != 3) {
        std::printf("00bc5ae0_TEST FAIL\n"); return 1;
    }
    std::printf("00bc5ae0_TEST PASS\n");
    return 0;
}
