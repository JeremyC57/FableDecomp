#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
struct CCharString {char s[24];CCharString(const char* p=""){strcpy(s,p);} CCharString& operator+=(char c){size_t l=strlen(s);s[l]=c;s[l+1]=0;return *this;}bool operator==(const CCharString& b)const{return strcmp(s,b.s)==0;}};
namespace std {
template<class T> struct list{typedef const T* const_iterator;T values[8];unsigned n;list():n(0){}void push_back(const T& p){values[n++]=p;}const_iterator begin()const{return values;}const_iterator end()const{return values+n;}};
template<class T> struct vector{T values[2];unsigned n;vector():n(0){}void push_back(const T& p){values[n++]=p;}unsigned size()const{return n;}const T& operator[](unsigned i)const{return values[i];}};
}
struct C3DVector {float x,y; C3DVector(float a=0,float b=0):x(a),y(b){} C3DVector operator-(const C3DVector& q)const{return C3DVector(x-q.x,y-q.y);}float GetMagnitude()const{return (float)sqrt(x*x+y*y);}};
template<class T>struct CIntelligentPointer {T* p; CIntelligentPointer(T* x=0):p(x){} T* operator->()const{return p;}T* Get()const{return p;}bool operator!=(int zero)const{return zero==0 && p!=0;}};
template<class T>struct CDefPointer {T* p; CDefPointer():p(0){} T* operator->()const{return p;}};
struct CCrateStackDef {int StackType;};struct Def {CCrateStackDef def;void GetSubDef(CDefPointer<const CCrateStackDef>* p)const{p->p=&def;}};
struct CThing;struct CThingMarker;struct CTCCrateStack {CThing* added;int adds;CTCCrateStack():added(0),adds(0){}void AddCrate(CThing* p){added=p;++adds;}};
struct CThing {Def def;CTCCrateStack* stack;C3DVector pos;CThing():stack(0){}const Def& PeekBaseDef()const{return def;}const C3DVector& GetPos()const{return pos;}bool GetTC(CTCCrateStack** p){*p=stack;return stack!=0;}};
struct CTCPreCalculatedNavigationRoute {C3DVector dst;C3DVector GetRouteDestination()const{return dst;}};
enum {TCI_PRE_CALCULATED_NAVIGATION_ROUTE=166};
struct CThingMarker : CThing {CTCPreCalculatedNavigationRoute* route;CCharString name;bool HasTC(int n)const{return n==166&&route!=0;}const CCharString& GetScriptName()const{return name;}bool GetTC(CTCPreCalculatedNavigationRoute** p){*p=route;return route!=0;}};
struct CTCVillage {std::list<CIntelligentPointer<CThingMarker> > markers;const std::list<CIntelligentPointer<CThingMarker> >& PeekVillageMarkers()const{return markers;}};
struct CTCVillageMember {CTCVillage* village;CTCVillage* GetPVillage()const{return village;}};
struct CTCCarrying {CThing* carried;CThing* GetThingInPrimarySlot()const{return carried;}};
struct CThingAICreature : CThing {CTCVillageMember* village_member;CTCCarrying* carrying;bool PeekTC(const CTCVillageMember** p){*p=village_member;return *p!=0;}bool PeekTC(const CTCCarrying** p){*p=carrying;return *p!=0;}};
struct CVillageTaskCarryCrate {CThing* stack;bool valid,pick;bool IsValid()const{return valid;}CThing* GetCrateStack()const{return stack;}bool IsPickup()const{return pick;}};
struct CAIFilter_ShouldStillBeWorkingInWarehouse {CVillageTaskCarryCrate* task;CThingAICreature* creature;const CTCVillage* village;CAIFilter_ShouldStillBeWorkingInWarehouse(CIntelligentPointer<CVillageTaskCarryCrate> t,CIntelligentPointer<CThingAICreature> c,const CTCVillage* v):task(t.Get()),creature(c.Get()),village(v){}};
namespace { CCharString PICK_UP_ANIM_NAME_LEVEL1("p1"),PICK_UP_ANIM_NAME_LEVEL2("p2"),PUT_CRATE_IN_PILE_ANIM_NAME_LEVEL1("s1"),PUT_CRATE_IN_PILE_ANIM_NAME_LEVEL2("s2"); }
struct CAIStateGroup_WorkInWarehouse {
    CIntelligentPointer<CVillageTaskCarryCrate> PTask;
    CThingAICreature creature; bool result; int calls;
    const CThing* selected; const CTCVillage* v; CThing* stack;
    CThingAICreature& GetCreature(){return creature;}
    bool MoveCrates();
    bool act(bool pickup,CIntelligentPointer<CThing> s,float dist,const std::vector<const CCharString*>& anims,const CAIFilter_ShouldStillBeWorkingInWarehouse& f,const CThing* marker){
        ++calls; selected=marker;
        if(pickup!=PTask->pick||dist!=4.0f||s.Get()!=stack||f.task!=PTask.Get()||f.creature!=&creature||f.village!=(pickup?v:0)||anims.size()!=2)abort();
        if(anims[0]!=(pickup?&PICK_UP_ANIM_NAME_LEVEL1:&PUT_CRATE_IN_PILE_ANIM_NAME_LEVEL1)||anims[1]!=(pickup?&PICK_UP_ANIM_NAME_LEVEL2:&PUT_CRATE_IN_PILE_ANIM_NAME_LEVEL2))abort();
        return result;
    }
    bool FetchGoods(CIntelligentPointer<CThing> s,float dist,std::vector<const CCharString*> a,const CAIFilter_ShouldStillBeWorkingInWarehouse& f,const CThing* m){return act(true,s,dist,a,f,m);}
    bool PlaceGoods(CIntelligentPointer<CThing> s,float dist,std::vector<const CCharString*> a,const CAIFilter_ShouldStillBeWorkingInWarehouse& f,const CThing* m){return act(false,s,dist,a,f,m);}
};
bool CAIStateGroup_WorkInWarehouse::MoveCrates()
{
    if (PTask!=0 && PTask->IsValid()) {
        const CTCVillageMember* ptc_village_member;
        GetCreature().PeekTC(&ptc_village_member);
        CTCVillage* ptc_village=ptc_village_member->GetPVillage();
        const CThing* precalc_thing=0;
        CDefPointer<const CCrateStackDef> crate_stack_def;
        PTask->GetCrateStack()->PeekBaseDef().GetSubDef(&crate_stack_def);
        char stack_type_char='0'+crate_stack_def->StackType;
        CCharString script_name_to_look_for("Crate");
        script_name_to_look_for+=stack_type_char;
        typedef std::list<CIntelligentPointer<CThingMarker> >::const_iterator iterator;
        for (iterator it=ptc_village->PeekVillageMarkers().begin(),end_it=ptc_village->PeekVillageMarkers().end(); it!=end_it; ++it) {
            CThingMarker* pmarker=it->Get();
            if (pmarker && pmarker->HasTC(TCI_PRE_CALCULATED_NAVIGATION_ROUTE) && pmarker->GetScriptName()==script_name_to_look_for) {
                CTCPreCalculatedNavigationRoute* ptc_pre_calc_route=0;
                pmarker->GetTC(&ptc_pre_calc_route);
                float distance_to_target=(ptc_pre_calc_route->GetRouteDestination()-GetCreature().GetPos()).GetMagnitude();
                float beginnings_distance_to_target=(ptc_pre_calc_route->GetRouteDestination()-pmarker->GetPos()).GetMagnitude();
                const float MULTIPLIER_FOR_DISTANCE=1.2f;
                if (beginnings_distance_to_target<distance_to_target*MULTIPLIER_FOR_DISTANCE) {precalc_thing=pmarker;break;}
            }
        }
        CThing* crate_stack=PTask->GetCrateStack();
        CTCCrateStack* ptc_crate_stack=0;
        crate_stack->GetTC(&ptc_crate_stack);
        if (PTask->IsPickup()) {
            std::vector<const CCharString*> anims;
            anims.push_back(&PICK_UP_ANIM_NAME_LEVEL1);
            anims.push_back(&PICK_UP_ANIM_NAME_LEVEL2);
            if (FetchGoods(crate_stack,4.0f,anims,CAIFilter_ShouldStillBeWorkingInWarehouse(PTask,&GetCreature(),ptc_village),precalc_thing)) return true;
        } else {
            const CTCCarrying* ptc_carrying;
            GetCreature().PeekTC(&ptc_carrying);
            CIntelligentPointer<CThing> pcrate(ptc_carrying->GetThingInPrimarySlot());
            std::vector<const CCharString*> anims;
            anims.push_back(&PUT_CRATE_IN_PILE_ANIM_NAME_LEVEL1);
            anims.push_back(&PUT_CRATE_IN_PILE_ANIM_NAME_LEVEL2);
            if (PlaceGoods(crate_stack,4.0f,anims,CAIFilter_ShouldStillBeWorkingInWarehouse(PTask,&GetCreature(),0),precalc_thing)) {
                if (ptc_crate_stack) ptc_crate_stack->AddCrate(pcrate.Get());
                return true;
            }
        }
    }
    return false;
}

