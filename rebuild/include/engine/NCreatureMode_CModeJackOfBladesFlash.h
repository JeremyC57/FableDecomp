#ifndef FABLE_ENGINE_NCREATUREMODE_CMODEJACKOFBLADESFLASH_H
#define FABLE_ENGINE_NCREATUREMODE_CMODEJACKOFBLADESFLASH_H
// Original complete owner: both PDBs, CModeStandBase inheritance and native fields.
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"
#include "engine/CAnimFilterOfGroup.h"
#include "engine/CIntelligentPointer.h"
class CThingCreatureBase;
namespace NCreatureMode {
class CModeJackOfBladesFlash:public CModeStandBase {
public:
 static unsigned long FABLE_FASTCALL StatGetType();
 virtual unsigned long GetType()const;
 virtual void OnActivate();
#ifdef FABLE_RETAIL_JACK_FLASH_FRAME_PASS29
 virtual void FrameUpdate();
private:
 void Flash();
 void ExtinguishFlames();
#endif

private:
 long Timer,BurnCounter;
 CAnimFilterOfGroup Filter;
 CThingCreatureBase* PCreature;
 CIntelligentPointer<CThingCreatureBase> PTarget;
 CIntelligentPointer<CThing> PEmitter;
 CIntelligentPointer<CThing> PSpotLight;
};
}
FABLE_STATIC_ASSERT(sizeof(NCreatureMode::CModeJackOfBladesFlash)==0x98);
#endif
