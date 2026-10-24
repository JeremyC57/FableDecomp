// NUISystem::CScrollingComponent::ChangeState(unsigned long state) -- the state-component change; state 5 then moves on to state 1 and state 6 to state 0
// Leaf: ChangeState
// Retail: Fable.exe (TLC, VC7.1) @ 0x00547010, 56 bytes (`ret 4`).
// Callees: NUISystem::CChangingStateComponent::ChangeState(unsigned long) @ 0x0052CF40, own virtual slot 48 (+0xc0, this
// same ChangeState through the vtable).
// Label: CScrollingComponent vtable 0x012482CC slot 48 (rtti-vtables.tsv); Ego_r.pdb names
// ?ChangeState@CScrollingComponent@NUISystem@@UAEXK@Z.
// Shape: the state kept in edi across the base call; two `if`s, each virtual call with its own `ret 4`.
namespace NUISystem {
class CChangingStateComponent {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void ChangeState(unsigned long state);   // slot 48 (+0xc0); 0x0052CF40
};

class CScrollingComponent : public CChangingStateComponent {
public:
    virtual void ChangeState(unsigned long state);
};

void CScrollingComponent::ChangeState(unsigned long state)
{
    CChangingStateComponent::ChangeState(state);
    if (state == 5)
        ChangeState(1);
    else if (state == 6)
        ChangeState(0);
}
}