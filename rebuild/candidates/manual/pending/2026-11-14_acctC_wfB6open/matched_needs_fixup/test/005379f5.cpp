// Behaviour test for NUISystem::CList::ComputeChildrenVisibility() (0x005379F5): a copy of the row's logic on a small list
// model (children with a target position, a size, a colour and an event mask; the def view area, the screen-unit converter,
// the UI scale and the optional initial-position dummy). Checks the in-place view-area box (ConvertX / ConvertY, width and
// height kept in virtual units), the dummy move, the grow-by-one and scale step, both-corners containment, the fade in / out
// with the visibility bytes, the MOUSE_MOVEMENT re-observe pass and the selection repair.
#include <cstdio>
struct Vec { float X, Y; };
struct Box {
    float TLX, TLY, BRX, BRY;
    bool ContainsPoint(const Vec& p) const { if (p.X >= TLX && p.X < BRX && p.Y >= TLY && p.Y < BRY) return true; return false; }
};
static bool g_relative; static float g_backW, g_backH;
static float ConvertX(float x) { if (g_relative) return x / 640.0f * g_backW; return x; }
static float ConvertY(float y) { if (g_relative) return y / 480.0f * g_backH; return y; }
enum { EV_MOUSE = 1, EV_LEFT = 2, EV_RIGHT = 3, EV_SELECT = 4, EV_BACK = 5 };
enum { ST_UNSELECTED = 0, ST_SELECTED = 1, ST_CUSTOM = 16 };
struct Child {
    Vec pos, size; unsigned char alpha, targetAlpha; bool mine; unsigned events; long state; int updates; int colourChanges;
    void Observe(int e) { events |= 1u << e; }
    void Ignore(int e) { events &= ~(1u << e); }
};
struct List {
    Child c[6]; bool vis[6]; unsigned long n;
    long tlx, tly, brx, bry; Vec scale; bool hasDummy; Vec dummy;
    long state, target, requested; long selected; long customState;
    void Compute()
    {
        for (unsigned long i = 0; i < n; ++i) {
            if (!c[i].mine) continue;
            Vec position = c[i].pos;
            Vec size = c[i].size;
            size.X *= scale.X; size.Y *= scale.Y;
            Box child; child.TLX = position.X; child.TLY = position.Y; child.BRX = position.X + size.X; child.BRY = position.Y + size.Y;
            Box window;
            window.TLX = ConvertX((float)tlx); window.TLY = ConvertY((float)tly);
            window.BRX = ConvertX((float)tlx) - tlx + brx; window.BRY = ConvertY((float)tly) - tly + bry;
            if (hasDummy) { window.TLY = dummy.Y; window.BRY = bry + dummy.Y - tly; }
            window.BRX += 1.0f; window.BRY += 1.0f; window.TLX -= 1.0f; window.TLY -= 1.0f;
            window.BRX = (window.BRX - window.TLX) * scale.X + window.TLX;
            window.BRY = (window.BRY - window.TLY) * scale.Y + window.TLY;
            Vec tl = { child.TLX, child.TLY }, br = { child.BRX, child.BRY };
            if (window.ContainsPoint(tl) && window.ContainsPoint(br)) {
                if (c[i].targetAlpha == 0) { c[i].alpha = c[i].targetAlpha = 255; ++c[i].colourChanges; vis[i] = true; }
                if (!vis[i]) vis[i] = true;
            } else {
                if (c[i].targetAlpha != 0) { c[i].alpha = c[i].targetAlpha = 0; ++c[i].colourChanges; vis[i] = false; }
                if (vis[i]) vis[i] = false;
            }
        }
        for (unsigned long j = 0; j < n; ++j) {
            if (!c[j].mine) continue;
            if (vis[j]) { if (state == ST_SELECTED && target == ST_SELECTED && requested == ST_SELECTED) c[j].Observe(EV_MOUSE); }
            else { if (state == ST_SELECTED && target == ST_SELECTED && requested == ST_SELECTED) c[j].Ignore(EV_MOUSE); }
        }
        if (selected >= 0 && (unsigned long)selected < n && !vis[selected]) {
            long first = -1, last = -1, newSelected = 0;
            for (unsigned long k = 0; k < n; ++k) {
                if (first == -1 && vis[k]) first = k;
                if (first != -1 && !vis[k]) { last = k - 1; break; }
            }
            if (last == -1) last = n - 1;
            if (selected > last) newSelected = last;
            else if (first != -1) newSelected = first;
            if (newSelected != -1) {
                c[selected].Ignore(EV_LEFT); c[selected].Ignore(EV_RIGHT); c[selected].Ignore(EV_SELECT); c[selected].Ignore(EV_BACK);
                c[selected].state = ST_UNSELECTED;
                for (int a = 0; a < 10; ++a) ++c[selected].updates;
                selected = newSelected;
                c[selected].state = ST_SELECTED;
                for (int b = 0; b < 10; ++b) ++c[selected].updates;
                c[selected].Observe(EV_LEFT); c[selected].Observe(EV_RIGHT); c[selected].Observe(EV_SELECT); c[selected].Observe(EV_BACK);
                customState = selected + ST_CUSTOM;
            }
        }
    }
};
static void Setup(List& l)
{
    l.n = 6; l.tlx = 100; l.tly = 100; l.brx = 300; l.bry = 200; l.scale.X = 1.0f; l.scale.Y = 1.0f; l.hasDummy = false;
    l.dummy.X = 0.0f; l.dummy.Y = 0.0f; l.state = l.target = l.requested = ST_SELECTED; l.selected = 0; l.customState = -1;
    for (int i = 0; i < 6; ++i) {
        Child& c = l.c[i];
        c.pos.X = 110.0f; c.pos.Y = 60.0f + 30.0f * i; c.size.X = 100.0f; c.size.Y = 20.0f; c.alpha = c.targetAlpha = 0; c.mine = true;
        c.events = (1u << EV_MOUSE) | (1u << EV_LEFT) | (1u << EV_RIGHT) | (1u << EV_SELECT) | (1u << EV_BACK); c.state = i == 0 ? ST_SELECTED : ST_UNSELECTED;
        c.updates = 0; c.colourChanges = 0; l.vis[i] = false;
    }
}
static int Fail(int n) { std::printf("005379f5_TEST FAIL %d\n", n); return 1; }
int main()
{
    List l;
    g_relative = false; g_backW = 1280.0f; g_backH = 960.0f;
    Setup(l);
    l.c[5].mine = false; l.c[5].targetAlpha = 77;
    l.Compute();
    // rows at y = 60 (outside: top above 99), 90 (outside), 120, 150 (inside: 99 <= y, y + 20 < 201), 180 (bottom 200 < 201: inside), 210 (not ours)
    if (l.vis[0] || l.vis[1] || !l.vis[2] || !l.vis[3] || !l.vis[4] || l.vis[5]) return Fail(1);
    if (l.c[2].targetAlpha != 255 || l.c[2].colourChanges != 1 || l.c[0].colourChanges != 0 || l.c[5].targetAlpha != 77) return Fail(2);
    // the selected child 0 left the view: the first visible child takes the selection, with ten updates on each side
    if (l.selected != 2 || l.c[0].state != ST_UNSELECTED || l.c[2].state != ST_SELECTED || l.c[0].updates != 10 || l.c[2].updates != 10) return Fail(3);
    if ((l.c[0].events & (1u << EV_LEFT)) || !(l.c[2].events & (1u << EV_BACK)) || l.customState != 2 + ST_CUSTOM) return Fail(4);
    if ((l.c[0].events & (1u << EV_MOUSE)) || !(l.c[3].events & (1u << EV_MOUSE)) || !(l.c[5].events & (1u << EV_MOUSE))) return Fail(5);
    // second pass: nothing changes colour again, a visible child whose target alpha is 0 is faded in once more
    l.c[3].targetAlpha = 0;
    l.Compute();
    if (l.c[3].colourChanges != 2 || l.c[2].colourChanges != 1 || l.selected != 2) return Fail(6);
    // the dummy moves the window down by 60: rows 120 / 150 leave (top 159), 180 and 210 stay inside (bottom 261)
    Setup(l); l.hasDummy = true; l.dummy.Y = 160.0f; l.selected = 5;
    for (int i = 0; i < 6; ++i) { l.c[i].targetAlpha = 255; l.vis[i] = true; }
    l.Compute();
    if (l.vis[2] || l.vis[3] || !l.vis[4] || !l.vis[5] || l.c[3].targetAlpha != 0 || l.c[3].colourChanges != 1 || l.selected != 5) return Fail(7);
    // relative coordinates: the corner is converted (x * 2, y * 2), width and height stay in virtual units
    Setup(l); g_relative = true; l.selected = -1;
    l.c[0].pos.X = 205.0f; l.c[0].pos.Y = 205.0f;      // inside 199..401 x 199..301
    l.c[1].pos.X = 110.0f; l.c[1].pos.Y = 120.0f;      // inside the unconverted window only
    l.c[2].pos.X = 300.0f; l.c[2].pos.Y = 282.0f;      // bottom 302 >= 301: outside
    l.c[3].pos.X = 302.0f; l.c[3].pos.Y = 210.0f;      // right 402 >= 401: outside
    l.Compute();
    if (!l.vis[0] || l.vis[1] || l.vis[2] || l.vis[3] || l.selected != -1) return Fail(8);
    // the UI scale stretches the grown window about its top-left corner and scales the child size
    Setup(l); g_relative = false; l.scale.X = 2.0f; l.scale.Y = 0.5f; l.selected = -1;
    l.c[0].pos.X = 250.0f; l.c[0].pos.Y = 100.0f;      // right 450 < 99 + 202 * 2 = 503, bottom 110 < 99 + 102 * 0.5 = 150
    l.c[1].pos.X = 250.0f; l.c[1].pos.Y = 141.0f;      // bottom 151 >= 150: outside
    l.c[2].pos.X = 98.0f;  l.c[2].pos.Y = 100.0f;      // left of 99: outside
    l.Compute();
    if (!l.vis[0] || l.vis[1] || l.vis[2]) return Fail(9);
    // an unselected list leaves the event masks alone; a selection past the visible run falls back to its last child
    Setup(l); l.scale.X = 1.0f; l.scale.Y = 1.0f; l.state = ST_UNSELECTED; l.selected = 5;
    l.Compute();
    if (!(l.c[0].events & (1u << EV_MOUSE)) || l.selected != 4 || l.c[4].state != ST_SELECTED || l.c[5].updates != 10) return Fail(10);
    std::printf("005379f5_TEST PASS\n");
    return 0;
}
