// 00542a30 NUISystem::CIconText::Draw
// The shared enum and Ego_d identify 0x40 as EPF_2D and 0xC0 as EPF_2D | EPF_BLOCK_ON_LOAD.
#define private public
#define protected public
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_CRGBCOLOUR_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_C2DVECTOR_ORIGINAL_COPIES_PASS26
#define FABLE_RETAIL_CTEXT_DRAW_PRIMITIVE_BODIES
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_LAYER_ENUM_FROM_CMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_BOX_CTOR_BODY
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_C2DBOXF_COPY
// Leaf: ?Draw@CIconText@NUISystem@@UAEXAAVCIEngine@@PAVCEnginePrimitiveHandle@@JPAJPAVCComponent@2@@Z
// Flags: /Gr
// Retail-asm: 0x00542C97 `fistp dword ptr [esp+28h]` (and 0x00542CA7, the y rounding) -- the engine's inline-asm float-to-long
//   helper GFFloatToLongNear (shared engine/GFFloatToLongNear.h, Retail-asm-helper); no C++ form gives the bare dword fistp.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CORNERS
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_ORIGINAL_DIMENSIONS
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#ifdef _MSC_VER
// Inline-budget: retail 0041cc14 selects the size-built screen width accessor.
#pragma optimize("s",on)
#endif
#include "engine/NUISystem_CUIScreenUnitConverter.h"
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
#include "engine/EEnginePrimitiveType.h"
#include "engine/CDefPointer.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/NUISystem_CIconText.h"
#include "engine/NUISystem_CManager.h"

#include "engine/CIEngine.h"
#include "engine/CEnginePrimitive2DText.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/GFFloatToLongNear.h"
#define FABLE_RETAIL_STLP_MEM_FUN_REF_ORIGINAL_PASS22
#include "stlport/fable_algorithm.h"
namespace NUISystem {
void CIconText::Draw(CIEngine& engine, CEnginePrimitiveHandle* p_handle, long layer, long* pprimitive_index, CComponent* pparent)
{
    if (RenderColour.A <= 0 || RenderZoom.X <= 0.0f) {
        if (NoRenderNextFrame == 1) {
            if (p_handle) {
                engine.ClearChildPrimitive(*p_handle, (*pprimitive_index)++);
                engine.ClearChildPrimitive(*p_handle, (*pprimitive_index)++);
            } else {
                do {
                    TextHandle.Clear();
                    DropShadowHandle.Clear();
                    std::for_each(IconHandles.begin(), IconHandles.end(), std::mem_fun_ref(&CEnginePrimitiveHandle::Clear));
                } while (0);
            }
            return;
        }
        NoRenderNextFrame = 1;
    } else
        NoRenderNextFrame = 0;
    CRGBColour colour = RenderColour;
    C2DVector render_position = RelativeRenderPosition;
    if (render_position.X >= CUIScreenUnitConverter::GetBackBufferWidth() || render_position.Y >= CUIScreenUnitConverter::GetBackBufferWidth()) {
        do {
            TextHandle.Clear();
            DropShadowHandle.Clear();
            std::for_each(IconHandles.begin(), IconHandles.end(), std::mem_fun_ref(&CEnginePrimitiveHandle::Clear));
        } while (0);
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
        render_position.X -= Size.X * 0.5f;
        break;
    case RIGHT:
        render_position.X -= Size.X;
        break;
    }
    render_position.X = (float)GFFloatToLongNear(render_position.X);
    render_position.Y = (float)GFFloatToLongNear(render_position.Y);
    C2DVector render_zoom = RelativeRenderZoom;
    if (Unavailable) {
        primitive = CEnginePrimitive2DText(0, render_position, CManager::GetInstance()->GetLayer(render_layer), render_zoom.X, DisplayText, *PFont, black, ENGINE_INTERPOLATION_SET_DEFAULT);
        C2DVector position(render_position);
        position.X += 2.0f;
        position.Y += 2.0f;
        primitive_bottom_right = CEnginePrimitive2DText(0, position, CManager::GetInstance()->GetLayer(render_layer + 1), render_zoom.X, DisplayText, *PFont, colour, ENGINE_INTERPOLATION_SET_DEFAULT);
    } else {
        primitive = CEnginePrimitive2DText(0, render_position, CManager::GetInstance()->GetLayer(render_layer), render_zoom.X, DisplayText, *PFont, colour, ENGINE_INTERPOLATION_SET_DEFAULT);
        C2DVector position(render_position);
        position.X += 2.0f;
        position.Y += 2.0f;
        primitive_bottom_right = CEnginePrimitive2DText(0, position, CManager::GetInstance()->GetLayer(render_layer + 1), render_zoom.X, DisplayText, *PFont, black, ENGINE_INTERPOLATION_SET_DEFAULT);
    }
    if (p_handle && pprimitive_index) {
        engine.SetChildPrimitive(*p_handle, (*pprimitive_index)++, primitive, 0x40);
        engine.SetChildPrimitive(*p_handle, (*pprimitive_index)++, primitive_bottom_right, 0x40);
    } else {
        engine.SetPersistentPrimitive(TextHandle, primitive, 0x40);
        engine.SetPersistentPrimitive(DropShadowHandle, primitive_bottom_right, 0x40);
        IconHandles.resize(Icons.size());
        for (long index = 0; index < Icons.size(); ++index) {
            CEmbeddedIcon& icon = Icons[index];
            C2DVector position = render_position + icon.Offset;
            C2DExtentsF range(icon.Size.X * render_zoom.X, icon.Size.Y * render_zoom.Y);
            C2DBoxF clip_box(position, range);
            CEnginePrimitive2DSprite primitive_sprite(0, clip_box, CManager::GetInstance()->GetLayer(render_layer), icon.Graphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_DEFAULT, 0.0f, 0.0f,
                                            colour.A, colour.R, colour.G, colour.B, 2, C2DVector(0.0f,0.0f), C2DVector(0.0f,0.0f));
            engine.SetPersistentPrimitive(IconHandles[index], primitive_sprite, 0xC0);
        }
    }
}
}

