// Behaviour test for 0x00530740 NUISystem::CComponent::IsPCStyle: reads PCStyle from the loaded def and releases the temporary def pointer (destroying the def on the last reference)
#include <cstdio>
static int g_destroyed = 0;
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void Slot0() { }
    virtual void Destroy() { ++g_destroyed; }
};
struct CUIDef : CDefVirtual, CRefCounted {
    bool PCStyle;
    void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
struct CDefPointer_CUIDef {
    const CUIDef* Data;
    explicit CDefPointer_CUIDef(const CUIDef* data) : Data(data) { }
    ~CDefPointer_CUIDef() { if (Data) const_cast<CUIDef*>(Data)->ReleaseTemporary(); }
    const CUIDef* operator->() const { return Data; }
};
static CUIDef g_def;
struct CComponent {
    virtual CDefPointer_CUIDef LoadAndReturnDef() const;
    virtual bool IsPCStyle() const;
    virtual ~CComponent() { }
};
CDefPointer_CUIDef CComponent::LoadAndReturnDef() const
{
    ++g_def.m_count;
    return CDefPointer_CUIDef(&g_def);
}
bool CComponent::IsPCStyle() const
{
    return LoadAndReturnDef()->PCStyle;
}
int main()
{
    CComponent c;
    g_def.m_count = 1; g_def.PCStyle = true;
    if (!c.IsPCStyle() || g_def.m_count != 1 || g_destroyed != 0) { std::printf("00530740_TEST FAIL\n"); return 1; }
    g_def.m_count = 0; g_def.PCStyle = false;
    if (c.IsPCStyle() || g_def.m_count != 0 || g_destroyed != 1) { std::printf("00530740_TEST FAIL\n"); return 1; }
    std::printf("00530740_TEST PASS\n");
    return 0;
}