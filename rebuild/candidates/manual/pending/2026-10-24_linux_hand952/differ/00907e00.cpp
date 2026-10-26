// CCombatSequence_TrollRockThrowRock::OnCreate() -- queue a CCreatureAction_TrollGetProjectile then a CCreatureAction_TrollThrowProjectile on the sequence's creature and target, then the base OnCreate
// Grade: DIFFER(188v188) -- register choice only: the second PushAction argument is built in eax (retail `lea ecx,[esp+4] ; push ecx`) and the release's Info reload before operator delete uses ecx (retail edx); same length, every other byte equal
// Leaf: ?OnCreate@CCombatSequence_TrollRockThrowRock@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00907E00, 188 bytes (192 with int3 padding), thiscall, plain `ret`.
// Identity (proven): CCombatSequence_TrollRockThrowRock::OnCreate; evidence: the body is slot 1 of vtable 0x012799EC, whose RTTI is .?AVCCombatSequence_TrollRockThrowRock@@
//   (slot 1 is OnCreate in every combat-sequence vtable: the landed CCombatSequence_* OnCreate rows sit there; FableWin.pdb
//   ?OnCreate@CCombatSequenceBase@@UAEXXZ is the base's).
// Label: the manifest's "?OnCreate@CCombatSequence_TrollRockThrowRock@@UAEXXZ" is propagated across family H031 (seven two-action OnCreate bodies).
// Actions: CCreatureAction_TrollGetProjectile (0x114 bytes, ctor 0x009082D0) then CCreatureAction_TrollThrowProjectile (0x130 bytes, ctor 0x008C7400): each constructor's stored
//   vtable RTTI names its class, and each class's FableWin.pdb constructor takes (CThingCreatureBase&, CThingCreatureBase&).
// Callees: operator new 0x00BFEA1A, CCountedPointer<CCombatActionBase>::reset(CCombatActionBase*) 0x00905FD0 (FableWin.pdb
//   ?reset@?$CCountedPointer@VCCombatActionBase@@@@QAEXPAVCCombatActionBase@@@Z; drops the old count block, adopts the
//   object with a fresh one), CCombatSequenceBase::PushAction(CCombatAction&) 0x008B0B10 (Ego_r.pdb, protected; the
//   push_back into CombatActions), CCombatSequenceBase::OnCreate 0x008B0650, operator delete 0x00BFE9BC (the inline
//   counted-pointer release of the local).
// Layout: Ego_r.pdb CCombatSequenceBase with retail's 12-byte vector: CombatActions +0x10, Creature +0x1C, Target +0x20.
//   CCombatAction (Ego_r.pdb, 8 bytes) is one CCountedPointer<CCombatActionBase> CreatureAction; CCPPointerInfo
//   {RefCount, DeleteFunc, Data}, the delete function called with the object in ecx in retail.
class CThingCreatureBase;

class CCombatActionBase {
public:
    virtual ~CCombatActionBase();
};

struct CCPPointerInfo {
    unsigned long RefCount;                         // +0
    void (__fastcall* DeleteFunc)(void*);           // +4
    void* Data;                                     // +8
};

template <class T> class CCountedPointer {
public:
    T*              Data;                           // +0
    CCPPointerInfo* Info;                           // +4
    CCountedPointer() : Data(0), Info(0) { }
    ~CCountedPointer() { ReleaseData(); }
    void ReleaseData()
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
    void reset(T* data);                            // 0x00905FD0 (CCombatActionBase)
};

class CCombatAction {
public:
    CCountedPointer<CCombatActionBase> CreatureAction;   // +0
};

class CCreatureAction_TrollGetProjectile : public CCombatActionBase {
public:
    unsigned char _body[0x114 - 4];
    CCreatureAction_TrollGetProjectile(CThingCreatureBase& creature, CThingCreatureBase& target);   // 0x009082D0
};
class CCreatureAction_TrollThrowProjectile : public CCombatActionBase {
public:
    unsigned char _body[0x130 - 4];
    CCreatureAction_TrollThrowProjectile(CThingCreatureBase& creature, CThingCreatureBase& target);   // 0x008C7400
};

class CCombatSequenceBase {
public:
    virtual ~CCombatSequenceBase();
    virtual void OnCreate();                        // 0x008B0650
    bool                Running;                    // +0x04
    long                NoAvailableActions;         // +0x08
    long                CurrentActionIndex;         // +0x0C
    unsigned char       CombatActions[12];          // +0x10 vector<CCombatAction>
    CThingCreatureBase* Creature;                   // +0x1C
    CThingCreatureBase* Target;                     // +0x20
protected:
    void PushAction(CCombatAction& action);         // 0x008B0B10
};

class CCombatSequence_TrollRockThrowRock : public CCombatSequenceBase {
public:
    virtual void OnCreate();
};

void CCombatSequence_TrollRockThrowRock::OnCreate()
{
    CCombatAction action;
    action.CreatureAction.reset(new CCreatureAction_TrollGetProjectile(*Creature, *Target));
    PushAction(action);
    action.CreatureAction.reset(new CCreatureAction_TrollThrowProjectile(*Creature, *Target));
    PushAction(action);
    CCombatSequenceBase::OnCreate();
}
