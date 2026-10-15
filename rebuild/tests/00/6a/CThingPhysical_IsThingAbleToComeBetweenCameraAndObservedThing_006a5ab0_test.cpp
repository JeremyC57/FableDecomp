#include <cstdio>
static int g_destroyed = 0;
struct CThingPhysicalDef {
    virtual void v000() {}
    virtual void Destroy() { ++g_destroyed; }
    long          m_refCount;
    unsigned char _flags42_0 : 2;
    unsigned char AbleToComeBetweenCameraAndObservedThing : 1;
};
template <class T> struct CDefPointer {
    T* m_pDef;
    CDefPointer() : m_pDef(0) {}
    CDefPointer(const CDefPointer& other) : m_pDef(other.m_pDef) { if (m_pDef) ++m_pDef->m_refCount; }
    ~CDefPointer() { if (--m_pDef->m_refCount == 0) m_pDef->Destroy(); }
    T* operator->() const { return m_pDef; }
};
struct CThingPhysical {
    CDefPointer<CThingPhysicalDef>  PBaseDef;
    long                            CanComeBetweenCameraAndObservedThing;
    CDefPointer<CThingPhysicalDef> GetDef() const { return PBaseDef; }
    bool IsThingAbleToComeBetweenCameraAndObservedThing();
};
bool CThingPhysical::IsThingAbleToComeBetweenCameraAndObservedThing()
{
    long setting = CanComeBetweenCameraAndObservedThing;
    if (setting == -1)
        return GetDef()->AbleToComeBetweenCameraAndObservedThing;
    return setting == 1;
}
int main()
{
    int bad = 0;
    CThingPhysicalDef def; def.m_refCount = 1; def._flags42_0 = 3; def.AbleToComeBetweenCameraAndObservedThing = 1;
    CThingPhysical t; t.PBaseDef.m_pDef = &def;
    t.CanComeBetweenCameraAndObservedThing = 1;
    if (!t.IsThingAbleToComeBetweenCameraAndObservedThing()) bad = 1;
    t.CanComeBetweenCameraAndObservedThing = 0;
    if (t.IsThingAbleToComeBetweenCameraAndObservedThing()) bad = 2;
    t.CanComeBetweenCameraAndObservedThing = -1;
    if (!t.IsThingAbleToComeBetweenCameraAndObservedThing() || def.m_refCount != 1 || g_destroyed) bad = 3;
    def.AbleToComeBetweenCameraAndObservedThing = 0;
    if (t.IsThingAbleToComeBetweenCameraAndObservedThing() || def.m_refCount != 1) bad = 4;
    if (bad) { std::printf("006a5ab0_TEST FAIL\n"); return 1; }
    std::printf("006a5ab0_TEST PASS\n");
    return 0;
}