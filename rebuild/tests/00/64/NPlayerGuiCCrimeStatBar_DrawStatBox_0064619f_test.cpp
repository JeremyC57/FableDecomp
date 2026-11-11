#include <stdio.h>
#include <vector>
#include <string>
static bool failed;static int fail_line;
#define CHECK(x) do{if(!(x)){failed=true;fail_line=__LINE__;return;}}while(0)
class C2DVector{ public:float X,Y;C2DVector(){}C2DVector(float x,float y):X(x),Y(y){}C2DVector operator+(const C2DVector&v)const{return C2DVector(X+v.X,Y+v.Y);}};
class C2DExtentsI{ public:int X,Y;C2DExtentsI(int x,int y):X(x),Y(y){}};
class CRGBColour{ public:unsigned char B,G,R,A;CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a){R=r;G=g;B=b;A=a;}};
class CWideString{ public:};
enum EEngine2DLayer{ENGINE_2D_LAYER_1=68,ENGINE_2D_LAYER_2=69};
enum EEnginePrimitiveBlendMode{ENGINE_PRIMITIVE_BLEND_ALPHA=2};
enum EEngineInterpolationSet{ENGINE_INTERPOLATION_SET_0=0};
static std::string trace;
class CDisplayManager{ public:int width;C2DExtentsI GetRenderTargetDimensions(){trace+='D';return C2DExtentsI(width,768);}};
static CDisplayManager display;
CDisplayManager* GPDisplayManager=&display;
class CGraphicDataBank{ public:unsigned int GetGraphicMaxWidth(unsigned int g)const{trace+='W';return g==1?256:512;}};
class CPlayerGui{ public:float Alpha;static CPlayerGui&Get(){static CPlayerGui p;return p;}unsigned char ScaleAlpha(unsigned char a)const{return (unsigned char)(a*Alpha);}};
class CEnginePrimitiveHandle{ public:int index;};
class CEnginePrimitive{ public:int kind;C2DVector pos,uv0,uv1;int layer,bank,frame,bank2,frame2,blend,interpolation;float a0,a1;CRGBColour c0,c1;unsigned int flags;bool id_zero;CEnginePrimitive():c0(0,0,0,0),c1(0,0,0,0){}};
struct CEnginePrimitive2DClockSprite:CEnginePrimitive{
 CEnginePrimitive2DClockSprite(unsigned int id,const C2DVector&p,EEngine2DLayer l,unsigned int b,unsigned int f,const CRGBColour&c,unsigned int b2,unsigned int f2,const CRGBColour&d,EEnginePrimitiveBlendMode mode,float start,float end,const C2DVector&u,const C2DVector&v,unsigned int fl,EEngineInterpolationSet is){kind=1;id_zero=id==0;pos=p;layer=l;bank=b;frame=f;c0=c;bank2=b2;frame2=f2;c1=d;blend=mode;a0=start;a1=end;uv0=u;uv1=v;flags=fl;interpolation=is;trace+='C';}
};
struct CEnginePrimitive2DSprite:CEnginePrimitive{
 CEnginePrimitive2DSprite(unsigned int id,const C2DVector&p,EEngine2DLayer l,unsigned int b,unsigned int f,EEnginePrimitiveBlendMode mode,EEngineInterpolationSet is,float start,float end,unsigned char a,unsigned char r,unsigned char g,unsigned char blue,unsigned int fl,const C2DVector&u,const C2DVector&v){kind=2;id_zero=id==0;pos=p;layer=l;bank=b;frame=f;bank2=frame2=0;c0=CRGBColour(r,g,blue,a);blend=mode;interpolation=is;a0=start;a1=end;uv0=u;uv1=v;flags=fl;trace+='I';}
};
class CIEngine{ public:std::vector<CEnginePrimitive>draws;std::vector<int>handles;std::vector<unsigned long>flags;void SetPersistentPrimitive(CEnginePrimitiveHandle&h,const CEnginePrimitive&p,unsigned long f){draws.push_back(p);handles.push_back(h.index);flags.push_back(f);trace+='P';}};
class CEnginePrimitivePrepareInfo{ public:CIEngine&Engine;CEnginePrimitivePrepareInfo(CIEngine&e):Engine(e){}};
class CPlayerGuiDef{ public:C2DVector CrimeBarIconOffset,CrimeBarBorderOffset;};
namespace NPlayerGui{
inline C2DVector GFToScreen(const C2DVector&p){return p;}
class CStatBar{ public:const CEnginePrimitivePrepareInfo*got_info;const CGraphicDataBank*got_bank;const C2DVector*got_pos;const CWideString*got_text;long got_icon,got_border,got_effect;unsigned char got_alpha;bool got_mirror,got_centre;int base_calls;
 void DrawStatBox(const CEnginePrimitivePrepareInfo&i,const CGraphicDataBank&b,const C2DVector&p,long ig,long bg,long e,const CWideString&t,unsigned char a,bool m,bool c){++base_calls;got_info=&i;got_bank=&b;got_pos=&p;got_icon=ig;got_border=bg;got_effect=e;got_text=&t;got_alpha=a;got_mirror=m;got_centre=c;trace+='B';}
};
class CCrimeStatBar:public CStatBar{ public:CPlayerGuiDef GuiDef;float NormalisedTimeLeft;CEnginePrimitiveHandle PrimitiveHandles[5];void DrawStatBox(const CEnginePrimitivePrepareInfo&,const CGraphicDataBank&,const C2DVector&,long,long,long,const CWideString&,unsigned char,bool,bool);};
}
void NPlayerGui::CCrimeStatBar::DrawStatBox(const CEnginePrimitivePrepareInfo& info,const CGraphicDataBank& graphic_bank,
 const C2DVector& tl_pos,long icon_graphic,long border_graphic,long effect_index,const CWideString& stat_text,
 unsigned char alpha,bool mirror,bool centre)
{
 C2DVector draw_pos=tl_pos+GuiDef.CrimeBarIconOffset;
 if(border_graphic>0) {
  C2DVector offset_pos((GPDisplayManager->GetRenderTargetDimensions().X-graphic_bank.GetGraphicMaxWidth(border_graphic))/2.0f,
   draw_pos.Y+GuiDef.CrimeBarBorderOffset.Y);
  float angle=NormalisedTimeLeft;
  CEnginePrimitive2DClockSprite border_sprite(0,offset_pos,ENGINE_2D_LAYER_2,border_graphic,0,CRGBColour(0,0,0,alpha),
   border_graphic,0,CRGBColour(255,255,255,alpha),ENGINE_PRIMITIVE_BLEND_ALPHA,angle,angle,C2DVector(0,0),C2DVector(0,0),2,ENGINE_INTERPOLATION_SET_0);
  info.Engine.SetPersistentPrimitive(PrimitiveHandles[4],border_sprite,0xc0);
 }
 if(icon_graphic>0) {
  C2DVector offset_pos((GPDisplayManager->GetRenderTargetDimensions().X-graphic_bank.GetGraphicMaxWidth(icon_graphic))/2.0f,draw_pos.Y);
  CEnginePrimitive2DSprite icon_sprite(0,GFToScreen(offset_pos),ENGINE_2D_LAYER_1,icon_graphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,
   ENGINE_INTERPOLATION_SET_0,0,0,CPlayerGui::Get().ScaleAlpha(alpha),255,255,255,0,C2DVector(0,0),C2DVector(0,0));
  info.Engine.SetPersistentPrimitive(PrimitiveHandles[3],icon_sprite,0xc0);
 }
 CStatBar::DrawStatBox(info,graphic_bank,tl_pos,0,0,0,stat_text,alpha,mirror,centre);
}

