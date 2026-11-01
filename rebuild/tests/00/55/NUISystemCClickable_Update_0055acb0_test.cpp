// Test for 0x0055ACB0 NUISystem::CClickable::Update: while not hovered the click latches are cleared; then CHoverable::Update.
#include <cstdio>
static int g_hoverUpdates = 0; static float g_lastDelta = 0.0f;
namespace NUISystem {
class CHoverable { public:
    virtual ~CHoverable() { }
    virtual void Update(float deltaTime) { ++g_hoverUpdates; g_lastDelta = deltaTime; }
    long StateBeforeHovered; bool WasHovered;
};
class CClickable : public CHoverable { public:
    virtual void Update(float deltaTime);
    long StateBeforeLeftClicked; bool WasLeftClicked; long StateBeforeRightClicked; bool WasRightClicked;
};
void CClickable::Update(float deltaTime)
{
    if (!WasHovered && WasLeftClicked) WasLeftClicked = false;
    if (!WasHovered && WasRightClicked) WasRightClicked = false;
    CHoverable::Update(deltaTime);
}
}
int main()
{
    NUISystem::CClickable c;
    c.WasHovered = true; c.WasLeftClicked = true; c.WasRightClicked = true;
    NUISystem::CHoverable* h = &c; h->Update(0.5f);
    if (!c.WasLeftClicked || !c.WasRightClicked || g_hoverUpdates != 1 || g_lastDelta != 0.5f) { std::printf("0055acb0_TEST FAIL\n"); return 1; }
    c.WasHovered = false; h->Update(0.25f);
    if (c.WasLeftClicked || c.WasRightClicked || g_hoverUpdates != 2 || g_lastDelta != 0.25f) { std::printf("0055acb0_TEST FAIL\n"); return 1; }
    c.WasLeftClicked = false; c.WasRightClicked = true; h->Update(0.0f);
    if (c.WasLeftClicked || c.WasRightClicked || g_hoverUpdates != 3) { std::printf("0055acb0_TEST FAIL\n"); return 1; }
    std::printf("0055acb0_TEST PASS\n");
    return 0;
}