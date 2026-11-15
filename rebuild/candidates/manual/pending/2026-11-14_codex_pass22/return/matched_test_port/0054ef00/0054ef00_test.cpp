#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include <float.h>
#include "rebuild_abi.h"
#include "fable_compiler.h"
#ifndef _MSC_VER
#define __declspec(x)
#define FABLE_PORTABLE
#endif
#define FABLE_DUAL_PATH_TEST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_CTOR4_INITIALISE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define FABLE_RETAIL_CTEXT_DRAW_PRIMITIVE_BODIES
// Fixture enum ranges retain native integer contracts without inventing game enumerator names.
enum EEnginePrimitiveType{FixturePrimitive2DText=0x27};
enum EEngineInterpolationSet{FixtureInterpolationDefault=0};
enum EEngine2DLayer{FixtureLayerMin=-4096,FixtureLayerMax=4096};
int FixtureStringLive=0;
#define private public
#define protected public
#include "engine/C2DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitive2DText.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CCountedPointer.h"
#include "engine/GFFloatToLongNear.h"
#undef private
#undef protected
static int FontLive=0,PrimitiveLive=0,Notifications=0,Deletions=0;
class CFontBank{public:int ID;CFontBank():ID(73){++FontLive;}~CFontBank(){--FontLive;}};
CBaseClass::CBaseClass(){}
CBaseClass::~CBaseClass(){}
void CIVCountedPointeeBase::Delete()const{delete this;}
CEnginePersistentPrimitiveBase::CEnginePersistentPrimitiveBase(long type):Type(type){}
CEnginePersistentPrimitiveBase::~CEnginePersistentPrimitiveBase(){}
void CEnginePersistentPrimitiveBase::NotifyReleased(const CEnginePrimitiveHandle&){}
bool CEnginePersistentPrimitiveBase::WasRendered()const{return true;}
class FixturePersistent:public CEnginePersistentPrimitiveBase {public:FixturePersistent():CEnginePersistentPrimitiveBase(39){++PrimitiveLive;}~FixturePersistent(){--PrimitiveLive;}virtual void CollectStats(std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<const unsigned long,unsigned long> > >&,EEnginePrimitiveStatIndex,unsigned long)const{}virtual void NotifyReleased(const CEnginePrimitiveHandle&){++Notifications;}virtual void Delete()const{++Deletions;delete this;}};
static unsigned Bits(float v){unsigned x;assert(sizeof x==sizeof v);memcpy(&x,&v,4);return x;}
struct Call {char Kind;unsigned Index,Flags;float X,Y,Z,Scale;unsigned Colour;long Layer;unsigned Text;int Font;};
static Call Calls[2];static int CallCount=0,LayerCalls=0;static long Layers[2];static CEnginePrimitiveHandle*ExpectedParent;static CEnginePrimitiveHandle*ExpectedOwn[2];
static unsigned ColourBits(const CRGBColour&c){return unsigned(c.B)|(unsigned(c.G)<<8)|(unsigned(c.R)<<16)|(unsigned(c.A)<<24);}
class CIEngine {public:virtual~CIEngine(){}virtual void SetPersistentPrimitive(CEnginePrimitiveHandle&h,const CEnginePrimitive&p,unsigned long flags){assert(CallCount<2&&&h==ExpectedOwn[CallCount]);Capture('P',0,p,flags);}virtual void SetChildPrimitive(CEnginePrimitiveHandle&h,unsigned long index,const CEnginePrimitive&p,unsigned long flags){assert(&h==ExpectedParent);Capture('D',(unsigned)index,p,flags);}virtual void ClearChildPrimitive(CEnginePrimitiveHandle&h,unsigned long index){assert(&h==ExpectedParent&&CallCount<2);Call c;memset(&c,0,sizeof c);c.Kind='C';c.Index=(unsigned)index;Calls[CallCount++]=c;}void Capture(char kind,unsigned index,const CEnginePrimitive&p,unsigned long flags){assert(CallCount<2);const CEnginePrimitive2DText&t=static_cast<const CEnginePrimitive2DText&>(p);assert(t.Type==FixturePrimitive2DText&&t.InterpolationSet==FixtureInterpolationDefault);assert(t.ThemeChannelDesc.ChannelIndex0==0&&t.ThemeChannelDesc.ChannelIndex1==0&&t.ThemeChannelDesc.ChannelBlendFactor==0);Call c;memset(&c,0,sizeof c);c.Kind=kind;c.Index=index;c.Flags=(unsigned)flags;c.X=t.Pos.X;c.Y=t.Pos.Y;c.Z=t.Pos.Z;c.Scale=t.Scale;c.Colour=ColourBits(t.Colour);c.Layer=(long)t.Layer;c.Text=t.Text.Text[0];c.Font=t.PFont->ID;Calls[CallCount++]=c;}};
namespace NUISystem {
enum ETextAlignement{LEFT,CENTER,RIGHT};
class CComponent{};
class CManager {public:static CManager*GetInstance();virtual EEngine2DLayer GetLayer(long layer){assert(LayerCalls<2);Layers[LayerCalls++]=layer;return (EEngine2DLayer)(1000+layer);}};
static CManager Manager;CManager*CManager::GetInstance(){return &Manager;}
}
#define private public
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_ORIGINAL_DIMENSIONS
#include "engine/NUISystem_CUIScreenUnitConverter.h"
#undef private
namespace NUISystem {
bool CUIScreenUnitConverter::IsUsingRelativeCoords=false;
float CUIScreenUnitConverter::BackBufferWidth=128.0f;
float CUIScreenUnitConverter::BackBufferHeight=96.0f;
float CUIScreenUnitConverter::DefaultBackBufferWidth=640.0f;
float CUIScreenUnitConverter::DefaultBackBufferHeight=480.0f;
static const unsigned short FixtureValueText[2]={65,0};
class CText:public CComponent {public:CRGBColour RenderColour;C2DVector RenderZoom,RelativeRenderPosition,RelativeRenderZoom,Size;bool Independent,LayerIndependent;ETextAlignement Align;signed char Layer;CWideString Value;CCountedPointer<CFontBank>PFont;CEnginePrimitiveHandle Handle,HandleDropShadowBottomRight;bool ScaleText,Unavailable;unsigned char NoRenderNextFrame;CText():PFont(new CFontBank){Value.Text=FixtureValueText;}virtual bool IsIndependent()const{return Independent;}virtual bool IsLayerIndependent()const{return LayerIndependent;}virtual ETextAlignement GetAlignment()const{return Align;}virtual void Draw(CIEngine&,CEnginePrimitiveHandle*,long,long*,CComponent*);};
}

