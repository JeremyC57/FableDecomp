#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <vector>
#ifndef _MSC_VER
#define FABLE_FASTCALL
#else
#define FABLE_FASTCALL __fastcall
#endif
static int live_strings,clear_calls,draw_calls,sprite_calls,conversions;
class C2DVector {public:float X,Y;C2DVector(){}C2DVector(float x,float y):X(x),Y(y){}};
class C2DExtentsF {public:float X,Y;C2DExtentsF(float x,float y):X(x),Y(y){}};
class C2DBoxF {public:float TLX,TLY,BRX,BRY;C2DBoxF():TLX(0),TLY(0),BRX(0),BRY(0){}C2DBoxF(const C2DVector&p,const C2DExtentsF&e):TLX(p.X),TLY(p.Y),BRX(p.X+e.X),BRY(p.Y+e.Y){}C2DBoxF(const C2DBoxF&b):TLX(b.TLX),TLY(b.TLY),BRX(b.BRX),BRY(b.BRY){}};
class CWideString {public:char text[96];CWideString(){text[0]=0;++live_strings;}CWideString(const CWideString&s){strcpy(text,s.text);++live_strings;}~CWideString(){--live_strings;}const CWideString&operator+=(const CWideString&s){strcat(text,s.text);return *this;}};
CWideString FABLE_FASTCALL GFIntToWideString(long n){CWideString s;sprintf(s.text,"%ld",n);++conversions;return s;}
CWideString FABLE_FASTCALL operator+(const wchar_t*w,const CWideString&s){CWideString out;int i=0;while(w[i]){out.text[i]=(char)w[i];++i;}out.text[i]=0;strcat(out.text,s.text);return out;}
class CRGBColour {public:unsigned char R,G,B;CRGBColour(unsigned char r,unsigned char g,unsigned char b):R(r),G(g),B(b){}};
enum EEngine2DLayer{ENGINE_2D_LAYER_6=74};
enum EEnginePrimitiveBlend{ENGINE_PRIMITIVE_BLEND_ALPHA=2};
enum EEngineInterpolationSet{ENGINE_INTERPOLATION_SET_0=0};
class CFontBank {public:long Height;long GetFontHeight()const{return Height;}};
class CGraphicDataBank {public:unsigned long Width,Height;mutable int calls;CGraphicDataBank():calls(0){}unsigned long GetGraphicFrameWidth(long g,long f)const{assert(g==7&&f==0);++calls;return Width;}unsigned long GetGraphicFrameHeight(long g,long f)const{assert(g==7&&f==0);++calls;return Height;}};
class CEnginePrimitiveHandle{};
class CEnginePrimitive2DSprite {public:C2DBoxF box;unsigned char alpha;
 CEnginePrimitive2DSprite(int id,const C2DBoxF&b,EEngine2DLayer layer,long graphic,int frame,EEnginePrimitiveBlend blend,EEngineInterpolationSet interpolation,float a,float z,unsigned char opacity,unsigned char r,unsigned char g,unsigned char blue,int p,const C2DVector&u,const C2DVector&v):box(b),alpha(opacity){assert(id==0&&layer==ENGINE_2D_LAYER_6&&graphic==7&&frame==0&&blend==ENGINE_PRIMITIVE_BLEND_ALPHA&&interpolation==ENGINE_INTERPOLATION_SET_0&&a==0&&z==0&&r==255&&g==255&&blue==255&&p==2&&u.X==0&&u.Y==0&&v.X==0&&v.Y==0);}
};
class CIEngine {public:C2DBoxF box;unsigned char alpha;void SetPersistentPrimitive(CEnginePrimitiveHandle&,const CEnginePrimitive2DSprite&p,unsigned long f){assert(f==0xc0&&clear_calls==1);box=p.box;alpha=p.alpha;++sprite_calls;}};
class CEnginePrimitivePrepareInfo {public:CIEngine*Engine;};
namespace NPlayerGui {
class CDrawQuestInfo {public:CGraphicDataBank*Bank;
 class CQuestInfoElementCounter {public:long Graphic,Value,MaxValue;float Scale;std::vector<CEnginePrimitiveHandle>PrimitiveHandles;void Clear(){assert(clear_calls==0);++clear_calls;PrimitiveHandles.clear();}float DrawAt(const C2DVector&,const CEnginePrimitivePrepareInfo&);};
};
}
class CPlayerGui {public:NPlayerGui::CDrawQuestInfo quest;float Alpha;C2DVector pos;char last_text[96];float scale;const CFontBank*font;static CPlayerGui*Self;static CPlayerGui&Get(){return *Self;}NPlayerGui::CDrawQuestInfo&GetQuestInfo(){return quest;}unsigned char ScaleAlpha(unsigned char a)const{return (unsigned char)(Alpha*a);}void Draw2DText(const C2DVector&p,const CWideString&t,const CFontBank&f,const CRGBColour&c,float s,EEngine2DLayer l,bool b){assert(clear_calls==1&&c.R==255&&c.G==255&&c.B==255&&l==ENGINE_2D_LAYER_6&&b);pos=p;strcpy(last_text,t.text);scale=s;font=&f;++draw_calls;}};CPlayerGui*CPlayerGui::Self=0;
class CMainGameComponent {public:CFontBank font;static CMainGameComponent*Self;static CMainGameComponent*Get(){return Self;}const CFontBank&PeekMainFont()const{return font;}};CMainGameComponent*CMainGameComponent::Self=0;
void FABLE_FASTCALL GFACoordToVCoord(const C2DVector&in,C2DVector&out){out.X=in.X*2.0f;out.Y=in.Y*4.0f;}
namespace NPlayerGui {
inline C2DBoxF FABLE_FASTCALL GFToScreen(const C2DBoxF& box){return box;}
float CDrawQuestInfo::CQuestInfoElementCounter::DrawAt(const C2DVector& pos,const CEnginePrimitivePrepareInfo& info)
{
 Clear();
 float graphic_width=0.0f;
 float graphic_height=0.0f;
 if(Graphic>0){
  PrimitiveHandles.resize(1);
  const CGraphicDataBank& graphic_bank=*CPlayerGui::Get().GetQuestInfo().Bank;
  graphic_width=(float)graphic_bank.GetGraphicFrameWidth(Graphic,0);
  graphic_height=(float)graphic_bank.GetGraphicFrameHeight(Graphic,0);
  C2DBoxF sprite_box(pos,C2DExtentsF(graphic_width*Scale,graphic_height*Scale));
  CEnginePrimitive2DSprite graphic_sprite(0,GFToScreen(sprite_box),ENGINE_2D_LAYER_6,Graphic,0,
    ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_0,0.0f,0.0f,CPlayerGui::Get().ScaleAlpha(255),255,255,255,2,
    C2DVector(0.0f,0.0f),C2DVector(0.0f,0.0f));
  info.Engine->SetPersistentPrimitive(PrimitiveHandles[0],graphic_sprite,0xc0);
 }
 const CFontBank& font_bank=CMainGameComponent::Get()->PeekMainFont();
 C2DVector text_pos(pos.X+graphic_width*Scale,pos.Y);
 text_pos.Y+=(graphic_height-(float)font_bank.GetFontHeight())*0.5f;
 C2DVector v_text_pos;
 GFACoordToVCoord(text_pos,v_text_pos);
 CWideString text=GFIntToWideString(Value);
 if(MaxValue>0)text+=L" / "+GFIntToWideString(MaxValue);
 CPlayerGui::Get().Draw2DText(v_text_pos,text,font_bank,CRGBColour(255,255,255),Scale,ENGINE_2D_LAYER_6,true);
 if(graphic_height>0.0f){double result=graphic_height*Scale+2.0f;return (float)result;}
 double result=font_bank.GetFontHeight()*Scale;
 return (float)result;
}
}

