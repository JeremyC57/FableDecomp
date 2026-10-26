#include <cstdio>

// Model of CScrollableList::UnhideComponent: all eight states get the position and full alpha, the component slides
// to state 5's position and fades to state 5's colour; an instant move (time <= 0.0001) into an active list raises
// state flag 0x19, anything else clears it.
class C2DVector { public: float X, Y; };
class CRGBColour { public: unsigned char B, G, R, A; };
struct CUIState { unsigned long StateChangeFlag; long StateChangeType; C2DVector Position; unsigned char pad[8]; CRGBColour Colour; };

struct CComponent {
    CUIState states[8];
    C2DVector movedTo; float moveTime; int moves;
    CRGBColour fadedTo; float fadeTime; int fades;
    int setFlag, clearFlag;
    CUIState* GetStateMap() { return states; }
    void ChangePosition(const C2DVector& p, float t, bool) { movedTo = p; moveTime = t; ++moves; }
    void FadeColour(const CRGBColour& c, float t, long) { fadedTo = c; fadeTime = t; ++fades; }
    void SetStateFlag(long f) { setFlag = (int)f; }
    void ClearStateFlag(long f) { clearFlag = (int)f; }
};
struct CScrollableList {
    long Active;
    void UnhideComponent(CComponent* component, float positionTime, float fadeTime, C2DVector position)
    {
        for (unsigned long i = 0; i < 8; ++i)
            component->GetStateMap()[i].Position = position;
        component->ChangePosition(component->GetStateMap()[5].Position, positionTime, false);
        for (unsigned long i = 0; i < 8; ++i)
            component->GetStateMap()[i].Colour.A = 0xff;
        component->FadeColour(component->GetStateMap()[5].Colour, fadeTime, 0);
        if (!(positionTime > 0.0001f) && Active)
            component->SetStateFlag(0x19);
        else
            component->ClearStateFlag(0x19);
    }
};

static bool Run(float t, long active, bool expectSet)
{
    CComponent c;
    for (int i = 0; i < 8; ++i) { c.states[i].Position.X = 0; c.states[i].Position.Y = 0; c.states[i].Colour.A = 0; c.states[i].Colour.B = (unsigned char)i; }
    c.moves = c.fades = 0; c.setFlag = c.clearFlag = 0;
    CScrollableList list; list.Active = active;
    C2DVector p; p.X = 4.0f; p.Y = 8.0f;
    list.UnhideComponent(&c, t, 0.75f, p);
    for (int i = 0; i < 8; ++i)
        if (c.states[i].Position.X != 4.0f || c.states[i].Position.Y != 8.0f || c.states[i].Colour.A != 0xff) return false;
    if (c.moves != 1 || c.movedTo.Y != 8.0f || c.moveTime != t || c.fades != 1 || c.fadedTo.B != 5 || c.fadeTime != 0.75f) return false;
    if (expectSet) return c.setFlag == 0x19 && c.clearFlag == 0;
    return c.clearFlag == 0x19 && c.setFlag == 0;
}

int main()
{
    if (!Run(0.0f, 1, true) || !Run(0.00005f, 1, true) || !Run(0.0f, 0, false) || !Run(0.5f, 1, false) || !Run(0.5f, 0, false))
        { std::printf("005556f0_TEST FAIL\n"); return 1; }
    std::printf("005556f0_TEST PASS\n");
    return 0;
}