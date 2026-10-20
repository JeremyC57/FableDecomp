// CCombatSequence_WhisperAdvance::OnCreate() -- queue strafe-backward, strafe-forward and an interruptable mid attack, then the base OnCreate
// Leaf: ?OnCreate@CCombatSequence_WhisperAdvance
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00909BA0, 272 bytes (incl. int3 padding).
// Callees: operator new 0x00BFEA1A, CCreatureAction_StrafeBackward ctor 0x008BFD60, CCreatureAction_StrafeForward
//          ctor 0x008BFD00, CCombatActionBase ctor 0x00857F30 (inlined InterruptableMidAttack ctor, vtable
//          0x01262EDC from rtti-vtables.tsv), CCountedPointer::Reset 0x00905FD0, AddAction 0x008B0B10,
//          base OnCreate 0x008B0650, operator delete 0x00BFE9BC.
// Shape: the landed VillagerCombo OnCreate (0x00908A20) model: one counted pointer local, three
// `new action(owner +0x1c, target +0x20)` assignments through the out-of-line Reset, each handed to
// AddAction, and the local's destructor inlined at the end. The first two actions (0x114 bytes) have
// out-of-line constructors; the third is CCreatureAction_InterruptableMidAttack (0x118 bytes) whose
// inline ctor forwards to CCombatActionBase(owner, target, 150, CSoundPair()) and stores its vtable; the
// zeroed CSoundPair stores are what keep zero in ebx for the whole body. The engine header
// CCombatSequence_WhisperAdvance.h is a flattened blob, so the sequence is modelled locally. Owner/target are
// read through inline GetCreature()/GetTarget() reference getters (direct `*Creature` reads rotate eax/ecx/edx).
struct CThingCreatureBase;
struct CThing;

struct CSoundPair { long first; long last; CSoundPair() : first(0), last(0) { } };

struct CCreatureActionBase {
    virtual ~CCreatureActionBase();
    unsigned char nativeFields[0xa4];      // native action base is 0xa8 bytes
};
struct CCombatActionBase : CCreatureActionBase {
    virtual ~CCombatActionBase();
    unsigned char nativeCombatFields[0x6c]; // retail combat action base is 0x114 bytes
    CCombatActionBase(CThingCreatureBase& owner, CThing& target, long actionCode, const CSoundPair& sounds); // 0x00857F30
};
struct CCreatureAction_StrafeBackward : CCombatActionBase {
    CCreatureAction_StrafeBackward(CThingCreatureBase& owner, CThing& target);   // 0x008BFD60
};
struct CCreatureAction_StrafeForward : CCombatActionBase {
    CCreatureAction_StrafeForward(CThingCreatureBase& owner, CThing& target);    // 0x008BFD00
};
struct CCreatureAction_InterruptableMidAttack : CCombatActionBase {
    long InterruptTime;                     // +0x114, untouched by this ctor
    virtual ~CCreatureAction_InterruptableMidAttack();
    CCreatureAction_InterruptableMidAttack(CThingCreatureBase& owner, CThing& target)
        : CCombatActionBase(owner, target, 150, CSoundPair()) { }
};

struct SCountBlock {
    long references;
    void (__fastcall* destroy)(void*);
    void* object;
};
struct CCountedPointer {
    void* object;
    SCountBlock* count;
    CCountedPointer() : object(0), count(0) { }
    CCountedPointer& operator=(void* value) { Reset(value); return *this; }
    void Reset(void* value);                // 0x00905FD0
    ~CCountedPointer()
    {
        if (count) {
            --count->references;
            if (count->references == 0) {
                count->destroy(count->object);
                delete count;
            }
        }
    }
};

struct CCombatSequenceBase {
    void* nativeVptr;
    unsigned char nativePrefix[0x18];
    CThingCreatureBase* Creature;           // +0x1c
    CThing* Target;                         // +0x20
    CThingCreatureBase& GetCreature() const { return *Creature; }
    CThing& GetTarget() const { return *Target; }
    void AddAction(const CCountedPointer& action); // 0x008B0B10
    void OnCreate();                        // 0x008B0650 (base slot, called directly)
};
struct CCombatSequence_WhisperAdvance : CCombatSequenceBase {
    void OnCreate();
};

void CCombatSequence_WhisperAdvance::OnCreate()
{
    CCountedPointer action;
    action = new CCreatureAction_StrafeBackward(GetCreature(), GetTarget());
    CCombatSequenceBase::AddAction(action);
    action = new CCreatureAction_StrafeForward(GetCreature(), GetTarget());
    CCombatSequenceBase::AddAction(action);
    action = new CCreatureAction_InterruptableMidAttack(GetCreature(), GetTarget());
    CCombatSequenceBase::AddAction(action);
    CCombatSequenceBase::OnCreate();
}
