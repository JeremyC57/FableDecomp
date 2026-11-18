#include <stdio.h>
#include <stdlib.h>
#include "engine/fable_ai_action_enums.h"
static void check(bool condition) { if (!condition) abort(); }
template<class T> class CIntelligentPointer { public:
    CIntelligentPointer(T* p=0):ptr(p){}
    T* Get() const {return ptr;}
    T* operator->() const {return ptr;}
    T& operator*() const {return *ptr;}
private: T* ptr;
};
class CThingObject {};
class CThingAICreature;
class CVillageTaskPickCrops { public:
    CThingObject crop; int checks,allowed_checks;
    CThingObject* GetCrop(){return &crop;}
    bool IsCreatureAssignedToTask(const CThingAICreature&){return checks++<allowed_checks;}
};
class CTCVillage { public: bool accept;int calls;
    bool SetCropTaskAsAcceptedByThing(CVillageTaskPickCrops&,const CThingAICreature&) {++calls;return accept;}
};
class CTCVillageMember { public: CTCVillage* village;CTCVillage* GetPVillage()const{return village;} };
enum EComponent { TCI_VILLAGE_MEMBER };
class CThingAICreature { public: CTCVillageMember* member;
    bool GetTC(EComponent id,CTCVillageMember** out){check(id==TCI_VILLAGE_MEMBER);*out=member;return member!=0;}
};
class CCreatureAction_PickCrop {public:
    CThingAICreature& actor; CThingObject& crop;
    CCreatureAction_PickCrop(CThingAICreature& a,CThingObject& c):actor(a),crop(c){}
};
class CAIStateGroup_VillageTaskPickCrops { public:
    CIntelligentPointer<CVillageTaskPickCrops> PTask; CThingAICreature actor;
    int moves,picks,exits,pending; bool work,drop_task,move_failed,pick_failed; CThingObject* expected_crop;
    void Update();
    CThingAICreature& GetCreature(){return actor;}
    void Terminate(){++exits;}
    bool IsTimeForWork()const{return work;}
    EAIActionReturn AIAction_MoveToThingProximity(EAIActionRepeat r,CIntelligentPointer<CThingObject> c,float proximity,bool& route,EMovementType m,
                                                  bool b,bool f,bool d,bool e,float duration){
        check(r==ACTION_REPEAT_CALLBACK && c.Get()==expected_crop && proximity==0.2f && m==AI_MOVEMENT_WALKING && b && !f && d && e && duration==0.0f);
        check(route==(moves!=0));route=true;
        if(drop_task)PTask=CIntelligentPointer<CVillageTaskPickCrops>(0);
        return moves++<pending ? AI_ACTION_IN_PROGRESS : move_failed ? AI_ACTION_FAILED : AI_ACTION_FINISHED;
    }
    EAIActionReturn AIAction_PerformActionAndWait(const CCreatureAction_PickCrop& action,bool wait){
        check(&action.actor==&actor && &action.crop==expected_crop && !wait);++picks;return pick_failed?AI_ACTION_FAILED:AI_ACTION_FINISHED;
    }
};
void CAIStateGroup_VillageTaskPickCrops::Update()
{
    CTCVillageMember* ptc_village_mem;
    if (!PTask.Get()) {
        Terminate();
        return;
    }
    if (!GetCreature().GetTC(TCI_VILLAGE_MEMBER, &ptc_village_mem)) {
        Terminate();
        return;
    }
    CTCVillage* pvillage = ptc_village_mem->GetPVillage();
    if (!pvillage || !pvillage->SetCropTaskAsAcceptedByThing(*PTask, GetCreature())) {
        PTask = CIntelligentPointer<CVillageTaskPickCrops>(0);
        Terminate();
        return;
    }
    CIntelligentPointer<CThingObject> pcrop(PTask->GetCrop());
    bool nav_route_calculated = false;
    EAIActionReturn ret;
    do {
        if (!PTask.Get() || !PTask->IsCreatureAssignedToTask(GetCreature()) || !IsTimeForWork()) {
            Terminate();
            return;
        }
        ret = AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK, pcrop, 0.2f,
            nav_route_calculated, AI_MOVEMENT_WALKING, true, false, true, true, 0.0f);
    } while (ret == AI_ACTION_IN_PROGRESS);
    if (ret == AI_ACTION_FINISHED && PTask.Get() && PTask->IsCreatureAssignedToTask(GetCreature())) {
        if (AIAction_PerformActionAndWait(CCreatureAction_PickCrop(GetCreature(), *pcrop), false) != AI_ACTION_FINISHED) {
            Terminate();
            return;
        }
    }
    Terminate();
    return;
}

int main() {
    for(int c=0;c<13;++c) {
        CAIStateGroup_VillageTaskPickCrops state; CVillageTaskPickCrops task; CTCVillage village; CTCVillageMember member;
        state.PTask=c==0?0:&task; state.actor.member=c==1?0:&member; member.village=c==2?0:&village;
        village.accept=c!=3; village.calls=0; task.checks=0;task.allowed_checks=c==4?0:c==8?1:8;
        state.work=c!=5; state.moves=state.picks=state.exits=0;state.pending=c==6||c==11?2:0;
        state.drop_task=c==6||c==7;state.move_failed=c==9;state.pick_failed=c==10;state.expected_crop=&task.crop;
        state.Update();
        check(state.exits==1 && village.calls==(c>=3?1:0));
        check(state.moves==(c<6?0:c==11?3:1));
        check(state.picks==(c>=10?1:0));
        check((state.PTask.Get()==0)==(c==0||c==2||c==3||c==6||c==7));
        if(c==1)check(task.checks==0);
        if(c==8)check(task.checks==2);
    }
    puts("00940310_TEST PASS"); return 0;
}