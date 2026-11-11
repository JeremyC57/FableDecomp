#ifndef FABLE_ENGINE_CMODETHUNDERLIGHTNINGSTORM_H
#define FABLE_ENGINE_CMODETHUNDERLIGHTNINGSTORM_H
// Complete original owner from both original PDBs and native offsets.
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"
#include "engine/CAnimFilterOfGroup.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
class CSpecialAbilitiesThunderLightningStormDef;
namespace NCreatureMode {
class CModeThunderLightningStorm:public CModeStandBase{
public:
 static unsigned long FABLE_FASTCALL StatGetType();
 virtual unsigned long GetType() const;
 static CTCCreatureModeManager::CModeBase * FABLE_FASTCALL Alloc(CThing & p0);
 CModeThunderLightningStorm(const NCreatureMode::CModeThunderLightningStorm & p0);
 CModeThunderLightningStorm(CThing & p0);
 virtual const CAnimFilterBase & GetAnimFilter() const;
 virtual void OnActivate();
 virtual void OnDeactivate();
 virtual void FrameUpdate();
private:
 void UpdateLightningBeams();
 void CreateLightningBeams();
 void CreateSkyBeam();
 void CreateHandGlow();
 void UpdateDamage();
 void StopLoopingSound();
 void StartLoopingSound();
 CAnimFilterOfGroup Filter;
 std::vector<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing> > > LightningBeamParticleEmitters;
 std::vector<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing> > > LightningBeamEndParticleEmitters;
 CIntelligentPointer<CThing> PSkyBeam;
 CIntelligentPointer<CThing> PCreatorEffect;
 CIntelligentPointer<CThing> PHandGlow;
 CDefPointer<CSpecialAbilitiesThunderLightningStormDef const > PLightningStormDef;
 std::vector<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing> > > HitThings;
 bool MainBeamsCreated;
 long Timer;
 float StormRotation;
 float StormRadius;
 unsigned long LoopingSound;
public:
 virtual ~CModeThunderLightningStorm();
};
FABLE_STATIC_ASSERT(sizeof(CModeThunderLightningStorm)==200);
}
#endif
