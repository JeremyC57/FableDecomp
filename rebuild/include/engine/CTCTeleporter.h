#ifndef FABLE_ENGINE_CTCTELEPORTER_H
#define FABLE_ENGINE_CTCTELEPORTER_H
#include "engine/CTCBase.h"
// Layout: original donor28 and native Active+10. The original packed intelligent
// pointer is not touched here; its named eight-byte storage preserves its extent.
class CTCTeleporter : public CTCBase {
private:
 unsigned char PDef[4];
 bool Active,Activated,HeroInsideRadius;
 unsigned char PParticle[8];
 bool WaitingForTutorialToFinish;
public:
 static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_TELEPORTER; }
 bool IsActive() const { return Active; }
#ifdef FABLE_RETAIL_CTCTELEPORTER_TELEPORTHERO
 // Nested opt-in (chunk 1011, row 0x005FBFA0 CTCInventoryMap::TeleportToActiveRegion): Ego_r.pdb 0x005A4F20
 // ?TeleportHero@CTCTeleporter@@QAEXXZ, out of line in retail at 0x007DFCF0. Declaration only.
 void TeleportHero();                                                 // 0x007DFCF0
#endif
#ifdef FABLE_RETAIL_CTCTELEPORTER_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x007E0520
public:
    virtual bool IsComponentSerialisable() const;  // slot 23, 0x007E0530
public:
#endif
};
FABLE_STATIC_ASSERT(sizeof(CTCTeleporter)==28);
#endif
