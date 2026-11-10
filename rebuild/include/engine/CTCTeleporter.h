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
};
FABLE_STATIC_ASSERT(sizeof(CTCTeleporter)==28);
#endif
