#include <cstdio>
// Behaviour test for 00643eb2.

// NPlayerGui::CDrawQuestInfo::DrawBox: marks the drawer as drawing and draws the box grown by 8 left/top/bottom and 2 right on layer 0x4b.
struct C2DVector { float X, Y; };
struct C2DBoxF { C2DVector TopLeft, BottomRight; };
static C2DBoxF g_drawn; static long g_layer; static void* g_handles;
static void DrawBoxPrimitive(void* info, void* handles, const C2DBoxF& box, long layer) { (void)info; g_drawn = box; g_layer = layer; g_handles = handles; }
struct CDrawQuestInfo {
    bool Drawing; int PrimitiveHandles;
    void DrawBox(void* info, const C2DBoxF& box)
    {
        Drawing = true;
        C2DBoxF border = box;
        border.TopLeft.X -= 8.0f; border.TopLeft.Y -= 8.0f; border.BottomRight.X += 2.0f; border.BottomRight.Y += 8.0f;
        DrawBoxPrimitive(info, &PrimitiveHandles, border, 0x4b);
    }
};
int main()
{
    CDrawQuestInfo q; q.Drawing = false;
    C2DBoxF box = { { 10.0f, 20.0f }, { 30.0f, 40.0f } };
    q.DrawBox(0, box);
    int ok = q.Drawing && g_layer == 0x4b && g_handles == &q.PrimitiveHandles && g_drawn.TopLeft.X == 2.0f && g_drawn.TopLeft.Y == 12.0f
          && g_drawn.BottomRight.X == 32.0f && g_drawn.BottomRight.Y == 48.0f && box.TopLeft.X == 10.0f;
    if (!ok) { std::printf("00643eb2_TEST FAIL\n"); return 1; }
    std::printf("00643eb2_TEST PASS\n");
    return 0;
}
