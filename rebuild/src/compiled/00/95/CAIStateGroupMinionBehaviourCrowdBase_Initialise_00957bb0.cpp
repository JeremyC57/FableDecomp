// CAIStateGroup_MinionBehaviourCrowdBase::Initialise(CAIBrain&, CThingAICreature&, long, EScriptAIPriority,
// const CCharString&) -- clear the crowd flag, run the leaf initialiser hook, then the base initialisation.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00957BB0, length 48 bytes, `ret 14h`.
// Callees: the virtual at slot 17 (+0x44) on `this` (the crowd leaf initialiser; rtti-vtables lists only slots
// 0/1/9 of vtable 0x0128C494, so the slot is named descriptively), then CAIStateGroupBase::Initialise
// @ 0x008FCEC0 with the five arguments forwarded.
// Shape: the vtable load is hoisted above the `mov byte ptr [esi+38h],0` store (scheduling), then the five
// argument re-loads/pushes. No engine header for this class; the layout is local (flag at +0x38 descriptive).
struct CAIBrain;
struct CThingAICreature;
struct CCharString;

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long id, long priority, const CCharString& name);   // slot 1, 0x008FCEC0
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08();
    virtual void Cleanup();                 // slot 9
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15();
    virtual void CleanupCrowd();            // slot 16 (+0x40)
    virtual void InitialiseCrowd();         // slot 17 (+0x44)
    unsigned char _pad[0x10];               // +0x04..+0x13
};

struct CAIStateGroup_MinionBehaviourCrowdBase : CAIStateGroupBase {
    unsigned char _pad14[0x24];             // +0x14 behaviour base + crowd state
    bool m_inCrowd;                         // +0x38
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long id, long priority, const CCharString& name);
};

void CAIStateGroup_MinionBehaviourCrowdBase::Initialise(CAIBrain& brain, CThingAICreature& creature, long id, long priority, const CCharString& name)
{
    m_inCrowd = false;
    InitialiseCrowd();
    CAIStateGroupBase::Initialise(brain, creature, id, priority, name);
}