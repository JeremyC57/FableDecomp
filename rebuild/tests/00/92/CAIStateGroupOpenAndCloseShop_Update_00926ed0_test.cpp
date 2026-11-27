#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <vector>
#include <algorithm>
#include <math.h>
#include "engine/fable_ai_action_enums.h"
#define FABLE_RETAIL_C3DVECTOR
#include "engine/C3DVector.h"
static void check(bool ok) { if (!ok) exit(2); }
struct CTCDoor {};
struct CTCTrespassable {
    int writes;
    bool value;
    void SetTrespassable(bool v) { ++writes; value=v; }
};
struct CRightHandedSet {
    C3DVector forward;
    CRightHandedSet() : forward(1,0,0) {}
    const C3DVector& GetForward() const { return forward; }
};
struct CTCPhysicsBase {
    CRightHandedSet orientation;
    const CRightHandedSet& GetRHSet() const { return orientation; }
};
struct CThing {
    bool alive, limbo, hasDoor;
    C3DVector pos;
    CTCPhysicsBase physics;
    CTCDoor door;
    CThing(float z=0) : alive(true), limbo(false), hasDoor(true), pos(0,0,z) {}
    const CTCPhysicsBase* GetPhysicsTC() const { return &physics; }
    const C3DVector& GetPos() const { return pos; }
    bool IsInLimbo() const { return limbo; }
    bool GetTC(CTCDoor** result) { if (hasDoor) *result=&door; return hasDoor; }
};
template<class T> struct CIntelligentPointer {
    T* pointer;
    CIntelligentPointer(T* p=0) : pointer(p) {}
    T* Get() const { return pointer; }
    T* operator->() const { return pointer; }
};
template<class T> bool IS_THING_ALIVE(const CIntelligentPointer<T>& p) {
    return p.Get() && p->alive;
}
bool OrderByHeight(CIntelligentPointer<const CThing> a, CIntelligentPointer<const CThing> b) {
    return IS_THING_ALIVE(a) && IS_THING_ALIVE(b) && a->GetPos().Z<b->GetPos().Z;
}
struct CTCThingOwner {
    std::list<CIntelligentPointer<CThing> > objects;
    const std::list<CIntelligentPointer<CThing> >& GetObjectList() const {return objects;}
};
static int faces;
struct CThingBuilding : CThing {
    CTCThingOwner owner;
    CTCTrespassable trespass;
    bool hasOwner,hasTrespass,locked;
    int lockCalls,polls,requestedDelay;
    bool GetTC(CTCTrespassable** p) { if (hasTrespass) *p=&trespass; return hasTrespass; }
    bool PeekTC(const CTCThingOwner** p) const { if (hasOwner) *p=&owner; return hasOwner; }
    bool IsPointInside(const C3DVector& p) const {
        check(p.Y==0 && (p.X==1.1f || p.X==-1.1f));
        return ((p.X>0 ? 1 : 2)&faces)!=0;
    }
    void LockAllDoors(bool flag,bool immediate) {check(!immediate);locked=flag;++lockCalls;}
    bool IsLocked() { check(locked);return polls++>=requestedDelay; }
    bool IsUnlocked() { check(!locked);return polls++>=requestedDelay; }
};
struct CTCShopKeeper {
    bool occupied;
    int calls;
    bool GetAreOtherPeopleInShop(bool flag) {check(!flag);++calls;return occupied;}
};
struct CThingAICreature {
    CThingBuilding* building;
    CTCShopKeeper* keeper;
    CThingBuilding* GetWorkBuilding() const {return building;}
    bool GetTC(CTCShopKeeper** out) { if(keeper)*out=keeper;return keeper!=0; }
};
struct CWorld { static int frame; static long GetFrame(){return frame++;} };
int CWorld::frame;
struct CAIStateGroup_OpenAndCloseShop {
    bool Lock,GoToInsideFace;
    int moves,looks,waits,ends,successDoor,expire,interruptWait;
    float heights[3],positions[3],lookedAt;
    CThingAICreature actor;
    void Update();
    CThingAICreature& GetCreature(){return actor;}
    void Terminate(){++ends;}
    EAIActionReturn AIAction_MoveToPos(EAIActionRepeat repeat,C3DVector pos,bool& route,
        EMovementType speed,bool a,bool b,const CThing* target,bool c,float endDistance) {
        check(repeat==ACTION_REPEAT_UNTIL_FINISHED && speed==AI_MOVEMENT_RUNNING &&
              a && b && !target && c && endDistance==0.0f && moves<3);
        heights[moves]=pos.Z; positions[moves]=pos.X; ++moves;route=true;
        if (expire==1) actor.building->alive=false;
        return successDoor==int(pos.Z)?AI_ACTION_FINISHED:AI_ACTION_FAILED;
    }
    void AIAction_LookAtPos(EAIActionRepeat repeat,C3DVector pos,long flags,float speed) {
        check(repeat==ACTION_REPEAT_UNTIL_FINISHED && flags==1 && speed==0.0027777f && pos.X==0 && pos.Y==0);
        lookedAt=pos.Z;++looks;
    }
    EAIActionReturn AIAction_DoNothing(){++waits;return waits==interruptWait?AI_ACTION_FAILED:AI_ACTION_IN_PROGRESS;}
};

