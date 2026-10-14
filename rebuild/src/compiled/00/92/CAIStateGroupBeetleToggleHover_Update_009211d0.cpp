// CAIStateGroup_BeetleToggleHover::Update() -- land or take off, then terminate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009211D0, length 84 bytes.
// Callees: CCreatureAction_BeetleLand::CCreatureAction_BeetleLand(CThingCreatureBase*) @ 0x00982C70
// (vtable 0x01288488) when IsLanding (+0x14, PDB name), else CCreatureAction_BeetleTakeOff
// @ 0x00982C10 (vtable 0x012884A0); CAIStateGroupBase::AIAction_PerformActionAndWait
// (CCreatureActionBase*, bool) @ 0x0097DB70, CCreatureActionBase::~CCreatureActionBase @ 0x0062E900,
// Terminate @ 0x008FCF40.
// Both actions are temporaries of the same size sharing one frame slot (0x114 bytes): `false` is
// pushed before the choice, each branch builds its temporary and the common tail (call, base
// destructor, Terminate) is merged by the compiler. The derived actions add no destructor. Each
// branch reads the creature into its own named local (that is what gives retail's eax/eax and
// ecx/eax load pairs; the bare expression allocates differently).
struct CThingCreatureBase;

struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0x110];             // +0x4..+0x113
    ~CCreatureActionBase();                 // 0x0062E900
};

struct CCreatureAction_BeetleLand : CCreatureActionBase {
    CCreatureAction_BeetleLand(CThingCreatureBase* creature);      // 0x00982C70
};

struct CCreatureAction_BeetleTakeOff : CCreatureActionBase {
    CCreatureAction_BeetleTakeOff(CThingCreatureBase* creature);   // 0x00982C10
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

struct CAIStateGroup_BeetleToggleHover : CAIStateGroupBase {
    bool IsLanding;                         // +0x14
    virtual void Update();
};

void CAIStateGroup_BeetleToggleHover::Update()
{
    if (IsLanding) {
        CThingCreatureBase* creature = PBrain->Creature;
        AIAction_PerformActionAndWait(&CCreatureAction_BeetleLand(creature), false);
    } else {
        CThingCreatureBase* creature = PBrain->Creature;
        AIAction_PerformActionAndWait(&CCreatureAction_BeetleTakeOff(creature), false);
    }
    Terminate();
}