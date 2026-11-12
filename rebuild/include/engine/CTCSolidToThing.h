#ifndef FABLE_ENGINE_CTCSOLIDTOTHING_H
#define FABLE_ENGINE_CTCSOLIDTOTHING_H
// New shared header (w22 agent1016, row 0x006A16A0 CTCCarrying::RemoveThingInCarrySlot), opt-in FABLE_RETAIL_CTCSOLIDTOTHING: the
// retail CTCSolidToThing over FABLE_RETAIL_CTCBASE. Members are not modelled; only what the row calls: Ego_r.pdb / FableWin.pdb
// ?AddThingNotSolidTo@CTCSolidToThing@@QAEXPBVCThing@@@Z (out of line at 0x00770FA0). StatGetInterfaceType: TCI_SOLID_TO_THING (77;
// the row tests bit 13 of the +0x28 bitset word, `test ah,20h`, then looks up key 0x4D). The AddTC name is "CTCSolidToThing" (0x0123B47C).
#ifdef FABLE_RETAIL_CTCSOLIDTOTHING
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
class CThing;
class CTCSolidToThing : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_SOLID_TO_THING; }
    void AddThingNotSolidTo(const CThing* thing);                    // 0x00770FA0
#ifdef FABLE_RETAIL_CTCSOLIDTOTHING_REMOVETHINGNOTSOLIDTO
    // Overlay opt-in (row 0x009735D0): Ego_r.pdb / FableWin.pdb ?RemoveThingNotSolidTo@CTCSolidToThing@@QAEXPBVCThing@@@Z, out of
    // line at 0x00770F20 (landed). Declaration only; no layout change.
    void RemoveThingNotSolidTo(const CThing* thing);                 // 0x00770F20
#endif
#ifdef FABLE_RETAIL_CTCSOLIDTOTHING_ISSOLIDTOTHING
    // Retail opt-in (wf9, row 0x00726160): Ego_r.pdb / FableWin.pdb ?IsSolidToThing@CTCSolidToThing@@QBE_NPBVCThing@@@Z, out of
    // line at 0x00770DE0 (landed). Declaration only; no layout change.
    bool IsSolidToThing(const CThing* thing) const;                  // 0x00770DE0
#endif
#ifdef FABLE_RETAIL_CTCSOLIDTOTHING_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals the declarations above lack, declaration only (no layout change; the
    // vtable already exists). Slots from the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots;
    // spellings from the Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x00771020
public:
#endif
};
#endif
#endif // FABLE_ENGINE_CTCSOLIDTOTHING_H
