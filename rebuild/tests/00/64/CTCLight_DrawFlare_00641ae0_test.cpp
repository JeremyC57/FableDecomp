#include <stdio.h>
#include <vector>
#include <string>
static bool failed;static int failed_line;
#define CHECK(x) do{if(!(x)){failed=true;failed_line=__LINE__;return;}}while(0)
static std::string trace;static int temporary_live,temporary_dtors,uid_calls;
class C3DVector{ public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}};
struct CUID{unsigned int Low,High;operator unsigned long()const{return Low;}};
class CThing{ public:CUID UniqueID;CUID GetUniqueID()const{++uid_calls;return UniqueID;}};
class CLightDef{ public:long HaloGraphic;};
enum EEnginePrimitiveBlendMode{ENGINE_PRIMITIVE_BLEND_ADDSMOOTH=4};
enum EEngineInterpolationSet{ENGINE_INTERPOLATION_SET_0=0};
class CEnginePrimitive{ public:int Type,InterpolationSet;CEnginePrimitive(int t,int s):Type(t),InterpolationSet(s){}};
struct CEnginePrimitiveFlareSprite:CEnginePrimitive{
 C3DVector WorldPos;unsigned long BankIndex,Frame;float LightSourceRadius,SizeX,SizeY,MinScale,FadeStart,Angle;EEnginePrimitiveBlendMode BlendMode;int Identifier;unsigned char Alpha;
 CEnginePrimitiveFlareSprite(CUID,const C3DVector&p,unsigned long bank,unsigned long frame,float angle,float radius,float sx,float sy,float minscale,EEnginePrimitiveBlendMode blend,unsigned long identifier,unsigned char alpha,float fade,EEngineInterpolationSet is)
 :CEnginePrimitive(17,is),WorldPos(p),BankIndex(bank),Frame(frame),LightSourceRadius(radius),SizeX(sx),SizeY(sy),MinScale(minscale),FadeStart(fade),Angle(angle),BlendMode(blend),Identifier((int)(unsigned int)identifier),Alpha(alpha){}
};
namespace NGraphicAppearance{
 enum EPrimitiveType{PRIM=3};
 class CPrimitive{ public:EPrimitiveType Type;long BankIndex;bool tracked;CPrimitive(EPrimitiveType t,long b=0):Type(t),BankIndex(b),tracked(true){++temporary_live;trace+='C';}CPrimitive():Type(PRIM),BankIndex(0),tracked(false){}~CPrimitive(){if(tracked){--temporary_live;++temporary_dtors;trace+='D';}}};
 class CGraphicAppearanceDrawInfo{ public:};
 class CTCGraphicAppearance{ public:
  float Fade;int add_calls,set_calls;CPrimitive*returned;bool add_correct;CPrimitive*selected;CEnginePrimitiveFlareSprite recorded;unsigned short flags;
  CTCGraphicAppearance():recorded(CUID(),C3DVector(),0,0,0,0,0,0,0,ENGINE_PRIMITIVE_BLEND_ADDSMOOTH,0,0,0,ENGINE_INTERPOLATION_SET_0){}
  float GetFadeDistance()const{trace+='F';return Fade;}
  CPrimitive*AddPersistentPrimitive(const CPrimitive&p){++add_calls;add_correct=p.Type==PRIM&&p.BankIndex==0&&temporary_live==1;trace+='A';return returned;}
  void SetPrimitiveGraphics(CPrimitive*p,const CEnginePrimitive&g,unsigned short f){++set_calls;selected=p;recorded=static_cast<const CEnginePrimitiveFlareSprite&>(g);flags=f;trace+='G';}
 };
}
class CTCLight{ public:
 CThing*ParentThing;CLightDef*PLightDef;bool Overridden;std::vector<NGraphicAppearance::CPrimitive*>FlareSprites;
 CThing&GetParentThing(){return *ParentThing;}long GetHaloGraphic()const{if(Overridden)return 0;return PLightDef->HaloGraphic;}
 void DrawFlare(long,const C3DVector&,NGraphicAppearance::CTCGraphicAppearance&,const NGraphicAppearance::CGraphicAppearanceDrawInfo&);
};
void CTCLight::DrawFlare(long flare_no,const C3DVector& pos,NGraphicAppearance::CTCGraphicAppearance& tc_appearance,
 const NGraphicAppearance::CGraphicAppearanceDrawInfo& draw_info)
{
 float fade_dist=tc_appearance.GetFadeDistance();
 if((unsigned int)flare_no>=FlareSprites.size()) {
  FlareSprites.resize(flare_no+1);
  FlareSprites[flare_no]=tc_appearance.AddPersistentPrimitive(NGraphicAppearance::CPrimitive((NGraphicAppearance::EPrimitiveType)3));
 }
 long halo_graphic=GetHaloGraphic();
 if(halo_graphic>0) {
  tc_appearance.SetPrimitiveGraphics(FlareSprites[flare_no],CEnginePrimitiveFlareSprite(GetParentThing().GetUniqueID(),pos,halo_graphic,0,
   0.0f,1.0f,2.5f,2.5f,0.33f,ENGINE_PRIMITIVE_BLEND_ADDSMOOTH,(unsigned long)GetParentThing().GetUniqueID(),(unsigned char)fade_dist,0.0f,ENGINE_INTERPOLATION_SET_0),0);
 }
}