int main(){CPlayerGui gui;CPlayerGui::Self=&gui;CMainGameComponent game;CMainGameComponent::Self=&game;CGraphicDataBank bank;gui.quest.Bank=&bank;CIEngine engine;CEnginePrimitivePrepareInfo info;info.Engine=&engine;long values[]={-12,0,7,123};long maximums[]={-1,0,24};float scales[]={0.5f,1.0f,2.0f};
for(int graphic=0;graphic<2;++graphic)for(int h=0;h<3;++h)for(int v=0;v<4;++v)for(int m=0;m<3;++m)for(int k=0;k<3;++k){
 NPlayerGui::CDrawQuestInfo::CQuestInfoElementCounter item;item.Graphic=graphic?7:0;item.Value=values[v];item.MaxValue=maximums[m];item.Scale=scales[k];item.PrimitiveHandles.resize(3);bank.Width=16;bank.Height=h*8;bank.calls=0;game.font.Height=8;gui.Alpha=0.5f;clear_calls=draw_calls=sprite_calls=conversions=0;assert(live_strings==0);
 C2DVector at(4,12);float result=item.DrawAt(at,info);float height=graphic?(float)bank.Height:0.0f;float width=graphic?16.0f:0.0f;
 assert(result==(height>0?height*item.Scale+2.0f:8.0f*item.Scale));assert(clear_calls==1&&draw_calls==1&&sprite_calls==graphic&&bank.calls==graphic*2&&live_strings==0);assert(conversions==(item.MaxValue>0?2:1));assert(item.PrimitiveHandles.size()==(graphic?1u:0u));
 assert(gui.pos.X==(4.0f+width*item.Scale)*2.0f&&gui.pos.Y==(12.0f+(height-8.0f)*0.5f)*4.0f);assert(gui.font==&game.font&&gui.scale==item.Scale);char expected[96];if(item.MaxValue>0)sprintf(expected,"%ld / %ld",item.Value,item.MaxValue);else sprintf(expected,"%ld",item.Value);assert(strcmp(gui.last_text,expected)==0);
 if(graphic)assert(engine.box.TLX==4&&engine.box.TLY==12&&engine.box.BRX==4+16*item.Scale&&engine.box.BRY==12+height*item.Scale&&engine.alpha==127);
}
puts("006509f8_TEST PASS");return 0;}