#include <algorithm>
#include <vector>
#include <cmath>
#include <cstdio>

struct C3DVector {
    float x,y,z;
    C3DVector(float a=0,float b=0,float c=0): x(a),y(b),z(c) {}
    C3DVector operator+(const C3DVector& p)const { return C3DVector(x+p.x,y+p.y,z+p.z); }
    C3DVector operator-(const C3DVector& p)const { return C3DVector(x-p.x,y-p.y,z-p.z); }
    C3DVector operator*(float k)const { return C3DVector(x*k,y*k,z*k); }
    float GetSquaredMagnitude()const { return x*x+y*y+z*z; }
    void AccurateNormalise() {
        float n=(float)std::sqrt((double)GetSquaredMagnitude());
        if(n) { x/=n;y/=n;z/=n; }
    }
    bool operator==(const C3DVector& p)const { return x==p.x && y==p.y && z==p.z; }
};
struct C2DBoxF {
    C3DVector centre;
    float radius;
    C2DBoxF(C3DVector p,float r): centre(p),radius(r) {}
    C2DBoxF ToC2DBoxI()const { return *this; }
};
struct CThingObject {
    C3DVector position;
    bool cover;
    float diameter;
    CThingObject(C3DVector p,bool c,float d):position(p),cover(c),diameter(d) {}
    C3DVector GetPos()const { return position; }
    const CThingObject* GetPhysicsTC()const { return this; }
    float GetDiameter()const { return diameter; }
};
struct CThingFilter_CanBeUsedAsCover {
    bool operator()(const CThingObject& p)const { return p.cover; }
};
namespace {
class CIsNotInList {
    std::vector<const CThingObject*> Things;
public:
    explicit CIsNotInList(const std::vector<const CThingObject*>& v):Things(v){}
    bool operator()(const CThingObject&)const;
};
}
template<class A,class B>struct Both {
    const A& first; const B& second;
    Both(const A& a,const B& b):first(a),second(b){}
    bool operator()(const CThingObject& p)const {return first(p) && second(p);}
};
template<class A,class B>Both<A,B> GFFilter_Bind2(const A& a,const B& b){return Both<A,B>(a,b);}
struct CThingCompare_Nearest {
    C3DVector origin;
    CThingCompare_Nearest(C3DVector p):origin(p){}
    bool operator()(const CThingObject* a,const CThingObject* b)const {
        return (a->GetPos()-origin).GetSquaredMagnitude()<(b->GetPos()-origin).GetSquaredMagnitude();
    }
};
struct CTCCreatureNavigation {
    bool success;
    mutable std::vector<C3DVector> requested;
    mutable std::vector<float> radius;
    mutable bool valid;
    CTCCreatureNavigation():success(true),valid(true){}
    bool GetNearbyWanderPosition(const C3DVector& p,float r,C3DVector& out,bool flag)const {
        valid=valid && !flag && &p==&out;
        requested.push_back(p);radius.push_back(r);
        if(success) out=out+C3DVector(0,0,2);
        return success;
    }
};
enum EAIActionReturn { Finished=2, Failed=3 };
enum { ACTION_REPEAT_UNTIL_FINISHED=1, AI_MOVEMENT_RUNNING=5 };
struct CAIStateGroup_TestTestTest {
    CTCCreatureNavigation navigation;
    std::vector<const CThingObject*> world;
    std::vector<C3DVector> moves;
    C3DVector creature;
    bool valid, actionSuccess; int searchCount, navigationLookups;
    CAIStateGroup_TestTestTest():creature(4,4,0),valid(true),actionSuccess(true),searchCount(0),navigationLookups(0){}
    CAIStateGroup_TestTestTest& GetCreature(){return *this;}
    void PeekTC(const CTCCreatureNavigation** out){++navigationLookups;*out=&navigation;}
    C3DVector GetPos()const {return creature;}
    CAIStateGroup_TestTestTest& PeekThingSearchTools(){return *this;}
    template<class T,class Predicate>void GetAllThingsInRoughArea(C2DBoxF box,Predicate pred,std::vector<T*>& result){
        ++searchCount;
        valid=valid && box.radius==15 && box.centre==creature && result.empty();
        for(size_t i=0;i<world.size();++i){
            C3DVector delta=world[i]->GetPos()-box.centre;
            if(std::fabs(delta.x)<=box.radius && std::fabs(delta.y)<=box.radius && pred(*world[i])) result.push_back(world[i]);
        }
    }
    EAIActionReturn AIAction_MoveToPos(int mode,C3DVector pos,bool& nav,int movement,bool a,bool b,const void* target,bool c,float proximity){
        valid=valid && mode==ACTION_REPEAT_UNTIL_FINISHED && movement==AI_MOVEMENT_RUNNING && a && b && !target && c && proximity==0;
        nav=actionSuccess;moves.push_back(pos);return nav?Finished:Failed;
    }
    void Update();
};
namespace {
bool CIsNotInList::operator()(const CThingObject& thing) const {
    return std::find(Things.begin(),Things.end(),&thing)==Things.end();
}
}
void CAIStateGroup_TestTestTest::Update()
{
    std::vector<const CThingObject*> visited_things;
    const CTCCreatureNavigation* ptc_creature_nav;
    GetCreature().PeekTC(&ptc_creature_nav);
    const float LOOK_FOR_THINGS_RADIUS = 15.0f;
    bool finished = false;
    do {
        std::vector<const CThingObject*> things;
        PeekThingSearchTools().GetAllThingsInRoughArea<const CThingObject>(
            C2DBoxF(GetCreature().GetPos(), LOOK_FOR_THINGS_RADIUS).ToC2DBoxI(),
            GFFilter_Bind2(CThingFilter_CanBeUsedAsCover(), CIsNotInList(visited_things)), things);
        long sizeeee = things.size();
        if (!things.empty()) {
            std::sort(things.begin(),things.end(),CThingCompare_Nearest(GetCreature().GetPos()));
            for (std::vector<const CThingObject*>::iterator it=things.begin();it!=things.end();++it) {
                const CThingObject* pthing = *it;
                float dist_sqr = (pthing->GetPos()-GetCreature().GetPos()).GetSquaredMagnitude();
            }
            const CThingObject* pthing = things.front();
            float diameter = pthing->GetPhysicsTC()->GetDiameter();
            C3DVector line = GetCreature().GetPos()-pthing->GetPos();
            line.AccurateNormalise();
            C3DVector pos = pthing->GetPos()+line*diameter;
            ptc_creature_nav->GetNearbyWanderPosition(pos,diameter / 2.0f,pos,false);
            bool nav;
            EAIActionReturn ret = AIAction_MoveToPos(ACTION_REPEAT_UNTIL_FINISHED,pos,nav,AI_MOVEMENT_RUNNING,true,true,0,true,0.0f);
            visited_things.push_back(pthing);
        } else {
            finished = true;
        }
    } while (!finished);
}

