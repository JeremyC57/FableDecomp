#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_BOX_OWNER_SPEED_PASS26
#define FABLE_RETAIL_C2DBOXF_EXTENTS_OWNER_SPEED_PASS26
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_BOX_CTOR_BODY
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_CDRAWQUESTINFO_ELEMENTS
// Leaf: ?DrawAt@CQuestInfoElementCounterList@CDrawQuestInfo@NPlayerGui@@UAEMABVC2DVector@@ABVCEnginePrimitivePrepareInfo@@@Z
// Flags: /O1 /Oy-
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_CTOR
#define FABLE_RETAIL_CPLAYERGUI_QUEST_DRAW
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CPLAYERGUI_DRAWTEXT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_MAINFONT
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_ORIGINAL_REFERENCES
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CGRAPHICDATABANK_ORIGINAL_METHODS_PASS22
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/NPlayerGui_CDrawQuestInfo.h"
#include "engine/CPlayerGui.h"
#include "engine/CMainGameComponent.h"
class CDataInputStream;class CDataOutputStream;
#include "engine/CGraphicDataBank.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
void FABLE_FASTCALL GFACoordToVCoord(const C2DVector&,C2DVector&);
#define FABLE_RETAIL_NPLAYERGUI_GFTOSCREEN_BOX_PASS23
#include "engine/NPlayerGui_Graphics.h"

#include <cstdio>
#include <cmath>
static int errors=0,clears=0,peeks=0,bankcalls=0,texts=0,draws=0,conversions=0,wide_live=0,wide_dtors=0,expected_big=0;static float scale=1,graphic_w=16,graphic_h=8,base_x=30,base_y=40,gui_alpha=1;static unsigned char expected_alpha=255;
#define CHECK(x) do{if(!(x)){++errors;std::printf("FAIL %d %s\n",__LINE__,#x);}}while(0)
static bool near(float a,float b){return std::fabs(a-b)<.001f;}
struct FixtureWide {FixtureWide(const wchar_t*s){CHECK(s[0]=='5'&&s[1]==0);++wide_live;}~FixtureWide(){--wide_live;++wide_dtors;}};
struct FixtureFont{};struct FixtureGuiDef{};
struct FixtureBank{unsigned long GetGraphicFrameWidth(unsigned long g,unsigned long f)const{++bankcalls;CHECK(g==19&&f==0);return(unsigned long)graphic_w;}unsigned long GetGraphicFrameHeight(unsigned long g,unsigned long f)const{++bankcalls;CHECK(g==19&&f==0);return(unsigned long)graphic_h;}};
struct FixtureHandles{int items[100],count,resizes;FixtureHandles():count(0),resizes(0){}void resize(unsigned n){++resizes;CHECK(n<100);count=n;for(unsigned i=0;i<n;++i)items[i]=100+i;}int&operator[](unsigned i){CHECK(i<(unsigned)count);return items[i];}};
struct FixtureSprite{FixtureSprite(unsigned __int64 id,const C2DBoxF&box,EEngine2DLayer layer,unsigned long bank,unsigned long frame,EEnginePrimitiveBlendMode blend,EEngineInterpolationSet interp,float a,float z,unsigned char alpha,unsigned char r,unsigned char g,unsigned char b,unsigned long flags,const C2DVector&u,const C2DVector&v){CHECK(id==0&&layer==ENGINE_2D_LAYER_6&&bank==19&&frame==0&&blend==ENGINE_PRIMITIVE_BLEND_ALPHA&&interp==ENGINE_INTERPOLATION_SET_0&&a==0&&z==0&&alpha==expected_alpha&&r==255&&g==255&&b==255&&flags==2&&u.X==0&&u.Y==0&&v.X==0&&v.Y==0);int i=draws;bool big=i<expected_big;float w=graphic_w*(big?1.2f:.75f),h=graphic_h*(big?1.2f:.75f);float cursor=base_x+graphic_w*scale*(i<expected_big?i:expected_big)+graphic_w*.75f*scale*(i>expected_big?i-expected_big:0);float x=cursor-(big?w*.1f:0),y=base_y-(big?h*.1f:0);CHECK(near(box.TLX,x)&&near(box.TLY,y)&&near(box.BRX,x+w*scale)&&near(box.BRY,y+h*scale));CHECK(wide_live==0&&wide_dtors==(i<expected_big?i+1:expected_big));}};
struct FixtureEngine{void SetPersistentPrimitive(int&h,const FixtureSprite&,unsigned long f){CHECK(h==100+draws&&f==0xc0);++draws;}};
struct FixtureInfo{FixtureEngine&Engine;FixtureInfo(FixtureEngine&e):Engine(e){}};
namespace NPlayerGui{struct FixtureDrawQuestInfo{FixtureBank&Bank;FixtureDrawQuestInfo(FixtureBank&b):Bank(b){}struct FixtureCounterList{long Graphic,Value,MaxValue;float Scale;FixtureHandles PrimitiveHandles;void Clear(){++clears;PrimitiveHandles.count=0;}float DrawAt(const C2DVector&,const FixtureInfo&);};};}
struct FixtureGui{NPlayerGui::FixtureDrawQuestInfo&quest;static FixtureGui*current;FixtureGui(NPlayerGui::FixtureDrawQuestInfo&q):quest(q){}static FixtureGui&Get(){return*current;}static const FixtureGuiDef&PeekGuiDef(){static FixtureGuiDef d;++peeks;CHECK(clears==1);return d;}NPlayerGui::FixtureDrawQuestInfo&GetQuestInfo(){return quest;}unsigned char ScaleAlpha(unsigned char a)const{return(unsigned char)(a*gui_alpha);}void Draw2DText(const C2DVector&p,const FixtureWide&,const FixtureFont&,const CRGBColour&colour,float s,EEngine2DLayer layer,bool flag){CHECK(wide_live==1&&colour.R==255&&colour.G==255&&colour.B==255&&colour.A==255&&s==scale&&layer==ENGINE_2D_LAYER_5&&!flag);CHECK(near(p.X,(base_x+texts*graphic_w*scale+scale*8)*2)&&near(p.Y,(base_y+scale*4)*2));++texts;}};FixtureGui*FixtureGui::current=0;
struct FixtureComponent{static FixtureComponent*Get(){static FixtureComponent x;return&x;}const FixtureFont&PeekMainFont()const{static FixtureFont f;return f;}};
void FABLE_FASTCALL GFACoordToVCoord(const C2DVector&in,C2DVector&out){++conversions;out.X=in.X*2;out.Y=in.Y*2;}
#define CDrawQuestInfo FixtureDrawQuestInfo
#define CQuestInfoElementCounterList FixtureCounterList
#define CPlayerGui FixtureGui
#define CPlayerGuiDef FixtureGuiDef
#define CGraphicDataBank FixtureBank
#define CFontBank FixtureFont
#define CMainGameComponent FixtureComponent
#define CWideString FixtureWide
#define CEnginePrimitivePrepareInfo FixtureInfo
#define CEnginePrimitive2DSprite FixtureSprite
float NPlayerGui::CDrawQuestInfo::CQuestInfoElementCounterList::DrawAt(const C2DVector& pos,const CEnginePrimitivePrepareInfo& info)
{
 Clear();
 if(Graphic>0) {
  const CPlayerGuiDef& gui_def=CPlayerGui::Get().PeekGuiDef();
  long num_big=Value/5;
  long num_small=Value%5;
  long total_sprites=num_big+num_small;
  PrimitiveHandles.resize(total_sprites);
  const CGraphicDataBank& graphic_bank=CPlayerGui::Get().GetQuestInfo().Bank;
  float graphic_width=(float)graphic_bank.GetGraphicFrameWidth(Graphic,0);
  float graphic_height=(float)graphic_bank.GetGraphicFrameHeight(Graphic,0);
  C2DVector draw_pos=pos;
  long handle_index=0;
  for(long i=0;i<total_sprites;++i) {
   C2DVector sprite_pos=draw_pos;
   float sprite_width,sprite_height;
   if(i>=num_big) {
    sprite_width=graphic_width*0.75f;
    sprite_height=graphic_height*0.75f;
    draw_pos.X+=sprite_width*Scale;
   } else {
    C2DVector text_pos;
    GFACoordToVCoord(C2DVector(Scale*8.0f,Scale*4.0f)+draw_pos,text_pos);
    const CFontBank& font_bank=CMainGameComponent::Get()->PeekMainFont();
    CPlayerGui::Get().Draw2DText(text_pos,L"5",font_bank,CRGBColour(255,255,255),Scale,ENGINE_2D_LAYER_5,false);
    draw_pos.X+=graphic_width*Scale;
    sprite_width=graphic_width*1.2f;
    sprite_height=graphic_height*1.2f;
    sprite_pos.X-=sprite_width*0.1f;
    sprite_pos.Y-=sprite_height*0.1f;
   }
   C2DBoxF sprite_box(sprite_pos,C2DExtentsF(sprite_width*Scale,sprite_height*Scale));
   CEnginePrimitive2DSprite graphic_sprite(0,GFToScreen(sprite_box),ENGINE_2D_LAYER_6,Graphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_0,0.0f,0.0f,CPlayerGui::Get().ScaleAlpha(255),255,255,255,2,C2DVector(0,0),C2DVector(0,0));
   info.Engine.SetPersistentPrimitive(PrimitiveHandles[handle_index],graphic_sprite,0xc0);
   ++handle_index;
  }
  return graphic_height*Scale+2.0f;
 }
 return 0.0f;
}

