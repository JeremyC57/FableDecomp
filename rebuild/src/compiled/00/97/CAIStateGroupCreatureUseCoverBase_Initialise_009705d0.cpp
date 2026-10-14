// CAIStateGroup_CreatureUseCoverBase::Initialise(CAIBrain&, CThingAICreature&, long, EScriptAIPriority, const CCharString&)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009705D0, length 44 bytes (+ int3 padding).
// Callees: CAIStateGroupBase::Initialise @ 0x008FCEC0 (five arguments forwarded), then the
// class's own virtual at slot 14 (+0x38) on `this` -- the leaf initialiser hook that the
// CreatureUseCoverBase family adds below CAIStateGroupBase's 14 slots (sibling of CleanupLeaf at
// slot 17); named Init here. The base is modelled with the 14 retail slots so the
// hook lands at +0x38.
struct CAIBrain;
struct CThingAICreature;
struct CCharString;
typedef long EScriptAIPriority;

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name);   // 0x008FCEC0
    virtual void OnActivate();
    virtual void InitCreaturePosition();
    virtual bool AllowPriorityOverlap();
    virtual bool IsStillValid();
    virtual const char* GetDebugText();
    virtual bool IsTimingCritical();
    virtual long GetStateGroupType();
    virtual void Cleanup();
    virtual void OnReplace();
    virtual bool IsRunnable();
    virtual void Update();
    virtual void InitStateGroup(CAIBrain& brain, CThingAICreature& creature);
    unsigned char _pad[0x10];               // +0x4..+0x13
};

struct CAIStateGroup_CreatureUseCoverBase : CAIStateGroupBase {
    unsigned char PTargetThing[8];          // +0x14 CIntelligentPointer<CThingCreatureBase>
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name);
    virtual void Init();          // slot 14 (+0x38)
};

void CAIStateGroup_CreatureUseCoverBase::Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name)
{
    CAIStateGroupBase::Initialise(brain, creature, priority, scriptPriority, name);
    Init();
}