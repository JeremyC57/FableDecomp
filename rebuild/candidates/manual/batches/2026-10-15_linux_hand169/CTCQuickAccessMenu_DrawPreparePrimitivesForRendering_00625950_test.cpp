#include <cstdio>
// Behaviour model of CTCQuickAccessMenu::DrawPreparePrimitivesForRendering (0x00625950): update every component, draw all
// but the first one on the first render frame, nothing when inactive or quitting.
struct Comp { int updates; int renders; float dt; };
struct Menu {
    bool active, quitting, first; Comp* comps; unsigned n; double last;
    void Draw(double now1, double now2) {
        if (active && !quitting) {
            float dt = (float)(now1 - last); last = now2;
            for (unsigned i = 0; i < n; ++i) {
                comps[i].updates++; comps[i].dt = dt;
                if (!first) comps[i].renders++; else first = false;
            }
        }
    }
};
int main()
{
    Comp c[3] = { {0,0,0}, {0,0,0}, {0,0,0} };
    Menu m = { true, false, true, c, 3, 1.0 };
    m.Draw(1.5, 1.6);
    if (c[0].updates != 1 || c[0].renders != 0 || c[1].renders != 1 || c[2].renders != 1 || c[2].dt != 0.5f || m.last != 1.6) { std::printf("00625950_TEST FAIL\n"); return 1; }
    m.quitting = true; m.Draw(2.0, 2.0);
    if (c[0].updates != 1) { std::printf("00625950_TEST FAIL\n"); return 1; }
    std::printf("00625950_TEST PASS\n");
    return 0;
}
