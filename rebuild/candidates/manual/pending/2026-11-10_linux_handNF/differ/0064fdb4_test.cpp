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
enum EEngine2DLayer { ENGINE_2D_LAYER_5=73, ENGINE_2D_LAYER_6=74 };enum EEnginePrimitiveBlendMode { ENGINE_PRIMITIVE_BLEND_ALPHA=2 };enum EEngineInterpolationSet { ENGINE_INTERPOLATION_SET_DEFAULT=0 };
const float DELTA_NOTIONAL_ZERO=0.0001f;
class CEnginePrimitiveHandle {public:int Index,Clears;CEnginePrimitiveHandle():Index(-1),Clears(0){} void Clear(){++Clears;} };
class CEnginePrimitive2DSprite {
public:C2DBoxF Box;C2DVector Pos;bool IsBox;long Graphic;int Layer;CRGBColour Colour;unsigned long Flags;
 CEnginePrimitive2DSprite(int,const C2DVector& p,EEngine2DLayer layer,unsigned long graphic,unsigned long frame,EEnginePrimitiveBlendMode blend,EEngineInterpolationSet interpolation,float a,float b,unsigned char alpha,unsigned char red,unsigned char green,unsigned char blue,unsigned long flags,const C2DVector& u,const C2DVector& v):Pos(p),IsBox(false),Graphic(graphic),Layer(layer),Colour(red,green,blue,alpha),Flags(flags){check(frame,blend,interpolation,a,b,u,v);}
 CEnginePrimitive2DSprite(int,const C2DBoxF& box,EEngine2DLayer layer,unsigned long graphic,unsigned long frame,EEnginePrimitiveBlendMode blend,EEngineInterpolationSet interpolation,float a,float b,unsigned char alpha,unsigned char red,unsigned char green,unsigned char blue,unsigned long flags,const C2DVector& u,const C2DVector& v):Box(box),Pos(0,0),IsBox(true),Graphic(graphic),Layer(layer),Colour(red,green,blue,alpha),Flags(flags){check(frame,blend,interpolation,a,b,u,v);}
 static void check(unsigned long f,EEnginePrimitiveBlendMode b,EEngineInterpolationSet i,float x,float y,const C2DVector& u,const C2DVector& v){assert(f==0&&b==2&&i==0&&x==0&&y==0&&u.X==0&&u.Y==0&&v.X==0&&v.Y==0);}
};
class CIEngine {
public:std::vector<CEnginePrimitive2DSprite> Sprites;std::vector<int> Handles;
 void SetPersistentPrimitive(CEnginePrimitiveHandle& h,const CEnginePrimitive2DSprite& p,unsigned long f){assert(f==0xc0);Sprites.push_back(p);Handles.push_back(h.Index);}
};
class CEnginePrimitivePrepareInfo {public:CIEngine* Engine;};

