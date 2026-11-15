#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifndef _MSC_VER
#define __int64 long long
#endif
static int PrimitiveAlive,EffectAlive,EffectDeletes,EffectConstructors,SubDefQueries,BulletQueries,PositionQueries,OrientationQueries;
static bool AllocationFails;
const float DELTA_NOTIONAL_ZERO=0.0001f;
class C3DVector {public:float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){} };
class CRightHandedSet {public:C3DVector Up,Forward;};
class C3DPositionInterpolator {public:C3DVector StartPosition,EndPosition;C3DVector GetInterpolated(float t)const{++PositionQueries;return C3DVector(StartPosition.X+(EndPosition.X-StartPosition.X)*t,StartPosition.Y+(EndPosition.Y-StartPosition.Y)*t,StartPosition.Z+(EndPosition.Z-StartPosition.Z)*t);}};
class C3DOrientationInterpolator {public:CRightHandedSet StartOrientation,EndOrientation;CRightHandedSet GetInterpolated(float t)const{++OrientationQueries;CRightHandedSet r;r.Up=C3DVector(StartOrientation.Up.X+(EndOrientation.Up.X-StartOrientation.Up.X)*t,StartOrientation.Up.Y+(EndOrientation.Up.Y-StartOrientation.Up.Y)*t,StartOrientation.Up.Z+(EndOrientation.Up.Z-StartOrientation.Up.Z)*t);r.Forward=C3DVector(StartOrientation.Forward.X+(EndOrientation.Forward.X-StartOrientation.Forward.X)*t,StartOrientation.Forward.Y+(EndOrientation.Forward.Y-StartOrientation.Forward.Y)*t,StartOrientation.Forward.Z+(EndOrientation.Forward.Z-StartOrientation.Forward.Z)*t);return r;}};
class CRGBColour {public:unsigned char B,G,R,A;CRGBColour():B(0),G(0),R(0),A(0){}CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a):B(b),G(g),R(r),A(a){}};
class CEnginePrimitiveThemeChannelDesc {public:unsigned char ChannelIndex0,ChannelIndex1,ChannelBlendFactor,Pad;CEnginePrimitiveThemeChannelDesc():ChannelIndex0(0),ChannelIndex1(0),ChannelBlendFactor(0),Pad(0){} };
enum EEnginePrimitiveType {ENGINE_PRIMITIVE_STATIC_MESH=8};enum EEngineInterpolationSet {ENGINE_INTERPOLATION_SET_0=0};
class CEnginePrimitive {public:EEnginePrimitiveType Type;EEngineInterpolationSet InterpolationSet;CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;CEnginePrimitive(EEnginePrimitiveType t,unsigned __int64,EEngineInterpolationSet i):Type(t),InterpolationSet(i){} };
class CIVCountedPointeeBase {public:mutable unsigned long IVRefCount;CIVCountedPointeeBase():IVRefCount(1){}virtual ~CIVCountedPointeeBase(){}virtual void Delete()const{delete this;}void AddRef()const{++IVRefCount;}long Release()const{return --IVRefCount;} };
template<class T>class CIVCountedPointer {CIVCountedPointeeBase* Object;public:CIVCountedPointer():Object(0){}CIVCountedPointer(T* p):Object(p){}CIVCountedPointer(const CIVCountedPointer& p):Object(p.Object){if(Object)Object->AddRef();}~CIVCountedPointer(){Release();}void Release(){if(Object){CIVCountedPointeeBase* object=Object;if(object->Release()==0)object->Delete();Object=0;}}CIVCountedPointer& operator=(const CIVCountedPointer& p){if(Object!=p.Object){Release();Object=p.Object;if(Object)Object->AddRef();}return *this;}T* operator->()const{return static_cast<T*>(Object);}T* Get()const{return static_cast<T*>(Object);} };
class CEngineMeshEffect:public CIVCountedPointeeBase {public:long MeshEffect;};
class CEngineMeshEffectDistanceFade:public CEngineMeshEffect {public:float NearClipDistance,NearFadeDistance,FarFadeDistance,FarClipDistance;CEngineMeshEffectDistanceFade(float a,float b,float c,float d):NearClipDistance(a),NearFadeDistance(b),FarFadeDistance(c),FarClipDistance(d){MeshEffect=2;++EffectAlive;++EffectConstructors;}virtual ~CEngineMeshEffectDistanceFade(){--EffectAlive;++EffectDeletes;}static void* operator new(size_t n)throw(){return AllocationFails?0:std::malloc(n);}static void operator delete(void* p){std::free(p);} };
class CEnginePrimitiveStaticMesh:public CEnginePrimitive {public:unsigned long MeshIndex;C3DVector WorldPos;float Size,FadeStart;CRightHandedSet RightHandedSet;CIVCountedPointer<CEngineMeshEffect> MeshEffect;CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;CRGBColour TintColour;unsigned char Alpha;unsigned char Lit:1;bool BulletTimeAffectsCloth;CEnginePrimitiveStaticMesh(unsigned __int64,unsigned long,const C3DVector&,const CRightHandedSet&,float,bool,unsigned char,CRGBColour,float,EEngineInterpolationSet,const CEnginePrimitiveThemeChannelDesc&,bool);};
class CEngineGraphic {public:unsigned long BankIndex;float AnimStep,RenderSizeX;unsigned char AdditiveAlpha,Type;};
class CAppearanceDef {public:mutable unsigned long RefCount;CEngineGraphic FadeOutWhenCloseGraphic;bool HasFadeOutWhenCloseGraphic;float FadeOutGraphicRenderFadeDistance,FadeOutGraphicNearRenderFadeDistance,FadeOutGraphicNearRenderClipDistance;};
class CThingBaseDef;
template<class T>class CDefPointer {public:T* Object;CDefPointer():Object(0){}CDefPointer(T* p):Object(p){if(Object)++Object->RefCount;}CDefPointer(const CDefPointer& p):Object(p.Object){if(Object)++Object->RefCount;}~CDefPointer(){if(Object)--Object->RefCount;}T* operator->()const{return Object;}};
class CThingBaseDef {public:mutable unsigned long RefCount;const CAppearanceDef* Appearance;void GetSubDef(CDefPointer<const CAppearanceDef>* p)const{++SubDefQueries;p->Object=Appearance;++Appearance->RefCount;}};
class CThing {public:CDefPointer<const CThingBaseDef> PBaseDef;bool BulletTime;unsigned __int64 UniqueID;CDefPointer<const CThingBaseDef> GetPBaseDef()const{return PBaseDef;}bool IsAffectedByBulletTime()const{++BulletQueries;return BulletTime;}unsigned __int64 GetUniqueID()const{return UniqueID;}};
class CTCBase {public:CThing* ParentThing;CThing& GetParentThing()const{return *ParentThing;}};
namespace NGlobalConsole {bool DrawProximityFadeOutMeshes;}
namespace NGraphicAppearance {
enum EPrimitiveType {GAPT_NONE,GAPT_STATIC_MESH,GAPT_ANIMATING_MESH,GAPT_MISC};
class CPrimitive {public:EPrimitiveType Type;long BankIndex;CPrimitive(EPrimitiveType t,long b=0):Type(t),BankIndex(b){++PrimitiveAlive;}~CPrimitive(){--PrimitiveAlive;}};
class CGraphicAppearanceDrawInfo {public:float Interpolate;C3DPositionInterpolator* Pos;C3DOrientationInterpolator* Orientation;unsigned long ReasonsForDraw;};
class CTCGraphicAppearance {public:float Scale,FallbackFade,WantFade,WantSize;unsigned char Alpha;CRGBColour Tint;CEnginePrimitiveThemeChannelDesc Lighting;CPrimitive Primitive;int Added,Refresh,Set,FadeQueries,TintQueries;bool WantBullet;C3DVector WantPos;CRightHandedSet WantOrientation;const C3DPositionInterpolator* WantPosPointer;const C3DOrientationInterpolator* WantOrientationPointer;
 CTCGraphicAppearance():Primitive(GAPT_NONE),Added(0),Refresh(0),Set(0),FadeQueries(0),TintQueries(0){}
 float GetScale()const{return Scale;}float GetFadeDistance(){++FadeQueries;return FallbackFade;}unsigned char GetAlpha()const{return Alpha;}CRGBColour GetTintColour(){++TintQueries;return Tint;}const CEnginePrimitiveThemeChannelDesc& PeekLightingChannel()const{return Lighting;}
 CPrimitive* AddPersistentPrimitive(const CPrimitive& p){assert(p.Type==GAPT_STATIC_MESH&&p.BankIndex==77&&PrimitiveAlive==2);++Added;Primitive.Type=p.Type;Primitive.BankIndex=p.BankIndex;return &Primitive;}
 void RefreshPrimitivePos(CPrimitive* p,const C3DPositionInterpolator& pos,const C3DOrientationInterpolator& orientation){assert(p==&Primitive&&&pos==WantPosPointer&&&orientation==WantOrientationPointer);++Refresh;}
 void SetPrimitiveGraphics(CPrimitive* p,const CEnginePrimitive& b,unsigned short extra){assert(p==&Primitive&&extra==0x3000&&PrimitiveAlive==1);const CEnginePrimitiveStaticMesh& m=static_cast<const CEnginePrimitiveStaticMesh&>(b);assert(m.Type==ENGINE_PRIMITIVE_STATIC_MESH&&m.InterpolationSet==ENGINE_INTERPOLATION_SET_0&&m.MeshIndex==77);assert(m.WorldPos.X==WantPos.X&&m.WorldPos.Y==WantPos.Y&&m.WorldPos.Z==WantPos.Z);assert(m.RightHandedSet.Up.X==WantOrientation.Up.X&&m.RightHandedSet.Up.Y==WantOrientation.Up.Y&&m.RightHandedSet.Up.Z==WantOrientation.Up.Z);assert(m.RightHandedSet.Forward.X==WantOrientation.Forward.X&&m.RightHandedSet.Forward.Y==WantOrientation.Forward.Y&&m.RightHandedSet.Forward.Z==WantOrientation.Forward.Z);assert(m.Size==WantSize&&m.FadeStart==WantFade&&m.Lit&&m.Alpha==Alpha&&m.BulletTimeAffectsCloth==WantBullet);assert(m.TintColour.R==Tint.R&&m.TintColour.G==Tint.G&&m.TintColour.B==Tint.B&&m.TintColour.A==Tint.A);assert(m.ThemeChannelDesc.ChannelIndex0==Lighting.ChannelIndex0&&m.ThemeChannelDesc.ChannelIndex1==Lighting.ChannelIndex1&&m.ThemeChannelDesc.ChannelBlendFactor==Lighting.ChannelBlendFactor);assert(m.CEnginePrimitive::ThemeChannelDesc.ChannelIndex0==0&&m.CEnginePrimitive::ThemeChannelDesc.ChannelIndex1==0&&m.CEnginePrimitive::ThemeChannelDesc.ChannelBlendFactor==0);if(AllocationFails)assert(!m.MeshEffect.Get()&&EffectAlive==0);else{const CEngineMeshEffectDistanceFade& e=static_cast<const CEngineMeshEffectDistanceFade&>(*m.MeshEffect.Get());assert(e.IVRefCount==1&&EffectAlive==1&&e.MeshEffect==2&&e.NearClipDistance==1.25f&&e.NearFadeDistance==2.5f&&e.FarFadeDistance==WantFade-5.0f&&e.FarClipDistance==WantFade);}++Set;}
};
class CAGraphicAppearanceComponent {public:virtual bool OnAppearanceDraw(CTCGraphicAppearance&,const CGraphicAppearanceDrawInfo&)=0;};
class CTCGraphicAppearanceStaticMeshFadeOutOnProximity:public CTCBase,public CAGraphicAppearanceComponent {public:CPrimitive* PrimHandle;virtual bool OnAppearanceDraw(CTCGraphicAppearance&,const CGraphicAppearanceDrawInfo&);};
}

