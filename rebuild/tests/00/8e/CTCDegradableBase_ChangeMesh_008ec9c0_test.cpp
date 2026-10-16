// behaviour test for 0x008EC9C0 CTCDegradableBase::ChangeMesh
#include <cstdio>
struct Graphic { unsigned int BankIndex; float AnimStep; float RenderSizeX; bool AdditiveAlpha; unsigned char Type; };
struct Appearance { Graphic MainGraphic; unsigned int changed; void SetAsChanged(unsigned int r) { changed |= r; } };
struct Physics { const Graphic* last; void CreatePPhysicsMeshInfo(const Graphic& g) { last = &g; } };
struct Thing { Appearance* app; Physics* phys; };
static void ChangeMesh(Thing* t, const Graphic& g)
{
    t->app->MainGraphic = g;
    t->app->SetAsChanged(4);
    t->phys->CreatePPhysicsMeshInfo(g);
}
int main()
{
    Appearance a = { { 0, 0, 0, false, 0 }, 0 };
    Physics p = { 0 };
    Thing t = { &a, &p };
    Graphic g = { 77, 0.5f, 2.0f, true, 3 };
    ChangeMesh(&t, g);
    bool ok = a.MainGraphic.BankIndex == 77 && a.MainGraphic.RenderSizeX == 2.0f && a.MainGraphic.AdditiveAlpha
        && a.MainGraphic.Type == 3 && a.changed == 4 && p.last == &g;
    if (!ok) { std::printf("008ec9c0_TEST FAIL\n"); return 1; }
    std::printf("008ec9c0_TEST PASS\n");
    return 0;
}