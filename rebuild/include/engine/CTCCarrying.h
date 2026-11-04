#ifndef FABLE_ENGINE_CTCCARRYING_H
#define FABLE_ENGINE_CTCCARRYING_H
// New shared header (chunk 732, CThingPlayerCreature melee rows): the retail CTCCarrying over FABLE_RETAIL_CTCBASE (RTTI
// .?AVCTCCarrying@@ : CTCBase@0). Members are not modelled; only what the rows call: Ego_r.pdb / FableWin.pdb
// ?GetThingInPrimarySlot@CTCCarrying@@QBEPAVCThing@@XZ (retail 0x006A1440, landed) and ?RemoveThingFromPrimarySlot@CTCCarrying@@QAEPAVCThing@@_N@Z
// (0x006A1E00, landed). StatGetInterfaceType: TCI_CARRYING (70; FableWin ?StatGetInterfaceType@CTCCarrying@@SI?AW4ETCInterfaceType@@XZ,
// retail tests bit 6 of +0x28).
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
class CThing;
class CTCCarrying : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_CARRYING; }
    CThing* GetThingInPrimarySlot() const;                   // 0x006A1440
    CThing* RemoveThingFromPrimarySlot(bool);                // 0x006A1E00
#ifdef FABLE_RETAIL_CTCCARRYING
    // Chunk 756 (row 0x006CA380 CTCHero::SetWeaponsAsUsable): Ego_r.pdb ?IsThingCarryingWeapon@CTCCarrying@@SI_NABVCThing@@@Z (0x006A1540).
    static bool __fastcall IsThingCarryingWeapon(const CThing& thing);
#endif
#ifdef FABLE_RETAIL_CTCCARRYING_PUTTHING
    // Chunk 764 (row 0x00849F70 CCreatureAction_PickUpThingAtTable::PerformAction): Ego_r.pdb
    // ?PutThingInCarrySlot@CTCCarrying@@QAEXAAVCThing@@J_N@Z (0x006A1F80).
    void PutThingInCarrySlot(CThing& thing, long slot, bool immediately);
#endif
#ifdef FABLE_RETAIL_CTCCARRYING_ALLCARRIED
    // Nested opt-in (chunk 768, row 0x0089DA70 CGameScriptInterface::EntitySetAlpha): Ego_r.pdb (0x006C6970)
    // ?GetAllCarriedThings@CTCCarrying@@QBEXAAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@@Z, retail 0x006A2220 (const, the
    // STLport vector by reference). Declaration only; needs std::vector (stlport/fable_containers.h) before this header.
    void GetAllCarriedThings(std::vector<CThing*>& things) const;   // 0x006A2220
#endif
};
#endif
