#include <stdio.h>
#include <string>
static bool failed;static int line_failed;
#define CHECK(x) do{if(!(x)){failed=true;line_failed=__LINE__;return;}}while(0)
static int chars_live,chars_created,chars_destroyed,wide_live,wide_created,wide_destroyed,conversions,primitives,manager_calls,font_calls,pos_calls;static bool soundness;
class C3DVector{ public:float X,Y,Z;C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}C3DVector operator*(float v)const{return C3DVector(X*v,Y*v,Z*v);}C3DVector operator+(const C3DVector&v)const{return C3DVector(X+v.X,Y+v.Y,Z+v.Z);}};
class CWideString{ public:std::string text;bool alive;CWideString(const std::string&s):text(s),alive(true){++wide_live;++wide_created;}CWideString(const CWideString&o):text(o.text),alive(true){soundness=soundness&&o.alive;++wide_live;++wide_created;}~CWideString(){soundness=soundness&&alive;alive=false;--wide_live;++wide_destroyed;}};
class CCharString{ public:std::string text;bool alive;CCharString():alive(true){++chars_live;++chars_created;}CCharString(const CCharString&o):text(o.text),alive(true){soundness=soundness&&o.alive;++chars_live;++chars_created;}~CCharString(){soundness=soundness&&alive;alive=false;--chars_live;++chars_destroyed;}const CCharString&operator+=(const char*s){text+=s;return *this;}bool operator!=(const char*s)const{return text!=s;}CWideString ToWideString()const{soundness=soundness&&alive;++conversions;return CWideString(text);}};
class CRGBColour{ public:unsigned char B,G,R,A;CRGBColour(unsigned char r,unsigned char g,unsigned char b):B(b),G(g),R(r),A(255){}};
class CFontBank{ public:int id;};enum EEngineInterpolationSet{NO_INTERPOLATION=0};enum EEnginePrimitiveType{TEXT=5};
class CEnginePrimitive{ public:EEnginePrimitiveType Type;EEngineInterpolationSet InterpolationSet;unsigned char ChannelIndex0,ChannelIndex1,ChannelBlendFactor;CEnginePrimitive(EEnginePrimitiveType t,unsigned long id,EEngineInterpolationSet i):Type(t),InterpolationSet(i),ChannelIndex0(0),ChannelIndex1(0),ChannelBlendFactor(0){soundness=soundness&&id==0;}};
class CEnginePrimitiveText:public CEnginePrimitive{ public:C3DVector Pos;float Scale;CRGBColour Colour;CWideString Text;const CFontBank*PFont;bool ScaleIn3D;CEnginePrimitiveText(unsigned long id,const C3DVector&pos,float scale,const CWideString&text,const CFontBank&font,const CRGBColour&colour,bool scale_in_3d,EEngineInterpolationSet interpolation_set):CEnginePrimitive((EEnginePrimitiveType)5,id,interpolation_set),Pos(pos),Scale(scale),Colour(colour),Text(text),PFont(&font),ScaleIn3D(scale_in_3d){++primitives;}~CEnginePrimitiveText(){--primitives;}};
static std::string expected_text;static C3DVector expected_pos(0,0,0);static const CFontBank*expected_font;static int added;
class CIEngine{ public:virtual ~CIEngine(){}virtual void AddPrimitive(const CEnginePrimitive&p){const CEnginePrimitiveText&t=static_cast<const CEnginePrimitiveText&>(p);soundness=soundness&&t.Text.alive&&wide_live>=2&&chars_live==1&&primitives==1;++added;if(t.Text.text!=expected_text||t.Pos.X!=expected_pos.X||t.Pos.Y!=expected_pos.Y||t.Pos.Z!=expected_pos.Z||t.Scale!=1.0f||t.Colour.R!=166||t.Colour.G!=166||t.Colour.B!=166||t.Colour.A!=255||t.PFont!=expected_font||t.ScaleIn3D||t.Type!=TEXT||t.InterpolationSet!=NO_INTERPOLATION||t.ChannelIndex0||t.ChannelIndex1||t.ChannelBlendFactor)soundness=false;}};
class CEnginePrimitivePrepareInfo{ public:CIEngine&Engine;CEnginePrimitivePrepareInfo(CIEngine&e):Engine(e){}};
class CThing{ public:C3DVector pos;CThing():pos(0,0,0){}const C3DVector&GetPos()const{++pos_calls;return pos;}};
class CThingManager{ public:static bool DrawPerceptionDebug;bool IsDrawingPerceptionDebug()const{return DrawPerceptionDebug;}};bool CThingManager::DrawPerceptionDebug;
class CMainGameComponent{ public:CFontBank*font;const CFontBank&PeekDebugFont()const{++font_calls;return *font;}};
class CTCStealth{ public:bool Viewable,Hearable,Smellable,Sneaking;CThing*ParentThing;CThingManager*manager;CMainGameComponent*component;CThingManager&GetThingManager(){++manager_calls;return *manager;}CThing&GetParentThing(){return *ParentThing;}const CMainGameComponent&PeekComponent()const{return *component;}bool IsViewable()const{return Viewable;}bool IsHearable()const{return Hearable;}bool IsSmellable()const{return Smellable;}bool IsSneaking()const{return Sneaking;}void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);};
void CTCStealth::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&info)
{
 if(GetThingManager().IsDrawingPerceptionDebug()) {
  C3DVector up(0.0f,0.0f,1.0f);
  CCharString debug_text;
  if(IsViewable())debug_text+="V ";
  if(IsHearable())debug_text+="H ";
  if(IsSmellable())debug_text+="S ";
  if(IsSneaking())debug_text+="Sn ";
  if(debug_text!="")
   info.Engine.AddPrimitive(CEnginePrimitiveText(0,GetParentThing().GetPos()+up*3.0f,1.0f,debug_text.ToWideString(),PeekComponent().PeekDebugFont(),CRGBColour(0xa6,0xa6,0xa6),false,(EEngineInterpolationSet)0));
 }
}


