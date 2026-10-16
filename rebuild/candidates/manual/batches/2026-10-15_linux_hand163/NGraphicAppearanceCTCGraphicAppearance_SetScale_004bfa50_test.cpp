// behaviour test for 0x004BFA50 CTCGraphicAppearance::SetScale
#include <cstdio>
struct Anim { float Scale; };
struct Thing { Anim* anim; int renders; void AddToRenderList() { ++renders; } };
struct App {
    Thing* ParentThing; float Scale; float RenderSizeX; unsigned char Reasons;
    void SetScale(float s)
    {
        Scale = s;
        if (ParentThing->anim) ParentThing->anim->Scale = s * RenderSizeX;
        ParentThing->AddToRenderList();
        Reasons |= 4;
    }
};
int main()
{
    Anim an = { 0.0f };
    Thing t = { &an, 0 };
    App a = { &t, 1.0f, 3.0f, 1 };
    a.SetScale(2.0f);
    bool ok = a.Scale == 2.0f && an.Scale == 6.0f && t.renders == 1 && a.Reasons == 5;
    t.anim = 0; a.SetScale(4.0f);
    ok = ok && a.Scale == 4.0f && an.Scale == 6.0f && t.renders == 2;
    if (!ok) { std::printf("004bfa50_TEST FAIL\n"); return 1; }
    std::printf("004bfa50_TEST PASS\n");
    return 0;
}
