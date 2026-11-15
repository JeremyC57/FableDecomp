#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#ifndef _MSC_VER
#define __int64 long long
#endif
static int PrimitiveAlive,EffectAlive,EffectDeletes,EffectConstructors,SubDefQueries,BulletQueries,PositionQueries,OrientationQueries,ChangerAlive,StaticMeshAlive,GetPosQueries,RHQueries,MainGraphicQueries;
static bool AllocationFails;
const float DELTA_NOTIONAL_ZERO=0.0001f;
class C3DVector {public:float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){} };
class CRightHandedSet {public:C3DVector Up,Forward;};
class C3DPositionInterpolator {public:C3DVector StartPosition,EndPosition;C3DVector GetInterpolated(float t)const{++PositionQueries;return C3DVector(StartPosition.X+(EndPosition.X-StartPosition.X)*t,StartPosition.Y+(EndPosition.Y-StartPosition.Y)*t,StartPosition.Z+(EndPosition.Z-StartPosition.Z)*t);}};
class C3DOrientationInterpolator {public:CRightHandedSet StartOrientation,EndOrientation;CRightHandedSet GetInterpolated(float t)const{++OrientationQueries;CRightHandedSet r;r.Up=C3DVector(StartOrientation.Up.X+(EndOrientation.Up.X-StartOrientation.Up.X)*t,StartOrientation.Up.Y+(EndOrientation.Up.Y-StartOrientation.Up.Y)*t,StartOrientation.Up.Z+(EndOrientation.Up.Z-StartOrientation.Up.Z)*t);r.Forward=C3DVector(StartOrientation.Forward.X+(EndOrientation.Forward.X-StartOrientation.Forward.X)*t,StartOrientation.Forward.Y+(EndOrientation.Forward.Y-StartOrientation.Forward.Y)*t,StartOrientation.Forward.Z+(EndOrientation.Forward.Z-StartOrientation.Forward.Z)*t);return r;}};
class CRGBColour {public:unsigned char B,G,R,A;CRGBColour(unsigned char r,unsigned char g,unsigned char b):B(b),G(g),R(r),A(255){}CRGBColour():B(0),G(0),R(0),A(0){}CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a):B(b),G(g),R(r),A(a){}};
class CEnginePrimitiveThemeChannelDesc {public:unsigned char ChannelIndex0,ChannelIndex1,ChannelBlendFactor,Pad;CEnginePrimitiveThemeChannelDesc():ChannelIndex0(0),ChannelIndex1(0),ChannelBlendFactor(0),Pad(0){} };
enum EEnginePrimitiveType {ENGINE_PRIMITIVE_STATIC_MESH=8};enum EEngineInterpolationSet {ENGINE_INTERPOLATION_SET_0=0};
class CEnginePrimitive {public:EEnginePrimitiveType Type;EEngineInterpolationSet InterpolationSet;CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;CEnginePrimitive(EEnginePrimitiveType t,unsigned __int64,EEngineInterpolationSet i):Type(t),InterpolationSet(i){if((int)t==2)++ChangerAlive;if((int)t==8)++StaticMeshAlive;}~CEnginePrimitive(){if((int)Type==2)--ChangerAlive;if((int)Type==8)--StaticMeshAlive;} };
class CIVCountedPointeeBase {public:mutable unsigned long IVRefCount;CIVCountedPointeeBase():IVRefCount(1){}virtual ~CIVCountedPointeeBase(){}virtual void Delete()const{delete this;}void AddRef()const{++IVRefCount;}long Release()const{return --IVRefCount;} };
template<class T>class CIVCountedPointer {CIVCountedPointeeBase* Object;public:CIVCountedPointer():Object(0){}CIVCountedPointer(T* p):Object(p){}CIVCountedPointer(const CIVCountedPointer& p):Object(p.Object){if(Object)Object->AddRef();}~CIVCountedPointer(){Release();}void Release(){if(Object){CIVCountedPointeeBase* object=Object;if(object->Release()==0)object->Delete();Object=0;}}CIVCountedPointer& operator=(const CIVCountedPointer& p){if(Object!=p.Object){Release();Object=p.Object;if(Object)Object->AddRef();}return *this;}T* operator->()const{return static_cast<T*>(Object);}T* Get()const{return static_cast<T*>(Object);} };
class CEngineMeshEffect:public CIVCountedPointeeBase {public:long MeshEffect;CEngineMeshEffect(){MeshEffect=7;++EffectAlive;}virtual ~CEngineMeshEffect(){--EffectAlive;++EffectDeletes;}};
class CEnginePrimitiveStaticMesh:public CEnginePrimitive {public:unsigned long MeshIndex;C3DVector WorldPos;float Size,FadeStart;CRightHandedSet RightHandedSet;CIVCountedPointer<CEngineMeshEffect> MeshEffect;CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;CRGBColour TintColour;unsigned char Alpha;unsigned char Lit:1;bool BulletTimeAffectsCloth;CEnginePrimitiveStaticMesh(unsigned __int64,unsigned long,const C3DVector&,const CRightHandedSet&,float,bool,unsigned char,CRGBColour,float,EEngineInterpolationSet,const CEnginePrimitiveThemeChannelDesc&,bool);};
class CEngineGraphic {public:unsigned long BankIndex;float AnimStep,RenderSizeX;unsigned char AdditiveAlpha,Type;};

