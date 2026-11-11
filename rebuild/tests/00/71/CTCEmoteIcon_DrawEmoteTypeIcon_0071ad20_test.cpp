#include <stdio.h>
#include <string.h>
static bool failed,sound;static int fail_line;
#define CHECK(x) do{if(!(x)){failed=true;fail_line=__LINE__;return;}}while(0)
class C3DVector{ public:float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}};
static int world_calls,def_calls,pos_calls,focal_calls,draws,ctor_calls,notifies,deletes;static bool retain;
class CEnginePrimitiveHandle;
class CEnginePersistentPrimitiveBase{ public:int IVRefCount;virtual ~CEnginePersistentPrimitiveBase(){}virtual void Delete(){++deletes;}virtual void NotifyReleased(const CEnginePrimitiveHandle&){++notifies;if(retain)++IVRefCount;}int Release(){return --IVRefCount;}void AddRef(){++IVRefCount;}int GetRefCount()const{return IVRefCount;}};
template<class T>class CIVCountedPointer{ public:T*Object;CIVCountedPointer():Object(0){}bool IsNull()const{return Object==0;}unsigned int GetRefCount()const{return Object?Object->GetRefCount():0;}T*operator->()const{return Object;}void Release(){CEnginePersistentPrimitiveBase*object=Object;if(object){if(object->Release()==0)object->Delete();Object=0;}}void Reset(T*object){Release();Object=object;if(Object)Object->AddRef();}};
class CEnginePrimitiveHandle{ public:CIVCountedPointer<CEnginePersistentPrimitiveBase>Data;bool IsNull()const{return Data.IsNull();}void Clear(){if(Data.GetRefCount()==1)Data->NotifyReleased(*this);Data.Reset(0);}};
class CRGBColour{ public:unsigned char B,G,R,A;CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a):B(b),G(g),R(r),A(a){}};
enum EEnginePrimitiveBlendMode{BLEND=3};enum EEngineInterpolationSet{INTERP=0};enum EEnginePrimitiveBlendOp{OP=0};
class CEnginePrimitive{ public:int Type;EEngineInterpolationSet InterpolationSet;CEnginePrimitive(int t,EEngineInterpolationSet i):Type(t),InterpolationSet(i){}};
struct CEnginePrimitiveSprite:CEnginePrimitive{C3DVector WorldPos;unsigned int BankIndex,Frame;float SizeX,SizeY,FadeStart,Angle;CRGBColour Colour;unsigned short Flags;EEnginePrimitiveBlendMode BlendMode;EEnginePrimitiveBlendOp BlendOp;CEnginePrimitiveSprite(const C3DVector&pos,unsigned int bank,unsigned int frame,unsigned short flags,float angle,float sx,float sy,EEnginePrimitiveBlendMode blend,unsigned char a,unsigned char r,unsigned char g,unsigned char b,float fade,EEngineInterpolationSet is,EEnginePrimitiveBlendOp op):CEnginePrimitive(7,is),WorldPos(pos),BankIndex(bank),Frame(frame),SizeX(sx),SizeY(sy),FadeStart(fade),Angle(angle),Colour(r,g,b,a),Flags(flags),BlendMode(blend),BlendOp(op){++ctor_calls;}};
static CEnginePrimitiveHandle*expected_handle;static float expected_size;static unsigned char expected_alpha;static unsigned int expected_bank;static CEnginePersistentPrimitiveBase replacement;
class CIEngine{ public:virtual ~CIEngine(){}virtual void SetPersistentPrimitive(CEnginePrimitiveHandle&h,const CEnginePrimitive&p,unsigned int f){++draws;const CEnginePrimitiveSprite&s=static_cast<const CEnginePrimitiveSprite&>(p);sound=sound&&&h==expected_handle&&h.IsNull()&&f==0&&s.WorldPos.X==-8.5f&&s.WorldPos.Y==2.0f&&s.WorldPos.Z==4.5f&&s.Type==7&&s.InterpolationSet==INTERP&&s.BankIndex==expected_bank&&s.Frame==0&&s.SizeX==expected_size&&s.SizeY==expected_size&&s.FadeStart==0&&s.Angle==0&&s.Colour.A==expected_alpha&&s.Colour.R==255&&s.Colour.G==255&&s.Colour.B==255&&s.Flags==0x90&&s.BlendMode==BLEND&&s.BlendOp==OP;h.Data.Object=&replacement;}};
class CEnginePrimitivePrepareInfo{ public:CIEngine&Engine;CEnginePrimitivePrepareInfo(CIEngine&e):Engine(e){}};
namespace NGraphicAppearance{class CTCGraphicAppearance{ public:bool loaded;bool GetStaticFocalPosIfLoaded(C3DVector&out)const{++focal_calls;if(loaded)out=C3DVector(-8.5f,2.0f,4.0f);return loaded;}};}
class CThing{ public:C3DVector Pos;NGraphicAppearance::CTCGraphicAppearance Appearance;const C3DVector&GetPos()const{++pos_calls;return Pos;}NGraphicAppearance::CTCGraphicAppearance*GetTCGraphicAppearance(){return &Appearance;}};
class CWorld{ public:C3DVector Centre;virtual ~CWorld(){}virtual C3DVector GetGameLODCentre()const{++world_calls;return Centre;}};
class CGlobalDef{ public:float EmoteIconDrawDistance;};class CGameDefinitionManager{ public:CGlobalDef Global;const CGlobalDef&PeekGlobalDef()const{++def_calls;return Global;}};
inline float GFSqr(float n){return n*n;}
inline bool GFWorld3DDistanceSmallerThan(const C3DVector&pos1,const C3DVector&pos2,float compare){float sqr_dist=GFSqr(pos1.X-pos2.X)+GFSqr(pos1.Y-pos2.Y)+GFSqr(pos1.Z-pos2.Z);float sqr_comp=GFSqr(compare);return sqr_dist<sqr_comp;}
class CTCEmoteIcon{ public:CThing*ParentThing;CWorld*World;CGameDefinitionManager*Definitions;long EmoteIcon;float EmoteScale;unsigned char EmoteAlpha;CEnginePrimitiveHandle PrimitiveHandle;CThing&GetParentThing(){return *ParentThing;}const CWorld&PeekWorld()const{return *World;}const CGameDefinitionManager&GetDefinitionManager()const{return *Definitions;}void DrawEmoteTypeIcon(const CEnginePrimitivePrepareInfo&);};
void CTCEmoteIcon::DrawEmoteTypeIcon(const CEnginePrimitivePrepareInfo& info)
{
 if(GFWorld3DDistanceSmallerThan(PeekWorld().GetGameLODCentre(),GetParentThing().GetPos(),GetDefinitionManager().PeekGlobalDef().EmoteIconDrawDistance)) {
  C3DVector position_to_draw;
  if(GetParentThing().GetTCGraphicAppearance()->GetStaticFocalPosIfLoaded(position_to_draw)) {
   position_to_draw.Z+=0.5f;
   CEnginePrimitiveSprite prim(position_to_draw,EmoteIcon,0,0x90,0.0f,EmoteScale,EmoteScale,(EEnginePrimitiveBlendMode)3,EmoteAlpha,255,255,255,0.0f,(EEngineInterpolationSet)0,(EEnginePrimitiveBlendOp)0);
   if(!PrimitiveHandle.IsNull())PrimitiveHandle.Clear();
   info.Engine.SetPersistentPrimitive(PrimitiveHandle,prim,0);
  }
 }else if(!PrimitiveHandle.IsNull())PrimitiveHandle.Clear();
}