CEnginePrimitive2DText::CEnginePrimitive2DText(unsigned __int64 uid, const C2DVector& pos,
    EEngine2DLayer layer, float scale, const CWideString& text, const CFontBank& font,
    const CRGBColour& colour, EEngineInterpolationSet interp_set)
    : CEnginePrimitive(ENGINE_PRIMITIVE_2D_TEXT, uid, interp_set), Pos(pos.X,pos.Y,0),
      Layer(layer), Scale(scale), Colour(colour), Text(text), PFont(&font) {}


CEnginePrimitive2DText& CEnginePrimitive2DText::operator=(const CEnginePrimitive2DText& __that)
{
    CEnginePrimitive::operator=(__that);
    Pos = __that.Pos;
    Layer = __that.Layer;
    Scale = __that.Scale;
    Colour = __that.Colour;
    Text = __that.Text;
    PFont = __that.PFont;
    return *this;
}
#undef private
#undef protected
#include <stdio.h>
using namespace NUISystem;

namespace std {
template<class It,class F> F FABLE_FASTCALL for_each(It first,It last,F f){
    for (;first!=last;++first) f(*first);
    return f;
}
}
static int errors, resize_count;
static void expect(bool ok){if(!ok)++errors;}
namespace std {
template<> void vector<CEnginePrimitiveHandle>::resize(size_type n) {
    ++resize_count;
    expect(n<=2);
    _M_finish=_M_start+n;
}
}
CWideString::CWideString(){*(void**)this=0;}
CWideString::CWideString(const CWideString& s){*(void**)this=*(void*const*)&s;}
const CWideString& CWideString::operator=(const CWideString& s){*(void**)this=*(void*const*)&s;return *this;}
CWideString::~CWideString(){}
bool CUIScreenUnitConverter::IsUsingRelativeCoords;
float CUIScreenUnitConverter::DefaultBackBufferWidth=64,CUIScreenUnitConverter::BackBufferWidth=80;
static void* mgr_slots[60];
static void** mgr=mgr_slots;
CManager* __fastcall CManager::GetInstance(){return (CManager*)&mgr;}
static bool ind,lay;
static ETextAlignement align_value;
static bool __fastcall independent(void*,void*){return ind;}
static bool __fastcall layer_independent(void*,void*){return lay;}
static ETextAlignement __fastcall align_cb(void*,void*){return align_value;}
static EEngine2DLayer __fastcall layer_cb(void*,void*,long layer){return (EEngine2DLayer)(layer+7);}
struct Event {int kind;const void* handle;long index;unsigned long flags;
    float x,y,r,b,scale;long layer,graphic;unsigned colour;};