#include <string.h>
#ifdef _MSC_VER
namespace std { template<class T> inline const T& max(const T& a,const T& b){return a<b?b:a;} }
#endif
class CGraphicDataBank {
public:mutable std::vector<int> Queries;
 unsigned long GetGraphicMaxWidth(unsigned long n)const{Queries.push_back((int)n*10+1);return n==5?12:n==6?16:8;}
 unsigned long GetGraphicMaxHeight(unsigned long n)const{Queries.push_back((int)n*10+2);return n==5?24:n==6?40:4;}
};
class CBaseObject {public:bool Dead;CBaseObject():Dead(false){}bool IsDead()const{return Dead;}};
class CThingPhysical:public CBaseObject {public:bool Limbo;CThingPhysical():Limbo(false){}bool IsInLimbo()const{return Limbo;}};
template<class T>class CIntelligentPointer {public:T* Item;CIntelligentPointer():Item(0){}T* operator->()const{return Item;}};
template<class T>bool IS_THING_ALIVE(T* const& p){return p&&!p->IsDead();}
template<class T>bool IS_THING_ALIVE(const CIntelligentPointer<T>& p){return p.Item&&!p.Item->IsDead();}
template<class T>bool operator==(const CBaseObject* p,const CIntelligentPointer<T>& v){return p==v.Item;}
class CTCTargetingBase {public:CThingPhysical* Target;CTCTargetingBase():Target(0){}CThingPhysical* GetPTargetedThing()const{return Target;}};
class CThingPlayerCreature:public CBaseObject {public:CTCTargetingBase Targeting;void PeekTC(const CTCTargetingBase** p)const{*p=&Targeting;}};
class CPlayer {public:CThingPlayerCreature* Hero;CPlayer():Hero(0){}CThingPlayerCreature* GetPControlledCreature()const{return Hero;}};
class CPlayerManager {public:CPlayer Player;CPlayer& GetMainPlayer(){return Player;}};
class CMainGameComponent {public:CPlayerManager Manager;static CMainGameComponent* Get();CPlayerManager& GetPlayerManager(){return Manager;}};
static CMainGameComponent game;CMainGameComponent* CMainGameComponent::Get(){return &game;}
class CPlayerGuiDef {public:long HeroSprintBarLeftGraphic,HeroSprintBarMiddleGraphic,HeroSprintBarRightGraphic,HeroSprintBarInnerGraphic;long QuestInfoGraphicWidth;};
namespace NPlayerGui {
class CDrawQuestInfo {public:class CQuestInfoElementBar {
public:std::vector<CEnginePrimitiveHandle> PrimitiveHandles;float Scale,Value,MaxValue,MinValue;CRGBColour Colour,FadeColour;long Graphic1,Graphic2;bool ThingValid;CIntelligentPointer<const CThingPhysical> PThing;
float DrawAt(const C2DVector&,const CEnginePrimitivePrepareInfo&);
};};}
static CPlayerGuiDef guiDef;
class CPlayerGui {public:float Alpha;CGraphicDataBank* Bank;static CPlayerGui& Get();static const CPlayerGuiDef& PeekGuiDef(){return guiDef;}const CGraphicDataBank& PeekGraphicBank()const{return *Bank;}unsigned char GetAlpha()const{return(unsigned char)(255.0f*Alpha);}unsigned char ScaleAlpha(unsigned char a)const{return(unsigned char)(a*Alpha);}};
static CPlayerGui playerGui;CPlayerGui& CPlayerGui::Get(){return playerGui;}
// Exact portable twin of the existing shared rounding routine, nearest-even integer store and bit equality fixup.
inline long X87FistpNear(double v){
 if(!(v>=-2147483648.5&&v<2147483647.5))return -2147483647L-1L;
 long t=(long)v;double r=v-(double)t;
 if(r>0.5)++t;else if(r< -0.5)--t;else if((r==0.5||r== -0.5)&&(t&1L)!=0)t+=(r>0.0)?1L:-1L;
 return t;
}
inline unsigned int FloatBits(float f){unsigned int u;memcpy(&u,&f,sizeof u);return u;}
inline long GFFloatToLongFloor(float value){long result=X87FistpNear((double)value-0.5);float next=(float)((double)result+1.0);if(FloatBits(value)==FloatBits(next))result=(long)(int)((unsigned int)result+1u);return result;}
float NPlayerGui::CDrawQuestInfo::CQuestInfoElementBar::DrawAt(const C2DVector& position,const CEnginePrimitivePrepareInfo& info)
{
    if(ThingValid && (!IS_THING_ALIVE(PThing) || PThing->IsInLimbo()))return 0.0f;
    const CThingPlayerCreature* hero=CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
    if(IS_THING_ALIVE(hero)) {
#ifdef _MSC_VER
#pragma inline_depth()
#endif
    const CPlayerGuiDef& def=CPlayerGui::PeekGuiDef();
    PrimitiveHandles.resize(6);
    const CGraphicDataBank& bank=CPlayerGui::Get().PeekGraphicBank();
    float width=(float)bank.GetGraphicMaxWidth(def.HeroSprintBarLeftGraphic);
    float height=(float)bank.GetGraphicMaxHeight(def.HeroSprintBarLeftGraphic);
    float maxHeight=height;
    float iconWidth=0.0f,iconHeight=0.0f;
    if(Graphic1>0) {
        iconWidth=(float)bank.GetGraphicMaxWidth(Graphic1);
        iconHeight=(float)bank.GetGraphicMaxHeight(Graphic1);
        maxHeight=std::max(maxHeight,iconHeight);
    }
    float otherWidth=0.0f;
    if(Graphic2>0) {
        otherWidth=(float)bank.GetGraphicMaxWidth(Graphic2);
        float otherHeight=(float)bank.GetGraphicMaxHeight(Graphic2);
        maxHeight=std::max(maxHeight,otherHeight);
    }
    float available=def.QuestInfoGraphicWidth-6.0f;
    C2DVector iconPos(position),barPos(position),fillPos(position);
    if(Graphic1>0) {
        available-=GFFloatToLongFloor(iconWidth*Scale);
        barPos.X+=GFFloatToLongFloor(iconWidth*Scale);
        float drawWidth=iconWidth,drawHeight=iconHeight;
        EEngine2DLayer layer=ENGINE_2D_LAYER_6;
        if(IS_THING_ALIVE(PThing)) {
            const CTCTargetingBase* targeting;
            hero->PeekTC(&targeting);
            if(targeting->GetPTargetedThing()==PThing) {
                layer=ENGINE_2D_LAYER_5;
                drawWidth*=1.2f;drawHeight*=1.2f;
                iconPos.X-=drawWidth*0.1f;iconPos.Y-=drawHeight*0.1f;
            }
        }
        C2DVector size((float)GFFloatToLongFloor(drawWidth*Scale),(float)GFFloatToLongFloor(drawHeight*Scale));
        C2DVector end1=iconPos+size;
        CEnginePrimitive2DSprite sprite(0,C2DBoxF(iconPos,end1),layer,Graphic1,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0,0,CPlayerGui::Get().GetAlpha(),255,255,255,2,C2DVector(0,0),C2DVector(0,0));
        info.Engine->SetPersistentPrimitive(PrimitiveHandles[0],sprite,0xc0);
    }
    if(Graphic2>0) {
        available-=GFFloatToLongFloor(otherWidth*Scale);
        fillPos.X+=GFFloatToLongFloor(iconWidth*Scale)+available;
        C2DVector size((float)GFFloatToLongFloor(otherWidth*Scale),(float)GFFloatToLongFloor(iconHeight*Scale));
        C2DVector end2=fillPos+size;
        CEnginePrimitive2DSprite sprite(0,C2DBoxF(fillPos,end2),ENGINE_2D_LAYER_6,Graphic2,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0,0,CPlayerGui::Get().GetAlpha(),255,255,255,2,C2DVector(0,0),C2DVector(0,0));
        info.Engine->SetPersistentPrimitive(PrimitiveHandles[1],sprite,0xc0);
    }
    fillPos.X=barPos.X;
    C2DVector leftSize(width,(float)GFFloatToLongFloor(height*Scale));
    C2DVector end3=barPos+leftSize;
    CEnginePrimitive2DSprite leftSprite(0,C2DBoxF(barPos,end3),ENGINE_2D_LAYER_6,def.HeroSprintBarLeftGraphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0,0,CPlayerGui::Get().GetAlpha(),255,255,255,2,C2DVector(0,0),C2DVector(0,0));
    info.Engine->SetPersistentPrimitive(PrimitiveHandles[2],leftSprite,0xc0);
    barPos.X+=width;
    float middleWidth;
    {
        C2DVector size(available-width*2.0f,(float)GFFloatToLongFloor(height*Scale));
        C2DVector end(barPos.X+size.X,barPos.Y+size.Y);
        middleWidth=end.X-barPos.X;
        if(middleWidth>DELTA_NOTIONAL_ZERO) {
            CEnginePrimitive2DSprite sprite(0,C2DBoxF(barPos,end),ENGINE_2D_LAYER_6,def.HeroSprintBarMiddleGraphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0,0,CPlayerGui::Get().GetAlpha(),255,255,255,2,C2DVector(0,0),C2DVector(0,0));
            info.Engine->SetPersistentPrimitive(PrimitiveHandles[3],sprite,0xc0);
        }
    }
    barPos.X+=middleWidth;
    C2DVector rightSize(width,(float)GFFloatToLongFloor(height*Scale));
    C2DVector end4=barPos+rightSize;
    CEnginePrimitive2DSprite rightSprite(0,C2DBoxF(barPos,end4),ENGINE_2D_LAYER_6,def.HeroSprintBarRightGraphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0,0,CPlayerGui::Get().GetAlpha(),255,255,255,2,C2DVector(0,0),C2DVector(0,0));
    info.Engine->SetPersistentPrimitive(PrimitiveHandles[4],rightSprite,0xc0);
    float percent=Value/(MaxValue-MinValue);
    if(percent>DELTA_NOTIONAL_ZERO) {
        CRGBColour colour((unsigned char)(Colour.R*percent+FadeColour.R*(1.0f-percent)),(unsigned char)(Colour.G*percent+FadeColour.G*(1.0f-percent)),(unsigned char)(Colour.B*percent+FadeColour.B*(1.0f-percent)),(unsigned char)(Colour.A*percent+FadeColour.A*(1.0f-percent)));
        fillPos.X+=4.0f;
        C2DVector size((available-8.0f)*percent,height*Scale);
        C2DVector end5=fillPos+size;
        CEnginePrimitive2DSprite sprite(0,C2DBoxF(fillPos,end5),ENGINE_2D_LAYER_5,def.HeroSprintBarInnerGraphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0,0,CPlayerGui::Get().ScaleAlpha(colour.A),colour.R,colour.G,colour.B,2,C2DVector(0,0),C2DVector(0,0));
        info.Engine->SetPersistentPrimitive(PrimitiveHandles[5],sprite,0xc0);
    } else PrimitiveHandles[5].Clear();
    return maxHeight*Scale+2.0f;
    }
    return 0.0f;
}