static void one(int enabled,int mask,int position,int font_id)
{
 chars_live=chars_created=chars_destroyed=wide_live=wide_created=wide_destroyed=conversions=primitives=manager_calls=font_calls=pos_calls=added=0;soundness=true;
 CThing parent;const float values[]={-32.0f,-0.5f,0.0f,128.0f};parent.pos=C3DVector(values[position],values[3-position],values[(position+1)%4]);expected_pos=C3DVector(parent.pos.X,parent.pos.Y,parent.pos.Z+3.0f);CFontBank font;font.id=font_id;expected_font=&font;CThingManager manager;CMainGameComponent component;component.font=&font;CIEngine engine;CEnginePrimitivePrepareInfo info(engine);
 CTCStealth c;c.Viewable=(mask&1)!=0;c.Hearable=(mask&2)!=0;c.Smellable=(mask&4)!=0;c.Sneaking=(mask&8)!=0;c.ParentThing=&parent;c.manager=&manager;c.component=&component;CThingManager::DrawPerceptionDebug=enabled!=0;expected_text="";const char*labels[]={"V ","H ","S ","Sn "};for(int i=0;i<4;++i)if(mask&(1<<i))expected_text+=labels[i];bool draw=enabled&&mask;
 c.DrawPreparePrimitivesForRendering(info);CHECK(soundness);CHECK(manager_calls==1);CHECK(added==(draw?1:0));CHECK(conversions==(draw?1:0));CHECK(font_calls==(draw?1:0)&&pos_calls==(draw?1:0));CHECK(chars_created==(enabled?1:0)&&chars_destroyed==chars_created&&chars_live==0);CHECK(wide_live==0&&wide_created==wide_destroyed&&primitives==0);CHECK(font.id==font_id&&parent.pos.X==values[position]&&parent.pos.Y==values[3-position]&&parent.pos.Z==values[(position+1)%4]);CHECK(CThingManager::DrawPerceptionDebug==bool(enabled));CHECK(c.Viewable==bool(mask&1)&&c.Hearable==bool(mask&2)&&c.Smellable==bool(mask&4)&&c.Sneaking==bool(mask&8));
}
int main(){int cases=0;for(int e=0;e<2;++e)for(int m=0;m<16;++m)for(int p=0;p<4;++p)for(int f=0;f<4;++f){one(e,m,p,f);if(failed){printf("007471f0_TEST FAIL line%d case%d\n",line_failed,cases);return 1;}++cases;}if(cases!=512)return 2;puts("007471f0_TEST PASS");return 0;}