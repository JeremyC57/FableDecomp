#include <cstdio>

static long g_frame = 0;
static long GetCurrentWorldFrame() { return g_frame; }
struct CEnginePersistentPrimitiveBase {
    bool m_rendered;
    virtual ~CEnginePersistentPrimitiveBase() { }
    virtual bool WasRenderedLastFrame() { return m_rendered; }
};
struct CPrimitive { long Type; long header; CEnginePersistentPrimitiveBase* Data; long BankIndex; };
struct CPrimitiveListNode { CPrimitiveListNode* m_next; CPrimitiveListNode* m_prev; CPrimitive m_value; };
struct CPrimitiveList { CPrimitiveListNode* m_head; };
struct CTCGraphicAppearance {
    CPrimitiveList Primitives;
    long FrameUpdatedWasRenderedFlag;
    unsigned char RenderedLastFrame : 1;
    unsigned char PrimitivesDrawn : 1;
    void UpdateRenderedLastFrameFlag()
    {
        FrameUpdatedWasRenderedFlag = GetCurrentWorldFrame();
        if (PrimitivesDrawn) {
            for (CPrimitiveListNode* node = Primitives.m_head->m_next; node != Primitives.m_head; node = node->m_next) {
                CEnginePersistentPrimitiveBase* data = node->m_value.Data;
                if (data && data->WasRenderedLastFrame()) {
                    RenderedLastFrame = 1;
                    return;
                }
            }
        }
        RenderedLastFrame = 0;
    }
};

int main()
{
    CEnginePersistentPrimitiveBase p; p.m_rendered = true;
    CPrimitiveListNode head, n1, n2;
    head.m_next = &n1; n1.m_next = &n2; n2.m_next = &head;
    n1.m_value.Data = 0; n2.m_value.Data = &p;
    CTCGraphicAppearance a; a.Primitives.m_head = &head; a.PrimitivesDrawn = 1; a.RenderedLastFrame = 0;
    g_frame = 42; a.UpdateRenderedLastFrameFlag();
    if (a.FrameUpdatedWasRenderedFlag != 42 || !a.RenderedLastFrame) { std::printf("004bc620_TEST FAIL\n"); return 1; }
    p.m_rendered = false; a.UpdateRenderedLastFrameFlag();
    if (a.RenderedLastFrame) { std::printf("004bc620_TEST FAIL\n"); return 1; }
    p.m_rendered = true; a.PrimitivesDrawn = 0; a.RenderedLastFrame = 1; g_frame = 43; a.UpdateRenderedLastFrameFlag();
    if (a.RenderedLastFrame || a.FrameUpdatedWasRenderedFlag != 43) { std::printf("004bc620_TEST FAIL\n"); return 1; }
    std::printf("004bc620_TEST PASS\n");
    return 0;
}
