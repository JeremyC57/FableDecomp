// NEntityEvents::CEventVillagerInteraction::GetLifeTime() const -- the event lives 60 frames past the last "still waiting" frame
// Leaf: ?GetLifeTime@CEventVillagerInteraction@NEntityEvents@@UBEJXZ
// Retail: Fable.exe @ 0x00982390, 10 bytes, thiscall plain `ret`, long in eax.
// Identity (proven): vtable slot 3 (+0x0C) of NEntityEvents::CEventVillagerInteraction 0x012995A8. The ten event classes
// that override slot 3 in rtti-vtables.tsv (CEventBase, CEventExpressionPerformedTo, CEventFiredShot, CEventHitBase,
// CEventKillLocked, CEventVillagerInteraction, ...) all have ?GetLifeTime@<Class>@NEntityEvents@@UBEJXZ in Ego_r.pdb /
// FableWin.pdb, the only PDB virtual covering the whole set.
// Label correction: catalog "NEntityEvents::CEventVillagerInteraction::vfunc_3".
// Callees: none.
// Layout: Ego_r.pdb CEventBase (vptr, WorldFrame +4) and CEventVillagerInteraction (ImComingToYou +8, POther +9 as the PDB records it
// CIntelligentPointer<CThingAICreature>, PInteractionDef +0x14, ImReady +0x18, ReadyOnFrame +0x1C, StillWaitingFrame
// +0x20; size 0x24, no STL members). No engine header: local model.
// Shape: `StillWaitingFrame - WorldFrame + 60` keeps retail's sub-then-add order.
class CThingAICreature;
class CVillagerInteractionsDef;

namespace NEntityEvents {
class CEventBase {
public:
    virtual ~CEventBase();
    virtual long GetLifeTime() const;
    long WorldFrame;                                        // +0x04
};

class CEventVillagerInteraction : public CEventBase {
public:
    virtual long GetLifeTime() const;

    bool ImComingToYou;                                     // +0x08
    unsigned char POther[8];                                // +0x09 (PDB offset) CIntelligentPointer<CThingAICreature>
    const CVillagerInteractionsDef* PInteractionDef;        // +0x14 CDefPointer<const CVillagerInteractionsDef>
    bool ImReady;                                           // +0x18
    long ReadyOnFrame;                                      // +0x1C
    long StillWaitingFrame;                                 // +0x20
};

long CEventVillagerInteraction::GetLifeTime() const
{
    return StillWaitingFrame - WorldFrame + 60;
}
}