static bool scenario(unsigned mask,bool navok,bool actionok) {
    CAIStateGroup_TestTestTest group;group.navigation.success=navok;group.actionSuccess=actionok;
    CThingObject farCover(C3DVector(4,12,0),true,4),nearCover(C3DVector(8,4,0),true,2);
    CThingObject nonCover(C3DVector(5,4,0),false,6),outside(C3DVector(20,4,0),true,4);
    if(mask&1)group.world.push_back(&farCover);
    if(mask&2)group.world.push_back(&nearCover);
    if(mask&4)group.world.push_back(&nonCover);
    if(mask&8)group.world.push_back(&outside);
    group.Update();
    std::vector<C3DVector> expected;std::vector<float> radii;
    if(mask&2){expected.push_back(C3DVector(6,4,0));radii.push_back(1);}
    if(mask&1){expected.push_back(C3DVector(4,8,0));radii.push_back(2);}
    if(group.navigation.requested!=expected || group.navigation.radius!=radii)return false;
    if(navok)for(size_t i=0;i<expected.size();++i)expected[i].z+=2;
    return group.moves==expected && group.navigation.valid && group.valid && group.navigationLookups==1 && group.searchCount==(int)expected.size()+1;
}
int main(){
    for(unsigned mask=0;mask<16;++mask)for(unsigned flags=0;flags<4;++flags)
        if(!scenario(mask,(flags&1)!=0,(flags&2)!=0))return 1;
    std::puts("0091f140_TEST PASS");return 0;
}