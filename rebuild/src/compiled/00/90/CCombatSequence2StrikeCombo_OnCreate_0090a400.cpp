// CCombatSequence2StrikeCombo::OnCreate() -- queue the sequence's two actions (a new CCreatureAction_MidAttackFacing, then a new CCreatureAction_MidAttackFacing, each on the sequence's creature and target) and run the base OnCreate
// Leaf: ?OnCreate@CCombatSequence2StrikeCombo@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0090A400, 188 bytes, thiscall, plain `ret` (virtual).
// Identity (proven): slot 1 of the CCombatSequence2StrikeCombo vtable 0x0127A0CC (rtti-vtables); FableWin.pdb ?OnCreate@CCombatSequence2StrikeCombo@@UAEXXZ; the actions are named
//   by the RTTI vtables their constructors store and FableWin.pdb's ??0<action>@@QAE@AAVCThingCreatureBase@@0@Z.
// Callees: operator new 0x00BFEA1A, CCreatureAction_MidAttackFacing::CCreatureAction_MidAttackFacing 0x0090A690, CCreatureAction_MidAttackFacing::CCreatureAction_MidAttackFacing 0x0090A690, CCountedPointer<CCreatureActionBase>::Reset
//   0x00905FD0, CCombatSequenceBase::AddAction 0x008B0B10, CCombatSequenceBase::OnCreate 0x008B0650, the count block's delete
//   function and operator delete 0x00BFE9BC (the local's inline destructor).
// Shape (family K003, representative 0x009061E0): the landed one-action OnCreate 0x009062A0 with a second Reset/AddAction pair on the
//   same counted-pointer local (zeroed before the first allocation), retyped with the PDB names; creature and target reach the
//   constructors through the inline accessors (the members read directly flip the second AddAction's address register).
class CThingCreatureBase;
class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();
};
class CCreatureAction_MidAttackFacing : public CCreatureActionBase {               // retail 0x114 bytes
public:
    CCreatureAction_MidAttackFacing(CThingCreatureBase& creature, CThingCreatureBase& target);   // 0x0090A690
    unsigned char _data[0x110];
};
class CCPPointerInfo {                                       // the shared count block
public:
    long RefCount;                                          // +0
    void (__fastcall* DeleteFunc)(void* data);              // +4, Data in ecx
    void* Data;                                             // +8
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                   // +0
    CCPPointerInfo* Info;                                   // +4
    CCountedPointer() : Data(0), Info(0) { }
    void Reset(T* object);                                  // 0x00905FD0 for CCreatureActionBase
    CCountedPointer& operator=(T* object) { Reset(object); return *this; }
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
};
class CCombatSequenceBase {                                 // Ego_r.pdb 44 bytes (retail 0x28: 12-byte vector)
public:
    virtual void v00();
    virtual void OnCreate();                                // slot 1, 0x008B0650
    bool Running;                                           // +0x04
    long NoAvailableActions;                                // +0x08
    long CurrentActionIndex;                                // +0x0C
    unsigned char CombatActions[0xC];                       // +0x10 vector<CCombatAction>
    CThingCreatureBase* Creature;                           // +0x1C
    CThingCreatureBase* Target;                             // +0x20
    bool CreateCalled;                                      // +0x24
    void AddAction(const CCountedPointer<CCreatureActionBase>& action);   // 0x008B0B10
    CThingCreatureBase& GetCreature() const { return *Creature; }   // inline accessors (the landed sibling model)
    CThingCreatureBase& GetTarget() const { return *Target; }
};
class CCombatSequence2StrikeCombo : public CCombatSequenceBase {
public:
    virtual void OnCreate();
};
void CCombatSequence2StrikeCombo::OnCreate()
{
    CCountedPointer<CCreatureActionBase> action;
    action = new CCreatureAction_MidAttackFacing(GetCreature(), GetTarget());
    AddAction(action);
    action = new CCreatureAction_MidAttackFacing(GetCreature(), GetTarget());
    AddAction(action);
    CCombatSequenceBase::OnCreate();
}