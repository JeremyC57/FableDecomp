// CCombatSequence_TrollDoubleSwipeNonMelee::OnCreate() -- queue two CCreatureAction_TrollSwipe actions for the sequence's creature and target, then the base OnCreate
// Leaf: ?OnCreate@CCombatSequence_TrollDoubleSwipeNonMelee@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00907840, 237 bytes, thiscall, plain `ret`.
// Identity (proven): vtable 0x0127976C (RTTI .?AVCCombatSequence_TrollDoubleSwipeNonMelee@@) slot 1 (OnCreate; CCombatSequenceBase::OnCreate 0x008B0650 is the base call); Ego_r.pdb ?OnCreate@CCombatSequence_TrollDoubleSwipeNonMelee@@UAEXXZ; both actions store vtable 0x0128E654 (RTTI .?AVCCreatureAction_TrollSwipe@@, Ego_r.pdb ??0CCreatureAction_TrollSwipe@@QAE@AAVCThingCreatureBase@@0@Z, inline here), code 150, 0x114 bytes.
// Callees: operator new 0x00BFEA1A (0x114 bytes), CCombatActionBase::CCombatActionBase(CThingCreatureBase&, CThing&, long,
//   const CSoundPair&) 0x00857F30, CCountedPointer<CCombatActionBase>::Reset(CCombatActionBase*) 0x00905FD0,
//   CCombatSequenceBase::PushAction(CCombatAction&) 0x008B0B10, CCombatSequenceBase::OnCreate() 0x008B0650, operator delete 0x00BFE9BC.
// Shape (family L095): the landed CCombatSequenceWhisperBoast::OnCreate 0x0090A0B0 model with the one CCombatAction local reset and
//   pushed twice.
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
class CCreatureAction_TrollSwipe : public CCombatActionBase {               // retail 0x114 bytes
public:
    CCreatureAction_TrollSwipe(CThingCreatureBase& owner, CThingCreatureBase& target) : CCombatActionBase(owner, target, 150, CSoundPair()) { }
    virtual ~CCreatureAction_TrollSwipe();
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
class CCombatSequence_TrollDoubleSwipeNonMelee : public CCombatSequenceBase {
public:
    virtual void OnCreate();
};
void CCombatSequence_TrollDoubleSwipeNonMelee::OnCreate()
{
    CCombatAction action;
    action.CreatureAction.Reset(new CCreatureAction_TrollSwipe(*Creature, *Target));
    PushAction(action);
    action.CreatureAction.Reset(new CCreatureAction_TrollSwipe(*Creature, *Target));
    PushAction(action);
    CCombatSequenceBase::OnCreate();
}