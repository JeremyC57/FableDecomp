// Behaviour test for 0x00B935C0 CEngineSceneGridCell::CEngineSceneGridCell(parent): defaults, flags and the seeded preload offset.
#include <cstdio>

static int g_baseLive = 0, g_notified = 0, g_destroyed = 0;
struct CBaseClass { CBaseClass() { ++g_baseLive; } ~CBaseClass() { --g_baseLive; } virtual void v00() {} };
struct CEnginePrimitiveHandle;
struct CEnginePersistentPrimitiveBase {
    long m_refCount;
    virtual void Destroy() { ++g_destroyed; }
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle*) { ++g_notified; }
};
template <class T> struct CIVCountedPointer {
    T* m_p;
    CIVCountedPointer() : m_p(0) {}
    ~CIVCountedPointer() { Release(); }
    void Release() { T* p = m_p; if (p) { if (--p->m_refCount == 0) p->Destroy(); m_p = 0; } }
    CIVCountedPointer& operator=(T* p) { Release(); m_p = p; return *this; }
    T* operator->() const { return m_p; }
    long GetRefCount() const { return m_p ? m_p->m_refCount : 0; }
};
struct CEnginePrimitiveHandle : CBaseClass {
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;
    ~CEnginePrimitiveHandle()
    {
        if (Data.GetRefCount() == 1) Data->OnLastHandleReleased(this);
        Data = 0;
    }
};
struct CPrim { int id; };
static CPrim* g_cleared = 0;
static void ClearPrimitiveList(CPrim* first) { g_cleared = first; }
static unsigned int GFROR13(unsigned int v) { return (v >> 13) | (v << 19); }
struct CRandomSeed {
    unsigned int m_seed;
    unsigned int Rand(unsigned int range)
    {
        m_seed = m_seed * 0x24a1 + 0x24df;
        m_seed = GFROR13(m_seed);
        return range ? m_seed % range : 0;
    }
};
static CRandomSeed g_randomSeed; static unsigned int g_preloadFrameRange = 0;
struct CEngineSceneGridCell {
    CEngineSceneGridCell* Parent; CPrim* FirstPrimitive;
    CEnginePrimitiveHandle GroupPrimitiveHandle[2];
    long LocalCount; unsigned int TotalCount, RenderMask, OccludedLayers;
    unsigned char BoundingBoxValid : 1, TotalCountValid : 1, RenderMaskValid : 1, WasRendered : 1, HasOcclusionResults : 1;
    unsigned char OccludedFrames, PreloadFrames;
    CEngineSceneGridCell(CEngineSceneGridCell* parent) : Parent(parent), FirstPrimitive(0)
    {
        BoundingBoxValid = 0; TotalCountValid = 1; RenderMaskValid = 1; WasRendered = 0; HasOcclusionResults = 0;
        LocalCount = 0; TotalCount = 0; RenderMask = 0; OccludedLayers = 0; OccludedFrames = 0;
        PreloadFrames = (unsigned char)g_randomSeed.Rand(g_preloadFrameRange);
    }
    ~CEngineSceneGridCell() { ClearPrimitiveList(FirstPrimitive); }
};

int main()
{
    g_randomSeed.m_seed = 12345; g_preloadFrameRange = 0;
    CEngineSceneGridCell root(0);
    if (root.PreloadFrames != 0 || root.Parent != 0 || root.FirstPrimitive != 0 || !root.TotalCountValid || !root.RenderMaskValid
        || root.BoundingBoxValid || root.WasRendered || root.HasOcclusionResults || root.LocalCount || root.TotalCount
        || root.GroupPrimitiveHandle[1].Data.m_p != 0 || g_baseLive != 2) { std::printf("00b935c0_TEST FAIL\n"); return 1; }
    unsigned int s1 = 12345u * 0x24a1u + 0x24dfu; s1 = (s1 >> 13) | (s1 << 19);
    if (g_randomSeed.m_seed != s1) { std::printf("00b935c0_TEST FAIL\n"); return 1; }
    g_preloadFrameRange = 7;
    CEngineSceneGridCell child(&root);
    unsigned int s2 = s1 * 0x24a1u + 0x24dfu; s2 = (s2 >> 13) | (s2 << 19);
    if (child.Parent != &root || child.PreloadFrames != (unsigned char)(s2 % 7) || g_randomSeed.m_seed != s2) { std::printf("00b935c0_TEST FAIL\n"); return 1; }
    std::printf("00b935c0_TEST PASS\n");
    return 0;
}