class CEnginePrimitiveChanger:public CEnginePrimitive {
public:long ActiveIndex,InitialSize;bool PreloadAll;CIVCountedPointer<CEngineMeshEffect> MeshEffect;
 CEnginePrimitiveChanger(long active_index,long initial_size,bool preload_all,EEngineInterpolationSet interp_set)
  :CEnginePrimitive((EEnginePrimitiveType)2,0,interp_set),ActiveIndex(active_index),InitialSize(initial_size),PreloadAll(preload_all){}
};
template<class T>class CArray:public std::vector<T> {};
namespace NGraphicAppearance {class CReplaceableMeshes {public:CArray<CEngineGraphic> Vector;};}
class CReplaceableMeshDef {public:mutable unsigned long RefCount;NGraphicAppearance::CReplaceableMeshes Meshes;};
template<class T>class CDefPointer {public:T* Object;CDefPointer():Object(0){}~CDefPointer(){if(Object)--Object->RefCount;}T* operator->()const{return Object;}};
class CTCPhysicsBase {public:CRightHandedSet RH;virtual const CRightHandedSet& GetRHSet()const{++RHQueries;return RH;}};
class CThing {public:bool BulletTime;C3DVector Position;CTCPhysicsBase* Physics;bool IsAffectedByBulletTime()const{++BulletQueries;return BulletTime;}const C3DVector& GetPos()const{++GetPosQueries;return Position;}CTCPhysicsBase* GetPhysicsTC()const{return Physics;}};
class CTCBase {public:CThing* ParentThing;CThing& GetParentThing()const{return *ParentThing;}};
class CEnginePrimitiveHandle {public:unsigned long Tag;};
namespace NGraphicAppearance {
enum EPrimitiveType {GAPT_NONE,GAPT_STATIC_MESH,GAPT_ANIMATING_MESH,GAPT_MISC};
class CPrimitive {public:EPrimitiveType Type;long BankIndex;CEnginePrimitiveHandle PrimitiveHandle;CPrimitive(EPrimitiveType t,long b=0):Type(t),BankIndex(b){PrimitiveHandle.Tag=123;++PrimitiveAlive;}~CPrimitive(){--PrimitiveAlive;}};
class CGraphicAppearanceDrawInfo {public:float Interpolate;C3DPositionInterpolator* Pos;C3DOrientationInterpolator* Orientation;};
class CTCGraphicAppearance {public:CEngineGraphic Graphic;CEnginePrimitiveThemeChannelDesc Lighting;CIVCountedPointer<CEngineMeshEffect> Effect;CPrimitive Primitive;unsigned long WantIndex,EffectBaseline;bool WasExisting;int Added,Set,Refresh,EffectGets;C3DPositionInterpolator* WantPos;C3DOrientationInterpolator* WantOrientation;
 CTCGraphicAppearance():Primitive(GAPT_NONE),Added(0),Set(0),Refresh(0),EffectGets(0){}
 const CEngineGraphic& GetMainGraphic()const{++MainGraphicQueries;return Graphic;}
 const CEnginePrimitiveThemeChannelDesc& PeekLightingChannel()const{return Lighting;}
 CIVCountedPointer<CEngineMeshEffect> GetPMeshEffect(){++EffectGets;assert(ChangerAlive==1&&StaticMeshAlive==0);return Effect;}
 CPrimitive* AddPersistentPrimitive(const CPrimitive& p){assert(p.Type==GAPT_STATIC_MESH&&p.BankIndex==1234&&PrimitiveAlive==2);++Added;Primitive.Type=p.Type;Primitive.BankIndex=p.BankIndex;return &Primitive;}
 void SetPrimitiveGraphics(CPrimitive* p,const CEnginePrimitive& b,unsigned short flags){assert(p==&Primitive&&flags==0&&ChangerAlive==1&&StaticMeshAlive==0&&PrimitiveAlive==1);const CEnginePrimitiveChanger& c=static_cast<const CEnginePrimitiveChanger&>(b);assert((int)c.Type==2&&c.InterpolationSet==ENGINE_INTERPOLATION_SET_0&&c.ActiveIndex==(long)WantIndex&&c.InitialSize==0&&c.PreloadAll);assert(c.ThemeChannelDesc.ChannelIndex0==0&&c.ThemeChannelDesc.ChannelIndex1==0&&c.ThemeChannelDesc.ChannelBlendFactor==0);if(Set==0&&!WasExisting)assert(!c.MeshEffect.Get());else{assert(c.MeshEffect.Get()==Effect.Get());if(Effect.Get())assert(Effect->IVRefCount==EffectBaseline+1);}++Set;}
 void RefreshPrimitivePos(CPrimitive* p,const C3DPositionInterpolator& pos,const C3DOrientationInterpolator& orientation){assert(p==&Primitive&&&pos==WantPos&&&orientation==WantOrientation&&Set==(WasExisting?1:2)&&ChangerAlive==1);++Refresh;}
};
class CAGraphicAppearanceComponent {public:virtual bool OnAppearanceDraw(CTCGraphicAppearance&,const CGraphicAppearanceDrawInfo&)=0;};
class CTCGraphicAppearanceReplaceableMesh:public CTCBase,public CAGraphicAppearanceComponent {public:bool Replaced;unsigned long MeshIndex;CPrimitive* PrimHandle;CDefPointer<const CReplaceableMeshDef> PDef;virtual bool OnAppearanceDraw(CTCGraphicAppearance&,const CGraphicAppearanceDrawInfo&);};
}
class CIEngine {public:NGraphicAppearance::CPrimitive* WantPrimitive;const CThing* Thing;const CEnginePrimitiveThemeChannelDesc* Lighting;unsigned long Count;
 virtual void SetChildPrimitive(CEnginePrimitiveHandle& handle,unsigned long index,const CEnginePrimitive& b,unsigned long flags){assert(&handle==&WantPrimitive->PrimitiveHandle&&index==Count&&flags==0&&ChangerAlive==1&&StaticMeshAlive==1);const CEnginePrimitiveStaticMesh& m=static_cast<const CEnginePrimitiveStaticMesh&>(b);assert((int)m.Type==8&&m.InterpolationSet==ENGINE_INTERPOLATION_SET_0&&m.MeshIndex==10+13*index);assert(m.WorldPos.X==Thing->Position.X&&m.WorldPos.Y==Thing->Position.Y&&m.WorldPos.Z==Thing->Position.Z);assert(m.RightHandedSet.Up.X==Thing->Physics->RH.Up.X&&m.RightHandedSet.Up.Y==Thing->Physics->RH.Up.Y&&m.RightHandedSet.Up.Z==Thing->Physics->RH.Up.Z);assert(m.RightHandedSet.Forward.X==Thing->Physics->RH.Forward.X&&m.RightHandedSet.Forward.Y==Thing->Physics->RH.Forward.Y&&m.RightHandedSet.Forward.Z==Thing->Physics->RH.Forward.Z);assert(m.Size==1.0f&&m.FadeStart==0&&m.Lit&&m.Alpha==255&&m.BulletTimeAffectsCloth==Thing->BulletTime);assert(m.TintColour.R==255&&m.TintColour.G==255&&m.TintColour.B==255&&m.TintColour.A==255&&!m.MeshEffect.Get());assert(m.ThemeChannelDesc.ChannelIndex0==Lighting->ChannelIndex0&&m.ThemeChannelDesc.ChannelIndex1==Lighting->ChannelIndex1&&m.ThemeChannelDesc.ChannelBlendFactor==Lighting->ChannelBlendFactor);assert(m.CEnginePrimitive::ThemeChannelDesc.ChannelIndex0==0&&m.CEnginePrimitive::ThemeChannelDesc.ChannelIndex1==0&&m.CEnginePrimitive::ThemeChannelDesc.ChannelBlendFactor==0);++Count;}
};
class CDisplayEngine {public:CIEngine* Engine3D;CIEngine& Get3DEngine(){return *Engine3D;}};
class CMainGameComponent {public:static CMainGameComponent* Current;CDisplayEngine* Display;static CMainGameComponent* Get(){return Current;}CDisplayEngine& GetDisplayEngine(){return *Display;}};CMainGameComponent* CMainGameComponent::Current;

