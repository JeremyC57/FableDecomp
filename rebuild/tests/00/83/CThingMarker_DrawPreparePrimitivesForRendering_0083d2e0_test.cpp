#include <stdio.h>
#include <stdlib.h>
static void check(bool ok) { if (!ok) exit(2); }
static int events[30], n, effects;
static void event(int x) { events[n++]=x; }
struct C3DVector { int value; C3DVector(int v=0):value(v){} };
struct CRightHandedSet { int value; CRightHandedSet(int v=0):value(v){} };
struct CRGBColour {
    unsigned char r,g,b,a;
    CRGBColour(unsigned char R,unsigned char G,unsigned char B,unsigned char A):r(R),g(G),b(B),a(A){}
};
struct CEnginePrimitiveThemeChannelDesc {};
enum EEngineInterpolationSet { ENGINE_INTERPOLATION_SET_DEFAULT=0 };
struct FixtureState { bool base_ok,editor,appearance,hold_effect,use_appearance,has_bank; } state;
struct CEnginePrimitiveStaticMesh {
    unsigned __int64 uid; unsigned long mesh; int pos,rhs; float size;
    CEnginePrimitiveStaticMesh(unsigned __int64 id,unsigned long m,const C3DVector& p,const CRightHandedSet& r,float s,bool lit,unsigned char alpha,CRGBColour c,float fade,EEngineInterpolationSet e,const CEnginePrimitiveThemeChannelDesc&,bool bullet):uid(id),mesh(m),pos(p.value),rhs(r.value),size(s) {
        event(7);check(lit&&alpha==255&&c.r==255&&c.g==255&&c.b==255&&c.a==255&&fade==0&&e==ENGINE_INTERPOLATION_SET_DEFAULT&&bullet);
    }
    ~CEnginePrimitiveStaticMesh(){ if(state.hold_effect){check(effects==1);--effects;event(9);} }
};
struct CIEngine { void AddPrimitive(const CEnginePrimitiveStaticMesh& m) {
    event(8); bool alt=state.use_appearance&&state.has_bank;check(m.uid==0xabcdef0187654321ui64&&m.mesh==(alt?13u:9u)&&m.size==(alt?8.0f:5.0f)&&m.pos==31&&m.rhs==41);if(state.hold_effect)++effects;
} };
struct CEnginePrimitivePrepareInfo { struct Interp { struct Curr { float WFInterpolate; } Current; } Interpolate; CIEngine& Engine; CEnginePrimitivePrepareInfo(CIEngine& engine):Engine(engine){} };
struct CMainGameComponent { bool IsEditorActive() const {event(2);return state.editor;} };
struct CTCPhysicsBase {
    C3DVector prev_pos,cur_pos;CRightHandedSet prev_rhs,cur_rhs;
    CTCPhysicsBase():prev_pos(21),cur_pos(22),prev_rhs(51),cur_rhs(52){}
    C3DVector GetPosition(float t){event(6);check(t==0.25f);return C3DVector(31);}
    CRightHandedSet GetRHSet(float t){event(5);check(t==0.25f);return CRightHandedSet(41);}
    const C3DVector& GetLastPosition(){return prev_pos;} const C3DVector& GetPosition(){return cur_pos;}
    const CRightHandedSet& GetLastRHSet(){return prev_rhs;} const CRightHandedSet& GetRHSet(){return cur_rhs;}
};
struct C3DPositionInterpolator { int a,b; C3DPositionInterpolator(const C3DVector& x,const C3DVector& y):a(x.value),b(y.value){event(10);}~C3DPositionInterpolator(){event(14);} };
struct C3DOrientationInterpolator { int a,b; C3DOrientationInterpolator(const CRightHandedSet& x,const CRightHandedSet& y):a(x.value),b(y.value){event(11);}~C3DOrientationInterpolator(){event(13);} };
namespace NGraphicAppearance { struct CTCGraphicAppearance {void Draw(const CEnginePrimitivePrepareInfo& info,const C3DPositionInterpolator& p,const C3DOrientationInterpolator& r){event(12);check(p.a==21&&p.b==22&&r.a==51&&r.b==52&&info.Interpolate.Current.WFInterpolate==0.25f);} }; }
struct CEngineGraphic {unsigned long BankIndex;float RenderSizeX;};
struct CAppearanceDef { CEngineGraphic Graphic; };
template<class T> struct CDefPointer {T* p;CDefPointer():p(0){} T* operator->()const{return p;}};
struct BaseDef {CAppearanceDef appearance;bool GetSubDef(CDefPointer<const CAppearanceDef>* out){event(3);out->p=state.use_appearance?&appearance:0;return state.use_appearance;}};
struct CThingPhysical {bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&){event(1);return state.base_ok;}};
struct CThingMarker:CThingPhysical {
    BaseDef* PBaseDef; CAppearanceDef fallback;float Scale;CMainGameComponent main;CTCPhysicsBase physics;NGraphicAppearance::CTCGraphicAppearance ap;
    CThingMarker(BaseDef& b):PBaseDef(&b),Scale(2){fallback.Graphic.BankIndex=9;fallback.Graphic.RenderSizeX=2.5f;}
    const CAppearanceDef* PeekDefinition()const{event(4);return &fallback;}
    unsigned __int64 GetUniqueID()const{return 0xabcdef0187654321ui64;}
    CTCPhysicsBase* GetPhysicsTC(){return &physics;}
    NGraphicAppearance::CTCGraphicAppearance* GetTCGraphicAppearance(){return state.appearance?&ap:0;}
    const CMainGameComponent& PeekComponent()const{return main;}
    CEngineGraphic GetCurrentGraphic()const;
    bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);
};
bool CThingMarker::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    if (CThingPhysical::DrawPreparePrimitivesForRendering(info)) {
    if (PeekComponent().IsEditorActive()) {
        CEngineGraphic def_graphic = GetCurrentGraphic();
        info.Engine.AddPrimitive(CEnginePrimitiveStaticMesh(GetUniqueID(),def_graphic.BankIndex,
            GetPhysicsTC()->GetPosition(info.Interpolate.Current.WFInterpolate),
            GetPhysicsTC()->GetRHSet(info.Interpolate.Current.WFInterpolate),
            Scale * def_graphic.RenderSizeX,true,255,CRGBColour(255,255,255,255),0,ENGINE_INTERPOLATION_SET_DEFAULT,CEnginePrimitiveThemeChannelDesc(),true));
    }
    NGraphicAppearance::CTCGraphicAppearance* ptc_appearance = GetTCGraphicAppearance();
    if (ptc_appearance) {
        C3DPositionInterpolator interpolated_pos(GetPhysicsTC()->GetLastPosition(),GetPhysicsTC()->GetPosition());
        C3DOrientationInterpolator interpolated_rhs(GetPhysicsTC()->GetLastRHSet(),GetPhysicsTC()->GetRHSet());
        ptc_appearance->Draw(info,interpolated_pos,interpolated_rhs);
    }
    return true;
    }
    return false;
}

