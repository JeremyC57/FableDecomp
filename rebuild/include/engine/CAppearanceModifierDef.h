#ifndef FABLE_ENGINE_CAPPEARANCEMODIFIERDEF_H
#define FABLE_ENGINE_CAPPEARANCEMODIFIERDEF_H
#include "engine/CSubDefClassBase.h"
#include "engine/EClothingSuitPart.h"
#include "stlport/fable_containers.h"
class CAppearanceModifierScalingDef;
#ifdef FABLE_RETAIL_CAPPEARANCEMODIFIERDEF_STATGETTYPENAME
#include "fable_string.h"
#endif
class CAppearanceModifierDef:public CSubDefClassBase {
public:
#ifdef FABLE_RETAIL_CAPPEARANCEMODIFIERDEF_STATGETTYPENAME
    // Per-TU opt-in (agentCLO 2026-11-10, row 0x005B8743): FableWin.pdb ?StatGetTypeName@CAppearanceModifierDef@@SI?AVCCharString@@XZ,
    // inline in the release build; the "CAppearanceModifierDef" literal the landed GetSubDef<CAppearanceModifierDef> 0x0057FF92 passes.
    static CCharString __fastcall StatGetTypeName() { return CCharString("CAppearanceModifierDef", -1); }
#endif
    unsigned char HideMaterials[12],MorphGraphics[12];long CoversBodyAreaFlags;bool IsRemoveable;long HeroSuit;EClothingSuitPart SuitPart;long AppearanceType;
    float SoundRadiusMultiplier,VisibilityMultiplier;long ArmourDefIndex;float OveralProtectionWeighting,Scariness,Attractiveness,Goodstrength;
    float GetTypeBalancedAttractiveness(const std::vector<CAppearanceModifierScalingDef>&)const;
    float GetTypeBalancedScariness(const std::vector<CAppearanceModifierScalingDef>&)const;
    float GetTypeBalancedGoodstrength(const std::vector<CAppearanceModifierScalingDef>&)const;
};
FABLE_STATIC_ASSERT(sizeof(CAppearanceModifierDef)==0x70);
FABLE_STATIC_ASSERT(offsetof(CAppearanceModifierDef,SuitPart)==0x4c);
#endif