#undef CDrawQuestInfo
#undef CQuestInfoElementCounterList
#undef CPlayerGui
#undef CPlayerGuiDef
#undef CGraphicDataBank
#undef CFontBank
#undef CMainGameComponent
#undef CWideString
#undef CEnginePrimitivePrepareInfo
#undef CEnginePrimitive2DSprite
int main(){int values[]={0,1,4,5,6,9,10,12,17};float scales[]={0,.5f,1,1.75f};for(int test=0;test<38;++test){FixtureBank bank;NPlayerGui::FixtureDrawQuestInfo quest(bank);FixtureGui gui(quest);FixtureGui::current=&gui;NPlayerGui::FixtureDrawQuestInfo::FixtureCounterList counter;counter.Graphic=test<36?19:test==36?0:-2;counter.Value=values[(test/4)%9];counter.MaxValue=1;counter.Scale=scales[test%4];scale=counter.Scale;gui_alpha=(test%3)*.5f;expected_alpha=(unsigned char)(255*gui_alpha);expected_big=counter.Value/5;int total=expected_big+counter.Value%5;clears=peeks=bankcalls=texts=draws=conversions=wide_live=wide_dtors=0;FixtureEngine engine;FixtureInfo info(engine);float result=counter.DrawAt(C2DVector(base_x,base_y),info);bool visible=test<36;CHECK(clears==1&&peeks==(visible?1:0)&&bankcalls==(visible?2:0)&&texts==(visible?expected_big:0)&&draws==(visible?total:0)&&conversions==texts&&wide_live==0&&wide_dtors==texts);CHECK(counter.PrimitiveHandles.resizes==(visible?1:0)&&counter.PrimitiveHandles.count==(visible?total:0));CHECK(near(result,visible?graphic_h*scale+2:0)&&counter.MaxValue==1);}std::puts(errors?"00650c1d_TEST FAIL":"00650c1d_TEST PASS");return errors?1:0;}