static void one(int old_size,int index,int override_case,int halo_case,int fade_case,int uid_case,int point_case)
{
 const long halos[]={-1,0,1,255};const float fades[]={0,1,128,254,255};const unsigned int ids[]={0u,5u,0x7fffffffu,0xffffffffu};
 NGraphicAppearance::CPrimitive retained[5],created;CTCLight c;CThing parent;parent.UniqueID.Low=ids[uid_case];parent.UniqueID.High=0x12345678u;CLightDef def;def.HaloGraphic=halos[halo_case];c.ParentThing=&parent;c.PLightDef=&def;c.Overridden=override_case!=0;for(int i=0;i<old_size;++i)c.FlareSprites.push_back(&retained[i]);
 NGraphicAppearance::CTCGraphicAppearance appearance;appearance.Fade=fades[fade_case];appearance.add_calls=appearance.set_calls=0;appearance.returned=&created;appearance.add_correct=false;appearance.selected=0;appearance.flags=99;NGraphicAppearance::CGraphicAppearanceDrawInfo info;C3DVector pos(point_case*2.0f,point_case*-4.0f,point_case*8.0f);
 temporary_live=temporary_dtors=uid_calls=0;trace="";c.DrawFlare(index,pos,appearance,info);
 bool grow=index>=old_size,draw=!override_case&&halos[halo_case]>0;CHECK(c.FlareSprites.size()==(unsigned)(grow?index+1:old_size));CHECK(appearance.add_calls==(grow?1:0));CHECK(!grow||appearance.add_correct);CHECK(temporary_live==0&&temporary_dtors==(grow?1:0));
 for(int j=0;j<old_size;++j)CHECK(c.FlareSprites[j]==&retained[j]);if(grow){for(int j=old_size;j<index;++j)CHECK(c.FlareSprites[j]==0);CHECK(c.FlareSprites[index]==&created);}
 CHECK(appearance.set_calls==(draw?1:0)&&uid_calls==(draw?2:0));std::string expected="F";if(grow)expected+="CAD";if(draw)expected+='G';CHECK(trace==expected);
 if(draw){const CEnginePrimitiveFlareSprite&p=appearance.recorded;CHECK(appearance.selected==c.FlareSprites[index]&&appearance.flags==0);CHECK(p.Type==17&&p.InterpolationSet==0);CHECK(p.WorldPos.X==pos.X&&p.WorldPos.Y==pos.Y&&p.WorldPos.Z==pos.Z);CHECK(p.BankIndex==(unsigned long)halos[halo_case]&&p.Frame==0);CHECK(p.LightSourceRadius==1.0f&&p.SizeX==2.5f&&p.SizeY==2.5f);union{float f;unsigned int u;}bits;bits.f=p.MinScale;CHECK(bits.u==0x3ea8f5c3u);CHECK(p.FadeStart==0&&p.Angle==0&&p.BlendMode==ENGINE_PRIMITIVE_BLEND_ADDSMOOTH);CHECK((unsigned int)p.Identifier==ids[uid_case]&&p.Alpha==(unsigned char)fades[fade_case]);}
 CHECK(parent.UniqueID.Low==ids[uid_case]&&parent.UniqueID.High==0x12345678u&&def.HaloGraphic==halos[halo_case]);CHECK(pos.X==point_case*2.0f&&pos.Y==point_case*-4.0f&&pos.Z==point_case*8.0f);
}
int main(){unsigned long cases=0;for(int n=0;n<6;++n)for(int i=0;i<9;++i)for(int o=0;o<2;++o)for(int h=0;h<4;++h)for(int f=0;f<5;++f)for(int u=0;u<4;++u)for(int p=-1;p<2;++p){one(n,i,o,h,f,u,p);if(failed){printf("00641ae0_TEST FAIL line%d case%lu\n",failed_line,cases);return 1;}++cases;}if(cases!=25920)return 2;puts("00641ae0_TEST PASS");return 0;}