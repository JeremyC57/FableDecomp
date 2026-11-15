// Full OnAppearanceDraw and genuine Sprite constructor bodies with portable graphics fixtures.
#include <assert.h>
#include <stdio.h>
#include <math.h>
static bool Close(float a,float b){return fabs((double)a-b)<0.00001;}
enum ETCInterfaceType {TCI_EXPERIENCE_ORB=105};
enum EEnginePrimitiveType {ENGINE_PRIMITIVE_SPRITE=7};
enum EEnginePrimitiveBlendMode {ENGINE_PRIMITIVE_BLEND_SOLID=0,ENGINE_PRIMITIVE_BLEND_BOOLEAN_ALPHA=1,ENGINE_PRIMITIVE_BLEND_ALPHA=2,ENGINE_PRIMITIVE_BLEND_ADDITIVE=3};
enum EEngineInterpolationSet {ENGINE_INTERPOLATION_SET_0=0};enum EEnginePrimitiveBlendOp {ENGINE_PRIMITIVE_BLEND_OP_ADD=0};
class C3DVector {public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}C3DVector operator+(const C3DVector& p)const{return C3DVector(X+p.X,Y+p.Y,Z+p.Z);}C3DVector operator-(const C3DVector& p)const{return C3DVector(X-p.X,Y-p.Y,Z-p.Z);}C3DVector operator*(float scale)const{return C3DVector(X*scale,Y*scale,Z*scale);}const C3DVector Interpolate(const C3DVector& vector,float interp)const{return *this+(vector-*this)*interp;}};
class C2DVector {public:float X,Y;C2DVector(float x,float y):X(x),Y(y){}};
class CRGBColour {public:unsigned char B,G,R,A;CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a):B(b),G(g),R(r),A(a){}};
class CEnginePrimitiveThemeChannelDesc {public:unsigned char R,G,B;CEnginePrimitiveThemeChannelDesc():R(0),G(0),B(0){}};
class CEnginePrimitive {public:EEnginePrimitiveType Type;EEngineInterpolationSet InterpolationSet;CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;CEnginePrimitive(EEnginePrimitiveType type,EEngineInterpolationSet set):Type(type),InterpolationSet(set){}};
class CEnginePrimitiveSprite:public CEnginePrimitive {public:C3DVector WorldPos;unsigned long BankIndex,Frame;float SizeX,SizeY,FadeStart,Angle;CRGBColour Colour;unsigned short Flags;EEnginePrimitiveBlendMode BlendMode;EEnginePrimitiveBlendOp BlendOp;C2DVector Offset;
 CEnginePrimitiveSprite(const C3DVector&,unsigned long,unsigned long,unsigned short,float,float,float,EEnginePrimitiveBlendMode,unsigned char,unsigned char,unsigned char,unsigned char,float,EEngineInterpolationSet,EEnginePrimitiveBlendOp);
};
class C3DPositionInterpolator {public:C3DVector StartPosition,EndPosition;C3DVector GetInterpolated(float interp)const{return StartPosition.Interpolate(EndPosition,interp);}};
class CEngineGraphic {public:unsigned long BankIndex;float AnimStep,RenderSizeX;bool AdditiveAlpha;unsigned char Type;};
static int BankReads,FrameQueries,GraphicQueries,InfoFrameQueries,PrimitiveAlive,StageQueries;
class CGraphicInfoFrame {public:unsigned short Width,Height;unsigned char Transparency;unsigned long GetHeight()const{return Height;}unsigned long GetWidth()const{return Width;}bool IsBooleanAlpha()const{return Transparency==2 || Transparency==4;}bool IsAlpha()const{return Transparency!=0;}};
class CGraphicInfo {public:CGraphicInfoFrame Frames[10];const CGraphicInfoFrame* PeekInfoFrame(unsigned long frame)const{assert(frame<10);++InfoFrameQueries;return Frames+frame;}};
class CGraphicDataBank {public:CGraphicInfo Graphic;unsigned long GetNoFramesInGraphic(unsigned long bank)const{assert(bank==77);++FrameQueries;return 10;}const CGraphicInfo* PeekGraphic(unsigned long bank)const{assert(bank==77);++GraphicQueries;return &Graphic;}};
class CThing {public:bool Orb;CGraphicDataBank* Bank;bool HasTC(ETCInterfaceType type)const{assert(type==TCI_EXPERIENCE_ORB);return Orb;}const CGraphicDataBank* PeekGraphicBank()const{++BankReads;return Bank;}};
class CTCBase {public:CThing* ParentThing;CThing& GetParentThing(){return *ParentThing;}};
namespace NGraphicAppearance {
enum EPrimitiveType {GAPT_NONE=0,GAPT_STATIC_MESH=1,GAPT_ANIMATING_MESH=2,GAPT_MISC=3};
class CPrimitive {public:EPrimitiveType Type;long BankIndex;CPrimitive(EPrimitiveType type,long bank_index=0):Type(type),BankIndex(bank_index){++PrimitiveAlive;}~CPrimitive(){--PrimitiveAlive;}};
class CGraphicAppearanceDrawInfo {public:float Interpolate;C3DPositionInterpolator* Pos;};
class CTCGraphicAppearance {public:CEngineGraphic Graphic;unsigned char Alpha;bool SelfIlluminating;CPrimitive Primitive;int Added,Set,AlphaReads;CEnginePrimitiveSprite const* DuringCall;
 C3DVector WantPos;float WantAngle;unsigned long WantFrame;unsigned short WantFlags;EEnginePrimitiveBlendMode WantBlend;unsigned char WantAlpha;
 CTCGraphicAppearance():Primitive(GAPT_NONE),Added(0),Set(0),AlphaReads(0),DuringCall(0){}
 const CEngineGraphic& GetMainGraphic()const{return Graphic;}float GetFullScale()const{return 2.5f;}
 unsigned char GetAlpha(){++AlphaReads;return Alpha;}bool IsSelfIlluminating()const{return SelfIlluminating;}
 CPrimitive* AddPersistentPrimitive(const CPrimitive& primitive){assert(primitive.Type==GAPT_MISC && primitive.BankIndex==0 && PrimitiveAlive==2);++Added;Primitive.Type=primitive.Type;return &Primitive;}
 void SetPrimitiveGraphics(CPrimitive* primitive,const CEnginePrimitive& value,unsigned short extra){assert(primitive==&Primitive && extra==0 && PrimitiveAlive==1);const CEnginePrimitiveSprite& sprite=static_cast<const CEnginePrimitiveSprite&>(value);assert(sprite.Type==ENGINE_PRIMITIVE_SPRITE && sprite.InterpolationSet==ENGINE_INTERPOLATION_SET_0);assert(sprite.ThemeChannelDesc.R==0 && sprite.ThemeChannelDesc.G==0 && sprite.ThemeChannelDesc.B==0);assert(Close(sprite.WorldPos.X,WantPos.X) && Close(sprite.WorldPos.Y,WantPos.Y) && Close(sprite.WorldPos.Z,WantPos.Z));assert(sprite.BankIndex==77 && sprite.Frame==WantFrame && Close(sprite.SizeX,2.5f) && Close(sprite.SizeY,3.75f) && Close(sprite.Angle,WantAngle));assert(sprite.FadeStart==0 && sprite.BlendOp==(EEnginePrimitiveBlendOp)0 && sprite.BlendMode==WantBlend && sprite.Flags==WantFlags);assert(sprite.Colour.A==WantAlpha && sprite.Colour.R==255 && sprite.Colour.G==255 && sprite.Colour.B==255 && sprite.Offset.X==0 && sprite.Offset.Y==0);++Set;}
};
class CAGraphicAppearanceComponent {public:virtual bool OnAppearanceDraw(CTCGraphicAppearance&,const CGraphicAppearanceDrawInfo&)=0;};
class CTCGraphicAppearanceSprite:public CTCBase,public CAGraphicAppearanceComponent {public:float RotationAmountPerFrame,RotationAngle,OldRotationAngle,AnimStage,LastAnimStage,AnimSpeed;bool FadeOutWithAnim;CPrimitive* PrimHandle;float Stage,WantInterpolate;
 float GetInterpolatedAnimStage(float interp)const{assert(interp==WantInterpolate);++StageQueries;return Stage;}
 virtual bool OnAppearanceDraw(CTCGraphicAppearance&,const CGraphicAppearanceDrawInfo&);
};
}
CEnginePrimitiveSprite::CEnginePrimitiveSprite(const C3DVector& world_pos,unsigned long bank_index,unsigned long frame,unsigned short flags,float angle,float size_x,float size_y,EEnginePrimitiveBlendMode blend_mode,unsigned char alpha,unsigned char r,unsigned char g,unsigned char b,float fade_start,EEngineInterpolationSet interp_set,EEnginePrimitiveBlendOp blend_op)
    : CEnginePrimitive((EEnginePrimitiveType)7,interp_set),WorldPos(world_pos),BankIndex(bank_index),Frame(frame),SizeX(size_x),SizeY(size_y),FadeStart(fade_start),Angle(angle),Colour(r,g,b,alpha),Flags(flags),BlendMode(blend_mode),BlendOp(blend_op),Offset(0.0f,0.0f)
{
}
namespace NGraphicAppearance {
bool CTCGraphicAppearanceSprite::OnAppearanceDraw(CTCGraphicAppearance& tc_appearance,const CGraphicAppearanceDrawInfo& draw_info)
{
    const CEngineGraphic& graphic=tc_appearance.GetMainGraphic();
    unsigned long sprite_flags=0;
    if(graphic.Type==2)
        sprite_flags=1; // ENGINE_SPRITE_3D_FACE_ME, original EEngineSpriteFlag.
    if(GetParentThing().HasTC(TCI_EXPERIENCE_ORB))
        sprite_flags|=16; // ENGINE_SPRITE_ALIGN_BOTTOM.
    long no_frames_in_graphic=GetParentThing().PeekGraphicBank()->GetNoFramesInGraphic(graphic.BankIndex);
    float interpolated_anim_stage=GetInterpolatedAnimStage(draw_info.Interpolate);
    unsigned long frame=(unsigned long)(no_frames_in_graphic*interpolated_anim_stage);
    const CGraphicInfoFrame* graphic_frame=GetParentThing().PeekGraphicBank()->PeekGraphic(graphic.BankIndex)->PeekInfoFrame(frame);
    float size_x=tc_appearance.GetFullScale();
    float size_y=(float)graphic_frame->GetHeight()/graphic_frame->GetWidth()*size_x;
    float sprite_angle=(RotationAngle-OldRotationAngle)*draw_info.Interpolate+RotationAngle;
    EEnginePrimitiveBlendMode blend_mode;
    if(graphic.AdditiveAlpha)
        blend_mode=ENGINE_PRIMITIVE_BLEND_ADDITIVE;
    else if(graphic_frame->IsBooleanAlpha())
        blend_mode=ENGINE_PRIMITIVE_BLEND_ALPHA;
    else if(graphic_frame->IsAlpha())
        blend_mode=ENGINE_PRIMITIVE_BLEND_ALPHA;
    else
        blend_mode=ENGINE_PRIMITIVE_BLEND_ADDITIVE;
    long alpha=tc_appearance.GetAlpha();
    if(FadeOutWithAnim) {
        float source_alpha=tc_appearance.GetAlpha();
        alpha=(long)(source_alpha*(1.0f-interpolated_anim_stage));
    }
    if(!PrimHandle)
        PrimHandle=tc_appearance.AddPersistentPrimitive(CPrimitive(GAPT_MISC));
    if(!tc_appearance.IsSelfIlluminating())
        sprite_flags|=128; // ENGINE_SPRITE_LIT.
    C3DVector pos=draw_info.Pos->GetInterpolated(draw_info.Interpolate);
    tc_appearance.SetPrimitiveGraphics(PrimHandle,CEnginePrimitiveSprite(pos,graphic.BankIndex,frame,(unsigned short)sprite_flags,sprite_angle,size_x,size_y,blend_mode,(unsigned char)alpha,255,255,255,0.0f,ENGINE_INTERPOLATION_SET_0,(EEnginePrimitiveBlendOp)0),0);
    return true;
}
}