static void one(float radius,float dx,float dy,float dz,bool loaded,int count,bool keep,float scale,unsigned char alpha)
{
 sound=true;world_calls=def_calls=pos_calls=focal_calls=draws=ctor_calls=notifies=deletes=0;retain=keep;
 CWorld world;CThing thing;thing.Pos=C3DVector(-2,4,0.5f);thing.Appearance.loaded=loaded;world.Centre=C3DVector(thing.Pos.X+dx,thing.Pos.Y+dy,thing.Pos.Z+dz);CGameDefinitionManager definitions;definitions.Global.EmoteIconDrawDistance=radius;CIEngine engine;CEnginePrimitivePrepareInfo info(engine);CEnginePersistentPrimitiveBase object;object.IVRefCount=count;replacement.IVRefCount=1;
 CTCEmoteIcon emote;emote.ParentThing=&thing;emote.World=&world;emote.Definitions=&definitions;emote.EmoteIcon=alpha==255?-1:37;emote.EmoteScale=scale;emote.EmoteAlpha=alpha;emote.PrimitiveHandle.Data.Object=count?&object:0;expected_handle=&emote.PrimitiveHandle;expected_size=scale;expected_alpha=alpha;expected_bank=(unsigned int)emote.EmoteIcon;
 bool in_range=dx*dx+dy*dy+dz*dz<radius*radius;bool draw=in_range&&loaded;bool release=count&&(!in_range||loaded);bool notify=release&&count==1;int final_count=count-(release?1:0)+(notify&&keep?1:0);
 emote.DrawEmoteTypeIcon(info);
 CHECK(sound);CHECK(world_calls==1&&def_calls==1&&pos_calls==1);CHECK(focal_calls==(in_range?1:0));CHECK(draws==(draw?1:0)&&ctor_calls==draws);CHECK(notifies==(notify?1:0));CHECK(deletes==(release&&final_count==0?1:0));CHECK(object.IVRefCount==final_count);
 if(draw)CHECK(emote.PrimitiveHandle.Data.Object==&replacement);else if(release||count==0)CHECK(emote.PrimitiveHandle.IsNull());else CHECK(emote.PrimitiveHandle.Data.Object==&object);
 CHECK(emote.EmoteScale==scale&&emote.EmoteAlpha==alpha&&emote.ParentThing==&thing);CHECK(thing.Pos.X==-2&&thing.Pos.Y==4&&thing.Pos.Z==0.5f);CHECK(thing.Appearance.loaded==loaded);
}
int main(){int cases=0;const float radii[]={-2,0,0.5f,1,2,4},xs[]={-4,-1,0,0.5f,2},yz[]={-1,0,1},scales[]={0,0.5f,2};const int counts[]={0,1,2,4};const unsigned char alphas[]={0,1,128,255};for(int r=0;r<6;++r)for(int x=0;x<5;++x)for(int y=0;y<3;++y)for(int z=0;z<3;++z)for(int f=0;f<2;++f)for(int c=0;c<4;++c)for(int k=0;k<2;++k)for(int s=0;s<3;++s)for(int a=0;a<4;++a){one(radii[r],xs[x],yz[y],yz[z],f!=0,counts[c],k!=0,scales[s],alphas[a]);if(failed){printf("0071ad20_TEST FAIL line%d case%d\n",fail_line,cases);return 1;}++cases;}if(cases!=51840)return 2;puts("0071ad20_TEST PASS");return 0;}