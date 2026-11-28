// This fixture exercises the production method body with deterministic external services.
// Vector arithmetic and orientation use the shared engine types.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ROTATE_ANGLES
#define FABLE_RETAIL_CRIGHTHANDEDSET
#include "engine/CRightHandedSet.h"
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#include "stlport/fable_containers.h"
#include "stlport/fable_algorithm.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define FABLE_RETAIL_ASSERT(condition) ((void)0)

namespace BedFixture {
static void check(bool condition) { if (!condition) abort(); }
struct Counters {
    int home, position, type, wife, lockable, locked, tavern, house, action_query, action, occupy, rotate, eligible;
    Counters() : home(0), position(0), type(0), wife(0), lockable(0), locked(0), tavern(0), house(0),
                 action_query(0), action(0), occupy(0), rotate(0), eligible(0) {}
};
static Counters calls;
}
void C3DVector::Rotate(float xy,float yz)
{
    BedFixture::check(xy==0.25f && yz==0 && Y==0 && Z==0 && X==1);
    ++BedFixture::calls.rotate;
    float old_x=X; X=Y; Y=-old_x;
}
namespace BedFixture {

struct FixtureOrientation : public ::CRightHandedSet {
    FixtureOrientation(const C3DVector& u=C3DVector(0,0,1),const C3DVector& f=C3DVector(1,0,0)) {Up=u; Forward=f;}
};
struct FixtureThing;
struct FixtureBuilding;
struct FixtureCreature;
struct FixturePoints {
    bool available;
    bool GetFreeActionPoint(FixtureThing& creature,C3DVector& pos,FixtureOrientation& orientation,long& index);
};
struct FixtureOccupier {
    FixtureCreature* occupier;
    FixtureOccupier() : occupier(0) {}
    void SetOccupier(FixtureCreature* p) {check(p!=0 && !occupier); ++calls.occupy; occupier=p;}
};
struct Physics {
    FixtureOrientation orientation;
    const FixtureOrientation& GetRHSet() const {return orientation;}
};
template <class T> struct FixturePointer {
    T* value;
    FixturePointer(T* p=0):value(p){}
    T* operator->() const {check(value!=0); return value;}
    bool operator==(int null_value) const {check(null_value==0);return value==0;}
};
struct FixtureThing {
    C3DVector pos; bool dead,enabled;
    FixturePoints* points;
    FixtureOccupier* marker;
    Physics physics;
    FixtureThing() : dead(false),enabled(true),points(0),marker(0) {}
    const C3DVector& GetPos() const {++calls.position;return pos;}
    bool GetTC(FixturePoints** r) {++calls.action_query;if(points)*r=points; return points!=0;}
    bool GetTC(FixtureOccupier** r) {if(marker)*r=marker;return marker!=0;}
    Physics* GetPhysicsTC() {return &physics;}
};
template<class T> bool IS_THING_ALIVE(const T& p) {return !(p==0) && !p->dead;}
struct FixturePredicate {
    bool operator()(const FixturePointer<FixtureThing>& p) const {++calls.eligible;return p.value && p.value->enabled;}
};
struct FixtureOwner {
    std::list<FixturePointer<FixtureThing> > objects;
    std::list<FixturePointer<FixtureThing> >& GetObjectList(){return objects;}
};
enum {TCI_TAVERN=171,HUMAN_CHILD=1};
struct FixtureBuilding : FixtureThing {
    bool lockable,locked,tavern;
    FixtureOwner* owner;
    FixtureBuilding():lockable(true),locked(false),tavern(false),owner(0){}
    bool IsLockable() const {++calls.lockable;return lockable;}
    bool IsLocked() const {++calls.locked;return locked;}
    bool HasTC(int id) const {check(id==TCI_TAVERN);++calls.tavern;return tavern;}
    bool GetTC(FixtureOwner** p){++calls.house;if(owner)*p=owner;return owner!=0;}
};
struct FixtureWife {
    bool married;
    bool IsMarriedToThePlayer() const {++calls.wife;return married;}
};
struct FixtureCreature: FixtureThing {
    FixtureBuilding* home; int category; FixtureWife* wife;
    FixtureCreature():home(0),category(2),wife(0){}
    FixtureBuilding* GetHomeBuilding() const {++calls.home;return home;}
    int GetCreatureType() const {++calls.type;return category;}
    bool GetTC(FixtureWife** p){if(wife)*p=wife;return wife!=0;}
};
static FixtureThing* expected_creature;
bool FixturePoints::GetFreeActionPoint(FixtureThing& creature,C3DVector& pos,FixtureOrientation& orientation,long& index)
{
    check(&creature==expected_creature);++calls.action;
    if(!available)return false;
    pos=C3DVector(3,5,7); orientation=FixtureOrientation(C3DVector(0,0,-1),C3DVector(0,1,0));index=23;return true;
}
struct FixtureState {
    FixtureCreature* creature; FixturePointer<FixtureThing> PBed;
    bool IsBedSleepPointMarker, BedToRightOfCreature; long ActionPointIndex; C3DVector BedPos; FixtureOrientation Rhset;
    FixtureState(FixtureCreature* p):creature(p),IsBedSleepPointMarker(true),BedToRightOfCreature(true),ActionPointIndex(-7),BedPos(17,19,23){}
    FixtureCreature& GetCreature(){return *creature;}
    bool FindFreeBed(bool during_auto_placement);
};
bool FixtureState::FindFreeBed(bool during_auto_placement)
{
    IsBedSleepPointMarker = false;
    FixtureBuilding* pbuilding = GetCreature().GetHomeBuilding();
    C3DVector near_to = GetCreature().GetPos();
    FixtureWife* tc_wife;
    if (IS_THING_ALIVE(pbuilding) && (GetCreature().GetCreatureType() == HUMAN_CHILD
        || (GetCreature().GetTC(&tc_wife) && tc_wife->IsMarriedToThePlayer())
        || !pbuilding->IsLockable() || pbuilding->IsLocked()
        || during_auto_placement || pbuilding->HasTC(TCI_TAVERN))) {
        PBed = FixturePointer<FixtureThing>(0);
        FABLE_RETAIL_ASSERT(IS_THING_ALIVE(pbuilding));
        FixtureOwner* ptc_thing_owner;
        if (pbuilding->GetTC(&ptc_thing_owner)) {
            float best = 0.0f;
            const std::list<FixturePointer<FixtureThing> >& house_objects = ptc_thing_owner->GetObjectList();
            std::list<FixturePointer<FixtureThing> >::const_iterator bed_it = house_objects.begin();
            while (bed_it != house_objects.end()) {
                bed_it = std::find_if(bed_it, house_objects.end(), FixturePredicate());
                if (bed_it == house_objects.end())
                    break;
                if (PBed == 0 || (near_to - (*bed_it)->GetPos()).GetSquaredMagnitude() < best) {
                    PBed = *bed_it;
                    best = (near_to - (*bed_it)->GetPos()).GetSquaredMagnitude();
                }
                ++bed_it;
            }
        }
        if (IS_THING_ALIVE(PBed)) {
            FixturePoints* ptc_action_point;
            if (PBed->GetTC(&ptc_action_point)) {
                if (ptc_action_point->GetFreeActionPoint(GetCreature(), BedPos, Rhset, ActionPointIndex)) {
                    BedToRightOfCreature = false;
                    return true;
                }
            } else {
                IsBedSleepPointMarker = true;
                BedPos = PBed->GetPos();
                Rhset = PBed->GetPhysicsTC()->GetRHSet();
                FixtureOccupier* ptc_occupiable;
                // FableWin checks this call for success. Retail evaluates the call once.
                PBed->GetTC(&ptc_occupiable);
                FABLE_RETAIL_ASSERT(ptc_occupiable->GetOccupier() == 0);
                ptc_occupiable->SetOccupier(&GetCreature());
                C3DVector required_facing_dir = Rhset.GetForward();
                C3DVector right_vector = required_facing_dir;
                right_vector.Rotate(0.25f, 0.0f);
                float dot_product = right_vector.Dot(Rhset.GetRight());
                FABLE_RETAIL_ASSERT(fabs(dot_product) > DELTA_NOTIONAL_ZERO);
                if (dot_product > 0.0f)
                    BedToRightOfCreature = true;
                else
                    BedToRightOfCreature = false;
                return true;
            }
        }
    }
    PBed = FixturePointer<FixtureThing>(0);
    return false;
}
static void acceptance(int mask,int home_status,int owner_status)
{
    calls=Counters(); FixtureBuilding building; FixtureCreature creature; FixtureOwner owner;
    FixtureWife wife; wife.married=(mask&4)!=0; creature.wife=(mask&2)?&wife:0;
    creature.category=(mask&1)?HUMAN_CHILD:2; building.lockable=(mask&8)==0;building.locked=(mask&16)!=0;building.tavern=(mask&64)!=0;
    creature.home=home_status?&building:0;building.dead=home_status==2;building.owner=owner_status?&owner:0;
    bool during=(mask&32)!=0; bool living=home_status==1;
    bool child=(mask&1)!=0,married=(mask&2)!=0 && (mask&4)!=0;
    bool allowed=child||married||!building.lockable||building.locked||during||building.tavern;
    FixtureThing old; FixtureState group(&creature);group.PBed=&old;
    check(!group.FindFreeBed(during));check(group.PBed==0 && !group.IsBedSleepPointMarker);
    check(group.BedToRightOfCreature && group.ActionPointIndex==-7 && group.BedPos==C3DVector(17,19,23));
    check(calls.home==1 && calls.position==1 && calls.type==int(living));
    check(calls.wife==int(living&&!child&&creature.wife));
    check(calls.lockable==int(living&&!child&&!married));
    check(calls.locked==int(living&&!child&&!married&&building.lockable));
    check(calls.tavern==int(living&&!child&&!married&&building.lockable&&!building.locked&&!during));
    check(calls.house==int(living&&allowed));check(!calls.action_query&&!calls.occupy);
}
static void selection(int mask,int path,int up_sign)
{
    calls=Counters();FixtureBuilding house;FixtureCreature creature;FixtureOwner owner;creature.home=&house;
    creature.category=HUMAN_CHILD;creature.pos=C3DVector(10,20,30);expected_creature=&creature;house.owner=&owner;
    FixtureOccupier marker;FixturePoints action;action.available=(path==1);
    FixtureThing objects[5],old;static const int dx[5]={4,1,0,-1,5};static const int dy[5]={0,2,3,-2,0};
    static const int dz[5]={0,1,0,-1,2};int selected=-1,best_squared=0;
    owner.objects.push_back(FixturePointer<FixtureThing>(0));
    for(int i=0;i<5;++i){
        FixtureThing& bed=objects[i];bed.pos=C3DVector(float(10+dx[i]),float(20+dy[i]),float(30+dz[i]));
        bed.enabled=(mask&(1<<i))!=0;bed.points=path<=1?&action:0;bed.marker=&marker;
        bed.dead=path==3;bed.physics.orientation=FixtureOrientation(C3DVector(0,0,float(up_sign)),C3DVector(1,0,0));
        owner.objects.push_back(&bed);
        int d=dx[i]*dx[i]+dy[i]*dy[i]+dz[i]*dz[i];if(bed.enabled && (selected<0 || d<best_squared)){selected=i;best_squared=d;}
    }
    FixtureState g(&creature);g.PBed=&old;
    bool ok=g.FindFreeBed(false);bool live=(selected>=0 && path!=3),accepted=live&&(path==1||path==2);
    check(ok==accepted);check(g.PBed.value==(accepted?&objects[selected]:0));check(calls.eligible==6);
    check(calls.action_query==int(live));check(calls.action==int(live&&path<2));check(calls.occupy==int(live&&path==2));
    if(ok && path==1){check(!g.IsBedSleepPointMarker&&!g.BedToRightOfCreature&&g.ActionPointIndex==23);check(g.BedPos==C3DVector(3,5,7));check(g.Rhset.GetForward()==C3DVector(0,1,0));}
    else if(ok){check(g.IsBedSleepPointMarker&&g.ActionPointIndex==-7);check(g.BedPos==objects[selected].pos);check(g.Rhset.GetUp()==objects[selected].physics.orientation.GetUp());check(g.BedToRightOfCreature==(up_sign>0));check(marker.occupier==&creature);}
    else {check(!g.IsBedSleepPointMarker && g.BedToRightOfCreature && g.ActionPointIndex==-7 && g.BedPos==C3DVector(17,19,23));}
    check(calls.rotate==int(ok&&path==2));
}
}
int main()
{
    for(int m=0;m<128;++m)for(int home=0;home<3;++home)for(int owner=0;owner<2;++owner) BedFixture::acceptance(m,home,owner);
    for(int m=0;m<32;++m)for(int path=0;path<4;++path)for(int sign=-1;sign<=1;++sign) BedFixture::selection(m,path,sign);
    puts("0092d1f0_TEST PASS");return 0;
}