void CAIStateGroup_OpenAndCloseShop::Update()
{
    CIntelligentPointer<CThingBuilding> shop(GetCreature().GetWorkBuilding());
    if (!IS_THING_ALIVE(shop)) {
        Terminate();
        return;
    }
    CTCTrespassable* ptc_trespassable;
    if (Lock && shop->GetTC(&ptc_trespassable))
        ptc_trespassable->SetTrespassable(true);

    {
        std::vector<CIntelligentPointer<const CThing> > doors;
        const CTCThingOwner* owner;
        if (shop->PeekTC(&owner)) {
            const std::list<CIntelligentPointer<CThing> >& objects = owner->GetObjectList();
            for (std::list<CIntelligentPointer<CThing> >::const_iterator object_it = objects.begin();
                 object_it != objects.end(); ++object_it) {
                CTCDoor* door;
                if (IS_THING_ALIVE(*object_it) && (*object_it)->GetTC(&door) && !(*object_it)->IsInLimbo())
                    doors.push_back(object_it->Get());
            }
        }
        std::sort(doors.begin(), doors.end(), OrderByHeight);
        EAIActionReturn ret = AI_ACTION_FAILED;
        C3DVector pos;
        for (int n = 0; n < doors.size() && IS_THING_ALIVE(shop); ++n) {
            if (IS_THING_ALIVE(doors[n])) {
                pos = doors[n]->GetPos();
                C3DVector front = pos + doors[n]->GetPhysicsTC()->GetRHSet().GetForward() * 1.1f;
                C3DVector back = pos - doors[n]->GetPhysicsTC()->GetRHSet().GetForward() * 1.1f;
                if (GoToInsideFace == shop->IsPointInside(front)) {
                    bool nav_route_calculated;
                    ret = AIAction_MoveToPos(ACTION_REPEAT_UNTIL_FINISHED, front, nav_route_calculated,
                        AI_MOVEMENT_RUNNING, true, true, 0, true, 0.0f);
                    if (ret == AI_ACTION_FINISHED) break;
                } else if (GoToInsideFace == shop->IsPointInside(back)) {
                    bool nav_route_calculated;
                    ret = AIAction_MoveToPos(ACTION_REPEAT_UNTIL_FINISHED, back, nav_route_calculated,
                        AI_MOVEMENT_RUNNING, true, true, 0, true, 0.0f);
                    if (ret == AI_ACTION_FINISHED) break;
                }
            }
        }
        if (ret == AI_ACTION_FINISHED && IS_THING_ALIVE(shop)) {
            AIAction_LookAtPos(ACTION_REPEAT_UNTIL_FINISHED, pos, 1, 0.0027777f);
            CTCShopKeeper* tc_shopkeeper;
            if (Lock && GetCreature().GetTC(&tc_shopkeeper) && tc_shopkeeper->GetAreOtherPeopleInShop(false)) {
                Terminate();

#ifdef _MSC_VER
// Inline-budget: the shared early exit calls the vector destructor at 009273d4.
#pragma inline_depth(0)
#endif
                goto exit_shop;
#ifdef _MSC_VER
#pragma inline_depth()
#endif

            }
            shop->LockAllDoors(Lock, false);
            if (!Lock && shop->GetTC(&ptc_trespassable))
                ptc_trespassable->SetTrespassable(false);
            int start_wait = CWorld::GetFrame();
            while (Lock ? !shop->IsLocked() : !shop->IsUnlocked()) {
                if (CWorld::GetFrame() - start_wait > 75) break;
                if (AIAction_DoNothing() != AI_ACTION_IN_PROGRESS) {
                    Terminate();

#ifdef _MSC_VER
// Inline-budget: the shared early exit calls the vector destructor at 009273d4.
#pragma inline_depth(0)
#endif
                    goto exit_shop;
#ifdef _MSC_VER
#pragma inline_depth()
#endif

                }
            }
        }
        Terminate();
    }
// Inferred: both early exits destroy doors before the direct shop base destructor call.
exit_shop:
    return;
}

