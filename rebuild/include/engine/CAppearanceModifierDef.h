#ifndef FABLE_ENGINE_CAPPEARANCEMODIFIERDEF_H
#define FABLE_ENGINE_CAPPEARANCEMODIFIERDEF_H
#include "engine/CSubDefClassBase.h"
#ifdef FABLE_RETAIL_HERO_MODIFIER_GRAPHICS_PASS29
#include "engine/CAppearanceModifierGraphics.h"
#endif
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
#ifdef FABLE_RETAIL_HERO_MODIFIER_GRAPHICS_PASS29
    unsigned char HideMaterials[12]; CAppearanceModifierGraphics MorphGraphics;long CoversBodyAreaFlags;bool IsRemoveable;long HeroSuit;EClothingSuitPart SuitPart;long AppearanceType;
#else
    unsigned char HideMaterials[12],MorphGraphics[12];long CoversBodyAreaFlags;bool IsRemoveable;long HeroSuit;EClothingSuitPart SuitPart;long AppearanceType;
#endif
    float SoundRadiusMultiplier,VisibilityMultiplier;long ArmourDefIndex;float OveralProtectionWeighting,Scariness,Attractiveness,Goodstrength;
    float GetTypeBalancedAttractiveness(const std::vector<CAppearanceModifierScalingDef>&)const;
    float GetTypeBalancedScariness(const std::vector<CAppearanceModifierScalingDef>&)const;
    float GetTypeBalancedGoodstrength(const std::vector<CAppearanceModifierScalingDef>&)const;
#ifdef FABLE_RETAIL_CAPPEARANCEMODIFIERDEF_ALLOC
// Opt-in (wfE11, 2026-11-15, row 0x0044C72B CGameDefinitionManager::InitAndCompile, which registers its address): the static allocator
// ?Alloc@CAppearanceModifierDef@@SIPAVCDefClassBase@@XZ (Ego_r.pdb + FableWin.pdb). Declaration only (elaborated `class`: needs no include); no layout change.
public: static class CDefClassBase* FABLE_FASTCALL Alloc();
#endif
};
FABLE_STATIC_ASSERT_AT(23, sizeof(CAppearanceModifierDef)==0x70);
FABLE_STATIC_ASSERT_AT(24, offsetof(CAppearanceModifierDef,SuitPart)==0x4c);
#endif
