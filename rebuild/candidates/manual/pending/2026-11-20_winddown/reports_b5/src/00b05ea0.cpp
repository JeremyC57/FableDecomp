// 00b05ea0 NParticleEngine::CPSCSingleSprite::DatabaseCopy, PDB00ac7410.
// Leaf: ?DatabaseCopy@CPSCSingleSprite@NParticleEngine@@UAEXABVCPSCBase@2@@Z
// Layout: PDB derived members56..154 are retail52..150 due inherited spline vector -4.
// Field names and bitfield order recovered from struct_layouts_egor.tsv.
#define FABLE_RETAIL_CPSCSPLINEBASE_MEMBERS
#define FABLE_RETAIL_CPSCSPLINEBASE_PASS23
#define FABLE_RETAIL_CPSCSINGLESPRITE_MEMBERS
#include "engine/CPSCSingleSprite.h"
namespace NParticleEngine {
void CPSCSingleSprite::DatabaseCopy(const CPSCBase& base){
 CPSCSplineBase::DatabaseCopy(base);const CPSCSingleSprite& other=(const CPSCSingleSprite&)base;
 SpriteBankIndex=other.SpriteBankIndex;PositionParam=other.PositionParam;ForceAnimationTime=other.ForceAnimationTime;
 AnimationTimeSecs=other.AnimationTimeSecs;SelfIlluminating=other.SelfIlluminating;StartRenderSize=other.StartRenderSize;EndRenderSize=other.EndRenderSize;
 BlendMode=other.BlendMode;BlendOp=other.BlendOp;TrailBlendMode=other.TrailBlendMode;TrailBlendOp=other.TrailBlendOp;
 FaceMe2D=other.FaceMe2D;FaceMe3D=other.FaceMe3D;CrossedSprites=other.CrossedSprites;SpriteAlignment=other.SpriteAlignment;RotateAroundCentre=other.RotateAroundCentre;
 NoCrossedSprites=other.NoCrossedSprites;InitialAngle=other.InitialAngle;FadeInEndInteger=other.FadeInEndInteger;FadeOutBeginInteger=other.FadeOutBeginInteger;
 AlphaFadeEnable=other.AlphaFadeEnable;AlphaFadeMinimum=other.AlphaFadeMinimum;SizeFadeEnable=other.SizeFadeEnable;SizeFadeMinimum=other.SizeFadeMinimum;
 StartColour=other.StartColour;MidColour=other.MidColour;EndColour=other.EndColour;
 UseStartColour=other.UseStartColour;UseMidColour=other.UseMidColour;UseEndColour=other.UseEndColour;
 TrailBankIndex=other.TrailBankIndex;TrailWidth=other.TrailWidth;MaxTrailLength=other.MaxTrailLength;StayWithEmitter=other.StayWithEmitter;
 UsePosition=other.UsePosition;UseSplinePoints=other.UseSplinePoints;
}
}
