// Behaviour test for 0x00657550 NPlayerGui::CDrawQuickAccessIconsPC::ClearPrimitivesIfNecessary
#include <cstdio>
static int g_cleared = 0;
static int g_baseCalls = 0;
static int g_destroyed = 0;
static int g_lastNotified = 0;
struct CPrimitive {
    long m_refCount;
    void Destroy() { ++g_destroyed; }
    void OnLastHandleCleared() { ++g_lastNotified; }
};
class CEnginePrimitiveHandle { public:
    CPrimitive* m_p;
    void Clear()
    {
        if ((m_p ? m_p->m_refCount : 0) == 1)
            m_p->OnLastHandleCleared();
        CPrimitive* object = m_p;
        if (object) {
            if (--object->m_refCount == 0)
                object->Destroy();
            m_p = 0;
        }
        m_p = 0;
        ++g_cleared;
    }
};
struct CMemFunRef {
    void (CEnginePrimitiveHandle::*m_pfn)();
    explicit CMemFunRef(void (CEnginePrimitiveHandle::*pfn)()) : m_pfn(pfn) {}
    void operator()(CEnginePrimitiveHandle& h) const { (h.*m_pfn)(); }
};
CMemFunRef for_each(CEnginePrimitiveHandle* first, CEnginePrimitiveHandle* last, CMemFunRef f)
{
    for (; first != last; ++first) f(*first);
    return f;
}
struct CRetailHandleVector {
    CEnginePrimitiveHandle* m_pFirst;
    CEnginePrimitiveHandle* m_pLast;
    CEnginePrimitiveHandle* m_pEnd;
    CEnginePrimitiveHandle* begin() { return m_pFirst; }
    CEnginePrimitiveHandle* end() { return m_pLast; }
    void Set(CEnginePrimitiveHandle* f, long n) { m_pFirst = f; m_pLast = f + n; m_pEnd = f + n; }
};
struct CDrawQuickAccessIconsPC {
    bool Drawing;
    CRetailHandleVector PCPrimitiveHandles;
    void BaseClearPrimitivesIfNecessary() { ++g_baseCalls; }
    void ClearPrimitivesIfNecessary()
    {
        if (!Drawing)
            for_each(PCPrimitiveHandles.begin(), PCPrimitiveHandles.end(), CMemFunRef(&CEnginePrimitiveHandle::Clear));
        BaseClearPrimitivesIfNecessary();
    }
};
int main()
{
    int ok = 1;
    CPrimitive prims[2];
    CEnginePrimitiveHandle h[2];
    prims[0].m_refCount = 1; prims[1].m_refCount = 1;
    h[0].m_p = &prims[0]; h[1].m_p = &prims[1];
    CDrawQuickAccessIconsPC d; d.PCPrimitiveHandles.Set(h, 2);
    d.Drawing = true;
    d.ClearPrimitivesIfNecessary();
    ok = g_cleared == 0 && g_baseCalls == 1;
    d.Drawing = false;
    d.ClearPrimitivesIfNecessary();
    ok = ok && g_cleared == 2 && g_baseCalls == 2 && g_destroyed == 2 && h[1].m_p == 0;
    if (!ok) { std::printf("00657550_TEST FAIL\n"); return 1; }
    std::printf("00657550_TEST PASS\n");
    return 0;
}