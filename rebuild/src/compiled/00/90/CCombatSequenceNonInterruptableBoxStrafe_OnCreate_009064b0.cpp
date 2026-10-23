// CCombatSequence_NonInterruptableBoxStrafe::OnCreate() -- queue the box strafe: strafe backward, diagonal back-right, diagonal back-left, strafe backward, then a combat idle (code 50, no sounds) through AddAction, then the base OnCreate; the counted-pointer temporary is released inline
// Leaf: ?OnCreate@CCombatSequence_NonInterruptableBoxStrafe
// Retail: Fable.exe (TLC, VC7.1) @ 0x009064B0, 382 bytes (384 with padding).
// Callees: operator new 0x00BFEA1A (0x114 bytes each); the out-of-line action ctors (owner, target)
//          CCreatureAction_StrafeBackward 0x008BFD60, CCreatureAction_DiagonalStrafeBR 0x008BFCA0,
//          CCreatureAction_DiagonalStrafeBL 0x008BFC40 (all three catalogued as "CCombatAction_GenericResponseToDrain";
//          the vtables they store, 0x0127FDCC / 0x0127FB3C / 0x0127F9F4, name the classes); CCombatActionBase(
//          CThingCreatureBase*, CThing*, long, const CSoundPair&) 0x00857F30 for the inline CCreatureAction_CombatIdle
//          (vtable 0x012632B4); the counted pointer's Reset 0x00905FD0; CCombatSequenceBase::AddAction 0x008B0B10 and
//          OnCreate 0x008B0650; operator delete 0x00BFE9BC.
//
// Shape: the landed WhisperAdvance OnCreate model (0x00909BA0): one counted pointer assigned five times through
// Reset, owner/target read through inline reference getters and passed by reference (with by-pointer ctors the
// AddAction `lea` stays in eax instead of rotating eax/ecx/edx); edi is only saved for the inline CombatIdle.
// Layout: local model (engine/CCombatSequence_NonInterruptableBoxStrafe.h is a flattened base blob);
// CCombatSequenceBase owner +0x1c / target +0x20 as the landed family.
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
struct CCreatureAction_StrafeBackward : CCombatActionBase {   // vtable 0x0127FDCC
    CCreatureAction_StrafeBackward(CThingCreatureBase& owner, CThing& target);   // 0x008BFD60
};
struct CCreatureAction_DiagonalStrafeBR : CCombatActionBase {   // vtable 0x0127FB3C
    CCreatureAction_DiagonalStrafeBR(CThingCreatureBase& owner, CThing& target);  // 0x008BFCA0
};
struct CCreatureAction_DiagonalStrafeBL : CCombatActionBase {   // vtable 0x0127F9F4
    CCreatureAction_DiagonalStrafeBL(CThingCreatureBase& owner, CThing& target);  // 0x008BFC40
};
struct CCreatureAction_CombatIdle : CCombatActionBase {         // vtable 0x012632B4, 0x114 bytes
    virtual ~CCreatureAction_CombatIdle();
    CCreatureAction_CombatIdle(CThingCreatureBase& owner, CThing& target)
        : CCombatActionBase(owner, target, 50, CSoundPair()) { }
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
    CCountedPointer& operator=(void* value) { Reset_00905fd0(value); return *this; }
    void Reset_00905fd0(void* value);                // 0x00905FD0
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
struct CCombatSequence_NonInterruptableBoxStrafe : CCombatSequenceBase {
    void OnCreate();
};

void CCombatSequence_NonInterruptableBoxStrafe::OnCreate()
{
    CCountedPointer action;
    action = new CCreatureAction_StrafeBackward(GetCreature(), GetTarget());
    CCombatSequenceBase::AddAction(action);
    action = new CCreatureAction_DiagonalStrafeBR(GetCreature(), GetTarget());
    CCombatSequenceBase::AddAction(action);
    action = new CCreatureAction_DiagonalStrafeBL(GetCreature(), GetTarget());
    CCombatSequenceBase::AddAction(action);
    action = new CCreatureAction_StrafeBackward(GetCreature(), GetTarget());
    CCombatSequenceBase::AddAction(action);
    action = new CCreatureAction_CombatIdle(GetCreature(), GetTarget());
    CCombatSequenceBase::AddAction(action);
    CCombatSequenceBase::OnCreate();
}