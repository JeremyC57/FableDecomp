// Standalone C++03 behavior test: exact row body and real geometry arithmetic, recorded external rendering calls.
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <vector>
class C2DVector {
public:float X,Y;
 C2DVector(){} C2DVector(float x,float y):X(x),Y(y){}
 C2DVector(const C2DVector& p):X(p.X),Y(p.Y){}
 C2DVector operator+(const C2DVector& v)const{return C2DVector(X+v.X,Y+v.Y);}
};
class C2DBoxF {
public:float TLX,TLY,BRX,BRY;
 C2DBoxF():TLX(0),TLY(0),BRX(0),BRY(0){}
 C2DBoxF(float a,float b,float c,float d):TLX(a),TLY(b),BRX(c),BRY(d){}
 C2DBoxF(const C2DVector& a,const C2DVector& b):TLX(a.X),TLY(a.Y),BRX(b.X),BRY(b.Y){}
};
class CRGBColour {
public:unsigned char B,G,R,A;
 CRGBColour(){R=G=B=A=0;}
 CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a){R=r;G=g;B=b;A=a;}
 CRGBColour(const CRGBColour& c){R=c.R;G=c.G;B=c.B;A=c.A;}
};
enum EMiniGameType {MINIGAME_NONE=0, MINIGAME_FISHING=1, MINIGAME_DIGGING=2, MINIGAME_PICKPOCKET=3, MINIGAME_PICKLOCK=4, MINIGAME_STEAL=5, MINIGAME_TROPHY=6, MINIGAME_ORACLE=7};
enum EEngineInterpolationSet {ENGINE_INTERPOLATION_SET_0=0,ENGINE_INTERPOLATION_SET_1=1,ENGINE_INTERPOLATION_SET_2=2,ENGINE_INTERPOLATION_SET_NUM=3,ENGINE_INTERPOLATION_SET_ALWAYSUPDATE=0,ENGINE_INTERPOLATION_SET_PAUSED=1,ENGINE_INTERPOLATION_SET_BULLETTIME=2,ENGINE_INTERPOLATION_SET_DEFAULT=0};
enum EEnginePrimitiveBlendMode {ENGINE_PRIMITIVE_BLEND_SOLID=0,ENGINE_PRIMITIVE_BLEND_BOOLEAN_ALPHA=1,ENGINE_PRIMITIVE_BLEND_ALPHA=2,ENGINE_PRIMITIVE_BLEND_ADDITIVE=3,ENGINE_PRIMITIVE_BLEND_ADDSMOOTH=4,ENGINE_PRIMITIVE_BLEND_CONST_COLOUR=5,ENGINE_PRIMITIVE_BLEND_COUNT=6};
enum EEngine2DLayer {ENGINE_2D_RANGE_SIZE=16,ENGINE_2D_RANGE_0_START=0,ENGINE_2D_RANGE_1_START=16,ENGINE_2D_RANGE_2_START=32,ENGINE_2D_RANGE_3_START=48,ENGINE_2D_RANGE_4_START=64,ENGINE_2D_LAYER_M3=64,ENGINE_2D_LAYER_M2=65,ENGINE_2D_LAYER_M1=66,ENGINE_2D_LAYER_0=67,ENGINE_2D_LAYER_1=68,ENGINE_2D_LAYER_2=69,ENGINE_2D_LAYER_3=70,ENGINE_2D_LAYER_4=71,ENGINE_2D_MESH_LAYER=72,ENGINE_2D_LAYER_5=73,ENGINE_2D_LAYER_6=74,ENGINE_2D_LAYER_7=75,ENGINE_2D_LAYER_8=76,ENGINE_2D_LAYER_9=77,ENGINE_2D_LAYER_10=78,ENGINE_2D_LAYER_11=79,ENGINE_2D_LAYER_12=80,ENGINE_2D_LAYER_13=81,ENGINE_2D_LAYER_SPELLS=82,ENGINE_NUM_2D_LAYERS=83,ENGINE_2D_NULL_LAYER=-1,ENGINE_2D_LAYER_FIRST=0};
const float DELTA_NOTIONAL_ZERO=0.0001f;
class CEnginePrimitiveHandle {public:int Index;CEnginePrimitiveHandle():Index(-1){} };
class CEnginePrimitive2DSprite {
public:C2DBoxF Box;C2DVector Pos;bool IsBox;long Graphic;int Layer;CRGBColour Colour;unsigned long Flags;
 CEnginePrimitive2DSprite(int,const C2DVector& p,EEngine2DLayer layer,unsigned long graphic,unsigned long frame,EEnginePrimitiveBlendMode blend,EEngineInterpolationSet interpolation,float a,float b,unsigned char alpha,unsigned char red,unsigned char green,unsigned char blue,unsigned long flags,const C2DVector& u,const C2DVector& v):Pos(p),IsBox(false),Graphic(graphic),Layer(layer),Colour(red,green,blue,alpha),Flags(flags){check(frame,blend,interpolation,a,b,u,v);}
 CEnginePrimitive2DSprite(int,const C2DBoxF& box,EEngine2DLayer layer,unsigned long graphic,unsigned long frame,EEnginePrimitiveBlendMode blend,EEngineInterpolationSet interpolation,float a,float b,unsigned char alpha,unsigned char red,unsigned char green,unsigned char blue,unsigned long flags,const C2DVector& u,const C2DVector& v):Box(box),Pos(0,0),IsBox(true),Graphic(graphic),Layer(layer),Colour(red,green,blue,alpha),Flags(flags){check(frame,blend,interpolation,a,b,u,v);}
 static void check(unsigned long f,EEnginePrimitiveBlendMode b,EEngineInterpolationSet i,float x,float y,const C2DVector& u,const C2DVector& v){assert(f==0&&b==2&&i==0&&x==0&&y==0&&u.X==0&&u.Y==0&&v.X==0&&v.Y==0);}
};
static void (*renderHook)(int)=0;
class CIEngine {
public:std::vector<CEnginePrimitive2DSprite> Sprites;std::vector<int> Handles;
 void SetPersistentPrimitive(CEnginePrimitiveHandle& h,const CEnginePrimitive2DSprite& p,unsigned long f){assert(f==0xc0);Sprites.push_back(p);Handles.push_back(h.Index);if(renderHook)renderHook(h.Index);}
};
class CEnginePrimitivePrepareInfo {public:CIEngine* Engine;};
class CGraphicDataBank {
public:mutable std::vector<int> Queries;
 unsigned long GetGraphicFrameWidth(unsigned long graphic,unsigned long frame)const{assert(frame==0);Queries.push_back(graphic*10+1);return graphic==5?12:8;}
 unsigned long GetGraphicFrameHeight(unsigned long graphic,unsigned long frame)const{assert(frame==0);Queries.push_back(graphic*10+2);return graphic>=5?24:4;}
};
class CPlayerGuiDef {
public:long HeroSprintBarLeftGraphic,HeroSprintBarMiddleGraphic,HeroSprintBarRightGraphic,HeroSprintBarInnerGraphic,MiniGameFishingRightGraphic;C2DVector HeroSprintBarInnerGraphicOffset;
};
namespace NPlayerGui {
class CDrawQuestInfo {
public:enum EElementType {QUEST_INFO_ELEMENT_NONE=0, QUEST_INFO_ELEMENT_BAR=1, QUEST_INFO_ELEMENT_TIMER=2, QUEST_INFO_ELEMENT_COUNTER=3, QUEST_INFO_ELEMENT_COUNTER_LIST=4, QUEST_INFO_ELEMENT_COUNTER_TICK=5, QUEST_INFO_ELEMENT_MINIGAME_BAR=6};
 CGraphicDataBank* Bank;
 class CQuestInfoElementMiniGameBar {
 public:std::vector<CEnginePrimitiveHandle> PrimitiveHandles;float Scale,Percentage;long Graphic;EMiniGameType Type;CRGBColour Colour,FadeColour;
 float DrawAt(const C2DVector&,const CEnginePrimitivePrepareInfo&);
 };
};
}
static CPlayerGuiDef guiDef;
class CPlayerGui {
public:float Alpha;NPlayerGui::CDrawQuestInfo Quest;
 static CPlayerGui& Get();static const CPlayerGuiDef& PeekGuiDef(){return guiDef;}
 NPlayerGui::CDrawQuestInfo& GetQuestInfo(){return Quest;}
 unsigned char GetAlpha()const{return(unsigned char)(255.0f*Alpha);}
 unsigned char ScaleAlpha(unsigned char a)const{return(unsigned char)(a*Alpha);}
};
static CPlayerGui playerGui;CPlayerGui& CPlayerGui::Get(){return playerGui;}
float NPlayerGui::CDrawQuestInfo::CQuestInfoElementMiniGameBar::DrawAt(const C2DVector& pos,const CEnginePrimitivePrepareInfo& info)
{
    const CPlayerGuiDef& gui_def=CPlayerGui::PeekGuiDef();
    PrimitiveHandles.resize(6);
    const CGraphicDataBank& graphic_bank=*CPlayerGui::Get().GetQuestInfo().Bank;
    float left_graphic_width=(float)graphic_bank.GetGraphicFrameWidth(gui_def.HeroSprintBarLeftGraphic,0);
    float left_graphic_height=(float)graphic_bank.GetGraphicFrameHeight(gui_def.HeroSprintBarLeftGraphic,0);
    C2DVector draw_pos(pos);
    float height=Scale*16.0f;
    if(Graphic>0) {
        float sprite_width=(float)graphic_bank.GetGraphicFrameWidth(Graphic,0);
        float sprite_height=(float)graphic_bank.GetGraphicFrameHeight(Graphic,0);
        if(sprite_height>height)sprite_height=height;
        const float& iconX=draw_pos.X-(long)(sprite_width*0.5f);
        const float& iconY=draw_pos.Y-(long)(sprite_height*(1.0f/3.0f));

        CEnginePrimitive2DSprite icon(0,C2DVector(iconX,iconY),ENGINE_2D_LAYER_1,Graphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0,0,CPlayerGui::Get().GetAlpha(),255,255,255,0,C2DVector(0,0),C2DVector(0,0));
        info.Engine->SetPersistentPrimitive(PrimitiveHandles[4],icon,0xc0);
    }
    const float inner_width=48.0f;
    C2DVector left_pos(draw_pos.X,draw_pos.Y-Scale*12.0f);
    const C2DVector& left_end=left_pos+C2DVector(left_graphic_width,left_graphic_height*Scale);
    CEnginePrimitive2DSprite left_bar_sprite(0,C2DBoxF(left_pos,left_end),ENGINE_2D_LAYER_3,gui_def.HeroSprintBarLeftGraphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0,0,CPlayerGui::Get().GetAlpha(),255,255,255,2,C2DVector(0,0),C2DVector(0,0));
    info.Engine->SetPersistentPrimitive(PrimitiveHandles[0],left_bar_sprite,0xc0);
    C2DVector middle_pos(left_pos.X+left_graphic_width,left_pos.Y);
    const C2DVector& middle_end=middle_pos+C2DVector(inner_width,left_graphic_height*Scale);
    CEnginePrimitive2DSprite middle_bar_sprite(0,C2DBoxF(middle_pos,middle_end),ENGINE_2D_LAYER_3,gui_def.HeroSprintBarMiddleGraphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0,0,CPlayerGui::Get().GetAlpha(),255,255,255,2,C2DVector(0,0),C2DVector(0,0));
    info.Engine->SetPersistentPrimitive(PrimitiveHandles[1],middle_bar_sprite,0xc0);
    C2DBoxF right_box(C2DVector(middle_pos.X+inner_width,middle_pos.Y),C2DVector(middle_pos.X+inner_width,middle_pos.Y)+C2DVector(left_graphic_width,left_graphic_height*Scale));
    CEnginePrimitive2DSprite right_bar_sprite(0,right_box,ENGINE_2D_LAYER_3,gui_def.HeroSprintBarRightGraphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0,0,CPlayerGui::Get().GetAlpha(),255,255,255,2,C2DVector(0,0),C2DVector(0,0));
    info.Engine->SetPersistentPrimitive(PrimitiveHandles[2],right_bar_sprite,0xc0);
    float value_as_percentage=Percentage;
    C2DVector inner_pos=left_pos+gui_def.HeroSprintBarInnerGraphicOffset;
    float inner_bar_width=((left_graphic_width-gui_def.HeroSprintBarInnerGraphicOffset.X)*2.0f+inner_width)*value_as_percentage;
    const C2DVector& inner_end=inner_pos+C2DVector(inner_bar_width,left_graphic_height*Scale);
    CRGBColour interp_colour(Colour);
    if(Type==MINIGAME_PICKPOCKET||Type==MINIGAME_PICKLOCK||Type==MINIGAME_STEAL) {
        interp_colour.R=(unsigned char)(Colour.R*value_as_percentage+FadeColour.R*(1.0f-value_as_percentage));
        interp_colour.G=(unsigned char)(Colour.G*value_as_percentage+FadeColour.G*(1.0f-value_as_percentage));
        interp_colour.B=(unsigned char)(FadeColour.B*(1.0f-value_as_percentage)+Colour.B*value_as_percentage);
        interp_colour.A=(unsigned char)(Colour.A*value_as_percentage+FadeColour.A*(1.0f-value_as_percentage));
    }
    CEnginePrimitive2DSprite inner_bar_sprite(0,C2DBoxF(inner_pos,inner_end),ENGINE_2D_LAYER_2,gui_def.HeroSprintBarInnerGraphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0,0,CPlayerGui::Get().ScaleAlpha(interp_colour.A),interp_colour.R,interp_colour.G,interp_colour.B,2,C2DVector(0,0),C2DVector(0,0));
    info.Engine->SetPersistentPrimitive(PrimitiveHandles[3],inner_bar_sprite,0xc0);
    if(Type==MINIGAME_FISHING&&value_as_percentage>DELTA_NOTIONAL_ZERO&&gui_def.MiniGameFishingRightGraphic>0) {
        long graphic=gui_def.MiniGameFishingRightGraphic;
        graphic_bank.GetGraphicFrameWidth(graphic,0);
        float iconHeight=(float)graphic_bank.GetGraphicFrameHeight(graphic,0);
        if(iconHeight>height)iconHeight=height;
        const float& arrowY=draw_pos.Y-(long)(iconHeight*(1.0f/3.0f));
        const float& arrowX=(float)(long)(inner_bar_width+inner_pos.X);
        C2DVector sprite_pos(arrowX,arrowY);
        CEnginePrimitive2DSprite icon(0,sprite_pos,ENGINE_2D_LAYER_1,graphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0,0,CPlayerGui::Get().GetAlpha(),255,255,255,0,C2DVector(0,0),C2DVector(0,0));
        info.Engine->SetPersistentPrimitive(PrimitiveHandles[5],icon,0xc0);
    }
    return height;
}