CEnginePrimitiveStaticMesh::CEnginePrimitiveStaticMesh(unsigned __int64 uid,unsigned long mesh_index,const C3DVector& world_pos,const CRightHandedSet& rhset,float size,bool lit,unsigned char alpha,CRGBColour tint_colour,float fade_start,EEngineInterpolationSet interp_set,const CEnginePrimitiveThemeChannelDesc& theme_channel_desc,bool bullet_time_affects_cloth)
    :CEnginePrimitive((EEnginePrimitiveType)8,uid,interp_set),MeshIndex(mesh_index),WorldPos(world_pos),Size(size),FadeStart(fade_start),RightHandedSet(rhset),ThemeChannelDesc(theme_channel_desc),TintColour(tint_colour),Alpha(alpha),Lit(lit),BulletTimeAffectsCloth(bullet_time_affects_cloth)
{
}
namespace NGraphicAppearance {
bool CTCGraphicAppearanceReplaceableMesh::OnAppearanceDraw(CTCGraphicAppearance& tc_appearance,const CGraphicAppearanceDrawInfo& draw_info)
{
    const CEngineGraphic& graphic=tc_appearance.GetMainGraphic();
    if(!PrimHandle) {
        PrimHandle=tc_appearance.AddPersistentPrimitive(CPrimitive(GAPT_STATIC_MESH,graphic.BankIndex));
        CEnginePrimitiveChanger prim_changer(MeshIndex,0,true,(EEngineInterpolationSet)0);
        tc_appearance.SetPrimitiveGraphics(PrimHandle,prim_changer,0);
        unsigned long child_index=0;
        bool bullet_time_affects_cloth=GetParentThing().IsAffectedByBulletTime();
        for(CArray<CEngineGraphic>::const_iterator graphic_iterator=PDef->Meshes.Vector.begin();graphic_iterator!=PDef->Meshes.Vector.end();graphic_iterator++) {
            CEnginePrimitiveStaticMesh loop_graphic(0,graphic_iterator->BankIndex,GetParentThing().GetPos(),GetParentThing().GetPhysicsTC()->GetRHSet(),1.0f,true,255,CRGBColour(255,255,255),0.0f,(EEngineInterpolationSet)0,tc_appearance.PeekLightingChannel(),bullet_time_affects_cloth);
            CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().SetChildPrimitive(PrimHandle->PrimitiveHandle,child_index,loop_graphic,0);
            ++child_index;
        }
    }
    {
        CEnginePrimitiveChanger prim_changer(MeshIndex,0,true,(EEngineInterpolationSet)0);
        prim_changer.MeshEffect=tc_appearance.GetPMeshEffect();
        tc_appearance.SetPrimitiveGraphics(PrimHandle,prim_changer,0);
        tc_appearance.RefreshPrimitivePos(PrimHandle,*draw_info.Pos,*draw_info.Orientation);
        Replaced=false;
    }
    return true;
}
}

