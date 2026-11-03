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
};
FABLE_STATIC_ASSERT(sizeof(CTCTeleporter)==28);
#endif
