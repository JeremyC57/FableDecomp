// Behaviour test for CThing::DrawPreparePrimitivesForRendering @ 0x004C7540: limbo / not drawable -> false; else driver prepared (if any) and true.
#include <cstdio>
struct CRenderContext { int id; };
class CTCDriverBase { public:
    int calls; CRenderContext* last;
    CTCDriverBase() : calls(0), last(0) { }
    virtual void DrawPreparePrimitivesForRendering(CRenderContext* context) { ++calls; last = context; }
};
class CThing { public:
    bool drawable;
    CTCDriverBase* PTCDriver;
    unsigned char InLimbo : 1;
    virtual bool DrawIsDrawable() { return drawable; }
    CTCDriverBase* GetTCDriver() const { return PTCDriver; }
    bool DrawPreparePrimitivesForRendering(CRenderContext* context)
    {
        if (InLimbo || !DrawIsDrawable())
            return false;
        if (CTCDriverBase* driver = GetTCDriver())
            driver->DrawPreparePrimitivesForRendering(context);
        return true;
    }
};
int main()
{
    CRenderContext ctx; ctx.id = 1;
    CTCDriverBase d;
    CThing t; t.drawable = true; t.PTCDriver = &d; t.InLimbo = 1;
    if (t.DrawPreparePrimitivesForRendering(&ctx) || d.calls != 0) { std::printf("004c7540_TEST FAIL\n"); return 1; }
    t.InLimbo = 0; t.drawable = false;
    if (t.DrawPreparePrimitivesForRendering(&ctx) || d.calls != 0) { std::printf("004c7540_TEST FAIL\n"); return 1; }
    t.drawable = true;
    if (!t.DrawPreparePrimitivesForRendering(&ctx) || d.calls != 1 || d.last != &ctx) { std::printf("004c7540_TEST FAIL\n"); return 1; }
    t.PTCDriver = 0;
    if (!t.DrawPreparePrimitivesForRendering(&ctx)) { std::printf("004c7540_TEST FAIL\n"); return 1; }
    std::printf("004c7540_TEST PASS\n");
    return 0;
}