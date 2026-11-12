// CAIStateGroupBase::AIAction_CombatPerformActionAndWait(CCreatureActionBase* action, bool stopWhenReplaced) -- post the action in a brain output (flag 8), then yield while the creature is still performing an action (optionally only while it is still this action's id), reporting 1 (stopping) or 2 (done)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0097DA40, 297 bytes (`ret 8`).
// Callees: CAIStateGroupBase::IsStopping 0x008FD010, CCreatureActionBase::GetActionID 0x00692620 (`mov eax,[ecx+28h]`;
// manifest "CWorld::DrawGetEnvironment" is propagated), CBrainOutput::CBrainOutput 0x00835620, the action's virtual
// slot 11 (+0x2c, a counted copy of itself), CCountedPointer<..>::Reset 0x00668D90, CAIBrain::SetOutput 0x0088B8F0,
// PBrain virtual slot 2 (+0x08, yield), CThingCreatureBase::IsPerformingAnyAction 0x006622A0,
// ~CBaseIntelligentPointer 0x00A01C10, operator delete 0x00BFE9BC.
// Shape: the landed sibling models (0x0097D8B0 AIAction_PerformAction, 0x0097D980 AIAction_PumpCombatActions: the
// implicit ~CBrainOutput inlined on both returns); the action id is taken before the output is built; the wait loop
// `while (performing && !IsStopping() && (!stopWhenReplaced || id == current->GetActionID())) SuspendSuspendableProcess();` (the bool
// hoisted into bl).
// The creature is reached by a direct cast at each use: an inline GetCreature() accessor rotates eax/edx in the loop.
// Layout: the creature's current action at +0x100 (IsPerformingAnyAction reads it), the action id at +0x28.
// Leaf: AIAction_CombatPerformActionAndWait
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
class CThing;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct CCountBlock                                  // CCountedPointer's shared block
{
    long Count;                                     // +0x00
    void (__fastcall* Destroy)(void* object);       // +0x04
    void* Object;                                   // +0x08
};

struct CCountedPointer                              // CCountedPointer<..> {object, block}
{
    void* Object;                                   // +0x00
    CCountBlock* Block;                             // +0x04
    void Reset(void* object);                       // 0x00668D90
    ~CCountedPointer()
    {
        if (Block)
        {
            --Block->Count;
            if (Block->Count == 0)
            {
                Block->Destroy(Block->Object);
                delete Block;
            }
        }
        Object = 0;
        Block = 0;
    }
};

class CBrainOutput                                 // 0x48 bytes
{ public:
    unsigned long Flags;                            // +0x00 (8: perform action, 0x20: pump combat actions)
    unsigned char _pad04[0x20 - 0x04];
    CCountedPointer PAction;                        // +0x20
    unsigned char _pad28[0x40 - 0x28];
    CBaseIntelligentPointer PTarget;                // +0x40 CIntelligentPointer<const CThing>
    CBrainOutput();                                 // 0x00835620

};

class CAIBrain
{ public:
    virtual void v00(); virtual void v01();
    virtual bool SuspendSuspendableProcess();                           // slot 2 (+0x08) 0x00A44840 ?SuspendSuspendableProcess@CSuspendableProcessMicrothread@@UAE_NXZ (Ego_r.pdb + FableWin.pdb, inherited; was the stand-in Yield; name-fix 2026-11-12)
    unsigned char _pad04[0x18 - 0x04];
    CThing* Creature;                               // +0x18
    void SetOutput(const CBrainOutput& output);  // 0x0088B8F0 ?SetOutput@CAIBrain@@QAEXABVCBrainOutput@@@Z (Ego_r.pdb + FableWin.pdb; was the stand-in SetOutput; name-fix 2026-11-12) (manifest "CQuestManager::RemoveAvailableQuestCardType" is propagated)
};

class CCreatureActionBase
{ public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10();
    virtual void* Clone() const;                    // slot 11 (+0x2c)
    long GetActionID() const;                       // 0x00692620
};

class CThingCreatureBase
{ public:
    unsigned char _pad[0x100];
    CCreatureActionBase* CurrentAction;             // +0x100
public:
    bool IsPerformingAnyAction() const;   // 0x006622A0
};

class CAIStateGroupBase
{ public:
    virtual void v00();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                     // +0x08
protected:
    bool IsStopping() const;                              // 0x008FD010 (Ego_r.pdb protected const)
public:
    long AIAction_CombatPerformActionAndWait(CCreatureActionBase* action, bool stopWhenReplaced);
};

long CAIStateGroupBase::AIAction_CombatPerformActionAndWait(CCreatureActionBase* action, bool stopWhenReplaced)
{
    if (IsStopping())
        return 1;
    long id = action->GetActionID();
    CBrainOutput output;
    output.Flags |= 8;
    output.PAction.Reset(action->Clone());
    PBrain->SetOutput(output);
    PBrain->SuspendSuspendableProcess();
    while (((CThingCreatureBase*)PBrain->Creature)->IsPerformingAnyAction() && !IsStopping() &&
           (!stopWhenReplaced || id == ((CThingCreatureBase*)PBrain->Creature)->CurrentAction->GetActionID()))
        PBrain->SuspendSuspendableProcess();
    if (IsStopping())
        return 1;
    return 2;
}