CEnginePrimitiveStaticMesh::CEnginePrimitiveStaticMesh(unsigned __int64 uid,unsigned long mesh_index,const C3DVector& world_pos,const CRightHandedSet& rhset,float size,bool lit,unsigned char alpha,CRGBColour tint_colour,float fade_start,EEngineInterpolationSet interp_set,const CEnginePrimitiveThemeChannelDesc& theme_channel_desc,bool bullet_time_affects_cloth)
    :CEnginePrimitive((EEnginePrimitiveType)8,uid,interp_set),MeshIndex(mesh_index),WorldPos(world_pos),Size(size),FadeStart(fade_start),RightHandedSet(rhset),ThemeChannelDesc(theme_channel_desc),TintColour(tint_colour),Alpha(alpha),Lit(lit),BulletTimeAffectsCloth(bullet_time_affects_cloth)
{
}

namespace NGraphicAppearance {
bool CTCGraphicAppearanceStaticMeshFadeOutOnProximity::OnAppearanceDraw(CTCGraphicAppearance& tc_appearance,const CGraphicAppearanceDrawInfo& draw_info)
{
    CDefPointer<const CAppearanceDef> pappearance_def;
    GetParentThing().GetPBaseDef()->GetSubDef(&pappearance_def);
    if(pappearance_def->HasFadeOutWhenCloseGraphic) {
        const CEngineGraphic& fade_out_graphic=pappearance_def->FadeOutWhenCloseGraphic;
        float scale=tc_appearance.GetScale()*fade_out_graphic.RenderSizeX;
        float fade_dist=pappearance_def->FadeOutGraphicRenderFadeDistance;
        if(fade_dist<DELTA_NOTIONAL_ZERO)
            fade_dist=tc_appearance.GetFadeDistance();
        if(!PrimHandle)
            PrimHandle=tc_appearance.AddPersistentPrimitive(CPrimitive(GAPT_STATIC_MESH,fade_out_graphic.BankIndex));
        else if(draw_info.ReasonsForDraw==1) {
            tc_appearance.RefreshPrimitivePos(PrimHandle,*draw_info.Pos,*draw_info.Orientation);
            return true;
        }
        if(NGlobalConsole::DrawProximityFadeOutMeshes) {
            bool bullet_time_affects_cloth=GetParentThing().IsAffectedByBulletTime();
            CEnginePrimitiveStaticMesh mesh_prim(GetParentThing().GetUniqueID(),fade_out_graphic.BankIndex,draw_info.Pos->GetInterpolated(draw_info.Interpolate),draw_info.Orientation->GetInterpolated(draw_info.Interpolate),scale,true,tc_appearance.GetAlpha(),tc_appearance.GetTintColour(),fade_dist,(EEngineInterpolationSet)0,tc_appearance.PeekLightingChannel(),bullet_time_affects_cloth);
            mesh_prim.MeshEffect=CIVCountedPointer<CEngineMeshEffect>(new CEngineMeshEffectDistanceFade(pappearance_def->FadeOutGraphicNearRenderClipDistance,pappearance_def->FadeOutGraphicNearRenderFadeDistance,fade_dist-5.0f,fade_dist));
            tc_appearance.SetPrimitiveGraphics(PrimHandle,mesh_prim,0x3000);
        }
    }
    return true;
}
}

