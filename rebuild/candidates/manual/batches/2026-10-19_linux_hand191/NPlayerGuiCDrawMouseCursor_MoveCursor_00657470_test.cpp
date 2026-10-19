// Behaviour test for NPlayerGui::CDrawMouseCursor::MoveCursor (0x00657470): scaled move (Y inverted) and clamping to
// [0, screen size - cursor width] per axis.
#include <cstdio>
struct C2DVector { float X, Y; };
struct C2DBoxF { C2DVector TopLeft, BottomRight; };
struct CPlayerGuiDef { float MouseCursorScaleX, MouseCursorScaleY; long MouseCursorGraphic; };
static CPlayerGuiDef g_def;
static CPlayerGuiDef* PeekGuiDef() { return &g_def; }
struct CGraphicBank { virtual unsigned long GetGraphicWidth(long graphic) { return graphic == 5 ? 32 : 0; } };
struct CPlayerGui { CGraphicBank* Bank; };
static CPlayerGui* g_pPlayerGui;
struct CScreen { C2DBoxF box; C2DBoxF GetScreenArea() const { return box; } };
struct CMainGameComponent { CScreen* Screen; };
struct CDrawMouseCursor {
    CMainGameComponent* Component; C2DVector Pos;
    void MoveCursor(const C2DVector& delta);
};
void CDrawMouseCursor::MoveCursor(const C2DVector& delta)
{
    CPlayerGuiDef* def = PeekGuiDef();
    float width = (float)g_pPlayerGui->Bank->GetGraphicWidth(def->MouseCursorGraphic);
    Pos.X += delta.X * def->MouseCursorScaleX;
    Pos.Y -= delta.Y * def->MouseCursorScaleY;
    C2DBoxF area = Component->Screen->GetScreenArea();
    float maxX = area.BottomRight.X - area.TopLeft.X - width;
    if (Pos.X < 0.0f)
        Pos.X = 0.0f;
    else if (maxX < Pos.X)
        Pos.X = maxX;
    float maxY = area.BottomRight.Y - area.TopLeft.Y - width;
    if (Pos.Y < 0.0f)
        Pos.Y = 0.0f;
    else if (maxY < Pos.Y)
        Pos.Y = maxY;
}

int main()
{
    CGraphicBank bank; CPlayerGui gui; gui.Bank = &bank; g_pPlayerGui = &gui;
    g_def.MouseCursorScaleX = 2.0f; g_def.MouseCursorScaleY = 1.0f; g_def.MouseCursorGraphic = 5;
    CScreen screen; screen.box.TopLeft.X = 0; screen.box.TopLeft.Y = 0; screen.box.BottomRight.X = 640; screen.box.BottomRight.Y = 480;
    CMainGameComponent comp; comp.Screen = &screen;
    CDrawMouseCursor c; c.Component = &comp; c.Pos.X = 100; c.Pos.Y = 100;
    int bad = 0;
    C2DVector d1 = { 10.0f, 20.0f };
    c.MoveCursor(d1);
    if (c.Pos.X != 120.0f || c.Pos.Y != 80.0f) bad = 1;
    C2DVector d2 = { -100.0f, 100.0f };
    c.MoveCursor(d2);
    if (c.Pos.X != 0.0f || c.Pos.Y != 0.0f) bad = 2;
    C2DVector d3 = { 1000.0f, -1000.0f };
    c.MoveCursor(d3);
    if (c.Pos.X != 608.0f || c.Pos.Y != 448.0f) bad = 3;
    if (bad) { std::printf("00657470_TEST FAIL\n"); return 1; }
    std::printf("00657470_TEST PASS\n");
    return 0;
}
