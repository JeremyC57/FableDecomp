// NUISystem::CScrollBar::Update(float deltaTime) -- when a drag ended without a left click held, snap the bar back to its pre-drag position immediately and raise its layer; then the base update
// Retail: Fable.exe (TLC, VC7.1) @ 0x00559600, 70 bytes (vtable 0x0124B7E4 slot 1 = NUISystem::CScrollBar).
// Callees: ChangePosition (primary vtable slot 31, +0x7C = 0x0055A630: `(const C2DVector&, float time,
// bool)`, called with PositionBeforeDrag, 0.0f, false), the base CDraggable/CClickable::Update(float)
// @ 0x0055ACB0 (CDraggable does not override it).
//
// Shape: two `mov al,[..] ; test al,al` bool tests (WasDragged, then !WasLeftClicked), the vtable fetch and
// the three pushes scheduled above the `WasDragged = false` byte store, `inc byte ptr` = `++Layer`.
// Retail NUISystem layout (Ego_r.pdb minus 0x28 inside CComponent, minus 0x30 from CChangingStateComponent
// on): CComponent::Layer +0x12F (PDB +0x157), CClickable::WasLeftClicked +0x170 (PDB +0x1A0),
// CDraggable::PositionBeforeDrag +0x194 / WasDragged +0x19C (PDB +0x1C4 / +0x1CC). No engine header exists
// at retail offsets; the chain is modelled as in the landed CDraggable constructor (0x0055A9C0).
// Leaf: ?Update@CScrollBar

namespace NUISystem {

struct C2DVector {
    float X, Y;
};

struct CComponentInterface {                     // primary vtable at +0
    virtual void v000();
    virtual void Update(float deltaTime);        // slot 1
    virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016();
    virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021();
    virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026();
    virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030();
    virtual void ChangePosition(const C2DVector& position, float time, bool relative);   // slot 31 (+0x7C)
};

struct CObserver_Retail {                        // vtable at +4, then its 0x10 bytes of state
    virtual void ProcessEvent();
    unsigned char _body[0x10];
};

struct CEventSinkInterface {                     // vtable at +0x18
    virtual void v0();
};

struct CDraggable_Retail : CComponentInterface, CObserver_Retail, CEventSinkInterface {   // retail 0x1AC bytes
    unsigned char _pad_0x1c[0x12f - 0x1c];
    char          Layer;                         // +0x12F CComponent::Layer
    unsigned char _pad_0x130[0x170 - 0x130];
    bool          WasLeftClicked;                // +0x170 CClickable
    unsigned char _pad_0x171[0x194 - 0x171];
    C2DVector     PositionBeforeDrag;            // +0x194 CDraggable
    bool          WasDragged;                    // +0x19C
    unsigned char _pad_0x19d[0x1ac - 0x19d];
    virtual void Update(float deltaTime);        // CClickable::Update 0x0055ACB0
};

class CScrollBar : public CDraggable_Retail { public:
    virtual void Update(float deltaTime);
};

void CScrollBar::Update(float deltaTime)
{
    if (WasDragged && !WasLeftClicked) {
        WasDragged = false;
        ChangePosition(PositionBeforeDrag, 0.0f, false);
        ++Layer;
    }
    CDraggable_Retail::Update(deltaTime);
}

}