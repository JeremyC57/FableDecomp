#include <cstdio>
// behaviour test: CPlayerGui::DrawDebugSafeAreas -- resize the handle vector to eight; with the debug flag set add the
// eight safe-area line sprites (outer frame alpha 0, inner frame opaque) to handles 0..7, otherwise clear every handle
class CEnginePrimitiveHandle { public:
    int m_header; int Data;
    void Clear() { m_header = 0; Data = 0; }
};
struct CMemFunRefClear {
    void (CEnginePrimitiveHandle::*m_pmf)();
    explicit CMemFunRefClear(void (CEnginePrimitiveHandle::*pmf)()) : m_pmf(pmf) { }
};
static CMemFunRefClear for_each(CEnginePrimitiveHandle* first, CEnginePrimitiveHandle* last, CMemFunRefClear f)
{
    for (; first != last; ++first) (first->*f.m_pmf)();
    return f;
}
static CEnginePrimitiveHandle g_store[16];
struct CRetailVector {
    CEnginePrimitiveHandle* _M_start; CEnginePrimitiveHandle* _M_finish;
    void resize(unsigned int n) { _M_start = g_store; _M_finish = g_store + n; }
};
class C2DVector { public: float X, Y; C2DVector(float x, float y) : X(x), Y(y) {} };
class C2DBoxF { public:
    C2DVector TopLeft, BottomRight;
    C2DBoxF(float l, float t, float r, float b) : TopLeft(l, t), BottomRight(r, b) {}
};
struct CEnginePrimitive2DSprite {
    C2DBoxF Box; int Layer; int Blend; unsigned char R, G, B, A;
    CEnginePrimitive2DSprite(unsigned int, const C2DBoxF& box, int layer, unsigned int, unsigned int, int blend, int, float, float,
                             unsigned char r, unsigned char g, unsigned char b, unsigned char a, unsigned int, const C2DVector&, const C2DVector&)
        : Box(box), Layer(layer), Blend(blend), R(r), G(g), B(b), A(a) {}
};
static float g_area; static int g_added, g_opaque, g_bad;
class CEnginePrimitiveRenderer { public:
    void AddPrimitive(CEnginePrimitiveHandle* h, const CEnginePrimitive2DSprite& s, int flags)
    {
        h->m_header = 1; h->Data = ++g_added;
        g_area += (s.Box.BottomRight.X - s.Box.TopLeft.X) * (s.Box.BottomRight.Y - s.Box.TopLeft.Y);
        if (s.A == 0xff) ++g_opaque;
        if (flags != 0x40 || s.Layer != 0x43 || s.Blend != 3 || s.R != 0xff || s.G != 0xff || s.B != 0xff) ++g_bad;
    }
};
class CEnginePrimitivePrepareInfo { public: CEnginePrimitiveRenderer* Renderer; };
class CPlayerGui { public:
    bool ShowDebugSafeAreas; CRetailVector SafeAreaHandles;
    void DrawDebugSafeAreas(const CEnginePrimitivePrepareInfo& info);
};
#define SPRITE(name, l, t, r, b, a) CEnginePrimitive2DSprite name(0, C2DBoxF(l, t, r, b), 0x43, 0, 0, 3, 0, 0.0f, 0.0f, 0xff, 0xff, 0xff, a, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f))
void CPlayerGui::DrawDebugSafeAreas(const CEnginePrimitivePrepareInfo& info)
{
    SafeAreaHandles.resize(8);
    if (ShowDebugSafeAreas) {
        SPRITE(top, 32.0f, 24.0f, 608.0f, 25.0f, 0); SPRITE(left, 32.0f, 24.0f, 33.0f, 456.0f, 0);
        SPRITE(right, 608.0f, 24.0f, 609.0f, 456.0f, 0); SPRITE(bottom, 32.0f, 456.0f, 609.0f, 457.0f, 0);
        SPRITE(innerTop, 48.0f, 36.0f, 592.0f, 37.0f, 0xff); SPRITE(innerLeft, 48.0f, 36.0f, 49.0f, 444.0f, 0xff);
        SPRITE(innerRight, 592.0f, 36.0f, 593.0f, 444.0f, 0xff); SPRITE(innerBottom, 48.0f, 444.0f, 593.0f, 445.0f, 0xff);
        info.Renderer->AddPrimitive(SafeAreaHandles._M_start, top, 0x40);
        info.Renderer->AddPrimitive(SafeAreaHandles._M_start + 1, left, 0x40);
        info.Renderer->AddPrimitive(SafeAreaHandles._M_start + 2, right, 0x40);
        info.Renderer->AddPrimitive(SafeAreaHandles._M_start + 3, bottom, 0x40);
        info.Renderer->AddPrimitive(SafeAreaHandles._M_start + 4, innerTop, 0x40);
        info.Renderer->AddPrimitive(SafeAreaHandles._M_start + 5, innerLeft, 0x40);
        info.Renderer->AddPrimitive(SafeAreaHandles._M_start + 6, innerRight, 0x40);
        info.Renderer->AddPrimitive(SafeAreaHandles._M_start + 7, innerBottom, 0x40);
    } else {
        for_each(SafeAreaHandles._M_start, SafeAreaHandles._M_finish, CMemFunRefClear(&CEnginePrimitiveHandle::Clear));
    }
}
int main()
{
    bool ok = true;
    CEnginePrimitiveRenderer renderer; CEnginePrimitivePrepareInfo info; info.Renderer = &renderer;
    CPlayerGui gui; gui.ShowDebugSafeAreas = true;
    g_store[8].m_header = 9;
    gui.DrawDebugSafeAreas(info);
    // outer frame: 576 + 432 + 432 + 577; inner frame: 544 + 408 + 408 + 545
    ok = ok && g_added == 8 && g_opaque == 4 && g_bad == 0 && g_area == 2017.0f + 1905.0f
            && g_store[0].Data == 1 && g_store[7].Data == 8 && g_store[8].m_header == 9 && gui.SafeAreaHandles._M_finish == g_store + 8;
    gui.ShowDebugSafeAreas = false;
    gui.DrawDebugSafeAreas(info);
    ok = ok && g_added == 8 && g_store[0].m_header == 0 && g_store[7].Data == 0 && g_store[8].m_header == 9;
    if (!ok) { std::printf("0043b050_TEST FAIL\n"); return 1; }
    std::printf("0043b050_TEST PASS\n");
    return 0;
}