static void nearValue(float x,float y){assert(fabs(x-y)<0.0001f);}
static void checkBox(const CEnginePrimitive2DSprite& p,int graphic,int layer,float x,float y,float w,float h){assert(p.IsBox&&p.Graphic==graphic&&p.Layer==layer&&p.Flags==2);nearValue(p.Box.TLX,x);nearValue(p.Box.TLY,y);nearValue(p.Box.BRX,x+w);nearValue(p.Box.BRY,y+h);}
static void run(bool icon,bool other,float scale,float value,float alpha,bool target,int invalid,long definitionWidth=96){
 CGraphicDataBank bank;CIEngine engine;CEnginePrimitivePrepareInfo info;info.Engine=&engine;playerGui.Bank=&bank;playerGui.Alpha=alpha;
 CThingPlayerCreature hero;CThingPhysical thing;game.Manager.Player.Hero=&hero;hero.Targeting.Target=target?&thing:0;
 guiDef.HeroSprintBarLeftGraphic=1;guiDef.HeroSprintBarMiddleGraphic=2;guiDef.HeroSprintBarRightGraphic=3;guiDef.HeroSprintBarInnerGraphic=4;guiDef.QuestInfoGraphicWidth=definitionWidth;
 NPlayerGui::CDrawQuestInfo::CQuestInfoElementBar bar;bar.Scale=scale;bar.Value=value;bar.MinValue=2;bar.MaxValue=6;bar.Colour=CRGBColour(80,120,160,200);bar.FadeColour=CRGBColour(16,32,48,64);bar.Graphic1=icon?5:0;bar.Graphic2=other?6:0;bar.ThingValid=false;bar.PThing.Item=&thing;
 if(invalid==1){bar.ThingValid=true;bar.PThing.Item=0;}if(invalid==2){bar.ThingValid=true;thing.Dead=true;}if(invalid==3){bar.ThingValid=true;thing.Limbo=true;}if(invalid==4)game.Manager.Player.Hero=0;if(invalid==5)hero.Dead=true;
 bar.PrimitiveHandles.resize(6);for(int n=0;n<6;++n)bar.PrimitiveHandles[n].Index=n;
 bar.PrimitiveHandles[3].Clears=37;
 const int middleIndex=bar.PrimitiveHandles[3].Index,middleClears=bar.PrimitiveHandles[3].Clears;
 float result=bar.DrawAt(C2DVector(100,200),info);
 if(invalid){assert(result==0&&engine.Sprites.empty()&&bank.Queries.empty());return;}
 nearValue(result,(other?40.0f:icon?24.0f:4.0f)*scale+2.0f);
 float percent=value/4.0f;bool fill=percent>DELTA_NOTIONAL_ZERO;
 float firstW=icon?12.0f:0.0f,firstH=icon?24.0f:0.0f;
 float available=(float)definitionWidth-6.0f-(icon?(float)floor(firstW*scale):0)-(other?(float)floor(16*scale):0);
 float left=100+(icon?(float)floor(firstW*scale):0);
 // Inputs here keep every endpoint exact; retain signed middle and fill extents.
 const float middleWidth=available-16.0f;
 const bool middle=middleWidth>DELTA_NOTIONAL_ZERO;
 assert(engine.Sprites.size()==(unsigned int)(2+(middle?1:0)+(icon?1:0)+(other?1:0)+(fill?1:0)));
 unsigned int k=0;
 if(icon){float w=firstW,h=firstH,x=100,y=200;int layer=0x4a;if(target){w*=1.2f;h*=1.2f;x-=w*0.1f;y-=h*0.1f;layer=0x49;}checkBox(engine.Sprites[k],5,layer,x,y,(float)floor(w*scale),(float)floor(h*scale));assert(engine.Handles[k++]==0);}
 if(other){checkBox(engine.Sprites[k],6,0x4a,100+(float)floor(firstW*scale)+available,200,(float)floor(16*scale),(float)floor(firstH*scale));assert(engine.Handles[k++]==1);}
 float height=(float)floor(4*scale);
 checkBox(engine.Sprites[k],1,0x4a,left,200,8,height);assert(engine.Handles[k++]==2);
 if(middle){checkBox(engine.Sprites[k],2,0x4a,left+8,200,middleWidth,height);assert(engine.Handles[k++]==3);}
 else {for(unsigned int skipped=0;skipped<engine.Handles.size();++skipped)assert(engine.Handles[skipped]!=3);}
 assert(bar.PrimitiveHandles[3].Index==middleIndex&&bar.PrimitiveHandles[3].Clears==middleClears);
 // Native advances by middleWidth even when it skipped the middle sprite.
 checkBox(engine.Sprites[k],3,0x4a,left+available-8,200,8,height);assert(engine.Handles[k++]==4);
 if(fill){const CEnginePrimitive2DSprite& p=engine.Sprites[k];checkBox(p,4,0x49,left+4,200,(available-8)*percent,4*scale);assert(engine.Handles[k++]==5);assert(p.Colour.R==(unsigned char)(80*percent+16*(1-percent)));assert(p.Colour.G==(unsigned char)(120*percent+32*(1-percent)));assert(p.Colour.B==(unsigned char)(160*percent+48*(1-percent)));assert(p.Colour.A==(unsigned char)((unsigned char)(200*percent+64*(1-percent))*alpha));assert(bar.PrimitiveHandles[5].Clears==0);}else assert(bar.PrimitiveHandles[5].Clears==1);
 for(unsigned int q=0;q<engine.Sprites.size()-(fill?1:0);++q){const CEnginePrimitive2DSprite& p=engine.Sprites[q];assert(p.Colour.A==(unsigned char)(255*alpha)&&p.Colour.R==255&&p.Colour.G==255&&p.Colour.B==255);}
 assert(bank.Queries.size()==(unsigned int)(2+(icon?2:0)+(other?2:0)));assert(bank.Queries[0]==11&&bank.Queries[1]==12);unsigned int queryIndex=2;if(icon){assert(bank.Queries[queryIndex++]==51);assert(bank.Queries[queryIndex++]==52);}if(other){assert(bank.Queries[queryIndex++]==61);assert(bank.Queries[queryIndex++]==62);}
}
int main(){const float scales[]={0.5f,1,2};const float values[]={-2,0,1,4};const float alphas[]={0,0.5f,1};for(int a=0;a<2;++a)for(int b=0;b<2;++b)for(int c=0;c<3;++c)for(int d=0;d<4;++d)for(int e=0;e<3;++e)for(int f=0;f<2;++f)run(a!=0,b!=0,scales[c],values[d],alphas[e],f!=0,0);for(int n=1;n<=5;++n)run(true,true,1,1,1,true,n);
 unsigned int boundaryCases=0;
 // At scale1 these widths give exact middle extents -1,0,+1 for each icon pair.
 for(int first=0;first!=2;++first)for(int second=0;second!=2;++second)
  for(int offset=-1;offset<=1;++offset)for(int valueCase=0;valueCase!=2;++valueCase){
   const long definitionWidth=22+(first?12:0)+(second?16:0)+offset;
   run(first!=0,second!=0,1.0f,valueCase?1.0f:0.0f,1.0f,false,0,definitionWidth);
   ++boundaryCases;
  }
 // Preserve a backwards fill rectangle: available7, middle-9, fill width-0.25.
 run(false,false,1.0f,1.0f,1.0f,false,0,13);++boundaryCases;
 assert(boundaryCases==25);
 for(int n=-40;n<=40;++n)nearValue((float)GFFloatToLongFloor(n*0.25f),(float)floor(n*0.25f));puts("0064fdb4_TEST PASS");return 0;}
