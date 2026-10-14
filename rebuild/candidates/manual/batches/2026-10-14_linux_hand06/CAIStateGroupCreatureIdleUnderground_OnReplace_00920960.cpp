// CAIStateGroup_CreatureIdleUnderground::OnReplace() -- make the creature rise up when the group is replaced
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00920960, length 48 bytes.
// Callees: CCreatureAction_RiseUp::CCreatureAction_RiseUp(CThingCreatureBase*) @ 0x008BD880,
// CThingCreatureBase::SetCurrentAction(CCreatureActionBase*) @ 0x006644F0,
// CCreatureActionBase::~CCreatureActionBase @ 0x0062E900.
// The creature (PBrain +0x4 -> Creature +0x18) is read once into esi: it is both the
// constructor argument and the object of SetCurrentAction. The action is a temporary in the
// call expression (address taken, VC extension C4238), destroyed through the base destructor
// after the call; it is the only frame slot (0x118 bytes).
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0x114];             // +0x4..+0x117
    ~CCreatureActionBase();                 // 0x0062E900
};

struct CThingCreatureBase {
    bool SetCurrentAction(CCreatureActionBase* action);   // 0x006644F0
};

struct CCreatureAction_RiseUp : CCreatureActionBase {
    CCreatureAction_RiseUp(CThingCreatureBase* creature);   // 0x008BD880
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* Creature;           // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                       // +0x04
    unsigned char _pad[0xc];                // +0x08..+0x13
};

struct CAIStateGroup_CreatureIdleUnderground : CAIStateGroupBase {
    virtual void OnReplace();
};

void CAIStateGroup_CreatureIdleUnderground::OnReplace()
{
    CThingCreatureBase* creature = PBrain->Creature;
    creature->SetCurrentAction(&CCreatureAction_RiseUp(creature));
}
