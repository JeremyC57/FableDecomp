// NTCScriptedControl::CActionWaitForThingToFinishPerformingTasks::Update() -- finish once the target is dead or its own scripted control is neither locked nor holding queued actions
// Leaf: ?Update@CActionWaitForThingToFinishPerformingTasks@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00713CD0, 123 bytes (+5 int3 padding); vtable 0x0128D7F0 slot 1.
// FableWin.pdb: ?Update@CActionWaitForThingToFinishPerformingTasks@NTCScriptedControl@@UAEXXZ.
// Callees: CScriptThing virtual slots 77 IsValid (+0x134), 71 Refresh (+0x11c), 75 IsAlive (+0x12c), 11 GetThing (+0x2c)
//          (names as the landed CActionFollowCreature::Update @ 0x00713EE0 and CQHangingTreeGoodScript::RemoveDregs);
//          the const CVectorMap::LowerBound @ 0x004365B0 (inlined const GetInterface, interface 0x1F = CTCScriptedControl:
//          the sign bit of the bitset dword +0x20, so `jns`).
// Layout: CActionBase {vptr, PTCScript +4, Activated +8, Terminated +9}; PTargetThing (CScriptThing, 12 bytes) +0x0C.
//          Retail CTCScriptedControl PActionList (STLport list sentinel pointer) +0x14, Locked +0x18 (as the landed
//          CTCScriptedControl::InterruptControl @ 0x00713850). engine/CScriptThing.h carries no virtuals, so the
//          script-thing slots are modelled locally like the other landed NTCScriptedControl rows.
// Shape: dead target, or no scripted control, branch to the single Terminated store; Locked or a non-empty queue returns.
struct CTCBase;
struct CThing;
struct CCPPointerInfo;
struct SActionNode { SActionNode* _M_next; SActionNode* _M_prev; void* Data; CCPPointerInfo* Info; };
struct CActionList {                               // STLport list<CCountedPointer<CActionBase>>
    SActionNode* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
};
struct CTCScriptedControl {                        // interface 0x1F
    void*        __vftable;
    CThing*      ParentThing;                      // +0x04 (CTCBase)
    bool         ToBeRemoved;                      // +0x08
    bool         IsInGlobalUpdateTCs;              // +0x09
    void*        PScriptHandleImp[2];              // +0x0c CIntelligentPointer
    CActionList  PActionList;                      // +0x14
    bool         Locked;                           // +0x18
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CThing {
    void*         __vftable;
    unsigned char _pad04[0x20 - 0x04];
    unsigned long UsingComponents[9];              // +0x20
    CInterfaceMap Components;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &Components;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CScriptThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;              // slot 11 (+0x2c)
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

namespace NTCScriptedControl {
class CActionBase {
public:
    virtual ~CActionBase();
    virtual void Update();                         // slot 1
    CTCScriptedControl* PTCScript;                 // +0x04
    bool                Activated;                 // +0x08
    bool                Terminated;                // +0x09
};

class CActionWaitForThingToFinishPerformingTasks : public CActionBase {
public:
    virtual void Update();
    CScriptThing PTargetThing;                     // +0x0C
};

void CActionWaitForThingToFinishPerformingTasks::Update()
{
    if (!PTargetThing.IsValid())
        PTargetThing.Refresh();
    if (PTargetThing.IsAlive()) {
        CTCScriptedControl* control;
        if (PTargetThing.GetThing()->GetInterface(&control, 0x1f)) {
            if (control->Locked || !control->PActionList.empty())
                return;
        }
    }
    Terminated = true;
}
}