#include <cstdio>
#include <cmath>

// Model of CScrollingViewport::ScrollChildren: the step is the front end's description offset (negated when scrolling
// up) or the viewport speed; every active child (scroll bars excluded) and every scroll-list child reports how far it
// can scroll inside the scaled window, the largest magnitude wins, it is clamped to the step unless the front end says
// unclamped, and the same children are then moved by (0, scroll) over 0.2 s.
struct C2DVector { float X, Y; };
struct C2DBoxF { float TLX, TLY, BRX, BRY; };
struct CFrontEnd { float offset; C2DVector scale; bool unclamped; };
static CFrontEnd g_fe;

struct CChild {
    bool active; float can; C2DBoxF seenBox; float moved; int moves;
    bool IsActive() const { return active; }
    float CanScroll(bool, const C2DBoxF& box) { seenBox = box; return can; }
    void ChangePositionDelta(const C2DVector& d, float t, bool) { if (d.X == 0.0f && t == 0.2f) { moved += d.Y; ++moves; } }
};
struct CViewport {
    C2DVector renderPos;
    CChild* children[4]; unsigned int nChildren;
    CChild* scrollList[2]; unsigned int nScroll;
    float ScrollingSpeed; C2DBoxF Window; CChild* PScrollBar; CChild* PScrollBarOutside;
    void ScrollChildren(bool up)
    {
        float delta;
        if (g_fe.offset != -1.0f) delta = up ? -g_fe.offset : g_fe.offset;
        else if (up) delta = -ScrollingSpeed;
        else delta = ScrollingSpeed;
        C2DBoxF box;
        box.TLX = Window.TLX * g_fe.scale.X + renderPos.X;
        box.TLY = Window.TLY * g_fe.scale.Y + renderPos.Y;
        box.BRX = (Window.BRX - Window.TLX) * g_fe.scale.X + box.TLX;
        box.BRY = (Window.BRY - Window.TLY) * g_fe.scale.Y + box.TLY;
        float scroll = 0.0f;
        unsigned int i;
        for (i = 0; i < nChildren; ++i)
            if (children[i]->IsActive() && children[i] != PScrollBar && children[i] != PScrollBarOutside) {
                float can = children[i]->CanScroll(up, box);
                if (std::fabs(scroll) < std::fabs(can)) scroll = can;
            }
        for (i = 0; i < nScroll; ++i)
            if (scrollList[i]->IsActive()) {
                float can = scrollList[i]->CanScroll(up, box);
                if (std::fabs(scroll) < std::fabs(can)) scroll = can;
            }
        if (!g_fe.unclamped && std::fabs(scroll) > std::fabs(delta)) scroll = delta;
        for (i = 0; i < nChildren; ++i)
            if (children[i]->IsActive() && children[i] != PScrollBar && children[i] != PScrollBarOutside) {
                C2DVector move; move.X = 0.0f; move.Y = scroll;
                children[i]->ChangePositionDelta(move, 0.2f, false);
            }
        for (i = 0; i < nScroll; ++i)
            if (scrollList[i]->IsActive()) {
                C2DVector move; move.X = 0.0f; move.Y = scroll;
                scrollList[i]->ChangePositionDelta(move, 0.2f, false);
            }
    }
};

static CChild Make(bool active, float can) { CChild c; c.active = active; c.can = can; c.moved = 0; c.moves = 0; return c; }

int main()
{
    CChild a = Make(true, -3.0f), b = Make(true, 7.0f), bar = Make(true, 100.0f), off = Make(false, 50.0f), s = Make(true, -9.0f);
    CViewport v;
    v.renderPos.X = 10; v.renderPos.Y = 20;
    v.children[0] = &a; v.children[1] = &bar; v.children[2] = &off; v.children[3] = &b; v.nChildren = 4;
    v.scrollList[0] = &s; v.nScroll = 1;
    v.ScrollingSpeed = 5.0f; v.Window.TLX = 1; v.Window.TLY = 2; v.Window.BRX = 5; v.Window.BRY = 10;
    v.PScrollBar = &bar; v.PScrollBarOutside = 0;
    g_fe.offset = -1.0f; g_fe.scale.X = 2.0f; g_fe.scale.Y = 0.5f; g_fe.unclamped = false;
    v.ScrollChildren(false);   // largest |can| = 9 (scroll list), clamped to the speed 5
    if (a.seenBox.TLX != 12.0f || a.seenBox.TLY != 21.0f || a.seenBox.BRX != 20.0f || a.seenBox.BRY != 25.0f) { std::printf("0053eda0_TEST FAIL\n"); return 1; }
    if (a.moved != 5.0f || b.moved != 5.0f || s.moved != 5.0f || bar.moves != 0 || off.moves != 0) { std::printf("0053eda0_TEST FAIL\n"); return 1; }
    g_fe.unclamped = true;
    v.ScrollChildren(true);    // unclamped: -9 wins
    if (a.moved != -4.0f || s.moved != -4.0f) { std::printf("0053eda0_TEST FAIL\n"); return 1; }
    g_fe.unclamped = false; g_fe.offset = 2.0f;
    v.ScrollChildren(true);    // clamped to the description offset, negated: -2
    if (b.moved != -6.0f || b.moves != 3) { std::printf("0053eda0_TEST FAIL\n"); return 1; }
    std::printf("0053eda0_TEST PASS\n");
    return 0;
}
