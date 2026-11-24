#include <stdio.h>
#include <string.h>
#include "rebuild_abi.h"
#define FABLE_RETAIL_C3DVECTOR
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/CEnginePrimitiveEnums.h"
struct CEngineInternalPrimitiveBase {};
struct FontFixture { mutable int calls;float GetFontMaxHeight()const{++calls;return 4;} };
struct CEngineInternalPrimitiveText:CEngineInternalPrimitiveBase {
 C3DVector Pos;float Scale;CRGBColour Colour;bool ScaleIn3D;const wchar_t* Text;FontFixture* Font;
};
struct CEngineSubPrimitiveText {
 CEngineInternalPrimitiveText* parent;C3DVector Pos;float Scale;CRGBColour Colour;const wchar_t* Text;const void* Font;const void* Mask;
 explicit CEngineSubPrimitiveText(CEngineInternalPrimitiveText* p):parent(p){}
};
struct CEngineSubPrimitiveChain {};
struct Camera {
 void TransformWorldToView(C3DVector& v)const {v.X+=1;v.Y-=2;}
 void TransformViewToScreen(const C3DVector& v,C3DVector& p) const {p.X=8+4*v.X/v.Z;p.Y=12-2*v.Y/v.Z;p.Z=0.5f+1/v.Z;}
};
struct Surface {unsigned long width;unsigned long GetWidth()const{return width;}};
struct Display {Surface buffer;Surface& GetBackBuffer(){return buffer;}};
namespace NEngineInternal {Camera camera;Camera* GPCamera=&camera;Display display;Display* GPDisplayManager=&display;}
struct CEnginePrimitiveManagerText {
 CEngineSubPrimitiveText* submitted;float distance;unsigned char alpha;unsigned long flags;
 void AddSubPrimitive(CEngineSubPrimitiveText* s,float z,unsigned char a,unsigned long f){submitted=s;distance=z;alpha=a;flags=f;}
 ERendered RenderPrimitive(CEngineInternalPrimitiveBase&,EEngineRenderLayer,CEngineSubPrimitiveChain&);
};
ERendered CEnginePrimitiveManagerText::RenderPrimitive(CEngineInternalPrimitiveBase& primitive_base,EEngineRenderLayer layer,CEngineSubPrimitiveChain& subprim_chain)
{
    CEngineInternalPrimitiveText& primitive=static_cast<CEngineInternalPrimitiveText&>(primitive_base);
    C3DVector view_pos(primitive.Pos.X,primitive.Pos.Y,primitive.Pos.Z);
    NEngineInternal::GPCamera->TransformWorldToView(view_pos);
    if(view_pos.Z>0.0001f) {
        C3DVector screen_pos;
        NEngineInternal::GPCamera->TransformViewToScreen(view_pos,screen_pos);
        float scale=primitive.Scale;
        if(primitive.ScaleIn3D) {
            scale/=view_pos.Z;
            scale/=primitive.Font->GetFontMaxHeight();
            scale*=NEngineInternal::GPDisplayManager->GetBackBuffer().GetWidth()>>1;
        }
        C3DVector coord;
        coord.X=screen_pos.X;
        coord.Y=screen_pos.Y;
        coord.Z=screen_pos.Z;
        CEngineSubPrimitiveText* subprim=new CEngineSubPrimitiveText(&primitive);
        subprim->Pos=coord;
        subprim->Scale=scale;
        subprim->Colour=primitive.Colour;
        subprim->Text=primitive.Text;
        subprim->Font=primitive.Font;
        subprim->Mask=0;
        AddSubPrimitive(subprim,screen_pos.Z,primitive.Colour.A,0);
    }
    return RENDERED_OBJECT;
}

int main(){
 const float depths[]={-1,0,0.0001f,2,4};
 for(int i=0;i<5;++i)for(int scaled=0;scaled<2;++scaled)for(unsigned int width=8;width<11;++width){
    FontFixture font;font.calls=0;CEnginePrimitiveManagerText mgr;mgr.submitted=0;
    CEngineInternalPrimitiveText t;t.Font=&font;t.ScaleIn3D=scaled!=0;t.Pos=C3DVector(3,10,depths[i]);
    t.Scale=8;t.Text=L"Hello";t.Colour.B=7;t.Colour.G=13;t.Colour.R=69;t.Colour.A=240;
    NEngineInternal::display.buffer.width=width;CEngineSubPrimitiveChain chain;
    if(mgr.RenderPrimitive(t,(EEngineRenderLayer)0,chain)!=RENDERED_OBJECT)return 1;
    if(depths[i]<=0.0001f){if(mgr.submitted || font.calls)return 2;continue;}
    const CEngineSubPrimitiveText* result=mgr.submitted;
    if(!result || result->parent!=&t || result->Pos.X!=8+16/depths[i] || result->Pos.Y!=12-16/depths[i] ||
       result->Pos.Z!=0.5f+1/depths[i] || result->Scale!=(scaled?8/depths[i]/4*(width/2):8))return 3;
    if(result->Text!=t.Text || memcmp(&result->Colour,&t.Colour,4) || result->Font!=&font || result->Mask ||
       mgr.distance!=result->Pos.Z || mgr.alpha!=240 || mgr.flags || font.calls!=scaled)return 4;
    delete result;
 }
 puts("00bc6930_TEST PASS");return 0;
}