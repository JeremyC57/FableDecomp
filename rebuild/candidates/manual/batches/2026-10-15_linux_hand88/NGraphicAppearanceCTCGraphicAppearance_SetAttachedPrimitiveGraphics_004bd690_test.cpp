#include <cstdio>

struct CEnginePrimitive { int id; };
struct CEnginePrimitiveAttachmentInfo { int id; };
struct CEnginePrimitiveHandle { long m_header; void* Data; };
struct CPrimitive { long Type; CEnginePrimitiveHandle PrimitiveHandle; long BankIndex; };
static CEnginePrimitiveHandle* g_prim = 0; static CEnginePrimitive* g_to = 0; static CEnginePrimitiveAttachmentInfo* g_info = 0;
static CEnginePrimitiveHandle* g_parent = 0; static long g_bone = 0; static unsigned long g_flags = 0xffff;
struct CEnginePrimitiveManager {
    virtual ~CEnginePrimitiveManager() { }
    virtual void AttachPrimitive(CEnginePrimitiveHandle* p, CEnginePrimitive* to, CEnginePrimitiveAttachmentInfo* info,
                                 CEnginePrimitiveHandle* parent, long bone, unsigned long flags)
    { g_prim = p; g_to = to; g_info = info; g_parent = parent; g_bone = bone; g_flags = flags; }
};
struct CEngineGlobals { CEnginePrimitiveManager* m_pPrimitiveManager; };
static CEnginePrimitiveManager g_manager;
static CEngineGlobals g_globals = { &g_manager };
static CEngineGlobals* g_pEngineGlobals = &g_globals;
struct CTCGraphicAppearance {
    signed char ByPassColorFilter; bool m_afterWater;
    bool IsDrawingAfterWater() { return m_afterWater; }
    void SetAttachedPrimitiveGraphics(CPrimitive* primitive, CEnginePrimitive* attachTo, CEnginePrimitiveAttachmentInfo* info, CPrimitive* parent)
    {
        unsigned short flags = 0;
        if (ByPassColorFilter > 0)
            flags = 0x800;
        if (IsDrawingAfterWater())
            flags |= 8;
        g_pEngineGlobals->m_pPrimitiveManager->AttachPrimitive(&primitive->PrimitiveHandle, attachTo, info, &parent->PrimitiveHandle, -1, flags);
    }
};

int main()
{
    CPrimitive prim, parent; CEnginePrimitive to = { 1 }; CEnginePrimitiveAttachmentInfo info = { 2 };
    CTCGraphicAppearance a; a.ByPassColorFilter = 0; a.m_afterWater = false;
    a.SetAttachedPrimitiveGraphics(&prim, &to, &info, &parent);
    if (g_prim != &prim.PrimitiveHandle || g_to != &to || g_info != &info || g_parent != &parent.PrimitiveHandle || g_bone != -1 || g_flags != 0)
        { std::printf("004bd690_TEST FAIL\n"); return 1; }
    a.ByPassColorFilter = 2; a.m_afterWater = true; a.SetAttachedPrimitiveGraphics(&prim, &to, &info, &parent);
    if (g_flags != 0x808) { std::printf("004bd690_TEST FAIL\n"); return 1; }
    a.ByPassColorFilter = -1; a.m_afterWater = true; a.SetAttachedPrimitiveGraphics(&prim, &to, &info, &parent);
    if (g_flags != 8) { std::printf("004bd690_TEST FAIL\n"); return 1; }
    std::printf("004bd690_TEST PASS\n");
    return 0;
}
