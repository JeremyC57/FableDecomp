// 00681790 CTCCreatureOpinionOfHero::TransformForcedOpinion
// Leaf: ?TransformForcedOpinion@CTCCreatureOpinionOfHero@@QBEMW4EOpinion@@M@Z
// Flags:
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#include "engine/CParentDefClassBase.h"
#define CTCCreatureOpinionOfHero CTCCreatureOpinionOfHeroLayout
#include "engine/CTCCreatureOpinionOfHero.h"
#undef CTCCreatureOpinionOfHero
enum EOpinion{};
class COpinionPersonalityTraits{public:float BaseOffset,HeroStatScaling,VillageOpinionScaling,TimeScalingRunIn,TimeScalingRunOut,PeakScalingIfPos,PeakScalingIfNeg,PersistScalingIfPos,PersistScalingIfNeg;};
// Layout: actual parent base ends39, pointer aligned at3c; traits stride36.
class COpinionPersonalityDef:public CParentDefClassBase{public:COpinionPersonalityTraits*PersonalityTraits;};
// Layout: supplied fields retained; original public const method restored.
class CTCCreatureOpinionOfHero:public CTCCreatureOpinionOfHeroLayout{public:float TransformForcedOpinion(EOpinion,float)const;};
float CTCCreatureOpinionOfHero::TransformForcedOpinion(EOpinion opinion,float value)const{
 const CDefPointer<const COpinionPersonalityDef>&def=*reinterpret_cast<const CDefPointer<const COpinionPersonalityDef>*>(&PPersonalityDef_Object);
 if(!def.IsNull()){
  const COpinionPersonalityTraits&traits=def->PersonalityTraits[opinion];
  return traits.BaseOffset+value*traits.HeroStatScaling;
 }
 return value;
}
