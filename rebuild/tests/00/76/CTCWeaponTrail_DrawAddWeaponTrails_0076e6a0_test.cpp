
#include <stdio.h>
#include <math.h>
struct C3DVector {float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){} C3DVector operator-(const C3DVector& o)const{return C3DVector(X-o.X,Y-o.Y,Z-o.Z);}float GetSquaredMagnitude()const{return X*X+Y*Y+Z*Z;}};
static float GFSqr(float x){return x*x;}static void GFLimit(float& v,float lo,float hi){if(v<lo)v=lo;else if(v>hi)v=hi;}
struct CGlobalDef {long GraphicWeaponTrailArrow,WeaponTrailArrowAlpha,WeaponTrailAlpha,WeaponTrialNumFramesToFadeOut;float WeaponTrailArrowAlphaFadeOffTime,WeaponTrailAlphaFadeOffTime;};
struct CWorld {C3DVector lod;C3DVector GetGameLODCentre()const{return lod;}long GetFrame()const{return 45;}};
struct CMainGameComponent {CWorld w;static CMainGameComponent* Get(){static CMainGameComponent x;return &x;}CWorld& GetWorld(){return w;}static long GetConstantFPS(){return 30;}};
namespace NThingShot {struct CTCShotDefinitionBase {long idx;long GetWeaponTrailGraphic()const{return idx;}};}
struct BaseDef{float RenderFadeDistance;bool DrawProjectileWeaponTrails,DrawWeaponTrails;};
struct DefRef{const BaseDef* p;bool IsNull()const{return !p;}const BaseDef* operator->()const{return p;}};
struct CThingGameObject {int graphic;long GetWeaponTrailGraphicIndex()const{return graphic;}};
struct Thing: CThingGameObject {BaseDef def;bool has,shot;NThingShot::CTCShotDefinitionBase tc;C3DVector pos;
 DefRef GetPBaseDef()const{DefRef v={has?&def:0};return v;}const BaseDef& PeekBaseDef()const{return def;}const C3DVector& GetPos()const{return pos;}bool PeekTC(const NThingShot::CTCShotDefinitionBase** out)const{if(shot)*out=&tc;return shot;}unsigned long GetUniqueID()const{return 9987;}};
