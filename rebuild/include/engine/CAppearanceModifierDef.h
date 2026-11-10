#ifndef FABLE_ENGINE_CAPPEARANCEMODIFIERDEF_H
#define FABLE_ENGINE_CAPPEARANCEMODIFIERDEF_H
#include "engine/CSubDefClassBase.h"
#include "engine/EClothingSuitPart.h"
#include "stlport/fable_containers.h"
class CAppearanceModifierScalingDef;
class CAppearanceModifierDef:public CSubDefClassBase {
public:
    unsigned char HideMaterials[12],MorphGraphics[12];long CoversBodyAreaFlags;bool IsRemoveable;long HeroSuit;EClothingSuitPart SuitPart;long AppearanceType;
    float SoundRadiusMultiplier,VisibilityMultiplier;long ArmourDefIndex;float OveralProtectionWeighting,Scariness,Attractiveness,Goodstrength;
    float GetTypeBalancedAttractiveness(const std::vector<CAppearanceModifierScalingDef>&)const;
    float GetTypeBalancedScariness(const std::vector<CAppearanceModifierScalingDef>&)const;
    float GetTypeBalancedGoodstrength(const std::vector<CAppearanceModifierScalingDef>&)const;
};
FABLE_STATIC_ASSERT(sizeof(CAppearanceModifierDef)==0x70);
FABLE_STATIC_ASSERT(offsetof(CAppearanceModifierDef,SuitPart)==0x4c);
#endif