static void one(int icon,int border,unsigned char alpha,int fade,int time,int dims,int pos,bool mirror,bool centre)
{
 trace="";display.width=dims*512;CPlayerGui::Get().Alpha=fade*0.5f;
 CIEngine engine;CEnginePrimitivePrepareInfo info(engine);CGraphicDataBank bank;CWideString text;
 C2DVector tl((pos-1)*16.0f,(pos-1)*8.0f);NPlayerGui::CCrimeStatBar bar;
 bar.GuiDef.CrimeBarIconOffset=C2DVector(128,4);bar.GuiDef.CrimeBarBorderOffset=C2DVector(256,-2);
 bar.NormalisedTimeLeft=time*0.5f;bar.base_calls=0;for(int j=0;j<5;++j)bar.PrimitiveHandles[j].index=j;
 bar.DrawStatBox(info,bank,tl,icon,border,77,text,alpha,mirror,centre);
 unsigned count=(border>0)+(icon>0);CHECK(engine.draws.size()==count);CHECK(bar.base_calls==1);CHECK(bar.got_info==&info&&bar.got_bank==&bank&&bar.got_pos==&tl&&bar.got_text==&text);CHECK(bar.got_icon==0&&bar.got_border==0&&bar.got_effect==0);CHECK(bar.got_alpha==alpha&&bar.got_mirror==mirror&&bar.got_centre==centre);
 std::string want;if(border>0)want+="DWCP";if(icon>0)want+="DWIP";want+='B';CHECK(trace==want);
 for(unsigned i=0;i<count;++i){const CEnginePrimitive&p=engine.draws[i];bool clock=border>0&&i==0;int graphic=clock?border:icon;unsigned int diff=(unsigned int)display.width-(graphic==1?256u:512u);float ex=(float)diff*0.5f;
 CHECK(p.kind==(clock?1:2)&&p.id_zero);CHECK(p.pos.X==ex&&p.pos.Y==tl.Y+(clock?2:4));CHECK(p.layer==(clock?69:68)&&p.bank==graphic&&p.frame==0);CHECK(p.blend==2&&p.interpolation==0);CHECK(p.a0==(clock?bar.NormalisedTimeLeft:0)&&p.a1==p.a0);CHECK(p.uv0.X==0&&p.uv0.Y==0&&p.uv1.X==0&&p.uv1.Y==0);CHECK(p.flags==(clock?2u:0u));CHECK(engine.flags[i]==192&&engine.handles[i]==(clock?4:3));
 if(clock){CHECK(p.bank2==graphic&&p.frame2==0);CHECK(p.c0.R==0&&p.c0.G==0&&p.c0.B==0&&p.c0.A==alpha);CHECK(p.c1.R==255&&p.c1.G==255&&p.c1.B==255&&p.c1.A==alpha);}else{unsigned int expected_alpha=fade==0?0:fade==1?(unsigned int)alpha/2:alpha;CHECK(p.c0.A==expected_alpha&&p.c0.R==255&&p.c0.G==255&&p.c0.B==255);}
 }
 CHECK(tl.X==(pos-1)*16.0f&&tl.Y==(pos-1)*8.0f);CHECK(bar.NormalisedTimeLeft==time*0.5f);
}
int main(){unsigned long count=0;const int values[]={-1,0,1,2};const unsigned char alphas[]={0,1,128,254,255};for(int i=0;i<4;++i)for(int b=0;b<4;++b)for(int a=0;a<5;++a)for(int f=0;f<3;++f)for(int t=0;t<3;++t)for(int d=0;d<3;++d)for(int p=0;p<3;++p)for(int m=0;m<2;++m)for(int c=0;c<2;++c){one(values[i],values[b],alphas[a],f,t,d,p,m!=0,c!=0);if(failed){printf("0064619f_TEST FAIL line%d case%lu\n",fail_line,count);return 1;}++count;}if(count!=25920)return 2;puts("0064619f_TEST PASS");return 0;}