#include <cstdio>
struct CRGBColour { unsigned char B, G, R, A; };
struct CComponent {
    CRGBColour Colour; float Time; bool Flag; int Calls;
    CRGBColour GetColour() const { return Colour; }
    void ChangeColour(const CRGBColour& c, float t, bool f) { Colour = c; Time = t; Flag = f; ++Calls; }
};
struct CSpellContainerList {
    CComponent* SpellSelectionChild; CComponent* SpellIndicationChild;
    void HideSpellSelectionChild()
    {
        if (SpellSelectionChild) { CRGBColour c = SpellSelectionChild->GetColour(); c.A = 0; SpellSelectionChild->ChangeColour(c, 0.2f, false); }
        if (SpellIndicationChild) { CRGBColour c = SpellIndicationChild->GetColour(); c.A = 0; SpellIndicationChild->ChangeColour(c, 0.2f, false); }
    }
};
int main()
{
    CComponent a; a.Colour.B = 1; a.Colour.G = 2; a.Colour.R = 3; a.Colour.A = 0x80; a.Calls = 0; a.Flag = true;
    CComponent b = a;
    CSpellContainerList l; l.SpellSelectionChild = &a; l.SpellIndicationChild = &b;
    l.HideSpellSelectionChild();
    if (a.Colour.A != 0 || a.Colour.R != 3 || a.Colour.B != 1 || a.Time != 0.2f || a.Flag || a.Calls != 1 || b.Colour.A != 0 || b.Calls != 1)
        { std::printf("00544190_TEST FAIL\n"); return 1; }
    l.SpellSelectionChild = 0; l.HideSpellSelectionChild();
    if (a.Calls != 1 || b.Calls != 2) { std::printf("00544190_TEST FAIL\n"); return 1; }
    std::printf("00544190_TEST PASS\n");
    return 0;
}