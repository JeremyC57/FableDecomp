// CCombatSequence_BanditIdleMiddle::OnCreate() -- queue a new CCreatureAction_CombatIdle for the sequence's creature and target, then the base OnCreate
// Leaf: ?OnCreate@CCombatSequence_BanditIdleMiddle@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00906940, 150 bytes, thiscall, plain `ret`.
// Identity (proven): CCombatSequence_BanditIdleMiddle vtable 0x0127935C slot 1 (OnCreate); the manifest label is the FableWin.pdb name; the inline action
//   constructor stores vtable 0x012632B4 (RTTI .?AVCCreatureAction_CombatIdle@@; FableWin.pdb ??0CCreatureAction_CombatIdle@@QAE@AAVCThingCreatureBase@@0@Z).
// Callees: operator new 0x00BFEA1A (0x114 bytes), CCombatActionBase::CCombatActionBase(CThingCreatureBase&, CThing&, long,
//   const CSoundPair&) 0x00857F30, CCountedPointer<CCombatActionBase>::Reset(CCombatActionBase*) 0x00905FD0,
//   CCombatSequenceBase::PushAction(CCombatAction&) 0x008B0B10, CCombatSequenceBase::OnCreate() 0x008B0650, the count block's
//   deleter and operator delete 0x00BFE9BC (the CCombatAction's inline destructor).
// Shape (family K043, representative 0x009068A0): a default CCombatAction (two zero words), its CreatureAction reset to the
//   null-tested new-expression (the action's inline constructor: base call with code 50 and a zero CSoundPair, vtable), the
//   protected PushAction, the base OnCreate, then the counted pointer's release (re-reading the block after the decrement).
//   Ego_r.pdb types (CCombatAction {CCountedPointer<CCombatActionBase> CreatureAction}; CCombatSequenceBase Creature +0x1C,
//   Target +0x20 in retail) replace the landed siblings' invented SCountBlock / CCountedPointer stand-ins.
class CThing { };
class CThingCreatureBase : public CThing { };
class CSoundPair {
public:
    CSoundPair() : FirstSound(0), LastSound(0) { }
    long FirstSound;
    long LastSound;
};
class CCombatActionBase {                                // retail sizeof 0x114
public:
    CCombatActionBase(CThingCreatureBase& owner, CThing& target, long actionCode, const CSoundPair& sound);   // 0x00857F30
    virtual ~CCombatActionBase();
    unsigned char _body[0x114 - 4];
};
class CCreatureAction_CombatIdle : public CCombatActionBase {
public:
    CCreatureAction_CombatIdle(CThingCreatureBase& owner, CThingCreatureBase& target) : CCombatActionBase(owner, target, 50, CSoundPair()) { }
    virtual ~CCreatureAction_CombatIdle();
};
class CCPPointerInfo {                                   // Ego_r.pdb, 12 bytes
public:
    long RefCount;                                       // +0x00
    void (__fastcall* DeleteFunc)(void* data);           // +0x04
    void* Data;                                          // +0x08
};
template <class T> class CCountedPointer {               // Ego_r.pdb: Data, Info
public:
    T* Data;                                             // +0x00
    CCPPointerInfo* Info;                                // +0x04
    CCountedPointer() : Data(0), Info(0) { }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    void Reset(T* data);                                 // 0x00905FD0 for CCombatActionBase
};
class CCombatAction {                                    // Ego_r.pdb, 8 bytes
public:
    CCountedPointer<CCombatActionBase> CreatureAction;   // +0x00
};
class CCombatSequenceBase {                              // Ego_r.pdb 44 bytes (retail: 12-byte vector)
public:
    virtual ~CCombatSequenceBase();
    virtual void OnCreate();                             // 0x008B0650
    bool Running;                                        // +0x04
    long NoAvailableActions;                             // +0x08
    long CurrentActionIndex;                             // +0x0C
    unsigned char CombatActions[0x0c];                   // +0x10 vector<CCombatAction> (12 bytes, not touched here)
    CThingCreatureBase* Creature;                        // +0x1C
    CThingCreatureBase* Target;                          // +0x20
    bool CreateCalled;                                   // +0x24
protected:
    void PushAction(CCombatAction& action);              // 0x008B0B10
};
class CCombatSequence_BanditIdleMiddle : public CCombatSequenceBase {
public:
    virtual void OnCreate();
};
void CCombatSequence_BanditIdleMiddle::OnCreate()
{
    CCombatAction action;
    action.CreatureAction.Reset(new CCreatureAction_CombatIdle(*Creature, *Target));
    PushAction(action);
    CCombatSequenceBase::OnCreate();
}