// 00644677 NPlayerGui::CDrawLetterBoxBlackBars::Draw
// Leaf: ?Draw@CDrawLetterBoxBlackBars@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
// Flags:
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DRAW_LETTERBOX_PASS29
#define FABLE_RETAIL_NPLAYERGUI_GFTOSCREEN_BOX_PASS23
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_SETASDRAWING
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_ORIGINAL_REFERENCES
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PEEKDISPLAYENGINE

#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CPlayerManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CDisplayEngine.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
#include "engine/C2DExtentsF.h"
#include "engine/NPlayerGui_CDrawLetterBoxBlackBars.h"
#include "engine/NPlayerGui_Graphics.h"
#include "engine/GFGetInterpolatedValue.h"

extern double VIRTUAL_SCREEN_WIDTH;
extern double VIRTUAL_SCREEN_HEIGHT;
float FABLE_FASTCALL GFVXToAX(float x);
float FABLE_FASTCALL GFVYToAY(float y);

#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
namespace NPlayerGui {
void CDrawLetterBoxBlackBars::Draw(const CEnginePrimitivePrepareInfo& info)
{
    SetAsDrawing(true);
    const CDisplayEngine& display_engine = Component.PeekDisplayEngine();
    const float screen_ratio = display_engine.GetLetterBoxRatio();
    const float alpha = display_engine.GetLetterBoxStrength();
    const float full_screen_height = GFVYToAY(static_cast<float>(VIRTUAL_SCREEN_HEIGHT));
    const float final_screen_height = GFVXToAX(static_cast<float>(VIRTUAL_SCREEN_WIDTH)) / screen_ratio;
    const float current_screen_height = GFGetInterpolatedValue(full_screen_height, final_screen_height, alpha);
    const float height_of_black_bars = (full_screen_height - current_screen_height) * 0.5f;

    C2DBoxF top_bar_area(C2DVector(-8.0f, -8.0f),
        C2DExtentsF(GFVXToAX(static_cast<float>(VIRTUAL_SCREEN_WIDTH)) + 8.0f, height_of_black_bars));
    CEnginePrimitive2DSprite top_bar(0, GFToScreen(top_bar_area), ENGINE_2D_LAYER_12,
        0, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_DEFAULT, 0.0f, 0.0f,
        255, 0, 0, 0, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info.Engine.SetPersistentPrimitive(PrimitiveHandles[0], top_bar, 0x40);

    C2DBoxF bottom_bar_area(C2DVector(-8.0f, GFVYToAY(static_cast<float>(VIRTUAL_SCREEN_HEIGHT)) - height_of_black_bars),
        C2DExtentsF(GFVXToAX(static_cast<float>(VIRTUAL_SCREEN_WIDTH)) + 8.0f, height_of_black_bars + 8.0f));
    CEnginePrimitive2DSprite bottom_bar(0, GFToScreen(bottom_bar_area), ENGINE_2D_LAYER_12,
        0, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_DEFAULT, 0.0f, 0.0f,
        255, 0, 0, 0, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info.Engine.SetPersistentPrimitive(PrimitiveHandles[1], bottom_bar, 0x40);
}
}

#include <assert.h>
#include <stdio.h>
#include <string.h>

double VIRTUAL_SCREEN_WIDTH;
double VIRTUAL_SCREEN_HEIGHT;
static float ratio, strength;
static unsigned int draws, constructions, widthCalls, heightCalls;
static float expected[2][4];
static const CEnginePrimitive2DSprite* sprite;
static unsigned char* handleBase;
template<class T> union Storage {
    double aligned;
    unsigned char bytes[sizeof(T)];
    T* pointer(){return reinterpret_cast<T*>(bytes);}
};
static Storage<CMainGameComponent> component;
static Storage<CDisplayEngine> display;
static Storage<NPlayerGui::CDrawLetterBoxBlackBars> row;
static Storage<CEnginePrimitivePrepareInfo> infoData;
float CDisplayEngine::GetLetterBoxRatio()const {assert(this==display.pointer());return ratio;}
float CDisplayEngine::GetLetterBoxStrength()const {assert(this==display.pointer());return strength;}
float FABLE_FASTCALL GFVXToAX(float x){assert(x==VIRTUAL_SCREEN_WIDTH);++widthCalls;return x*2.0f;}
float FABLE_FASTCALL GFVYToAY(float y){assert(y==VIRTUAL_SCREEN_HEIGHT);++heightCalls;return y*0.5f;}

CEnginePrimitive2DSprite::CEnginePrimitive2DSprite(unsigned __int64 id, const C2DBoxF& box,
    EEngine2DLayer layer, unsigned long bank, unsigned long frame, EEnginePrimitiveBlendMode blend,
    EEngineInterpolationSet interpolate, float start, float end, unsigned char alpha, unsigned char r,
    unsigned char g, unsigned char b, unsigned long flags, const C2DVector& uv0, const C2DVector& uv1)
    : CEnginePrimitive(static_cast<EEnginePrimitiveType>(0),interpolate)
{
    assert(draws==constructions && constructions<2);
    assert(id==0 && bank==0 && frame==0 && flags==0 && layer==ENGINE_2D_LAYER_12);
    assert(blend==ENGINE_PRIMITIVE_BLEND_ALPHA && interpolate==ENGINE_INTERPOLATION_SET_DEFAULT);
    assert(start==0 && end==0 && alpha==255 && r==0 && g==0 && b==0);
    assert(uv0.X==0 && uv0.Y==0 && uv1.X==0 && uv1.Y==0);
    const float* e=expected[constructions++];
    assert(box.TLX==e[0] && box.TLY==e[1] && box.BRX==e[2] && box.BRY==e[3]);
    sprite=this;
}
class RendererTrace {
public:
    virtual void Submit(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, unsigned long flags) {
        assert(constructions==draws+1 && &primitive==sprite && flags==0x40);
        assert(reinterpret_cast<unsigned char*>(&handle)==handleBase+draws*sizeof(CEnginePrimitiveHandle));
        assert(row.bytes[0x18]==1);
        ++draws;
    }
};
int main()
{
    RendererTrace trace;void** methods;memcpy(&methods,&trace,sizeof(methods));
    void* slots[24]={0};slots[23]=methods[0];void** table=slots;void* renderer=&table;
    memcpy(infoData.bytes+0x34,&renderer,sizeof(renderer));
    CMainGameComponent* comp=component.pointer();CDisplayEngine* view=display.pointer();
    memcpy(component.bytes+0x28,&view,sizeof(view));memcpy(row.bytes+0x10,&comp,sizeof(comp));
    Storage<CEnginePrimitiveHandle> handles[2];handleBase=handles[0].bytes;
    memcpy(row.bytes+0x1c,&handleBase,sizeof(handleBase));
    for (int width=0;width!=2;++width)for(int aspect=0;aspect!=2;++aspect)for(int alpha=0;alpha!=3;++alpha) {
        VIRTUAL_SCREEN_WIDTH=width?320.0:192.0;VIRTUAL_SCREEN_HEIGHT=480.0;
        ratio=aspect?2.0f:4.0f;strength=alpha*0.5f;
        float x=static_cast<float>(VIRTUAL_SCREEN_WIDTH)*2.0f;
        float y=static_cast<float>(VIRTUAL_SCREEN_HEIGHT)*0.5f;
        float bar=strength*(y-x/ratio)*0.5f;
        expected[0][0]=-8;expected[0][1]=-8;expected[0][2]=x;expected[0][3]=bar-8;
        expected[1][0]=-8;expected[1][1]=y-bar;expected[1][2]=x;expected[1][3]=y+8;
        draws=constructions=widthCalls=heightCalls=0;row.bytes[0x18]=0;
        row.pointer()->Draw(*infoData.pointer());
        assert(draws==2 && constructions==2 && widthCalls==3 && heightCalls==2);
    }
    puts("00644677_TEST PASS");return 0;
}