int main(){
 for(int flags=0;flags<128;++flags) for(int which=0;which<10;++which) for(int type=0;type<10;++type){
    bool has_task=(flags&1)!=0,valid=(flags&2)!=0,pick=(flags&4)!=0,success=(flags&8)!=0,has_stack=(flags&16)!=0,has_carried=(flags&32)!=0,empty=(flags&64)!=0;
    CAIStateGroup_WorkInWarehouse w;CVillageTaskCarryCrate task;CThing stack,carried;CTCCrateStack crate;CTCCarrying carry;
    CTCVillage v;CTCVillageMember member;member.village=&v;
    w.calls=0;w.selected=0;w.v=&v;w.stack=&stack;w.result=success;w.PTask=has_task?&task:0;
    task.stack=&stack;task.pick=pick;task.valid=valid;stack.def.def.StackType=type;stack.stack=has_stack?&crate:0;carry.carried=has_carried?&carried:0;
    w.creature.village_member=&member;w.creature.carrying=&carry;w.creature.pos=C3DVector(3,4);
    CThingMarker markers[4];CTCPreCalculatedNavigationRoute route;route.dst=C3DVector(0,0);CThing* expected=0;
    char name[]="Crate0";name[5]=(char)('0'+type);
    for(int i=0;i<4;++i){
        int k=(which+i*3)%10;
        markers[i].name=k==0?"Other":name;
        markers[i].route=k==1?0:&route;
        markers[i].pos=C3DVector((float)(k==2?6:k==3?7:k==4?5:k%8),0);
        if(!empty){v.markers.push_back((CThingMarker*)0);v.markers.push_back(&markers[i]);}
        if(!empty&&!expected&&k!=0&&k!=1&&(double)markers[i].pos.x<5.0*(double)1.2f)expected=&markers[i];
    }
    bool active=has_task&&valid,add=active&&!pick&&success&&has_stack;
    if(w.MoveCrates()!=(active&&success)||w.calls!=(active?1:0)||w.selected!=(active?expected:0)||crate.adds!=(add?1:0)||crate.added!=(add&&has_carried?&carried:0)){printf("case %d %d %d calls %d adds %d marker %d\n",flags,which,type,w.calls,crate.adds,w.selected==expected);return 1;}
 }
 puts("00943cf0_TEST PASS");return 0;
}