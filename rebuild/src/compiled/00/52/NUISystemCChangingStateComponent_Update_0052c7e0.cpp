// NUISystem::CChangingStateComponent::Update(float deltaTime) -- per-frame update of a front-end component that animates between UI states: advance the state machine, apply the target state's reset flags, run the base component update, and notify when the state settled
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0052C7E0, length 140 bytes (ret 4).
// Callees: the base NUISystem::CComponent::Update(float) @ 0x00531EC0 (called directly, non-virtually); virtual
// slots 49 (+0xc4, advances the state machine and returns whether anything changed), 135 (+0x21c, the CUIState
// record of a state id, null when absent) and 136 (+0x220, called when the update changed nothing).
// Layout: the PDB CComponent / CChangingStateComponent offsets hold up to +0x9c in retail (TargetColour +0x90,
// InitialPosition +0x4c, InitialZoom +0x74); the derived members sit 0x30 bytes lower than the PDB says (retail
// STL containers are smaller): TargetState +0x148 (PDB +0x178), PreviousUpdateChanged +0x154 (PDB +0x184).
// Shape: the three flag tests re-read the state record's first byte each time (the member stores in between may
// alias it); the 0x10 flag stores 0xff into the four colour bytes through one register (byte members), the 0x20 flag
// zeroes InitialPosition with immediate float stores Y first, the 0x40 flag sets InitialZoom to (1, 1) Y first
// through one eax load: a chained `X = Y = 1.0f` (separate stores would be two immediates; a C2DVector(x, y)
// temporary goes through an 8-byte frame).
struct CUIState {
    unsigned char Flags;                      // +0x00 (bit 0x10 colour, 0x20 position, 0x40 zoom resets)
};
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
class CRGBColour { public:
    unsigned char B, G, R, A;
};

namespace NUISystem {

class CComponent {
public:
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual bool UpdateStates();                         // slot 49 (+0xc4)
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual void v077();
    virtual void v078();
    virtual void v079();
    virtual void v080();
    virtual void v081();
    virtual void v082();
    virtual void v083();
    virtual void v084();
    virtual void v085();
    virtual void v086();
    virtual void v087();
    virtual void v088();
    virtual void v089();
    virtual void v090();
    virtual void v091();
    virtual void v092();
    virtual void v093();
    virtual void v094();
    virtual void v095();
    virtual void v096();
    virtual void v097();
    virtual void v098();
    virtual void v099();
    virtual void v100();
    virtual void v101();
    virtual void v102();
    virtual void v103();
    virtual void v104();
    virtual void v105();
    virtual void v106();
    virtual void v107();
    virtual void v108();
    virtual void v109();
    virtual void v110();
    virtual void v111();
    virtual void v112();
    virtual void v113();
    virtual void v114();
    virtual void v115();
    virtual void v116();
    virtual void v117();
    virtual void v118();
    virtual void v119();
    virtual void v120();
    virtual void v121();
    virtual void v122();
    virtual void v123();
    virtual void v124();
    virtual void v125();
    virtual void v126();
    virtual void v127();
    virtual void v128();
    virtual void v129();
    virtual void v130();
    virtual void v131();
    virtual void v132();
    virtual void v133();
    virtual void v134();
    virtual const CUIState* GetState(unsigned long state); // slot 135 (+0x21c)
    virtual void OnStatesSettled();                      // slot 136 (+0x220)
    unsigned char _pad04[0x48];
    C2DVector     InitialPosition;            // +0x4c
    unsigned char _pad54[0x20];
    C2DVector     InitialZoom;                // +0x74
    unsigned char _pad7c[0x14];
    CRGBColour    TargetColour;               // +0x90
    unsigned char _pad94[0xb4];
    void Update(float deltaTime);             // 0x00531EC0
};

class CChangingStateComponent : public CComponent {
public:
    unsigned long TargetState;                // +0x148
    unsigned long RequestedState;             // +0x14c
    float         UpdateTime;                 // +0x150
    bool          PreviousUpdateChanged;      // +0x154
    void Update(float deltaTime);
};

void CChangingStateComponent::Update(float deltaTime)
{
    PreviousUpdateChanged = UpdateStates();
    const CUIState* state = GetState(TargetState);
    if (state) {
        if (state->Flags & 0x10) {
            TargetColour.B = 0xff;
            TargetColour.G = 0xff;
            TargetColour.R = 0xff;
            TargetColour.A = 0xff;
        }
        if (state->Flags & 0x20) {
            InitialPosition.Y = 0.0f;
            InitialPosition.X = 0.0f;
        }
        if (state->Flags & 0x40)
            InitialZoom.X = InitialZoom.Y = 1.0f;
    }
    CComponent::Update(deltaTime);
    if (!PreviousUpdateChanged)
        OnStatesSettled();
}

}