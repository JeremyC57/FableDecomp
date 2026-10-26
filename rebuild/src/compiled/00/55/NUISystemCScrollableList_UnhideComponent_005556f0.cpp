// NUISystem::CScrollableList::UnhideComponent(CComponent*, float, float, C2DVector) -- bring a list component back: set the position of all eight of its UI states to the given point and slide it there, make every state fully opaque and fade to it, then raise (instant move into an active list) or clear the component's state flag 0x19
// Leaf: UnhideComponent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005556F0, length 736 bytes, ret 0x14. Mirror of HideComponent 0x00555440.
// Callees: the component's virtual GetStateMap (slot 82, +0x148), ChangePosition (slot 31, +0x7c), FadeColour (slot 38,
// +0x98) and its CStateHolder base (+4) slots 3/4 (+0x0c/+0x10); STLport map<unsigned long, CUIState>::operator[] 0x0042D5B1.
// Layout: CUIState Position +0x08, Colour +0x18 (alpha byte +0x1b); the list's flag word +0x14c.
// Threshold: 0.0001f (0x0129BA3C).
namespace NUISystem {
struct C2DVector {
    float X, Y;
};
struct CRGBColour {
    unsigned char B, G, R, A;
};
class CUIState { public:
    unsigned long StateChangeFlag;   // +0x00
    long          StateChangeType;   // +0x04
    C2DVector     Position;          // +0x08
    unsigned char _pad10[0x8];       // +0x10
    CRGBColour    Colour;            // +0x18
};
struct CStateMap {                   // STLport map<unsigned long, CUIState> (12 bytes)
    void* Header; unsigned long NodeCount; unsigned long Compare;
    CUIState& operator[](const unsigned long& key);   // 0x0042D5B1
};
struct CComponentBase {
    virtual void c000(); virtual void c001(); virtual void c002(); virtual void c003(); virtual void c004();
    virtual void c005(); virtual void c006(); virtual void c007(); virtual void c008(); virtual void c009();
    virtual void c010(); virtual void c011(); virtual void c012(); virtual void c013(); virtual void c014();
    virtual void c015(); virtual void c016(); virtual void c017(); virtual void c018(); virtual void c019();
    virtual void c020(); virtual void c021(); virtual void c022(); virtual void c023(); virtual void c024();
    virtual void c025(); virtual void c026(); virtual void c027(); virtual void c028(); virtual void c029();
    virtual void c030();
    virtual void ChangePosition(const C2DVector& position, float time, bool relative);   // slot 31 (+0x7c)
    virtual void c032(); virtual void c033(); virtual void c034(); virtual void c035(); virtual void c036();
    virtual void c037();
    virtual void FadeColour(const CRGBColour& colour, float time, long flags);         // slot 38 (+0x98)
    virtual void c039(); virtual void c040(); virtual void c041(); virtual void c042(); virtual void c043();
    virtual void c044(); virtual void c045(); virtual void c046(); virtual void c047(); virtual void c048();
    virtual void c049(); virtual void c050(); virtual void c051(); virtual void c052(); virtual void c053();
    virtual void c054(); virtual void c055(); virtual void c056(); virtual void c057(); virtual void c058();
    virtual void c059(); virtual void c060(); virtual void c061(); virtual void c062(); virtual void c063();
    virtual void c064(); virtual void c065(); virtual void c066(); virtual void c067(); virtual void c068();
    virtual void c069(); virtual void c070(); virtual void c071(); virtual void c072(); virtual void c073();
    virtual void c074(); virtual void c075(); virtual void c076(); virtual void c077(); virtual void c078();
    virtual void c079(); virtual void c080(); virtual void c081();
    virtual CStateMap& GetStateMap();                                                  // slot 82 (+0x148)
};
struct CStateHolder {                // the component's second base (+4)
    virtual void s00(); virtual void s01(); virtual void s02();
    virtual void SetStateFlag(long flag);     // slot 3 (+0x0c)
    virtual void ClearStateFlag(long flag);   // slot 4 (+0x10)
    virtual void UpdateStates();     // slot 5 (+0x14)
};
class CComponent : public CComponentBase, public CStateHolder { public:
};
class CScrollableList { public:
    unsigned char _pad000[0x14c];
    long          Active;            // +0x14c
    void UnhideComponent(CComponent* component, float positionTime, float fadeTime, C2DVector position);
};

void CScrollableList::UnhideComponent(CComponent* component, float positionTime, float fadeTime, C2DVector position)
{
    component->GetStateMap()[0].Position = position;
    component->GetStateMap()[1].Position = position;
    component->GetStateMap()[2].Position = position;
    component->GetStateMap()[3].Position = position;
    component->GetStateMap()[4].Position = position;
    component->GetStateMap()[5].Position = position;
    component->GetStateMap()[6].Position = position;
    component->GetStateMap()[7].Position = position;
    component->ChangePosition(component->GetStateMap()[5].Position, positionTime, false);
    component->GetStateMap()[0].Colour.A = 0xff;
    component->GetStateMap()[1].Colour.A = 0xff;
    component->GetStateMap()[2].Colour.A = 0xff;
    component->GetStateMap()[3].Colour.A = 0xff;
    component->GetStateMap()[4].Colour.A = 0xff;
    component->GetStateMap()[5].Colour.A = 0xff;
    component->GetStateMap()[6].Colour.A = 0xff;
    component->GetStateMap()[7].Colour.A = 0xff;
    component->FadeColour(component->GetStateMap()[5].Colour, fadeTime, 0);
    if (!(positionTime > 0.0001f) && Active)
        component->SetStateFlag(0x19);
    else
        component->ClearStateFlag(0x19);
}

}