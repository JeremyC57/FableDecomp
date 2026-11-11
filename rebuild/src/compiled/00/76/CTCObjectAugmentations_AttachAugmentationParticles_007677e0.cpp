// CTCObjectAugmentations::AttachAugmentationParticles -- construct periodic and attack attachment sets.
// Inline-budget: T59 plus guarded original find and periodic-vector destructor call boundaries; all bodies remain visible.
// Retail: 007677e0,1467 bytes plus five padding bytes in the assigned extent.
// Leaf: ?AttachAugmentationParticles@CTCObjectAugmentations@@AAEXXZ
// Flags: /d2inlT59
#define FABLE_RETAIL_AUGMENTATION_PARTICLES
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_BODY
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#include "engine/CThing.h"
#include "engine/CTCObjectAugmentations.h"
#include "engine/CMainGameComponent.h"
void CTCObjectAugmentations::AttachAugmentationParticles()
{
 ClearAllParticleEffects();
 if(!GetParentThing().IsInLimbo()&&!GetParentThing().HasTC((ETCInterfaceType)0x77)){
  typedef std::map<EObjectAugmentationType,CObjectAugmentationParticleSet> ParticleMap;
  ParticleMap::const_iterator findit=PDef->AugmentationParticles.find((EObjectAugmentationType)12);
  bool exists_lightning_and_flame_combo=findit!=PDef->AugmentationParticles.end()&&HasCombinationOfAugmentations((EObjectAugmentationType)12);
  ParticleMap::const_iterator it,endit;
  for(it=PDef->AugmentationParticles.begin(),endit=PDef->AugmentationParticles.end();it!=endit;++it){
   if(HasAugmentationOfType((*it).first)){
    long num=(*it).second.StartPoint.size();
    std::vector<ParticleAndEndPoints> peps;
    std::vector<AttackParticleAndEndPoints> apeps;
    for(long i=0;i<num;++i){
     if((*it).second.ParticleEffect>0){
      ParticleAndEndPoints pep;
      pep.PeriodicFrames=(long)((*it).second.PeriodSecs*CMainGameComponent::GetConstantFPS());
      pep.ParticleEffectID=(*it).second.ParticleEffect;
      pep.ParticleEffectIDToBlendTo=(*it).second.ParticleEffectToBlendTo;
      pep.StartPoint=(*it).second.StartPoint[i];
      pep.EndPoint=(*it).second.EndPoint[i];
      peps.push_back(pep);
     }
     if((*it).second.AttackParticleEffect>0&&(!exists_lightning_and_flame_combo||(*it).first==(EObjectAugmentationType)12)){
      AttackParticleAndEndPoints apep;
      apep.ParticleEffect=0;
      apep.ParticleEffectID=(*it).second.AttackParticleEffect;
      apep.ParticleEffectIDToBlendTo=(*it).second.AttackParticleEffectToBlendTo;
      apep.StartPoint=(*it).second.StartPoint[i];
      apep.EndPoint=(*it).second.EndPoint[i];
      apeps.push_back(apep);
     }
    }
    ParticleEffects.push_back(peps);
    AttackParticleEffects.push_back(apeps);
   }
   if(exists_lightning_and_flame_combo&&(*it).first==(EObjectAugmentationType)12){
    long num=(*it).second.StartPoint.size();
    std::vector<AttackParticleAndEndPoints> apeps;
    for(long i=0;i<num;++i){
     AttackParticleAndEndPoints apep;
     apep.ParticleEffect=0;
     apep.ParticleEffectID=(*it).second.AttackParticleEffect;
     apep.ParticleEffectIDToBlendTo=(*it).second.AttackParticleEffectToBlendTo;
     apep.StartPoint=(*it).second.StartPoint[i];
     apep.EndPoint=(*it).second.EndPoint[i];
     apeps.push_back(apep);
    }
    AttackParticleEffects.push_back(apeps);
   }
  }
 }
}