static void closeEnough(float a,float b){assert(fabs(a-b)<0.00001f);}
static void box(const CEnginePrimitive2DSprite& p,int graphic,int layer,float left,float top,float right,float bottom){assert(p.IsBox&&p.Graphic==graphic&&p.Layer==layer&&p.Flags==2);closeEnough(p.Box.TLX,left);closeEnough(p.Box.TLY,top);closeEnough(p.Box.BRX,right);closeEnough(p.Box.BRY,bottom);}
static void run(int type,float percent,float scale,bool icon,bool arrowGraphic,float alpha){
 CGraphicDataBank bank;CIEngine engine;CEnginePrimitivePrepareInfo info;info.Engine=&engine;
 playerGui.Alpha=alpha;playerGui.Quest.Bank=&bank;
 guiDef.HeroSprintBarLeftGraphic=1;guiDef.HeroSprintBarMiddleGraphic=2;guiDef.HeroSprintBarRightGraphic=3;guiDef.HeroSprintBarInnerGraphic=4;guiDef.MiniGameFishingRightGraphic=arrowGraphic?6:0;guiDef.HeroSprintBarInnerGraphicOffset=C2DVector(2,2);
 NPlayerGui::CDrawQuestInfo::CQuestInfoElementMiniGameBar bar;
 bar.Scale=scale;bar.Percentage=percent;bar.Graphic=icon?5:0;bar.Type=(EMiniGameType)type;bar.Colour=CRGBColour(80,120,160,200);bar.FadeColour=CRGBColour(16,32,48,64);
 bar.PrimitiveHandles.resize(6);for(int i=0;i<6;++i)bar.PrimitiveHandles[i].Index=i;
 float answer=bar.DrawAt(C2DVector(100,200),info);closeEnough(answer,16*scale);
 bool arrow=type==1&&percent>DELTA_NOTIONAL_ZERO&&arrowGraphic;
 assert(engine.Sprites.size()==(unsigned int)(4+(icon?1:0)+(arrow?1:0)));
 unsigned int k=0;
 if(icon){const CEnginePrimitive2DSprite& p=engine.Sprites[k];assert(engine.Handles[k++]==4&&!p.IsBox&&p.Graphic==5&&p.Layer==0x44&&p.Flags==0);float h=24;if(h>16*scale)h=16*scale;closeEnough(p.Pos.X,94);closeEnough(p.Pos.Y,200-(long)(h*(1.0f/3.0f)));assert(p.Colour.A==(unsigned char)(255*alpha)&&p.Colour.R==255&&p.Colour.G==255&&p.Colour.B==255);}
 float top=200-12*scale;
 box(engine.Sprites[k],1,0x46,100,top,108,top+4*scale);assert(engine.Handles[k++]==0);
 box(engine.Sprites[k],2,0x46,108,top,156,top+4*scale);assert(engine.Handles[k++]==1);
 box(engine.Sprites[k],3,0x46,156,top,164,top+4*scale);assert(engine.Handles[k++]==2);
 const CEnginePrimitive2DSprite& fill=engine.Sprites[k];box(fill,4,0x45,102,top+2,102+60*percent,top+2+4*scale);assert(engine.Handles[k++]==3);
 bool fade=type==3||type==4||type==5;
 unsigned char red=fade?(unsigned char)(80*percent+16*(1-percent)):80;
 unsigned char green=fade?(unsigned char)(120*percent+32*(1-percent)):120;
 unsigned char blue=fade?(unsigned char)(160*percent+48*(1-percent)):160;
 unsigned char opacity=fade?(unsigned char)(200*percent+64*(1-percent)):200;
 assert(fill.Colour.R==red&&fill.Colour.G==green&&fill.Colour.B==blue&&fill.Colour.A==(unsigned char)(opacity*alpha));
 if(arrow){const CEnginePrimitive2DSprite& p=engine.Sprites[k];assert(engine.Handles[k++]==5&&!p.IsBox&&p.Graphic==6&&p.Layer==0x44&&p.Flags==0);float h=24;if(h>16*scale)h=16*scale;closeEnough(p.Pos.X,(float)(long)(102+60*percent));closeEnough(p.Pos.Y,200-(long)(h*(1.0f/3.0f)));assert(p.Colour.A==(unsigned char)(255*alpha));}
 assert(bank.Queries.size()==(unsigned int)(2+(icon?2:0)+(arrow?2:0)));assert(bank.Queries[0]==11&&bank.Queries[1]==12);unsigned int q=2;if(icon){assert(bank.Queries[q++]==51);assert(bank.Queries[q++]==52);}if(arrow){assert(bank.Queries[q++]==61);assert(bank.Queries[q++]==62);}
 assert(bar.Scale==scale&&bar.Percentage==percent&&bar.Type==type&&bar.Graphic==(icon?5:0));
}

