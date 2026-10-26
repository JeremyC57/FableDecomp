// Behaviour test for 007e7c90: body copied from src/007e7c90.cpp (CTCFireheartMinigame::DrawPreparePrimitivesForRendering);
// erase / push_back_default / GetRenderTargetDimensions / the sprite ctor / AddPrimitive stubbed to record their arguments.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static bool feq(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-5f; }
class CEnginePrimitiveHandle {
public:
    long  m_header;
    void* Data;
};
static int g_erase = 0, g_push = 0;
template <class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void erase(T* first, T* last) { ++g_erase; CHECK(first == _M_start && last == _M_finish); _M_finish = _M_start; }
    void clear() { erase(_M_start, _M_finish); }
    void push_back_default() { ++g_push; memset(_M_finish, 0, sizeof(T)); ++_M_finish; }
};
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
class C2DBoxF { public:
    C2DVector TopLeft, BottomRight;
    C2DBoxF(float left, float top, float right, float bottom) : TopLeft(left, top), BottomRight(right, bottom) {}
};
class C2DExtentsI { public: long X, Y; };
static int g_dims = 0;
class CDisplayManager {
public:
    C2DExtentsI GetRenderTargetDimensions() const { ++g_dims; C2DExtentsI e; e.X = 640; e.Y = 480; return e; }
};
static CDisplayManager g_display;
CDisplayManager* g_pDisplayDevice = &g_display;
static int g_spriteCtor = 0; static C2DBoxF g_box(-1, -1, -1, -1); static long g_layer = 0, g_blend = 0; static unsigned char g_red = 0, g_alpha = 1;
static C2DVector g_uv0(-1, -1), g_uv1(-1, -1); static unsigned __int64 g_id = 99; static unsigned long g_e = 7;
class CEnginePrimitive2DSprite {
public:
    unsigned char _body[0x54];
    CEnginePrimitive2DSprite(unsigned __int64 id, const C2DBoxF& box, long layer, unsigned long a, unsigned long b, long blend,
                             long interpolation, float c, float d, unsigned char red, unsigned char green, unsigned char blue,
                             unsigned char alpha, unsigned long e, const C2DVector& uv0, const C2DVector& uv1)
    {
        ++g_spriteCtor; g_id = id; g_box = box; g_layer = layer; g_blend = blend; g_red = red; g_alpha = alpha; g_e = e; g_uv0 = uv0; g_uv1 = uv1;
        CHECK(a == 0 && b == 0 && interpolation == 0 && c == 0.0f && d == 0.0f && green == 0 && blue == 0);
        memset(_body, 0x5a, sizeof(_body));
    }
};
static int g_add = 0; static CEnginePrimitiveHandle* g_addHandle = 0; static const CEnginePrimitive2DSprite* g_addSprite = 0; static long g_addFlags = 0;
class CEnginePrimitiveRenderer {
public:
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {}
    virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {}
    virtual void v12() {} virtual void v13() {} virtual void v14() {} virtual void v15() {} virtual void v16() {} virtual void v17() {}
    virtual void v18() {} virtual void v19() {} virtual void v20() {} virtual void v21() {} virtual void v22() {}
    virtual void AddPrimitive(CEnginePrimitiveHandle* handle, const CEnginePrimitive2DSprite& primitive, long flags)
    { ++g_add; g_addHandle = handle; g_addSprite = &primitive; g_addFlags = flags; }
};
class CEnginePrimitivePrepareInfo {
public:
    unsigned char _pad00[0x34];
    CEnginePrimitiveRenderer* Renderer;
};
class CTCFireheartMinigame {
public:
    virtual void v00() {}
    unsigned char _pad04[0x88 - 0x04];
    bool QuitWithDelay;
    unsigned char _pad89[3];
    CRetailVector<CEnginePrimitiveHandle> Handles;
    void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);
};
void CTCFireheartMinigame::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    Handles.clear();
    if (QuitWithDelay) {
        C2DExtentsI size = g_pDisplayDevice->GetRenderTargetDimensions();
        C2DBoxF box(0.0f, 0.0f, (float)size.X, (float)size.Y);
        CEnginePrimitive2DSprite sprite(0, box, 0x4a, 0, 0, 2, 0, 0.0f, 0.0f, 0xff, 0, 0, 0, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        Handles.push_back_default();
        info.Renderer->AddPrimitive(Handles._M_finish - 1, sprite, 0x40);
    }
}
int main()
{
    CEnginePrimitiveHandle storage[4]; memset(storage, 0xcc, sizeof(storage));
    CTCFireheartMinigame game; memset(&game.QuitWithDelay, 0, sizeof(game) - 0x88);
    game.Handles._M_start = storage; game.Handles._M_finish = storage + 2; game.Handles._M_end_of_storage = storage + 4;
    CEnginePrimitiveRenderer renderer; CEnginePrimitivePrepareInfo info; info.Renderer = &renderer;
    game.QuitWithDelay = false;
    game.DrawPreparePrimitivesForRendering(info);
    CHECK(g_erase == 1); CHECK(game.Handles._M_finish == storage); CHECK(g_dims == 0 && g_spriteCtor == 0 && g_push == 0 && g_add == 0);
    game.QuitWithDelay = true;
    game.DrawPreparePrimitivesForRendering(info);
    CHECK(g_erase == 2); CHECK(g_dims == 1); CHECK(g_spriteCtor == 1); CHECK(g_push == 1); CHECK(g_add == 1);
    CHECK(g_id == 0); CHECK(feq(g_box.TopLeft.X, 0.0f) && feq(g_box.TopLeft.Y, 0.0f) && feq(g_box.BottomRight.X, 640.0f) && feq(g_box.BottomRight.Y, 480.0f));
    CHECK(g_layer == 0x4a); CHECK(g_blend == 2); CHECK(g_red == 0xff); CHECK(g_alpha == 0); CHECK(g_e == 0);
    CHECK(feq(g_uv0.X, 0.0f) && feq(g_uv0.Y, 0.0f) && feq(g_uv1.X, 0.0f) && feq(g_uv1.Y, 0.0f));
    CHECK(game.Handles._M_finish == storage + 1); CHECK(g_addHandle == storage); CHECK(g_addFlags == 0x40);
    CHECK(g_addSprite != 0 && g_addSprite->_body[0] == 0x5a);
    if (fails == 0) printf("007e7c90_TEST PASS\n"); else printf("007e7c90_TEST FAIL\n");
    return fails;
}