// Behaviour test for 0x005C2FE1 CTCInventoryWeapons::DrawPreparePrimitivesForRendering
#include <cstdio>
static double g_time = 10.0;
static double GFGetTime() { return g_time; }
struct CComponent {
    float updated; int renders;
    void Update(float dt) { updated += dt; }
    void Render(void*, long, long, long, long) { ++renders; }
};
struct CInv {
    CComponent* comps[2]; unsigned long count; bool FirstRenderFrame; double LastRenderTime; bool InventoryOpen;
    void Draw(void* viewport)
    {
        if (InventoryOpen) {
            float deltaTime = (float)(GFGetTime() - LastRenderTime);
            LastRenderTime = GFGetTime();
            for (unsigned long i = 0; i < count; ++i) {
                comps[i]->Update(deltaTime);
                if (FirstRenderFrame) FirstRenderFrame = false;
                else comps[i]->Render(viewport, 0, 0, 0, 0);
            }
        }
    }
};
int main()
{
    CComponent a = { 0, 0 }, b = { 0, 0 };
    CInv inv = { { &a, &b }, 2, true, 8.0, true };
    inv.Draw(0);
    bool ok = a.updated == 2.0f && b.updated == 2.0f && a.renders == 0 && b.renders == 1 && inv.LastRenderTime == 10.0 && !inv.FirstRenderFrame;
    inv.InventoryOpen = false; g_time = 20.0; inv.Draw(0); ok = ok && inv.LastRenderTime == 10.0;
    if (!ok) { std::printf("005c2fe1_TEST FAIL\n"); return 1; }
    std::printf("005c2fe1_TEST PASS\n");
    return 0;
}