int main()
{
 const int lengths[]={0,1,2,7};const float interpolations[]={0.0f,0.25f,1.0f};int cases=0;
 for(int flags=0;flags<16;++flags)for(int length=0;length<4;++length)for(int active=0;active<2;++active)for(int shared=0;shared<2;++shared)for(int interpolation=0;interpolation<3;++interpolation){
  assert(PrimitiveAlive==0&&EffectAlive==0&&ChangerAlive==0&&StaticMeshAlive==0);
  EffectDeletes=BulletQueries=GetPosQueries=RHQueries=MainGraphicQueries=0;
  {
   CReplaceableMeshDef def;def.RefCount=2;for(int i=0;i<lengths[length];++i){CEngineGraphic g;g.BankIndex=10+13*i;g.RenderSizeX=2.5f+0.25f*i;g.AnimStep=0.5f;g.Type=(unsigned char)i;g.AdditiveAlpha=(unsigned char)(i&1);def.Meshes.Vector.push_back(g);}
   CTCPhysicsBase physics;physics.RH.Up=C3DVector(2,4,6);physics.RH.Forward=C3DVector(-1,3,5);CThing thing;thing.Physics=&physics;thing.Position=C3DVector(-8,4,12);thing.BulletTime=(flags&4)!=0;
   C3DPositionInterpolator position;position.StartPosition=C3DVector(0,1,2);position.EndPosition=C3DVector(3,4,5);C3DOrientationInterpolator orientation;orientation.StartOrientation=physics.RH;orientation.EndOrientation=physics.RH;
   NGraphicAppearance::CGraphicAppearanceDrawInfo info;info.Pos=&position;info.Orientation=&orientation;info.Interpolate=interpolations[interpolation];
   NGraphicAppearance::CTCGraphicAppearance appearance;appearance.Graphic.BankIndex=1234;appearance.Graphic.RenderSizeX=99.5f;appearance.Graphic.AnimStep=0.75f;appearance.Graphic.Type=3;appearance.Graphic.AdditiveAlpha=1;appearance.Lighting.ChannelIndex0=3;appearance.Lighting.ChannelIndex1=5;appearance.Lighting.ChannelBlendFactor=201;appearance.WasExisting=(flags&1)!=0;appearance.WantPos=&position;appearance.WantOrientation=&orientation;appearance.WantIndex=(active&&lengths[length])?lengths[length]-1:0;
   if(flags&8){appearance.Effect=CIVCountedPointer<CEngineMeshEffect>(new CEngineMeshEffect);if(shared){appearance.Effect->AddRef();appearance.Effect->AddRef();appearance.Effect->AddRef();}}
   appearance.EffectBaseline=shared?4:1;
   CIEngine engine;engine.WantPrimitive=&appearance.Primitive;engine.Thing=&thing;engine.Lighting=&appearance.Lighting;engine.Count=0;CDisplayEngine display;display.Engine3D=&engine;CMainGameComponent game;game.Display=&display;CMainGameComponent::Current=((flags&1)&&shared)?0:&game;
   NGraphicAppearance::CTCGraphicAppearanceReplaceableMesh component;component.ParentThing=((flags&1)&&shared)?0:&thing;component.PrimHandle=(flags&1)?&appearance.Primitive:0;component.Replaced=(flags&2)!=0;component.MeshIndex=appearance.WantIndex;component.PDef.Object=((flags&1)&&shared)?0:&def;
   assert(component.OnAppearanceDraw(appearance,info));bool existed=(flags&1)!=0;
   assert(appearance.Added==(existed?0:1)&&appearance.Set==(existed?1:2)&&appearance.Refresh==1&&appearance.EffectGets==1);
   assert(engine.Count==(unsigned long)(existed?0:lengths[length])&&BulletQueries==(existed?0:1)&&GetPosQueries==(existed?0:lengths[length])&&RHQueries==(existed?0:lengths[length])&&MainGraphicQueries==1);
   assert(!component.Replaced&&component.MeshIndex==appearance.WantIndex&&component.PrimHandle==&appearance.Primitive&&component.ParentThing==(((flags&1)&&shared)?0:&thing)&&component.PDef.Object==(((flags&1)&&shared)?0:&def)&&def.RefCount==2);
   assert(ChangerAlive==0&&StaticMeshAlive==0&&PrimitiveAlive==1&&EffectDeletes==0&&EffectAlive==((flags&8)?1:0));if(flags&8){assert(appearance.Effect->IVRefCount==appearance.EffectBaseline);if(shared){appearance.Effect->Release();appearance.Effect->Release();appearance.Effect->Release();}}
   assert(thing.Position.X==-8&&thing.Position.Y==4&&thing.Position.Z==12&&appearance.Graphic.BankIndex==1234&&appearance.Graphic.RenderSizeX==99.5f);++cases;
  }
  assert(PrimitiveAlive==0&&EffectAlive==0&&ChangerAlive==0&&StaticMeshAlive==0&&EffectDeletes==((flags&8)?1:0));
 }
 assert(cases==768);std::puts("007a6760_TEST PASS");return 0;
}