// NCreatureMode::CModeDragonGetHit::FrameUpdate() -- base update, then count the get-hit timer down and at zero snap the dragon out of it at its target
// Leaf: ?FrameUpdate@CModeDragonGetHit
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E06E0, 76 bytes (+ int3 padding), `ret`; vtable 0x012683E4 slot 3.
// Callees: CModeBase::FrameUpdate @ 0x0073AC50 (qualified), CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
//   CCreatureAction_SnapDragonOutOf::CCreatureAction_SnapDragonOutOf(CThingCreatureBase*, CThing*) @ 0x008E49C0 (landed; the
//   manifest label "CCreatureAction_DragonRoar" is propagated: the ctor stores vtable 0x01286D94 = CCreatureAction_SnapDragonOutOf),
//   CThingCreatureBase::SetCurrentAction @ 0x006644F0, ~CCombatActionBase @ 0x0062E900.
// Name: FableWin.pdb `?FrameUpdate@CModeDragonGetHit@NCreatureMode@@UAEXXZ` (slot 3).
// Layout: Ego_r.pdb CModeDragonGetHit (0x80): CModeStandBase 0x68, Filter +0x68, PTarget +0x74, Timer +0x7c.
// Shape: `if (--Timer <= 0)` (the `dec ecx ; mov eax,ecx ; test ; mov [],ecx` form), the creature as a named local (edi),
//   the action temporary passed by reference (arguments evaluated right to left: GetPItem before the creature push).
class CThing;
class CCreatureActionBase;

class CThingCreatureBase { public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};

class CCreatureActionBase { public:               // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa4];
};
class CCombatActionBase : public CCreatureActionBase { public:   // retail sizeof 0x114
    unsigned char _combat[0x6c];
    virtual ~CCombatActionBase();                  // 0x0062E900
};
class CCreatureAction_SnapDragonOutOf : public CCombatActionBase { public:
    CCreatureAction_SnapDragonOutOf(CThingCreatureBase* owner, CThing* thing);   // 0x008E49C0
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThingCreatureBase* ParentThing;       // +0x04
    unsigned char       _body[0x14];
    void FrameUpdate();                    // 0x0073AC50
};
struct CModeStandBase : CModeBase {        // retail sizeof 0x68
    unsigned char _stand[0x4c];
};

namespace NCreatureMode {
struct CModeDragonGetHit : CModeStandBase {   // vtable 0x012683E4
    unsigned char           Filter[0x0c];  // +0x68 CAnimFilterOfGroup
    CIntelligentPointer<CThing> PTarget;       // +0x74 CIntelligentPointer<CThing>
    long                    Timer;         // +0x7c
    void FrameUpdate();
};
}

void NCreatureMode::CModeDragonGetHit::FrameUpdate()
{
    CModeBase::FrameUpdate();
    if (--Timer <= 0) {
        CThingCreatureBase* creature = ParentThing;
        creature->SetCurrentAction(CCreatureAction_SnapDragonOutOf(creature, PTarget.GetPItem()));
    }
}