int main() {
    const int delays[]={0,1,75,76,90};
    for(unsigned setting=0;setting<4096;++setting) for(int flow=0;flow<15;++flow) {
        CThingBuilding b; CThing low(1),high(2),invalid(4),limbo(5),dead(6);
        CAIStateGroup_OpenAndCloseShop ai; CTCShopKeeper keeper;
        const unsigned scene=(setting>>7)&3;
        ai.Lock=(setting&1)!=0; ai.GoToInsideFace=(setting&2)!=0;
        faces=(setting>>2)&3; ai.successDoor=(setting>>4)%3;
        b.hasOwner=(setting&64)!=0;b.hasTrespass=(setting&512)!=0;
        b.alive=scene!=1;ai.actor.building=scene==0?0:&b;
        ai.actor.keeper=(setting&1024)?&keeper:0;keeper.occupied=(setting&2048)!=0;keeper.calls=0;
        ai.expire=(flow%3)==2;ai.interruptWait=(flow%3)==1?1:0;
        ai.moves=ai.looks=ai.waits=ai.ends=0;ai.lookedAt=-1;
        b.lockCalls=b.polls=b.trespass.writes=0;b.trespass.value=false;
        b.requestedDelay=delays[flow/3];CWorld::frame=1000;
        invalid.hasDoor=false;limbo.limbo=true;dead.alive=false;
        b.owner.objects.push_back(&high);b.owner.objects.push_back(&dead);
        b.owner.objects.push_back(0);b.owner.objects.push_back(&invalid);
        b.owner.objects.push_back(&limbo);b.owner.objects.push_back(&low);
        bool live=scene>=2;
        bool front=((faces&1)!=0)==ai.GoToInsideFace;
        bool back=((faces&2)!=0)==ai.GoToInsideFace;
        bool canMove=live && b.hasOwner && (front||back);
        int moves=canMove?(ai.successDoor==1||ai.expire?1:2):0;
        bool arrived=canMove && ai.successDoor!=0 && !ai.expire;
        bool occupied=arrived && ai.Lock && ai.actor.keeper && keeper.occupied;
        bool operate=arrived && !occupied;
        ai.Update();
        check(ai.ends==1 && ai.moves==moves && ai.looks==int(arrived));
        for(int n=0;n<ai.moves;++n) check(ai.heights[n]==n+1 && ai.positions[n]==(front?1.1f:-1.1f));
        if(arrived) check(ai.lookedAt==ai.successDoor);
        check(keeper.calls==int(arrived && ai.Lock && ai.actor.keeper));
        check(b.lockCalls==int(operate));
        if(operate)check(b.locked==ai.Lock);
        check(b.trespass.writes==int(live && ai.Lock && b.hasTrespass)+int(operate && !ai.Lock && b.hasTrespass));
        if(b.trespass.writes)check(b.trespass.value==ai.Lock);
        int waits=b.requestedDelay<75?b.requestedDelay:75; if(ai.interruptWait && waits>1)waits=1;
        check(ai.waits==(operate?waits:0));
    }
    puts("00926ed0_TEST PASS");return 0;
}