static NPlayerGui::CDrawQuestInfo::CQuestInfoElementMiniGameBar* changingBar;
static C2DVector* changingPosition;
static void changeDuringRender(int handle) {
 if(handle==4)changingBar->Scale=2;
 if(handle==0){changingBar->Scale=3;*changingPosition=C2DVector(900,999);playerGui.Alpha=0.75f;}
 if(handle==2){changingBar->Percentage=0.25f;guiDef.HeroSprintBarInnerGraphicOffset=C2DVector(4,6);playerGui.Alpha=0.5f;}
 if(handle==3){changingBar->Percentage=0.75f;changingBar->Scale=4;guiDef.HeroSprintBarInnerGraphicOffset=C2DVector(9,9);playerGui.Alpha=0.25f;}
}
static void mutationCase() {
 CGraphicDataBank bank;CIEngine engine;CEnginePrimitivePrepareInfo info;info.Engine=&engine;
 playerGui.Alpha=1;playerGui.Quest.Bank=&bank;
 guiDef.HeroSprintBarLeftGraphic=1;guiDef.HeroSprintBarMiddleGraphic=2;guiDef.HeroSprintBarRightGraphic=3;guiDef.HeroSprintBarInnerGraphic=4;guiDef.MiniGameFishingRightGraphic=6;guiDef.HeroSprintBarInnerGraphicOffset=C2DVector(2,2);
 NPlayerGui::CDrawQuestInfo::CQuestInfoElementMiniGameBar bar;
 bar.Scale=1;bar.Percentage=0.5f;bar.Graphic=5;bar.Type=MINIGAME_FISHING;bar.Colour=CRGBColour(80,120,160,200);bar.FadeColour=CRGBColour(16,32,48,64);
 bar.PrimitiveHandles.resize(6);for(int i=0;i!=6;++i)bar.PrimitiveHandles[i].Index=i;
 C2DVector position(100,200);changingPosition=&position;changingBar=&bar;renderHook=&changeDuringRender;
 float answer=bar.DrawAt(position,info);renderHook=0;
 closeEnough(answer,16);assert(engine.Sprites.size()==6);
 assert(engine.Handles[0]==4&&engine.Handles[1]==0&&engine.Handles[2]==1&&engine.Handles[3]==2&&engine.Handles[4]==3&&engine.Handles[5]==5);
 closeEnough(engine.Sprites[0].Pos.X,94);closeEnough(engine.Sprites[0].Pos.Y,195);
 box(engine.Sprites[1],1,0x46,100,176,108,184);
 box(engine.Sprites[2],2,0x46,108,176,156,188);
 box(engine.Sprites[3],3,0x46,156,176,164,188);
 box(engine.Sprites[4],4,0x45,104,182,118,194);
 closeEnough(engine.Sprites[5].Pos.X,118);closeEnough(engine.Sprites[5].Pos.Y,195);
 assert(engine.Sprites[0].Colour.A==255&&engine.Sprites[1].Colour.A==255&&engine.Sprites[2].Colour.A==191&&engine.Sprites[3].Colour.A==191&&engine.Sprites[4].Colour.A==100&&engine.Sprites[5].Colour.A==63);
 assert(bank.Queries.size()==6&&bank.Queries[0]==11&&bank.Queries[1]==12&&bank.Queries[2]==51&&bank.Queries[3]==52&&bank.Queries[4]==61&&bank.Queries[5]==62);
}

int main(){mutationCase();const int types[]={0,1,2,3,4,5,6,7};const float percentages[]={0,0.25f,0.5f,1};const float scales[]={0.5f,1,2};const float alphas[]={0,0.5f,1};for(int a=0;a<8;++a)for(int b=0;b<4;++b)for(int c=0;c<3;++c)for(int d=0;d<2;++d)for(int e=0;e<2;++e)for(int f=0;f<3;++f)run(types[a],percentages[b],scales[c],d!=0,e!=0,alphas[f]);puts("00651273_TEST PASS");return 0;}
