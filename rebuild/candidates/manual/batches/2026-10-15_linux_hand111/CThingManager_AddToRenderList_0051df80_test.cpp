#include <cstdio>
// Behaviour test for CThingManager::AddToRenderList (0x0051DF80).
struct CThing { CThing* PRenderPrev; CThing* PRenderNext; };
struct ThingLList { CThing* PHead; CThing* PTail; };
struct CThingManager {
    ThingLList RenderList;
    void AddToRenderList(CThing* thing)
    {
        if (thing->PRenderNext == 0 && thing->PRenderPrev == 0 && RenderList.PHead != thing) {
            if (RenderList.PTail != 0) RenderList.PTail->PRenderNext = thing; else RenderList.PHead = thing;
            thing->PRenderPrev = RenderList.PTail;
            thing->PRenderNext = 0;
            RenderList.PTail = thing;
        }
    }
};
int main()
{
    CThing a, b, c; CThing* all[3] = { &a, &b, &c };
    for (int i = 0; i < 3; ++i) { all[i]->PRenderPrev = 0; all[i]->PRenderNext = 0; }
    CThingManager m; m.RenderList.PHead = 0; m.RenderList.PTail = 0;
    int fails = 0;
    m.AddToRenderList(&a);
    if (m.RenderList.PHead != &a || m.RenderList.PTail != &a || a.PRenderPrev || a.PRenderNext) ++fails;
    m.AddToRenderList(&a);                      // single element, already the head: no change
    if (m.RenderList.PTail != &a || a.PRenderPrev != 0) ++fails;
    m.AddToRenderList(&b);
    m.AddToRenderList(&c);
    if (a.PRenderNext != &b || b.PRenderPrev != &a || b.PRenderNext != &c || c.PRenderPrev != &b || m.RenderList.PTail != &c) ++fails;
    m.AddToRenderList(&b);                      // linked already: ignored
    if (m.RenderList.PTail != &c || b.PRenderNext != &c) ++fails;
    if (fails) { std::printf("0051df80_TEST FAIL\n"); return 1; }
    std::printf("0051df80_TEST PASS\n");
    return 0;
}