int main()
{
 const float interpolations[]={0.0f,0.25f,1.0f};const float fades[]={-1.0f,0.0f,0.00006103515625f,0.0001220703125f,8.0f};const unsigned char alphas[]={0,127,255};
 int cases=0;
 for(int flags=0;flags<32;++flags)for(int fade=0;fade<5;++fade)for(int reason=0;reason<3;++reason)for(int interp=0;interp<3;++interp)for(int alpha=0;alpha<3;++alpha)for(int scale=0;scale<2;++scale)for(int refs=0;refs<2;++refs){
  assert(PrimitiveAlive==0&&EffectAlive==0);PrimitiveAlive=EffectAlive=EffectDeletes=EffectConstructors=SubDefQueries=BulletQueries=PositionQueries=OrientationQueries=0;
  AllocationFails=(flags&16)!=0;NGlobalConsole::DrawProximityFadeOutMeshes=(flags&4)!=0;
  CAppearanceDef def;def.RefCount=2+refs;def.HasFadeOutWhenCloseGraphic=(flags&1)!=0;def.FadeOutWhenCloseGraphic.BankIndex=77;def.FadeOutWhenCloseGraphic.RenderSizeX=3.0f;def.FadeOutWhenCloseGraphic.AnimStep=0.5f;def.FadeOutWhenCloseGraphic.Type=0;def.FadeOutWhenCloseGraphic.AdditiveAlpha=0;def.FadeOutGraphicRenderFadeDistance=fades[fade];def.FadeOutGraphicNearRenderClipDistance=1.25f;def.FadeOutGraphicNearRenderFadeDistance=2.5f;
  CThingBaseDef base;base.RefCount=1+refs;base.Appearance=&def;CThing thing;thing.PBaseDef.Object=&base;thing.BulletTime=(flags&8)!=0;thing.UniqueID=(((unsigned __int64)0x12345678)<<32)|0x9abcdef0;
  C3DPositionInterpolator pos;pos.StartPosition=C3DVector(2,-4,6);pos.EndPosition=C3DVector(10,8,-2);
  C3DOrientationInterpolator orientation;orientation.StartOrientation.Up=C3DVector(0,0,1);orientation.EndOrientation.Up=C3DVector(4,8,5);orientation.StartOrientation.Forward=C3DVector(0,1,0);orientation.EndOrientation.Forward=C3DVector(8,5,4);
  NGraphicAppearance::CGraphicAppearanceDrawInfo draw;draw.Interpolate=interpolations[interp];draw.ReasonsForDraw=(unsigned long)reason;draw.Pos=&pos;draw.Orientation=&orientation;
  NGraphicAppearance::CTCGraphicAppearance appearance;appearance.Scale=scale?2.5f:0.5f;appearance.FallbackFade=16.0f;appearance.Alpha=alphas[alpha];appearance.Tint=CRGBColour(11,22,33,44);appearance.Lighting.ChannelIndex0=5;appearance.Lighting.ChannelIndex1=7;appearance.Lighting.ChannelBlendFactor=123;appearance.WantFade=fade<3?16.0f:fades[fade];appearance.WantSize=scale?7.5f:1.5f;appearance.WantBullet=thing.BulletTime;appearance.WantPosPointer=&pos;appearance.WantOrientationPointer=&orientation;float t=draw.Interpolate;appearance.WantPos=C3DVector(2+8*t,-4+12*t,6-8*t);appearance.WantOrientation.Up=C3DVector(4*t,8*t,1+4*t);appearance.WantOrientation.Forward=C3DVector(8*t,1+4*t,4*t);
  NGraphicAppearance::CTCGraphicAppearanceStaticMeshFadeOutOnProximity component;component.ParentThing=&thing;component.PrimHandle=(flags&2)?&appearance.Primitive:0;
  assert(component.OnAppearanceDraw(appearance,draw));
  bool has=(flags&1)!=0,existing=(flags&2)!=0,fast=has&&existing&&reason==1,submit=has&&!fast&&(flags&4)!=0;
  assert(appearance.Added==(has&&!existing?1:0)&&appearance.Refresh==(fast?1:0)&&appearance.Set==(submit?1:0));
  assert(appearance.FadeQueries==(has&&fade<3?1:0)&&appearance.TintQueries==(submit?1:0));
  assert(SubDefQueries==1&&base.RefCount==(unsigned long)(1+refs)&&def.RefCount==(unsigned long)(2+refs));
  assert(BulletQueries==(submit?1:0)&&PositionQueries==(submit?1:0)&&OrientationQueries==(submit?1:0));
  assert(EffectAlive==0&&EffectConstructors==(submit&&!AllocationFails?1:0)&&EffectDeletes==EffectConstructors);
  assert(component.PrimHandle==((has||existing)?&appearance.Primitive:0));assert(PrimitiveAlive==1);assert(component.ParentThing==&thing&&thing.UniqueID==((((unsigned __int64)0x12345678)<<32)|0x9abcdef0));
  assert(def.FadeOutGraphicRenderFadeDistance==fades[fade]&&def.FadeOutWhenCloseGraphic.BankIndex==77&&def.FadeOutWhenCloseGraphic.RenderSizeX==3.0f);++cases;
 }
 assert(cases==17280&&PrimitiveAlive==0&&EffectAlive==0);std::puts("0077bec0_TEST PASS");return 0;
}