// Original complete descriptor bodies; native114-byte ctor and91-byte assignment are audited separately.
CEnginePrimitive2DText::CEnginePrimitive2DText(unsigned __int64 id, const C2DVector& position, EEngine2DLayer layer, float scale, const CWideString& text, const CFontBank& font, const CRGBColour& colour, EEngineInterpolationSet interpolation_set)
    : CEnginePrimitive((EEnginePrimitiveType)0x27, id, interpolation_set), Pos(position.X,position.Y,0.0f), Layer(layer), Scale(scale), Colour(colour), Text(text), PFont(&font)
{
}
CEnginePrimitive2DText& CEnginePrimitive2DText::operator=(const CEnginePrimitive2DText& other)
{
    CEnginePrimitive::operator=(other);
    Pos=other.Pos;
    Layer=other.Layer;
    Scale=other.Scale;
    Colour=other.Colour;
    Text=other.Text;
    PFont=other.PFont;
    return *this;
}

namespace NUISystem {
void CText::Draw(CIEngine& engine, CEnginePrimitiveHandle* p_handle, long layer, long* pprimitive_index, CComponent* pparent)
{
    if (RenderColour.A <= 0 || RenderZoom.X <= 0.0f) {
        if (NoRenderNextFrame == 1) {
    if (p_handle) {
        engine.ClearChildPrimitive(*p_handle, (*pprimitive_index)++);
        engine.ClearChildPrimitive(*p_handle, (*pprimitive_index)++);
    } else {
        Handle.Clear();
        HandleDropShadowBottomRight.Clear();
    }
            return;
        }
        NoRenderNextFrame = 1;
    } else
        NoRenderNextFrame = 0;
    CRGBColour colour = RenderColour;
    C2DVector render_position = RelativeRenderPosition;
    float render_zoom_x = ScaleText ? RelativeRenderZoom.X : RenderZoom.X;
    if (render_position.X >= CUIScreenUnitConverter::GetBackBufferWidth() || render_position.Y >= CUIScreenUnitConverter::GetBackBufferHeight()) {
    if (p_handle) {
        engine.ClearChildPrimitive(*p_handle, (*pprimitive_index)++);
        engine.ClearChildPrimitive(*p_handle, (*pprimitive_index)++);
    } else {
        Handle.Clear();
        HandleDropShadowBottomRight.Clear();
    }
        return;
    }
        bool is_independent = IsIndependent();
        bool is_layerindependent = IsLayerIndependent();
        long render_layer;
        if (!is_independent && !is_layerindependent)
            render_layer = Layer + layer;
        else
            render_layer = Layer;
        CRGBColour black(0, 0, 0, colour.A);
        CEnginePrimitive2DText primitive;
        CEnginePrimitive2DText primitive_bottom_right;
        switch (GetAlignment()) {
        case CENTER:
            render_position.X -= render_zoom_x * Size.X * 0.5f;
            break;
        case RIGHT:
            render_position.X -= render_zoom_x * Size.X;
            break;
        }
        render_position.X = (float)GFFloatToLongNear(render_position.X);
        render_position.Y = (float)GFFloatToLongNear(render_position.Y);
        if (Unavailable) {
            primitive = CEnginePrimitive2DText(0, render_position, CManager::GetInstance()->GetLayer(render_layer), render_zoom_x, Value, *PFont, black, (EEngineInterpolationSet)0);
            C2DVector position(render_position.X + 2.0f, render_position.Y + 2.0f);
            primitive_bottom_right = CEnginePrimitive2DText(0, position, CManager::GetInstance()->GetLayer(render_layer + 1), render_zoom_x, Value, *PFont, colour, (EEngineInterpolationSet)0);
        } else {
            primitive = CEnginePrimitive2DText(0, render_position, CManager::GetInstance()->GetLayer(render_layer), render_zoom_x, Value, *PFont, colour, (EEngineInterpolationSet)0);
            C2DVector position(render_position.X + 2.0f, render_position.Y + 2.0f);
            primitive_bottom_right = CEnginePrimitive2DText(0, position, CManager::GetInstance()->GetLayer(render_layer + 1), render_zoom_x, Value, *PFont, black, (EEngineInterpolationSet)0);
        }
        if (p_handle && pprimitive_index) {
            engine.SetChildPrimitive(*p_handle, (*pprimitive_index)++, primitive, 0x40);
            engine.SetChildPrimitive(*p_handle, (*pprimitive_index)++, primitive_bottom_right, 0x40);
        } else {
            engine.SetPersistentPrimitive(Handle, primitive, 0x40);
            engine.SetPersistentPrimitive(HandleDropShadowBottomRight, primitive_bottom_right, 0x40);
        }
    return;
}
}

