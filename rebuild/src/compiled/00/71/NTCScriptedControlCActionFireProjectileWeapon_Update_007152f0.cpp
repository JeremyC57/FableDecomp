// NTCScriptedControl::CActionFireProjectileWeapon::Update() -- finish once the target is dead or the queue has run dry and the creature is no longer on the last action handed over; otherwise feed the next queued creature action (reload / aim / fire) to the creature whenever it is idle or its current action has finished
// Leaf: ?Update@CActionFireProjectileWeapon@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007152F0, 181 bytes (+11 int3 padding); vtable 0x01265960 slot 1.
// FableWin.pdb: ?Update@CActionFireProjectileWeapon@NTCScriptedControl@@UAEXXZ.
// Callees: CScriptThing virtual slots 77 IsValid (+0x134), 71 Refresh (+0x11c), 75 IsAlive (+0x12c);
//          CThingCreatureBase::IsPerformingAnyAction @ 0x006622A0, CThingCreatureBase::SetCurrentAction @ 0x006644F0,
//          CCreatureActionBase::GetActionID @ 0x00692620 (`mov eax,[ecx+28h]`; manifest "CWorld::DrawGetEnvironment" is a
//          propagated byte twin), the action list's erase(iterator) @ 0x00669AF0 (a byte-twin list<CCountedPointer<..>>
//          instantiation; the iterator goes by value and comes back through the hidden result in the frame word).
// Layout: CActionBase {vptr, PTCScript +4, Activated +8, Terminated +9}; PTarget (CScriptThing) +0x0C; ActionList (STLport
//          list<CCountedPointer<CCreatureActionBase>>, the 4-byte sentinel pointer) +0x18; CurrentActionID +0x1C (as the
//          landed ctor @ 0x00716790 and OnAdd @ 0x00715110). Retail CThingCreatureBase PCurrentAction +0x100 (as the landed
//          CAIStateGroupSummonerCharge::Update @ 0x0096FF10; engine/CThingCreatureBase.h has the PDB +0x11C, so the
//          creature is modelled locally); CCreatureActionBase ActionID +0x28, Finished +0x61.
// Shape: the creature is read once into ebx right after the refresh; `!IsAlive() || empty()` guards the finish test; the
//          front action is handed over through the counted pointer's inline Get(); erase(begin()) discards the result.
struct CCPPointerInfo;
class CCreatureActionBase {
public:
    unsigned char _pad00[0x28];
    long          ActionID;                        // +0x28
    unsigned char _pad2c[0x61 - 0x2c];
    bool          Finished;                        // +0x61
    long GetActionID() const;                      // 0x00692620
};
template<class T> struct CCountedPointer {
    T*              Data;                          // +0x00
    CCPPointerInfo* Info;                          // +0x04
    T* Get() const { return Data; }
};
struct SCreatureActionNode {
    SCreatureActionNode*                  _M_next; // +0x00
    SCreatureActionNode*                  _M_prev; // +0x04
    CCountedPointer<CCreatureActionBase>  _M_data; // +0x08
};
struct CCreatureActionListIterator {
    SCreatureActionNode* _M_node;
    CCreatureActionListIterator(SCreatureActionNode* node) : _M_node(node) { }
};
struct CCreatureActionListConstIterator {
    SCreatureActionNode* _M_node;
    CCreatureActionListConstIterator(const CCreatureActionListIterator& it) : _M_node(it._M_node) { }
};
struct CCreatureActionList {                       // STLport list<CCountedPointer<CCreatureActionBase>>
    SCreatureActionNode* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
    CCreatureActionListIterator begin() { return CCreatureActionListIterator(_M_node->_M_next); }
    CCountedPointer<CCreatureActionBase>& front() { return _M_node->_M_next->_M_data; }
    CCreatureActionListIterator erase(CCreatureActionListConstIterator position);   // 0x00669AF0
    void pop_front() { erase(begin()); }
};
class CThingCreatureBase {
public:
    unsigned char        _pad00[0x100];
    CCreatureActionBase* PCurrentAction;           // +0x100 (retail)
    bool IsPerformingAnyAction();                  // 0x006622A0
    bool SetCurrentAction(CCreatureActionBase* action);   // 0x006644F0
};
struct CScriptThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070();
    virtual void Refresh();                        // slot 71 (+0x11c)
    virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsAlive() const;                  // slot 75 (+0x12c)
    virtual bool IsDead() const;                   // slot 76 (+0x130)
    virtual bool IsValid() const;                  // slot 77 (+0x134)
    CScriptThing*   PImp_Data;                     // +0x04
    CCPPointerInfo* PImp_Info;                     // +0x08
};
struct CTCScriptedControl {
    void*               __vftable;
    CThingCreatureBase* ParentThing;               // +0x04 (CTCBase; a creature here)
};

namespace NTCScriptedControl {
class CActionBase {
public:
    virtual ~CActionBase();
    virtual void Update();                         // slot 1
    CTCScriptedControl* PTCScript;                 // +0x04
    bool                Activated;                 // +0x08
    bool                Terminated;                // +0x09
};

class CActionFireProjectileWeapon : public CActionBase {
public:
    virtual void Update();
    CScriptThing        PTarget;                   // +0x0C
    CCreatureActionList ActionList;                // +0x18
    long                CurrentActionID;           // +0x1C
};

void CActionFireProjectileWeapon::Update()
{
    if (!PTarget.IsValid())
        PTarget.Refresh();
    CThingCreatureBase* creature = PTCScript->ParentThing;
    if (!PTarget.IsAlive() || ActionList.empty()) {
        if (!creature->IsPerformingAnyAction() || creature->PCurrentAction->GetActionID() != CurrentActionID)
            Terminated = true;
    }
    if (ActionList.empty())
        return;
    if (creature->IsPerformingAnyAction() && !creature->PCurrentAction->Finished)
        return;
    creature->SetCurrentAction(ActionList.front().Get());
    CurrentActionID = creature->PCurrentAction->GetActionID();
    ActionList.pop_front();
}
}