// CTCBerserk::CancelForCutscene() -- once only: mark IsInCutscene, finish the parent creature's current action, clean up the ability and remove this TC (type 0x84) from the parent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0078D210, length 49 bytes (+ int3 padding).
// Callees: CThingCreatureBase::FinishCurrentAction @ 0x00662650, CTCBerserk::CleanupAbility(bool)
// @ 0x0078CD80, CThing::RemoveTC(long) @ 0x004C9840 (landed name).
// Layout: CTCBase ParentThing +4; IsInCutscene is +0x72 in retail (engine/CTCBerserk.h +0x76: the
// DependentThings vector is 4 bytes narrower in retail), so the class is modelled locally.
// Shape: the parent pointer is read into a local (edi) before the flag store; the guard is an `if`
// around the body (the early-out shares the single exit).
class CThing { public:
    void RemoveTC(long interfaceType);                      // 0x004C9840
};

class CThingCreatureBase : public CThing { public:
    void FinishCurrentAction();                             // 0x00662650
};

struct CTCBerserk_Retail {
    void*               __vftable;                          // +0x00
    CThingCreatureBase* ParentThing;                        // +0x04 (CTCBase)
    unsigned char       _pad08[0x72 - 0x08];
    bool                IsInCutscene;                       // +0x72 (PDB +0x76)
    void CleanupAbility(bool cutscene);                     // 0x0078CD80
    void CancelForCutscene();
};

void CTCBerserk_Retail::CancelForCutscene()
{
    if (!IsInCutscene) {
        CThingCreatureBase* parent = ParentThing;
        IsInCutscene = true;
        parent->FinishCurrentAction();
        CleanupAbility(true);
        parent->RemoveTC(0x84);
    }
}