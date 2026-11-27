#include <math.h>
#include <stdio.h>
#include <stdlib.h>
struct C2DVector { float X,Y; C2DVector(float x=0,float y=0):X(x),Y(y){} };
struct C2DExtentsF : C2DVector { C2DExtentsF(float x,float y):C2DVector(x,y){} };
struct C2DBoxF { float TLX,TLY,BRX,BRY; C2DBoxF(const C2DVector&p,const C2DExtentsF&e):TLX(p.X),TLY(p.Y),BRX(p.X+e.X),BRY(p.Y+e.Y){} };
struct CRGBColour { unsigned char R,G,B,A; CRGBColour(unsigned char r=0,unsigned char g=0,unsigned char b=0,unsigned char a=0):R(r),G(g),B(b),A(a){} };
struct C3DVector {float X,Y,Z; C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}};
struct C3DBoundingSphere {C3DVector v;float r;C3DBoundingSphere(const C3DVector&v_,float r_):v(v_),r(r_){}};
static int errors;
static void check(bool c){if(!c)++errors;}
struct C3DOrientationInterpolator {C3DOrientationInterpolator(long a,long b){check(a==71&&b==71);}};
enum EEngineInterpolationSet { no_interp };
struct C3DAnimationState {unsigned long index;float time;C3DAnimationState(unsigned long i,float t):index(i),time(t){}};
struct C3DAnimationBlendState {void SetBaseAnimation(const C3DAnimationState&s){check(s.index==8&&s.time==0);}};
static long action, mesh_reads, bank_reads, submitted_index, actual_alpha; static float bounds[4];
enum { NONE, CLEAR_HANDLE, CLEAR_CHILD, STATIC_PERSISTENT, STATIC_CHILD, ANIM_PERSISTENT };
struct CEnginePrimitiveHandle {void Clear(){action=CLEAR_HANDLE;}};
static void capture(unsigned long mesh,const C2DBoxF&b,unsigned alpha,const CRGBColour&c){check(mesh==37&&c.R==255&&c.G==255&&c.B==255&&c.A==255);bounds[0]=b.TLX;bounds[1]=b.TLY;bounds[2]=b.BRX;bounds[3]=b.BRY;actual_alpha=alpha;}
struct CEnginePrimitive2DStaticMesh {CEnginePrimitive2DStaticMesh(unsigned __int64 id,unsigned long mesh,const C2DBoxF&b,const C3DOrientationInterpolator&,EEngineInterpolationSet i,unsigned char a,CRGBColour c){check(id==0&&i==0);capture(mesh,b,a,c);}};
struct CEnginePrimitive2DAnimatingMesh {CEnginePrimitive2DAnimatingMesh(unsigned __int64 id,unsigned long mesh,const C3DAnimationBlendState&,const C2DBoxF&b,const C3DBoundingSphere&s,const C3DOrientationInterpolator&,EEngineInterpolationSet i,unsigned char a,CRGBColour c){check(!id&&!i&&!s.v.X&&!s.v.Y&&!s.v.Z&&s.r==1);capture(mesh,b,a,c);}};
struct CIEngine {void ClearChildPrimitive(CEnginePrimitiveHandle&,long index){action=CLEAR_CHILD;submitted_index=index;}void SetChildPrimitive(CEnginePrimitiveHandle&,long index,const CEnginePrimitive2DStaticMesh&,int flags){check(flags==64);action=STATIC_CHILD;submitted_index=index;}void SetPersistentPrimitive(CEnginePrimitiveHandle&,const CEnginePrimitive2DStaticMesh&,int flags){check(flags==64);action=STATIC_PERSISTENT;}void SetPersistentPrimitive(CEnginePrimitiveHandle&,const CEnginePrimitive2DAnimatingMesh&,int flags){check(flags==64);action=ANIM_PERSISTENT;}};
static long GFFloatToLongNear(float v){double n=floor(v),d=v-n;if(d>.5 || (d==.5 && fmod(n,2)!=0))++n;return (long)n;}
namespace NUISystem {
struct CComponent {};
struct CUIScreenUnitConverter {static float width; static float GetBackBufferWidth(){return width;}};float CUIScreenUnitConverter::width;
struct MeshInfo{long type; long GetDataType(){return type;}};
struct MeshBank {bool exists;MeshInfo info;bool EntryExists(unsigned long i){check(i==37);return exists;}const MeshInfo* unused();MeshInfo* PeekMesh(unsigned long i){++mesh_reads;check(i==37);return &info;}};
struct HandleRef{MeshBank* b;MeshBank*Get(){return b;}};
struct CManager{MeshBank b;static CManager*GetInstance(){static CManager m;return &m;}HandleRef GetMeshBank(){++bank_reads;HandleRef h={&b};return h;}};
struct CMesh {CRGBColour RenderColour;C2DVector RenderZoom,RelativeRenderPosition,RelativeRenderZoom;long NoRenderNextFrame;unsigned long Width,Height,MeshIndex,AnimationIndex;long Orientation;CEnginePrimitiveHandle Handle;void Draw(CIEngine&,CEnginePrimitiveHandle*,long,long*,CComponent*);};
}
namespace NUISystem {
void CMesh::Draw(CIEngine& engine, CEnginePrimitiveHandle* p_handle, long layer, long* pprimitive_index, CComponent* pparent)
{
    if (RenderColour.A <= 0 || (RenderZoom.X <= 0.0f && RenderZoom.Y <= 0.0f)) {
        if (NoRenderNextFrame == 1) {
            if (p_handle)
                engine.ClearChildPrimitive(*p_handle, (*pprimitive_index)++);
            else
                Handle.Clear();
            return;
        }
        NoRenderNextFrame = 1;
    } else {
        NoRenderNextFrame = 0;
    }

    C2DVector position(RelativeRenderPosition);
    C2DVector real_position(RelativeRenderPosition);
    C2DVector render_zoom(RelativeRenderZoom);
    real_position.X -= Width * render_zoom.X * 0.5f;
    real_position.Y -= Height * render_zoom.Y * 0.5f;
    CRGBColour colour(RenderColour);
    C2DExtentsF extend(Width * render_zoom.X, Height * render_zoom.Y);
    C2DBoxF clip_box(real_position, extend);
    clip_box.BRX = (float)GFFloatToLongNear(clip_box.BRX);
    clip_box.BRY = (float)GFFloatToLongNear(clip_box.BRY);
    clip_box.TLX = (float)GFFloatToLongNear(clip_box.TLX);
    clip_box.TLY = (float)GFFloatToLongNear(clip_box.TLY);
    if (clip_box.TLX >= CUIScreenUnitConverter::GetBackBufferWidth() || clip_box.TLY >= CUIScreenUnitConverter::GetBackBufferWidth()
        || clip_box.BRX <= 0.0f || clip_box.BRY <= 0.0f) {
        Handle.Clear();
        return;
    }

    C2DBoxF clip_box_rel(real_position, extend);
    clip_box_rel.BRX = (float)GFFloatToLongNear(clip_box_rel.BRX);
    clip_box_rel.BRY = (float)GFFloatToLongNear(clip_box_rel.BRY);
    clip_box_rel.TLX = (float)GFFloatToLongNear(clip_box_rel.TLX);
    clip_box_rel.TLY = (float)GFFloatToLongNear(clip_box_rel.TLY);
    if (CManager::GetInstance()->GetMeshBank().Get()->EntryExists(MeshIndex)) {
        if (CManager::GetInstance()->GetMeshBank().Get()->PeekMesh(MeshIndex)->GetDataType() == 5) {
            C3DAnimationBlendState anim_blend;
            anim_blend.SetBaseAnimation(C3DAnimationState(AnimationIndex, 0.0f));
            C3DBoundingSphere bounding_sphere(C3DVector(0.0f, 0.0f, 0.0f), 1.0f);
            CEnginePrimitive2DAnimatingMesh primitive(0, MeshIndex, anim_blend, clip_box_rel, bounding_sphere,
                                                      C3DOrientationInterpolator(Orientation, Orientation), (EEngineInterpolationSet)0,
                                                      colour.A, CRGBColour(255, 255, 255, 255));
            engine.SetPersistentPrimitive(Handle, primitive, 0x40);
        } else {
            CEnginePrimitive2DStaticMesh primitive(0, MeshIndex, clip_box_rel, C3DOrientationInterpolator(Orientation, Orientation),
                                                   (EEngineInterpolationSet)0, colour.A, CRGBColour(255, 255, 255, 255));
            if (p_handle && pprimitive_index)
                engine.SetChildPrimitive(*p_handle, (*pprimitive_index)++, primitive, 0x40);
            else
                engine.SetPersistentPrimitive(Handle, primitive, 0x40);
        }
    }
}
}
static long refround(float x){long n=(long)x;float d=x-(float)n;if(d>.5f||(d==.5f&&(n%2)))++n;else if(d<-.5f||(d==-.5f&&(n%2)))--n;return n;}
int main(){using namespace NUISystem;CIEngine e;CEnginePrimitiveHandle parent;CMesh m;m.Width=5;m.Height=7;m.MeshIndex=37;m.AnimationIndex=8;m.Orientation=71;MeshBank&bank=CManager::GetInstance()->b;unsigned hits[6]={0};long total=0;
for(unsigned mask=0;mask<1024;++mask)for(int q=0;q<16;++q){m.RenderColour.A=(mask&1)?223:0;m.RenderZoom=C2DVector((mask&2)?1.0f:-1.0f,(mask&4)?1.0f:0.0f);m.RelativeRenderZoom=C2DVector((q&1)?2.0f:1.0f,(q&2)?1.0f:-1.0f);m.NoRenderNextFrame=(mask&8)?1:0;
bank.exists=(mask&16)!=0;bank.info.type=(mask&32)?5:7;bool par=(mask&64)!=0,index=(mask&128)!=0;
float px=(mask&256)?100.0f:(q&4)?-10.5f:8.5f,py=(mask&512)?120.0f:(q&8)?-8.5f:26.5f;m.RelativeRenderPosition=C2DVector(px,py);CUIScreenUnitConverter::width=96.0f;
long idx=19;bool invis=!m.RenderColour.A||(m.RenderZoom.X<=0 && m.RenderZoom.Y<=0);if(invis&&m.NoRenderNextFrame==1&&par&&!index)continue;
int outcome;int reads=0;int infos=0;float dx=m.Width*m.RelativeRenderZoom.X,dy=m.Height*m.RelativeRenderZoom.Y;
long b[4]={refround(px-dx/2),refround(py-dy/2),refround(px+dx/2),refround(py+dy/2)};
if(invis&&m.NoRenderNextFrame==1)outcome=par?CLEAR_CHILD:CLEAR_HANDLE;
else if(b[0]>=96||b[1]>=96||b[2]<=0||b[3]<=0)outcome=CLEAR_HANDLE;
else{reads=1;if(!bank.exists)outcome=NONE;else{++reads;++infos;outcome=bank.info.type==5?ANIM_PERSISTENT:(par&&index)?STATIC_CHILD:STATIC_PERSISTENT;}}
action=NONE;mesh_reads=bank_reads=0;submitted_index=-1;actual_alpha=-1;m.Draw(e,par?&parent:0,3,index?&idx:0,0);check(action==outcome&&m.NoRenderNextFrame==invis&&bank_reads==reads&&mesh_reads==infos);
bool child=outcome==CLEAR_CHILD||outcome==STATIC_CHILD;check(idx==(child?20:19)&&submitted_index==(child?19:-1));
if(outcome>=STATIC_PERSISTENT){check(actual_alpha==m.RenderColour.A);for(int i=0;i<4;++i)check(bounds[i]==b[i]);}
++hits[outcome];++total;
}for(int i=0;i<6;++i)check(hits[i]>0);check(total>15000);if(errors){printf("errors %d\n",errors);return 1;}puts("005502e0_TEST PASS");return 0;}