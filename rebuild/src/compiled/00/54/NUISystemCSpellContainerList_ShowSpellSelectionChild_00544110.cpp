// NUISystem::CSpellContainerList::ShowSpellSelectionChild() -- fade the spell selection and spell indication children in (alpha 255 over 0.2 s)
// Leaf: ShowSpellSelectionChild
// Retail: Fable.exe (TLC, VC7.1) @ 0x00544110, 113 bytes (plain `ret`).
// Callees: each child's virtual slots 14 (+0x38, GetColour() const, the CRGBColour returned through the hidden pointer into
// the `push ecx` slot) and 38 (+0x98, ChangeColour(const CRGBColour&, float time, bool)).
// Label: CSpellContainerList vtable 0x012475C4 slot 145 (rtti-vtables.tsv); Ego_r.pdb names
// ?ShowSpellSelectionChild@CSpellContainerList@NUISystem@@UAEXXZ (slot 146, 0x00544190, is HideSpellSelectionChild: the same
// body with alpha 0) and ?ChangeColour@CComponent@NUISystem@@UAEXABVCRGBColour@@M_N@Z.
// Layout: Ego_r.pdb SpellSelectionChild +0x198 / SpellIndicationChild +0x19c; retail 0x30 lower (+0x168, +0x16c).
// CRGBColour is B, G, R, A bytes with an empty user constructor (the landed CUIState 0x0052E5D0 model), so GetColour returns it
// through a hidden pointer.
// Shape: per child: null test, the colour fetched into the frame word, the member re-read for the second call, the alpha
// byte stored after the two argument pushes.
namespace NUISystem {
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour() { }
};
class CComponent {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013();
    virtual CRGBColour GetColour() const;                                        // slot 14 (+0x38)
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022();
    virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030();
    virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037();
    virtual void ChangeColour(const CRGBColour& colour, float time, bool flag);  // slot 38 (+0x98)
};
class CSpellContainerList {
public:
    virtual void v000();
    unsigned char _pad04[0x168 - 4];
    CComponent* SpellSelectionChild;              // +0x168
    CComponent* SpellIndicationChild;             // +0x16c
    void ShowSpellSelectionChild();
};

void CSpellContainerList::ShowSpellSelectionChild()
{
    if (SpellSelectionChild) {
        CRGBColour colour = SpellSelectionChild->GetColour();
        colour.A = 0xff;
        SpellSelectionChild->ChangeColour(colour, 0.2f, false);
    }
    if (SpellIndicationChild) {
        CRGBColour colour = SpellIndicationChild->GetColour();
        colour.A = 0xff;
        SpellIndicationChild->ChangeColour(colour, 0.2f, false);
    }
}
}