int main()
{
 const unsigned char transparencies[]={0,1,2,4,5};const unsigned char alphas[]={0,127,255};const float interps[]={0.0f,0.25f,1.0f};const float stages[]={0.25f,0.5f,0.75f};
 for(int flags=0;flags<64;++flags)for(int transparency=0;transparency<5;++transparency)for(int alpha=0;alpha<3;++alpha)for(int interp=0;interp<3;++interp)for(int stage=0;stage<3;++stage){
  CGraphicDataBank bank;for(int i=0;i<10;++i){bank.Graphic.Frames[i].Width=64;bank.Graphic.Frames[i].Height=96;bank.Graphic.Frames[i].Transparency=transparencies[transparency];}
  CThing thing;thing.Orb=(flags&1)!=0;thing.Bank=&bank;C3DPositionInterpolator position;position.StartPosition=C3DVector(2,-4,6);position.EndPosition=C3DVector(10,8,-2);
  NGraphicAppearance::CGraphicAppearanceDrawInfo info;info.Interpolate=interps[interp];info.Pos=&position;
  NGraphicAppearance::CTCGraphicAppearance appearance;appearance.Graphic.BankIndex=77;appearance.Graphic.Type=(flags&2)?2:0;appearance.Graphic.AdditiveAlpha=(flags&4)!=0;appearance.Alpha=alphas[alpha];appearance.SelfIlluminating=(flags&8)!=0;
  NGraphicAppearance::CTCGraphicAppearanceSprite sprite;sprite.ParentThing=&thing;sprite.RotationAngle=2.0f;sprite.OldRotationAngle=1.0f;sprite.RotationAmountPerFrame=-5.0f;sprite.AnimStage=0.25f;sprite.LastAnimStage=0.125f;sprite.AnimSpeed=0.75f;sprite.FadeOutWithAnim=(flags&16)!=0;sprite.PrimHandle=(flags&32)?&appearance.Primitive:0;sprite.Stage=stages[stage];sprite.WantInterpolate=info.Interpolate;
  appearance.WantPos=C3DVector(2+8*info.Interpolate,-4+12*info.Interpolate,6-8*info.Interpolate);appearance.WantAngle=2+info.Interpolate;appearance.WantFrame=stage==0?2:stage==1?5:7;
  appearance.WantFlags=(unsigned short)((flags&2?1:0)|(flags&1?16:0)|(flags&8?0:128));appearance.WantBlend=(flags&4)||transparency==0?ENGINE_PRIMITIVE_BLEND_ADDITIVE:ENGINE_PRIMITIVE_BLEND_ALPHA;
  unsigned int expectedAlpha=alphas[alpha];if(flags&16)expectedAlpha=stage==0?expectedAlpha*3/4:stage==1?expectedAlpha/2:expectedAlpha/4;appearance.WantAlpha=(unsigned char)expectedAlpha;
  BankReads=FrameQueries=GraphicQueries=InfoFrameQueries=StageQueries=0;
  assert(sprite.OnAppearanceDraw(appearance,info));assert(appearance.Set==1 && appearance.Added==((flags&32)?0:1));assert(sprite.PrimHandle==&appearance.Primitive);assert(BankReads==2 && FrameQueries==1 && GraphicQueries==1 && InfoFrameQueries==1 && StageQueries==1);assert(appearance.AlphaReads==((flags&16)?2:1));
  assert(sprite.RotationAngle==2.0f && sprite.OldRotationAngle==1.0f && sprite.RotationAmountPerFrame==-5.0f && sprite.AnimStage==0.25f && sprite.LastAnimStage==0.125f && sprite.AnimSpeed==0.75f);
 }
 assert(PrimitiveAlive==0);puts("0077c4d0_TEST PASS");return 0;
}