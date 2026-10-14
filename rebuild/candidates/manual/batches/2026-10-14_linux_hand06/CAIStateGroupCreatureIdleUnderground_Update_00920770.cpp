// CAIStateGroup_CreatureIdleUnderground::Update()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00920770, length 23 bytes.
// Callees: CAIStateGroupBase::AIAction_DoNothing @ 0x0097E200 (returns EAIActionReturn),
// CAIStateGroupBase::Terminate @ 0x008FCF40 (sets Terminated at +0x10; tail-called).
// Spins the idle action while it reports AIACTION_RUNNING (4), then terminates the group.
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    long AIAction_DoNothing();              // 0x0097E200
    void Terminate();                       // 0x008FCF40
};

struct CAIStateGroup_CreatureIdleUnderground : CAIStateGroupBase {
    virtual void Update();
};

void CAIStateGroup_CreatureIdleUnderground::Update()
{
    while (AIAction_DoNothing() == 4) {
    }
    Terminate();
}
