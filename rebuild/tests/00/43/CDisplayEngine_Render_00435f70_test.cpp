// Behaviour model of CDisplayEngine::Render 0x00435F70: the public entry forwards its three arguments unchanged to DoRender, once.
#include <stdio.h>
struct Info { int frame; };
struct Display {
    int calls; const Info* seen; bool world; bool draw;
    Display() : calls(0), seen(0), world(false), draw(false) { }
    void Render(const Info* info, bool world_updated, bool render) { DoRender(info, world_updated, render); }
private:
    void DoRender(const Info* info, bool world_updated, bool render) { ++calls; seen = info; world = world_updated; draw = render; }
};
int main()
{
    Display d; Info a = { 7 }, b = { 9 };
    d.Render(&a, false, false);
    if (d.calls != 1 || d.seen != &a || d.world || d.draw) { printf("FAIL 1\n"); return 1; }
    d.Render(&b, true, true);
    if (d.calls != 2 || d.seen != &b || !d.world || !d.draw) { printf("FAIL 2\n"); return 1; }
    d.Render(&a, true, false);
    if (d.calls != 3 || d.seen != &a || !d.world || d.draw) { printf("FAIL 3\n"); return 1; }
    printf("PASS_00435f70\n");
    return 0;
}