static long OracleRound(float value){unsigned bits=Bits(value),sign=bits>>31;int exponent=int((bits>>23)&255)-127;if(exponent< -1)return 0;if(exponent>31)return -2147483647L-1;unsigned mantissa=(bits&0x7fffffU)|0x800000U,magnitude;if(exponent>=23)magnitude=mantissa<<(exponent-23);else{unsigned shift=23-exponent,remainder=mantissa&((1U<<shift)-1),half=1U<<(shift-1);magnitude=mantissa>>shift;if(remainder>half||(remainder==half&&(magnitude&1)))++magnitude;}if(magnitude>=0x80000000U)return -2147483647L-1;return sign?-(long)magnitude:(long)magnitude;}
static unsigned RoundCases=0,TargetCases=0;static FILE*Outputs;
static void CheckRound(unsigned bits){float value;memcpy(&value,&bits,4);long expected=OracleRound(value),native=GFFloatToLongNear(value),portable=fable_portable::GFFloatToLongNear(value);assert(native==expected&&portable==expected);if(Outputs)fprintf(Outputs,"R %u %08x %ld\n",RoundCases,bits,native);++RoundCases;}
static void TestRounding(){unsigned raw[]={0,0x80000000U,1,0x80000001U,0x007fffffU,0x807fffffU,0x00800000U,0x80800000U,0x7f800000U,0xff800000U,0x7fc12345U,0xffc54321U,0x7f800001U,0xff800001U,0x4effffffU,0x4f000000U,0xcf000000U,0xcf000001U};for(unsigned i=0;i<sizeof(raw)/sizeof(raw[0]);++i)CheckRound(raw[i]);for(int n=-2048;n<=2048;++n)for(int k=-2;k<=3;++k)CheckRound(Bits(float(n)+float(k)*0.25f));unsigned random=0x534ef00U;for(unsigned i=0;i<65536;++i){random=random*1664525U+1013904223U;CheckRound(random);}}
static bool TestCase(unsigned code){unsigned q=code;int alpha=q%2;q/=2;int zoomIndex=q%4;q/=4;unsigned char noRender=(unsigned char)(q%3);q/=3;int posIndex=q%6;q/=6;bool scaleText=q%2;q/=2;int alignment=q%4;q/=4;bool unavailable=q%2;q/=2;int independent=q%4;q/=4;int parentMode=q%3;q/=3;int handleState=q%3;q/=3;int layerCase=q%2;q/=2;int relativeCase=q%2;q/=2;assert(q==0);
 const float zooms[4]={-1.0f,0.0f,0.5f,2.0f};const float xs[6]={-3.5f,12.25f,120.75f,128.0f,24.5f,24.5f};const float ys[6]={4.5f,20.75f,32.25f,16.0f,96.0f,110.5f};float zoom=zooms[zoomIndex];bool invisible=alpha==0||zoom<=0.0f;NUISystem::CUIScreenUnitConverter::IsUsingRelativeCoords=relativeCase!=0;float width=relativeCase?128.0f:640.0f,height=relativeCase?96.0f:480.0f;bool release=(invisible&&noRender==1)||xs[posIndex]>=width||ys[posIndex]>=height;if(release&&parentMode==2)return false;
 assert(FontLive==0&&PrimitiveLive==0&&FixtureStringLive==0);CallCount=LayerCalls=Notifications=Deletions=0;memset(Calls,0,sizeof Calls);CIEngine engine;CEnginePrimitiveHandle parent;ExpectedParent=&parent;FixturePersistent*owners[2]={0,0};long primitiveIndex=5;
 {
  NUISystem::CText text;text.RenderColour.IntValue=0;text.RenderColour.B=11;text.RenderColour.G=22;text.RenderColour.R=33;text.RenderColour.A=(unsigned char)(alpha?161:0);text.RenderZoom=C2DVector(zoom,1.0f);text.RelativeRenderPosition=C2DVector(xs[posIndex],ys[posIndex]);text.RelativeRenderZoom=C2DVector(relativeCase?1.5f:0.75f,1.0f);text.Size=C2DVector(16.0f,8.0f);text.Independent=(independent&1)!=0;text.LayerIndependent=(independent&2)!=0;text.Align=(NUISystem::ETextAlignement)alignment;text.Layer=(signed char)(layerCase?4:-2);text.ScaleText=scaleText;text.Unavailable=unavailable;text.NoRenderNextFrame=noRender;
  ExpectedOwn[0]=&text.Handle;ExpectedOwn[1]=&text.HandleDropShadowBottomRight;
  for(int i=0;i<2;++i)if(handleState){owners[i]=new FixturePersistent;if(handleState==2)owners[i]->AddRef();ExpectedOwn[i]->Data.Object=owners[i];}
  CEnginePrimitiveHandle*p_handle=parentMode?&parent:0;long*index=parentMode==2?0:&primitiveIndex;long inputLayer=layerCase?8:-3;text.Draw(engine,p_handle,inputLayer,index,0);
  unsigned char expectedNoRender=invisible?1:0;assert(text.NoRenderNextFrame==expectedNoRender&&CallCount==(release?(parentMode?2:0):2)&&FixtureStringLive==1&&FontLive==1);assert(text.PFont.Info&&text.PFont.Info->RefCount==1);bool child=parentMode==1;int expectedNotifications=release&&parentMode==0&&handleState==1?2:0;int expectedDeletions=expectedNotifications;assert(Notifications==expectedNotifications&&Deletions==expectedDeletions);assert(text.Handle.Data.IsNull()==(handleState==0||(release&&parentMode==0)));assert(text.HandleDropShadowBottomRight.Data.IsNull()==text.Handle.Data.IsNull());
  if(release){assert(LayerCalls==0);for(int i=0;i<2;++i){if(parentMode==1){assert(Calls[i].Kind=='C'&&Calls[i].Index==(unsigned)(5+i));}else assert(Calls[i].Kind==0);}if(parentMode==0)assert(CallCount==0);}
  else{
   assert(LayerCalls==2);long expectedLayer=(layerCase?4:-2)+((independent==0)?inputLayer:0);assert(Layers[0]==expectedLayer&&Layers[1]==expectedLayer+1);float scale=scaleText?(relativeCase?1.5f:0.75f):zoom;long x16=(long)(xs[posIndex]*16.0f),scale16=(long)(scale*16.0f);if(alignment==1)x16-=scale16*8;else if(alignment==2)x16-=scale16*16;float expectedX=(float)OracleRound((float)x16/16.0f),expectedY=(float)OracleRound(ys[posIndex]);unsigned foreground=unsigned(text.RenderColour.B)|(unsigned(text.RenderColour.G)<<8)|(unsigned(text.RenderColour.R)<<16)|(unsigned(text.RenderColour.A)<<24),black=unsigned(text.RenderColour.A)<<24;
   for(int i=0;i<2;++i){assert(Calls[i].Kind==(child?'D':'P'));assert(Calls[i].Flags==0x40&&Calls[i].Text==65&&Calls[i].Font==73);assert(Calls[i].X==expectedX+float(i*2)&&Calls[i].Y==expectedY+float(i*2)&&Calls[i].Z==0.0f&&Calls[i].Scale==scale);assert(Calls[i].Layer==1000+expectedLayer+i);unsigned colour=(i==0)?(unavailable?black:foreground):(unavailable?foreground:black);assert(Calls[i].Colour==colour);if(child)assert(Calls[i].Index==(unsigned)(5+i));}
  }
  assert(primitiveIndex==(child?7:5));if(Outputs){fprintf(Outputs,"T %u %u %ld %d %d %d",code,(unsigned)text.NoRenderNextFrame,primitiveIndex,CallCount,Notifications,Deletions);for(int i=0;i<CallCount;++i)fprintf(Outputs," %c,%u,%u,%08x,%08x,%08x,%08x,%08x,%ld,%u,%d",Calls[i].Kind,Calls[i].Index,Calls[i].Flags,Bits(Calls[i].X),Bits(Calls[i].Y),Bits(Calls[i].Z),Bits(Calls[i].Scale),Calls[i].Colour,Calls[i].Layer,Calls[i].Text,Calls[i].Font);fprintf(Outputs,"\n");}
  text.Handle.Clear();text.HandleDropShadowBottomRight.Clear();if(handleState==2)for(int i=0;i<2;++i){assert(owners[i]->GetRefCount()==1);if(owners[i]->Release()==0)owners[i]->Delete();}
 }
 assert(FontLive==0&&PrimitiveLive==0&&FixtureStringLive==0);++TargetCases;return true;
}
int main(int argc,char**argv){
#ifdef _MSC_VER
 unsigned control=_controlfp(0,0);assert((control&_MCW_RC)==_RC_NEAR&&(control&_EM_INVALID)!=0);
#endif
 Outputs=argc>1?fopen(argv[1],"w"):0;assert(argc<=1||Outputs);TestRounding();for(unsigned i=0;i<331776;++i)TestCase(i);assert(RoundCases==90136);assert(TargetCases==283392);if(Outputs)fclose(Outputs);printf("0054ef00_TEST PASS\n");return 0;}
