#include <stdio.h>
#include <vector>
const float DELTA_NOTIONAL_ZERO=0.001f;
const int ENGINE_2D_LAYER_1=68,ENGINE_PRIMITIVE_BLEND_ALPHA=2;
enum EEngineInterpolationSet{ SET0 };
struct C2DVector {
    float X,Y;
    C2DVector(float x=0,float y=0):X(x),Y(y){}
    C2DVector operator+(const C2DVector& v)const{return C2DVector(X+v.X,Y+v.Y);}
};
struct CRGBColour{int r,g,b,a;CRGBColour(int R,int G,int B,int A):r(R),g(G),b(B),a(A){}};
struct CPlayerGuiDef{
    std::vector<int> AbilityButtons;
    C2DVector DPadRingsPos,DPadRingsChargeOffsets[2];
    long DPadRingsChargeGraphics[2];
    float DPadRingsChargeAngles[2];
} config;
struct CPlayerGui{static const CPlayerGuiDef& PeekGuiDef(){return config;}};
struct CEnginePrimitive2DClockSprite{
    C2DVector pos,uv1,uv2; int layer,first,second,mode,bits,setting,ident,frame1,frame2;
    CRGBColour dark,light;float start,finish;
    CEnginePrimitive2DClockSprite(int id,const C2DVector& p,int l,int f,int f1,CRGBColour d,int s,int f2,CRGBColour b,int m,float a,float z,
                                  const C2DVector& u,const C2DVector& v,int fl,EEngineInterpolationSet e):
        pos(p),uv1(u),uv2(v),layer(l),first(f),second(s),mode(m),bits(fl),setting(e),ident(id),frame1(f1),frame2(f2),dark(d),light(b),start(a),finish(z){}
};
static unsigned calls;static bool bad;static int want_index;static float want_angle;
struct EngineFixture{
    void SetPersistentPrimitive(int handle,const CEnginePrimitive2DClockSprite& p,int flags) const{
        ++calls;
        if(flags!=192 || handle!=17 || p.ident || p.frame1 || p.frame2 || p.setting || p.bits!=2 || p.mode!=2 || p.layer!=68 ||
           p.uv1.X || p.uv1.Y || p.uv2.X || p.uv2.Y || p.first!=100+want_index || p.second!=p.first ||
           p.dark.r || p.dark.g || p.dark.b || p.dark.a!=153 || p.light.r!=255 || p.light.g!=255 || p.light.b!=255 || p.light.a!=153 ||
           p.pos.X!=14+3*want_index || p.pos.Y!=26+4*want_index || p.start!=want_angle || p.finish!=want_angle)bad=true;
    }
};
struct CEnginePrimitivePrepareInfo{EngineFixture Engine;};
namespace NPlayerGui {
C2DVector GFAnchorToBottomLeft(const C2DVector& p){return C2DVector(p.X+10,p.Y+20);}
struct CDrawQuickAccessIcons{
    int ChargeUpPrimitiveHandle,ChargingButtonIndex;bool FullyCharged;float activation,ChargingButtonTime;unsigned queries;
    CDrawQuickAccessIcons():ChargeUpPrimitiveHandle(17),ChargingButtonIndex(5),queries(0){}
    float GetActivationTimeForButton(long i){++queries;if(i!=5)bad=true;return activation;}
    unsigned char ScaleAlpha(unsigned char a) const{if(a!=255)bad=true;return 153;}
    void DrawChargeGraphic(const CEnginePrimitivePrepareInfo&,const C2DVector&,long);
};
void CDrawQuickAccessIcons::DrawChargeGraphic(const CEnginePrimitivePrepareInfo& info,
                                             const C2DVector& button_offset, long button_index)
{
    const CPlayerGuiDef& gui_def = CPlayerGui::PeekGuiDef();
    float angle = 0.0f;
    if (FullyCharged) angle = 1.0f;
    else {
        float activation_time = GetActivationTimeForButton(ChargingButtonIndex);
        if (activation_time > DELTA_NOTIONAL_ZERO && activation_time > ChargingButtonTime)
            angle = ChargingButtonTime / activation_time;
    }
    if (angle > DELTA_NOTIONAL_ZERO) {
        long index = button_index % (long)gui_def.AbilityButtons.size();
        if (button_index == -2) index = 0; else index = 1;
        if (angle > 1.0f) angle = 1.0f;
        angle = angle / 2.0f + gui_def.DPadRingsChargeAngles[index];
        C2DVector charge_pos = GFAnchorToBottomLeft(gui_def.DPadRingsPos) + button_offset + gui_def.DPadRingsChargeOffsets[index];
        CEnginePrimitive2DClockSprite charge_sprite(0, charge_pos, ENGINE_2D_LAYER_1,
            gui_def.DPadRingsChargeGraphics[index], 0, CRGBColour(0,0,0,ScaleAlpha(255)),
            gui_def.DPadRingsChargeGraphics[index], 0, CRGBColour(255,255,255,ScaleAlpha(255)),
            ENGINE_PRIMITIVE_BLEND_ALPHA, angle, angle, C2DVector(0,0), C2DVector(0,0), 2, (EEngineInterpolationSet)0);
        info.Engine.SetPersistentPrimitive(ChargeUpPrimitiveHandle, charge_sprite, 0xc0);
    }
}
 }
int main(){
    config.AbilityButtons.resize(3);config.DPadRingsPos=C2DVector(1,2);
    config.DPadRingsChargeOffsets[0]=C2DVector(2,3);config.DPadRingsChargeOffsets[1]=C2DVector(5,7);
    config.DPadRingsChargeAngles[0]=0.125f;config.DPadRingsChargeAngles[1]=0.25f;
    config.DPadRingsChargeGraphics[0]=100;config.DPadRingsChargeGraphics[1]=101;
    const float values[]={-1,0,0.00048828125f,0.25f,0.5f,1,2};
    const int buttons[]={-2,-1,0,4};CEnginePrimitivePrepareInfo info;
    for(int full=0;full<2;++full)for(int b=0;b<4;++b)for(int at=0;at<7;++at)for(int held=0;held<7;++held){
        NPlayerGui::CDrawQuickAccessIcons action;action.FullyCharged=full!=0;action.activation=values[at];action.ChargingButtonTime=values[held];
        want_index=(buttons[b]==-2?0:1);float progress=0;
        if(full)progress=1;
        else if(values[at]>DELTA_NOTIONAL_ZERO && values[held]<values[at])progress=values[held]/values[at];
        want_angle=config.DPadRingsChargeAngles[want_index]+progress*0.5f;
        calls=0;action.DrawChargeGraphic(info,C2DVector(1,1),buttons[b]);
        if(bad || calls!=(unsigned)(progress>DELTA_NOTIONAL_ZERO) || action.queries!=(unsigned)!full)return 1;
    }
    puts("0064d7a6_TEST PASS");return 0;
}