static Event events[8];static int n;
static CFontBank* font;
static void* string_tag;
static void __fastcall clear_child(void*,void*,CEnginePrimitiveHandle& h,unsigned long index){
    Event& e=events[n++];e.kind=1;e.handle=&h;e.index=index;
}
static void record(int kind,CEnginePrimitiveHandle& h,unsigned long index,const CEnginePrimitive& p,unsigned long flags){
    Event& e=events[n++];e.kind=kind;e.handle=&h;e.index=index;e.flags=flags;
    if((int)p.Type==0x27){
        const CEnginePrimitive2DText& t=(const CEnginePrimitive2DText&)p;
        e.x=t.Pos.X;e.y=t.Pos.Y;e.scale=t.Scale;e.layer=t.Layer;e.colour=t.Colour.IntValue;
        expect(t.PFont==font && *(void*const*)&t.Text==string_tag && t.Pos.Z==0);
    }else{
        const CEnginePrimitive2DSprite& t=(const CEnginePrimitive2DSprite&)p;
        expect((int)p.Type==0x22 && t.Frame==0 && t.Flags==2 && t.BlendMode==2 && !t.ClipBoxIsPos);
        expect(!t.UVOffsetsStart.X&&!t.UVOffsetsStart.Y&&!t.UVOffsetsEnd.X&&!t.UVOffsetsEnd.Y);
        e.x=t.ClipBox.TLX;e.y=t.ClipBox.TLY;e.r=t.ClipBox.BRX;e.b=t.ClipBox.BRY;e.layer=t.Layer;e.graphic=t.BankIndex;e.colour=t.Colour.IntValue;
    }
}
static void __fastcall child(void*,void*,CEnginePrimitiveHandle& h,unsigned long index,const CEnginePrimitive& p,unsigned long flags){record(2,h,index,p,flags);}
static void __fastcall persistent(void*,void*,CEnginePrimitiveHandle& h,const CEnginePrimitive& p,unsigned long flags){record(3,h,0,p,flags);}
static void __fastcall notify(void*,void*,const CEnginePrimitiveHandle& h){Event& e=events[n++];e.kind=4;e.handle=&h;}
static void __fastcall release(void*,void*){}
struct Primitive {void** vt;long count;};
static void set_handle(CEnginePrimitiveHandle& h,Primitive* p){((void**)&h)[1]=p;}
static long nearest(float f){long whole=(long)f;float rem=f-whole;if(rem>.5f || (rem==.5f && whole%2))++whole;else if(rem<-.5f || (rem==-.5f&&whole%2))--whole;return whole;}
int main(){
    void* component_slots[152]={0};void* engine_slots[40]={0};void* released_slots[4]={0};
    component_slots[101]=(void*)independent;component_slots[104]=(void*)layer_independent;component_slots[151]=(void*)align_cb;
    engine_slots[23]=(void*)persistent;engine_slots[28]=(void*)child;engine_slots[30]=(void*)clear_child;
    released_slots[1]=(void*)release;released_slots[3]=(void*)notify;mgr_slots[36]=(void*)layer_cb;
    unsigned long raw[sizeof(CIconText)/4]={0},font_storage=0,parent_storage[2]={0},handle_storage[4]={0};
    CIconText& t=*(CIconText*)raw;*(void***)raw=component_slots;
    void** engine=engine_slots;CEnginePrimitiveHandle* parent=(CEnginePrimitiveHandle*)parent_storage;
    CEnginePrimitiveHandle* handles=(CEnginePrimitiveHandle*)handle_storage;
    CIconText::CEmbeddedIcon icons[2];font=(CFontBank*)&font_storage;string_tag=&icons;t.PFont.Data=font;*(void**)&t.DisplayText=string_tag;
    t.IconHandles._M_start=handles;t.Icons._M_start=icons;t.Layer=5;t.Size.X=7;
    for(int k=0;k<2;++k){icons[k].Offset.X=k+1.25f;icons[k].Offset.Y=k-2.5f;icons[k].Size.X=k+4;icons[k].Size.Y=k+3;icons[k].Graphic=k+27;}
    int count=0,hits[5]={0};
    for(unsigned mask=0;mask<1024;++mask)for(int axis=0;axis<4;++axis)for(int a=0;a<3;++a){
        bool par=(mask&1)!=0,idx=(mask&2)!=0,alpha=(mask&4)!=0,zoom=(mask&8)!=0,prior=(mask&16)!=0;
        bool invisible=!alpha||!zoom;if(invisible&&prior&&par&&!idx)continue;
        t.RenderColour=CRGBColour(31,57,93,alpha?211:0);t.RenderZoom.X=zoom?1.0f:0.0f;t.NoRenderNextFrame=prior;
        t.Unavailable=(mask&32)!=0;ind=(mask&64)!=0;lay=(mask&128)!=0;
        CUIScreenUnitConverter::IsUsingRelativeCoords=(mask&256)!=0;float width=(mask&256)?80.0f:64.0f;
        int num=(mask&512)?2:0;t.Icons._M_finish=icons+num;t.IconHandles._M_finish=handles+2;align_value=(ETextAlignement)a;
        float x=axis==1?width:axis==3?-7.5f:10.5f,y=axis==2?width:axis==3?-8.5f:20.5f;
        t.RelativeRenderPosition.X=x;t.RelativeRenderPosition.Y=y;t.RelativeRenderZoom.X=1.5f;t.RelativeRenderZoom.Y=2.5f;
        Primitive blocks[4];CEnginePrimitiveHandle* targets[4]={&t.TextHandle,&t.DropShadowHandle,handles,handles+1};
        for(int j=0;j<4;++j){blocks[j].vt=released_slots;blocks[j].count=1;set_handle(*targets[j],&blocks[j]);}
        n=resize_count=0;long index=11;
        t.CIconText::Draw(*(CIEngine*)&engine,par?parent:0,3,idx?&index:0,0);
        expect(t.NoRenderNextFrame==invisible);
        int action=invisible&&prior&&par?1:invisible&&prior||x>=width||y>=width?4:par&&idx?2:3;++hits[action];
        if(action==1){expect(n==2&&index==13&&!resize_count);for(int j=0;j<2;++j)expect(events[j].kind==1&&events[j].handle==parent&&events[j].index==11+j);}
        else if(action==4){expect(n==4&&!resize_count&&index==11);for(int j=0;j<4;++j)expect(events[j].kind==4&&events[j].handle==targets[j]&&blocks[j].count==0);}
        else{
            expect(n==(action==2?2:2+num));expect(resize_count==(action==3));expect(index==(action==2?13:11));
            long rl=(ind||lay?5:8)+7;float rx=(float)nearest(x-(a==1?3.5f:a==2?7.0f:0.0f)),ry=(float)nearest(y);
            unsigned black=t.RenderColour.IntValue&0xff000000;
            for(int j=0;j<2;++j){Event&e=events[j];expect(e.kind==action&&e.handle==(action==2?parent:targets[j])&&e.flags==64&&e.x==rx+2*j&&e.y==ry+2*j&&e.layer==rl+j&&e.scale==1.5f&&e.colour==((t.Unavailable==(j==0))?black:t.RenderColour.IntValue));if(action==2)expect(e.index==11+j);}
            if(action==3)for(int j=0;j<num;++j){Event&e=events[2+j];float xx=rx+icons[j].Offset.X,yy=ry+icons[j].Offset.Y;expect(e.kind==3&&e.handle==handles+j&&e.flags==192&&e.graphic==27+j&&e.layer==rl&&e.x==xx&&e.y==yy&&e.r==xx+icons[j].Size.X*1.5f&&e.b==yy+icons[j].Size.Y*2.5f&&e.colour==t.RenderColour.IntValue);}
        }++count;
    }
    for(int i=1;i<=4;++i)expect(hits[i]>0);expect(count>11000);
    if(errors){printf("errors %d cases %d\n",errors,count);return 1;}puts("00542a30_TEST PASS");return 0;
}