#include <cstdio>

// Model of CScrollableList::HideComponent: every one of the eight UI states gets the parking position and the alpha,
// then the component slides to state 6's position, fades to state 6's colour and refreshes its states.
struct C2DVector { float X, Y; };
struct CRGBColour { unsigned char B, G, R, A; };
struct CUIState { unsigned long StateChangeFlag; long StateChangeType; C2DVector Position; unsigned char pad[8]; CRGBColour Colour; };

struct CComponent {
    CUIState states[8];
    C2DVector movedTo; float moveTime; bool moveRelative; int moves;
    CRGBColour fadedTo; float fadeTime; long fadeFlags; int fades;
    int updates;
    CUIState* GetStateMap() { return states; }
    void ChangePosition(const C2DVector& p, float t, bool rel) { movedTo = p; moveTime = t; moveRelative = rel; ++moves; }
    void FadeColour(const CRGBColour& c, float t, long f) { fadedTo = c; fadeTime = t; fadeFlags = f; ++fades; }
    void UpdateStates() { ++updates; }
};

static void HideComponent(CComponent* component, float positionTime, float fadeTime, C2DVector position, unsigned char alpha)
{
    for (unsigned long i = 0; i < 8; ++i)
        component->GetStateMap()[i].Position = position;
    component->ChangePosition(component->GetStateMap()[6].Position, positionTime, false);
    for (unsigned long i = 0; i < 8; ++i)
        component->GetStateMap()[i].Colour.A = alpha;
    component->FadeColour(component->GetStateMap()[6].Colour, fadeTime, 0);
    component->UpdateStates();
}

int main()
{
    CComponent c;
    for (int i = 0; i < 8; ++i) {
        c.states[i].Position.X = (float)i; c.states[i].Position.Y = (float)-i;
        c.states[i].Colour.B = (unsigned char)i; c.states[i].Colour.G = 1; c.states[i].Colour.R = 2; c.states[i].Colour.A = 0xff;
    }
    c.moves = c.fades = c.updates = 0;
    C2DVector p; p.X = 12.5f; p.Y = -3.0f;
    HideComponent(&c, 0.25f, 0.5f, p, 0x40);
    for (int i = 0; i < 8; ++i)
        if (c.states[i].Position.X != 12.5f || c.states[i].Position.Y != -3.0f || c.states[i].Colour.A != 0x40 || c.states[i].Colour.B != (unsigned char)i)
            { std::printf("00555440_TEST FAIL\n"); return 1; }
    if (c.moves != 1 || c.movedTo.X != 12.5f || c.moveTime != 0.25f || c.moveRelative) { std::printf("00555440_TEST FAIL\n"); return 1; }
    if (c.fades != 1 || c.fadedTo.A != 0x40 || c.fadedTo.B != 6 || c.fadeTime != 0.5f || c.fadeFlags != 0) { std::printf("00555440_TEST FAIL\n"); return 1; }
    if (c.updates != 1) { std::printf("00555440_TEST FAIL\n"); return 1; }
    std::printf("00555440_TEST PASS\n");
    return 0;
}
