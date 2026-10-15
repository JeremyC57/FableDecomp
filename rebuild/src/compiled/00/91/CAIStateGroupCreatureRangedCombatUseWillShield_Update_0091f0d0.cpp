// CAIStateGroup_CreatureRangedCombatUseWillShield::Update() -- casts the physical shield spell (and waits for it), then terminates the state group
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091F0D0, length 97 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 on "" @ 0x0122D70E,
// CCreatureAction_CastPhysicalShieldSpell::CCreatureAction_CastPhysicalShieldSpell(CThingCreatureBase*, long, bool,
// const CCharString&, long, float) @ 0x008BB6F0 (`ret 18h`), CAIStateGroupBase::AIAction_PerformActionAndWait
// (CCreatureActionBase*, bool) @ 0x0097DB70, the action base destructor @ 0x0062E900, CAIStateGroupBase::Terminate
// @ 0x008FCF40 (Terminated = true).
// Shape: the empty name is a NAMED local in an inner scope (built before anything is pushed, destroyed after the
// action temporary and before Terminate), the creature a named local after it (eax chain; inline gives ecx/edx);
// the action is a temporary inside the call (the landed 0x00933BB0
// shape: `false` pushed first, then the constructor arguments right to left with 0.0f as `push 0`, the
// constructor's returned `this` pushed as the action, the base destructor after the call). Frame 0x128 = the
// string (+0) and the 0x124-byte retail action (+4).
// Model: local (engine/CAIStateGroup_CreatureRangedCombatUseWillShield.h is a flattened base blob).
struct CThingCreatureBase;

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0x120];                             // +0x04..+0x123
    ~CCreatureActionBase();                                 // 0x0062E900
};

struct CCreatureAction_CastPhysicalShieldSpell : CCreatureActionBase {
    CCreatureAction_CastPhysicalShieldSpell(CThingCreatureBase* caster, long spellLevel, bool force,
        const CCharString& soundName, long repeat, float duration);            // 0x008BB6F0
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* Creature;                           // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                                       // +0x04
    long ScriptAIPriority;                                  // +0x08
    void* Name_PStringData;                                 // +0x0c
    unsigned char Terminated;                               // +0x10
    unsigned char Priority;                                 // +0x11
    unsigned char Enabled;                                  // +0x12
    unsigned char _pad_0x13;
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    void Terminate();                                       // 0x008FCF40
};

struct CAIStateGroup_CreatureRangedCombatUseWillShield : CAIStateGroupBase {
    virtual void Update();
};

void CAIStateGroup_CreatureRangedCombatUseWillShield::Update()
{
    {
        CCharString soundName("", -1);
        CThingCreatureBase* creature = PBrain->Creature;
        AIAction_PerformActionAndWait(&CCreatureAction_CastPhysicalShieldSpell(creature, 2, true, soundName, 5, 0.0f), false);
    }
    Terminate();
}