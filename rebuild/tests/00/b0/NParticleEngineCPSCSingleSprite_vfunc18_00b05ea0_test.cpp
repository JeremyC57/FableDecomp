// BEGIN exact source snapshot SHA256 847507b971760ff4ca782db37e8bfe575c835f1a6fcb62ea8205e97272b69dfe
// 00b05ea0 NParticleEngine::CPSCSingleSprite::DatabaseCopy, PDB00ac7410.
// Layout: PDB derived members56..154 are retail52..150 due inherited spline vector -4.
// Field names and bitfield order recovered from struct_layouts_egor.tsv.
namespace NParticleEngine {
class CPSCBase { public:unsigned char Data[0x34];};
class CPSCSplineBase:public CPSCBase { public:void DatabaseCopy(const CPSCBase&);}; //00b22530
class CPSCSingleSprite:public CPSCSplineBase { public:
 long SpriteBankIndex;unsigned long StartColour,MidColour,EndColour;
 float AnimationTimeSecs,StartRenderSize,EndRenderSize;long SpriteAlignment,NoCrossedSprites;float InitialAngle;
 long FadeInEndInteger,FadeOutBeginInteger;float AlphaFadeMinimum,SizeFadeMinimum;unsigned long PositionParam;
 long BlendMode,BlendOp,TrailBlendMode,TrailBlendOp,TrailBankIndex;float TrailWidth;long MaxTrailLength,StayWithEmitterIntFactor;
 bool UseStartColour,UseMidColour,UseEndColour;
 bool SelfIlluminating:1,AlphaFadeEnable:1,RotateAroundCentre:1,FaceMe2D:1,FaceMe3D:1,CrossedSprites:1,SizeFadeEnable:1,ForceAnimationTime:1;
 bool StayWithEmitter:1;bool UsePosition,UseSplinePoints;
 void DatabaseCopy(const CPSCBase&);
};
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

// END exact source snapshot
#include <stdio.h>
#include <string.h>
static int bases;
void NParticleEngine::CPSCSplineBase::DatabaseCopy(const CPSCBase& b){++bases;memcpy(Data,b.Data,sizeof(Data));}
int main(){using namespace NParticleEngine;CPSCSingleSprite a,b;memset(&a,0,sizeof(a));memset(&b,0,sizeof(b));b.SpriteBankIndex=7;b.TrailWidth=2.5f;b.ForceAnimationTime=true;b.FaceMe2D=true;b.UseSplinePoints=true;b.StayWithEmitterIntFactor=99;a.StayWithEmitterIntFactor=12;b.Data[0]=42;
 a.DatabaseCopy(b);if(bases!=1||a.Data[0]!=42||a.SpriteBankIndex!=7||a.TrailWidth!=2.5f||!a.ForceAnimationTime||!a.FaceMe2D||!a.UseSplinePoints||a.StayWithEmitterIntFactor!=12)return 1;
 b.ForceAnimationTime=false;b.FaceMe2D=false;b.AlphaFadeEnable=true;a.DatabaseCopy(b);if(a.ForceAnimationTime||a.FaceMe2D||!a.AlphaFadeEnable)return 2;
 puts("00b05ea0_TEST PASS");return 0;}