static const CThingGameObject* GFDebugDynamicCast(const Thing* p,const CThingGameObject*){return p;}
struct Handle {int writes,clears;Handle():writes(0),clears(0){}void Clear(){++clears;}};
struct Handles {Handle items[4];int count,clears;Handles():count(0),clears(0){}void clear(){count=0;++clears;}void resize(int n){count=n;}Handle& operator[](int i){return items[i];}};
struct TrailList{int id;bool present;bool empty()const{return !present;}};
enum {ENGINE_INTERPOLATION_SET_DEFAULT=0};static long NO_TRAIL_ELEMENTS_PER_WORLD_UPDATE=5;
struct CEnginePrimitiveWeaponTrail {enum {WEAPON_TRAIL_TYPE_PROJECTILE=1};unsigned long uid;long id,gfx,frame,type,alpha,e;float skip,now,fade,last;
 CEnginePrimitiveWeaponTrail(unsigned long u,const TrailList& t,long g,long f,long ty,unsigned char al,float sk,float tm,float ft,float la,int se):uid(u),id(t.id),gfx(g),frame(f),type(ty),alpha(al),e(se),skip(sk),now(tm),fade(ft),last(la){}
};
struct Logged{int id,gfx,type,alpha;float skip,fade;};struct EngineType{Logged rows[4];int count,errors;void SetPersistentPrimitive(Handle& h,const CEnginePrimitiveWeaponTrail& t,int z){++h.writes;if(t.uid!=9987||t.now!=1.5f||t.frame!=0||t.e||t.last!=0||z)++errors;Logged& l=rows[count++];l.id=t.id;l.gfx=t.gfx;l.type=t.type;l.alpha=t.alpha;l.skip=t.skip;l.fade=t.fade;}};
struct CEnginePrimitivePrepareInfo {struct {struct {float WFInterpolate;}Current;}Interpolate;EngineType& Engine;CEnginePrimitivePrepareInfo(EngineType& e):Engine(e){}};
struct Manager {CGlobalDef gd;const CGlobalDef& PeekGlobalDef()const{return gd;}};
struct CTCWeaponTrail {Thing thing;Manager m;mutable Handle ProjectileWeaponTrailHandle;mutable Handles WeaponTrailHandles;TrailList ProjectileWeaponTrail,Trail[3];int NoTrails,TrailType,InactiveTicks[3];bool ActiveTrail[3];const Thing& GetParentThing()const{return thing;}const CWorld& PeekWorld()const{return CMainGameComponent::Get()->w;}const Manager& GetDefinitionManager()const{return m;}void DrawAddWeaponTrails(const CEnginePrimitivePrepareInfo&)const;};
void CTCWeaponTrail::DrawAddWeaponTrails(const CEnginePrimitivePrepareInfo& info) const {
    if(!GetParentThing().GetPBaseDef().IsNull()) {
        C3DVector lod_centre=CMainGameComponent::Get()->GetWorld().GetGameLODCentre();
        if((GetParentThing().GetPos()-lod_centre).GetSquaredMagnitude()>GFSqr(GetParentThing().GetPBaseDef()->RenderFadeDistance)) {
            ProjectileWeaponTrailHandle.Clear();
            WeaponTrailHandles.clear();
            return;
        }
    }
    float current_time=((float)PeekWorld().GetFrame())/CMainGameComponent::GetConstantFPS();
    const CGlobalDef& glob_def=GetDefinitionManager().PeekGlobalDef();
    ProjectileWeaponTrailHandle.Clear();
    WeaponTrailHandles.clear();
    if(!ProjectileWeaponTrail.empty()) {
        if(GetParentThing().PeekBaseDef().DrawProjectileWeaponTrails) {
            long gfxweapontrail=glob_def.GraphicWeaponTrailArrow;
            const NThingShot::CTCShotDefinitionBase* ptc_thing_shot_base;
            if(GetParentThing().PeekTC(&ptc_thing_shot_base)) gfxweapontrail=ptc_thing_shot_base->GetWeaponTrailGraphic();
            float interpolate=info.Interpolate.Current.WFInterpolate;
            float skip_points=1.0f-interpolate;
            if(gfxweapontrail>0) info.Engine.SetPersistentPrimitive(ProjectileWeaponTrailHandle,CEnginePrimitiveWeaponTrail(GetParentThing().GetUniqueID(),ProjectileWeaponTrail,gfxweapontrail,0,CEnginePrimitiveWeaponTrail::WEAPON_TRAIL_TYPE_PROJECTILE,(unsigned char)glob_def.WeaponTrailArrowAlpha,skip_points,current_time,glob_def.WeaponTrailArrowAlphaFadeOffTime,0.0f,ENGINE_INTERPOLATION_SET_DEFAULT),0);
        }
    } else if(GetParentThing().PeekBaseDef().DrawWeaponTrails && NoTrails>0) {
        const CThingGameObject& game_object=*GFDebugDynamicCast(&GetParentThing(),(const CThingGameObject*)0);
        long weapon_trail_index=game_object.GetWeaponTrailGraphicIndex();
        WeaponTrailHandles.resize(NoTrails);
        for(long c1=0;c1<NoTrails;++c1) if(!Trail[c1].empty() && weapon_trail_index>0) {
            unsigned long frame=0;
            float alpha_interp=1.0f,skip_points;
            if(ActiveTrail[c1]) {
                float interpolate=info.Interpolate.Current.WFInterpolate;
                skip_points=(1.0f-interpolate)*NO_TRAIL_ELEMENTS_PER_WORLD_UPDATE;
            } else {
                skip_points=0.0f;
                alpha_interp=1.0f-(float)InactiveTicks[c1]/glob_def.WeaponTrialNumFramesToFadeOut;
            }
            GFLimit(alpha_interp,0.0f,1.0f);
            info.Engine.SetPersistentPrimitive(WeaponTrailHandles[c1],CEnginePrimitiveWeaponTrail(GetParentThing().GetUniqueID(),Trail[c1],weapon_trail_index,frame,TrailType,(unsigned char)(glob_def.WeaponTrailAlpha*alpha_interp),skip_points,current_time,glob_def.WeaponTrailAlphaFadeOffTime,0.0f,ENGINE_INTERPOLATION_SET_DEFAULT),0);
        }
    }
}

