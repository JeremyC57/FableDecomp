#include <cstdio>
#include <list>
// Behaviour model of NUISystem::CHoverable::UpdateHovering (0x0055BF10).
struct Box { long TLX, TLY, BRX, BRY; };
static float g_mx = 10.0f, g_my = 20.0f, g_sx = 4.0f, g_sy = 6.0f;
static bool g_restoring = false, g_claimed = false;
struct Hover;
static std::list<Hover*> g_list;
struct Hover {
    bool Hovered; long priority; bool listening; long l, t, r, b; int unhovered, changed;
    Hover(long p) : Hovered(false), priority(p), listening(true), l(0), t(0), r(100), b(100), unhovered(0), changed(0) {}
    bool IsHovered(const Box& m) { return m.TLX >= l && m.TLX < r && m.TLY >= t && m.TLY < b; }
    void Update()
    {
        if (g_restoring) return;
        if (!listening) return;
        Box mouse;
        mouse.TLX = (long)g_mx; mouse.TLY = (long)g_my;
        mouse.BRX = (long)(g_mx + g_sx); mouse.BRY = (long)(g_my + g_sy);
        if (!Hovered) {
            if (IsHovered(mouse) && !g_claimed) {
                std::list<Hover*> copy(g_list);
                for (std::list<Hover*>::iterator it = copy.begin(); it != copy.end(); ++it) {
                    Hover* other = *it;
                    if (other->IsHovered(mouse) && priority < other->priority) {
                        ++other->unhovered; other->Hovered = false; ++other->changed;
                    }
                }
                Hovered = true; ++changed;
                g_list.push_back(this);
                return;
            }
        }
        if (Hovered && !IsHovered(mouse)) { ++unhovered; Hovered = false; ++changed; }
    }
};
int main()
{
    Hover a(5), b(1);
    a.Update();
    int ok = a.Hovered && g_list.size() == 1 && a.changed == 1;
    b.Update();                                   // lower priority value: unhovers a
    ok = ok && b.Hovered && !a.Hovered && a.unhovered == 1 && g_list.size() == 2;
    g_mx = 500.0f; b.Update();
    ok = ok && !b.Hovered && b.unhovered == 1;
    g_restoring = true; g_mx = 10.0f; b.Update();
    ok = ok && !b.Hovered;
    if (!ok) { std::printf("0055bf10_TEST FAIL\n"); return 1; }
    std::printf("0055bf10_TEST PASS\n");
    return 0;
}
