#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <map>
#include "engine/GFFloatToLongNear.h"
struct C2DVector {float X,Y; C2DVector(float x=0,float y=0):X(x),Y(y){}};
struct C2DBoxF {float l,t,r,b;C2DBoxF(float a=0,float bb=0,float c=0,float d=0):l(a),t(bb),r(c),b(d){}};
struct CRGBColour{unsigned char R,G,B,A;};
enum EEngine2DLayer {BASE_LAYER=0};enum EEnginePrimitiveBlendMode{ENGINE_PRIMITIVE_BLEND_ALPHA=2};enum EEngineInterpolationSet{ENGINE_INTERPOLATION_SET_DEFAULT=0};
struct CEnginePrimitiveHandle {int cleared;CEnginePrimitiveHandle():cleared(0){}void Clear(){++cleared;}};
struct CEnginePrimitive2DSprite {C2DBoxF box;unsigned long index;long layer;
CEnginePrimitive2DSprite(unsigned __int64,const C2DBoxF& b,EEngine2DLayer l,unsigned long idx,unsigned long,EEnginePrimitiveBlendMode,EEngineInterpolationSet,float,float,unsigned char,unsigned char,unsigned char,unsigned char,unsigned long,const C2DVector&,const C2DVector&):box(b),index(idx),layer(l){}
};
struct CIEngine{int action;long slot;unsigned long graphic;long layer;C2DBoxF box;
 CIEngine():action(0),slot(-1),graphic(0),layer(-1){}
 void ClearChildPrimitive(CEnginePrimitiveHandle&,long s){action=1;slot=s;}
 void SetChildPrimitive(CEnginePrimitiveHandle&,long s,const CEnginePrimitive2DSprite& p,unsigned long){action=2;slot=s;box=p.box;graphic=p.index;layer=p.layer;}
 void SetPersistentPrimitive(CEnginePrimitiveHandle&,const CEnginePrimitive2DSprite& p,unsigned long){action=3;box=p.box;graphic=p.index;layer=p.layer;}
};
struct CGraphicFrame{};static int loads,frameGets;
struct CGraphicInfoFrame{void BeginLoading(int,int)const{++loads;}void GetFrame(CGraphicFrame&,int,int)const{++frameGets;}};
struct Bank{CGraphicInfoFrame f;const CGraphicInfoFrame* PeekGraphicInfoFrame(unsigned long,int){return &f;}};
namespace NUISystem{
struct CComponent{};struct CManager{Bank bank;static CManager* GetInstance(){static CManager m;return &m;}Bank* GetGraphicsBank(){return &bank;}EEngine2DLayer GetLayer(long l){return(EEngine2DLayer)l;}};
struct CMorphingSprite:CComponent{
 CRGBColour RenderColour; C2DVector RenderZoom,RelativeRenderZoom,RelativeRenderPosition,Size;int NoRenderNextFrame,RenderCount,TargetState,Layer;unsigned long SpriteGraphicIndex;
 CEnginePrimitiveHandle Handle;std::list<CEnginePrimitiveHandle*> Handles;std::map<unsigned long,unsigned long> GraphicIndices;
 bool independent,layerIndependent;bool IsIndependent(){return independent;}bool IsLayerIndependent(){return layerIndependent;}
 ~CMorphingSprite(){for(std::list<CEnginePrimitiveHandle*>::iterator i=Handles.begin();i!=Handles.end();++i)delete *i;}
 void Draw(CIEngine&,CEnginePrimitiveHandle*,long,long*,CComponent*);
};
}
namespace NUISystem {
void CMorphingSprite::Draw(CIEngine& engine, CEnginePrimitiveHandle* parentHandle, long layer, long* childIndex, CComponent* parent)
{
    if (RenderColour.A > 0 && (!(RenderZoom.X <= 0.0f) || !(RenderZoom.Y <= 0.0f))) {
        NoRenderNextFrame = 0;
    } else {
        if (NoRenderNextFrame == 1) {
            if (RenderCount == 0) {
                if (parentHandle == 0) {
                    Handle.Clear();
                    return;
                }
            } else {
                if (Handles.size() >= (unsigned long)RenderCount) {
                    std::list<CEnginePrimitiveHandle*>::iterator it = Handles.begin();
                    const std::list<CEnginePrimitiveHandle*>::iterator it_end = Handles.end();
                    for (unsigned long i = 0; i < (unsigned long)(RenderCount - 1) && it != it_end; ++i)
                        ++it;
                    (*it)->Clear();
                    delete *it;
                    Handles.erase(it);
                } else {
                    Handles.push_back(new CEnginePrimitiveHandle);
                }
                if (parentHandle == 0)
                    return;
            }
            engine.ClearChildPrimitive(*parentHandle, (*childIndex)++);
            return;
        }
        NoRenderNextFrame = 1;
    }
    CEnginePrimitiveHandle* drawHandle;
    if (RenderCount > 0) {
        if (Handles.size() < (unsigned long)RenderCount) {
            drawHandle = new CEnginePrimitiveHandle;
            Handles.push_back(drawHandle);
        } else {
            std::list<CEnginePrimitiveHandle*>::iterator it = Handles.begin();
            const std::list<CEnginePrimitiveHandle*>::iterator it_end = Handles.end();
                    for (unsigned long i = 0; i < (unsigned long)(RenderCount - 1) && it != it_end; ++i)
                ++it;
            drawHandle = *it;
        }
    } else {
        drawHandle = &Handle;
    }
    ++RenderCount;
    C2DVector position((float)GFFloatToLongNear(RelativeRenderPosition.X), (float)GFFloatToLongNear(RelativeRenderPosition.Y));
    C2DVector size(Size.X * RelativeRenderZoom.X, Size.Y * RelativeRenderZoom.Y);
    C2DBoxF box(position.X, position.Y, position.X + size.X, position.Y + size.Y);
    CRGBColour colour = RenderColour;
    bool independent = IsIndependent();
    bool layerIndependent = IsLayerIndependent();
    EEngine2DLayer engineLayer;
    if (!independent && !layerIndependent)
        engineLayer = CManager::GetInstance()->GetLayer(Layer + layer);
    else
        engineLayer = CManager::GetInstance()->GetLayer(Layer);
    unsigned long graphicIndex = GraphicIndices[TargetState];
    if (SpriteGraphicIndex != 0)
        graphicIndex = SpriteGraphicIndex;
    if (graphicIndex > 0) {
        const CGraphicInfoFrame* frame = CManager::GetInstance()->GetGraphicsBank()->PeekGraphicInfoFrame(graphicIndex, 0);
        frame->BeginLoading(0, 0x10);
        CGraphicFrame graphicFrame;
        frame->GetFrame(graphicFrame, 0, 0x11);
    }
    CEnginePrimitive2DSprite sprite(0, box, engineLayer, graphicIndex, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_DEFAULT,
        0.0f, 0.0f, colour.A, colour.R, colour.G, colour.B, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    if (parentHandle && childIndex)
        engine.SetChildPrimitive(*parentHandle, (*childIndex)++, sprite, 0xc0);
    else
        engine.SetPersistentPrimitive(*drawHandle, sprite, 0xc0);
}
}
int main(){
 for(int k=0;k<8192;++k){NUISystem::CMorphingSprite s;CIEngine e;CEnginePrimitiveHandle h;
 int pass=(k>>4)%3,n=(k>>6)%3;bool parent=(k&1)!=0,hasIndex=parent || (k&2)!=0;
 s.RenderColour.R=15;s.RenderColour.G=24;s.RenderColour.B=45;s.RenderColour.A=(k&4)?0:129;
 s.RenderZoom=C2DVector((k&8)?-1.0f:1.0f,(k&512)?0.0f:1.0f);s.RelativeRenderZoom=C2DVector(1.5f,2.0f);
 s.RelativeRenderPosition=C2DVector(2.25f,-2.75f);s.Size=C2DVector(4,6);s.RenderCount=pass;s.NoRenderNextFrame=(k&256)?1:0;
 s.independent=(k&1024)!=0;s.layerIndependent=(k&2048)!=0;s.GraphicIndices[0]=(k&128)?0:9;s.SpriteGraphicIndex=(k&4096)?12:0;s.TargetState=0;s.Layer=3;
 for(int j=0;j<n;++j)s.Handles.push_back(new CEnginePrimitiveHandle);
 long index=5;loads=frameGets=0;bool visible=s.RenderColour.A>0&&(s.RenderZoom.X>0||s.RenderZoom.Y>0);bool clear=!visible&&s.NoRenderNextFrame;
 s.Draw(e,parent?&h:0,8,hasIndex?&index:0,0);
 int expected=clear?(parent?1:0):(parent&&hasIndex?2:3);
 if(e.action!=expected || index!=5+(expected==1||expected==2) || s.RenderCount!=pass+!clear)return 1;
 int desired=n;if(clear&&pass>0)desired=(n>=pass)?n-1:n+1;else if(!clear&&pass>0&&n<pass)++desired;
 if((int)s.Handles.size()!=desired || s.Handle.cleared!=(clear&&!pass&&!parent))return 2;
 if(!clear){unsigned long g=s.SpriteGraphicIndex?s.SpriteGraphicIndex:s.GraphicIndices[0];
 if(e.graphic!=g||loads!=(g!=0)||frameGets!=loads||e.layer!=((s.independent||s.layerIndependent)?3:11)||e.box.l!=2||e.box.t!=-3||e.box.r!=8||e.box.b!=9)return 3;}
 }
 puts("00554740_TEST PASS");return 0;
}