CEngineGraphic CThingMarker::GetCurrentGraphic() const
{
    CDefPointer<const CAppearanceDef> appearance;
    if (PBaseDef->GetSubDef(&appearance) && appearance->Graphic.BankIndex > 0)
        return appearance->Graphic;
    return PeekDefinition()->Graphic;
}
int main(){
    for(unsigned mask=0;mask<64;++mask){
        state.base_ok=(mask&1)!=0;state.editor=(mask&2)!=0;state.appearance=(mask&4)!=0;state.hold_effect=(mask&8)!=0;state.use_appearance=(mask&16)!=0;state.has_bank=(mask&32)!=0;
        n=effects=0;BaseDef b;b.appearance.Graphic.BankIndex=state.has_bank?13:0;b.appearance.Graphic.RenderSizeX=4;
        CThingMarker marker(b);CIEngine engine;CEnginePrimitivePrepareInfo info(engine);info.Interpolate.Current.WFInterpolate=0.25f;
        check(marker.DrawPreparePrimitivesForRendering(info)==state.base_ok);
        int expect[20],k=0;expect[k++]=1;
        if(state.base_ok){expect[k++]=2;if(state.editor){expect[k++]=3;if(!state.use_appearance||!state.has_bank)expect[k++]=4;
            expect[k++]=5;expect[k++]=6;expect[k++]=7;expect[k++]=8;if(state.hold_effect)expect[k++]=9;}
            if(state.appearance)for(int e=10;e<=14;++e)expect[k++]=e;}
        check(n==k&&effects==0);for(int j=0;j<n;++j)check(events[j]==expect[j]);
    }
    puts("0083d2e0_TEST PASS");return 0;
}