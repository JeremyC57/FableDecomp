// CAIStateGroup_CreatureGoUnderground::Update() -- perform the go-underground action, then terminate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00920730, length 59 bytes (+ int3 padding).
// Callees: CCreatureAction_GoUnderground::CCreatureAction_GoUnderground(CThingCreatureBase*)
// @ 0x008EC6A0, CAIStateGroupBase::AIAction_PerformActionAndWait(CCreatureActionBase*, bool)
// @ 0x0097DB70, CCreatureActionBase::~CCreatureActionBase @ 0x0062E900, Terminate @ 0x008FCF40.
// The action is a temporary inside the call expression: `false` is pushed before the temporary
// is constructed (VC evaluates right to left), the constructor's returned `this` is pushed as
// the first argument, and the base destructor runs after the call (the derived class adds no
// destructor, so only CCreatureActionBase's out-of-line one is called). The frame is exactly
// the temporary (0x114 bytes); the creature is read into a named local first (that is what keeps
// it in eax rather than ecx for the push). Taking a temporary's address is the VC extension the retail
// code relied on (C4238).
struct CThingCreatureBase;

struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0x110];             // +0x4..+0x113
    ~CCreatureActionBase();                 // 0x0062E900
};

struct CCreatureAction_GoUnderground : CCreatureActionBase {
    CCreatureAction_GoUnderground(CThingCreatureBase* creature);    // 0x008EC6A0
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* Creature;           // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                       // +0x04
    unsigned char _pad[0xc];                // +0x08..+0x13
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    void Terminate();                       // 0x008FCF40
};

struct CAIStateGroup_CreatureGoUnderground : CAIStateGroupBase {
    virtual void Update();
};

void CAIStateGroup_CreatureGoUnderground::Update()
{
    CThingCreatureBase* creature = PBrain->Creature;
    AIAction_PerformActionAndWait(&CCreatureAction_GoUnderground(creature), false);
    Terminate();
}