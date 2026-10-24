// NUISystem::CScrollingComponent::Update(float deltaTime) -- the state-component update; when the state changed last update, bounce between states 1 and 3
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) @ 0x00546FC0, 72 bytes (`ret 4`).
// Callees: NUISystem::CChangingStateComponent::Update(float) @ 0x0052C7E0 (landed), own virtual slots 137 (+0x224,
// ChangedStateLastUpdate(), 0x0041C5E0 in this vtable) and 48 (+0xc0, ChangeState(unsigned long)).
// Label: CScrollingComponent vtable 0x012482CC slot 1 (rtti-vtables.tsv); Ego_r.pdb names
// ?Update@CScrollingComponent@NUISystem@@UAEXM@Z.
// Layout: CurrentState at retail +0x144 (landed CChangingStateComponent layout, 0x0052C8B0).
// Shape: a two-case switch on the current state compiled as `dec ; je` / `sub 2 ; jne`, each case a virtual call with its
// own `ret 4`.
namespace NUISystem {
class CChangingStateComponent {
public:
    virtual void v000();
    virtual void Update(float deltaTime);         // slot 1; 0x0052C7E0
    virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025();
    virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033();
    virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void ChangeState(unsigned long state);   // slot 48 (+0xc0)
    virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056();
    virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072();
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080();
    virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096();
    virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104();
    virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112();
    virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120();
    virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128();
    virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136();
    virtual bool ChangedStateLastUpdate();        // slot 137 (+0x224)
    unsigned char _pad04[0x144 - 4];
    unsigned long CurrentState;                   // +0x144
};

class CScrollingComponent : public CChangingStateComponent {
public:
    virtual void Update(float deltaTime);
};

void CScrollingComponent::Update(float deltaTime)
{
    CChangingStateComponent::Update(deltaTime);
    if (ChangedStateLastUpdate()) {
        switch (CurrentState) {
        case 1:
            ChangeState(3);
            break;
        case 3:
            ChangeState(1);
            break;
        }
    }
}
}