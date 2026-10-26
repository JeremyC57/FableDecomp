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