int main(){
 for(int mask=0;mask<256;++mask)for(int phase=0;phase<4;++phase)for(int gi=-1;gi<3;++gi)for(int count=0;count<=3;++count){
 CTCWeaponTrail t;EngineType e;e.count=e.errors=0;CEnginePrimitivePrepareInfo info(e);float f=phase*0.25f;info.Interpolate.Current.WFInterpolate=f;
 t.thing.has=(mask&1)!=0;t.thing.def.RenderFadeDistance=3;t.thing.pos=C3DVector(mask&2?5:3,0,0);t.thing.def.DrawProjectileWeaponTrails=(mask&4)!=0;t.thing.def.DrawWeaponTrails=(mask&8)!=0;t.thing.shot=(mask&16)!=0;t.thing.tc.idx=gi;t.thing.graphic=gi;t.m.gd.GraphicWeaponTrailArrow=2;t.m.gd.WeaponTrailArrowAlpha=70;t.m.gd.WeaponTrailAlpha=160;t.m.gd.WeaponTrialNumFramesToFadeOut=4;t.m.gd.WeaponTrailArrowAlphaFadeOffTime=2.25f;t.m.gd.WeaponTrailAlphaFadeOffTime=3.5f;t.TrailType=3;t.NoTrails=count;t.ProjectileWeaponTrail.present=(mask&32)!=0;t.ProjectileWeaponTrail.id=99;
 for(int i=0;i<3;++i){t.Trail[i].id=i;t.Trail[i].present=(mask&(64<<(i%2)))!=0;t.ActiveTrail[i]=((phase+i)%3)==0;t.InactiveTicks[i]=(phase+i)*2-1;}
 t.DrawAddWeaponTrails(info);if(e.errors||t.ProjectileWeaponTrailHandle.clears!=1||t.WeaponTrailHandles.clears!=1)return 1;
 bool pass=!(t.thing.has&&(mask&2));bool melee=pass&&!t.ProjectileWeaponTrail.present&&t.thing.def.DrawWeaponTrails&&count>0;int expected=0;
 if(t.WeaponTrailHandles.count!=(melee?count:0))return 2;
 if(pass&&t.ProjectileWeaponTrail.present&&t.thing.def.DrawProjectileWeaponTrails&&(t.thing.shot?gi:2)>0){Logged& l=e.rows[expected++];if(l.id!=99||l.gfx!=(t.thing.shot?gi:2)||l.type!=1||l.alpha!=70||l.skip!=1-f||l.fade!=2.25f)return 3;}
 if(t.ProjectileWeaponTrailHandle.writes!=expected)return 4;
 for(int j=0;j<count;++j){bool draw=melee&&gi>0&&t.Trail[j].present;if(t.WeaponTrailHandles.items[j].writes!=int(draw))return 5;if(!draw)continue;Logged& l=e.rows[expected++];float af=t.ActiveTrail[j]?1.f:(4-t.InactiveTicks[j])*.25f;if(af<0)af=0;if(af>1)af=1;if(l.id!=j||l.gfx!=gi||l.type!=3||l.alpha!=int(160*af)||l.skip!=(t.ActiveTrail[j]?(1-f)*5:0)||l.fade!=3.5f)return 6;}
 if(e.count!=expected)return 7;
 }puts("0076e6a0_TEST PASS");return 0;}