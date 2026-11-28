// The fixture uses the row's original filter factory and the exact GetSellersReply body.
#include <stdio.h>
#include "rebuild_abi.h"
#include "engine/EAIAction.h"
#include "engine/EMessageEventType.h"
#define FABLE_RETAIL_FILTER_BIND3_FUNCTION
#include "engine/CFilter_Bind3.h"

class CThingCreatureBase {};
class CMessageEvent {
public:
    EMessageEventType type;
    const CThingCreatureBase* origin;
    const CThingCreatureBase* target;
};
class CMessageEventFilter_IsOfType {
    EMessageEventType type;
public:
    CMessageEventFilter_IsOfType(EMessageEventType value):type(value){}
    bool operator()(const CMessageEvent& event)const{return event.type==type;}
};
class CMessageEventFilter_IsOriginatedByThing {
    const CThingCreatureBase& origin;
public:
    CMessageEventFilter_IsOriginatedByThing(const CThingCreatureBase& value):origin(value){}
    bool operator()(const CMessageEvent& event)const{return event.origin==&origin;}
};
class CMessageEventFilter_IsTargettedAtThing {
    const CThingCreatureBase& target;
public:
    CMessageEventFilter_IsTargettedAtThing(const CThingCreatureBase& value):target(value){}
    bool operator()(const CMessageEvent& event)const{return event.target==&target;}
};

class CAIStateGroup_BuyFromSeller {
    const CMessageEvent* GetSellersReply(const CThingCreatureBase* seller);
    CThingCreatureBase& GetCreature(){return buyer;}
    template<class Filter> EAIActionReturn AIAction_WaitForMessageCallback(EAIActionRepeat repeat,const Filter& filter,const CMessageEvent** pevent){
        ++calls;
        callback = repeat;
        *pevent = 0;
        for(unsigned i=0;i<count;++i)
            if(filter(events[i])){*pevent=events+i;break;}
        return outcome;
    }
public:
    CThingCreatureBase buyer;
    CMessageEvent events[4];unsigned count;EAIActionReturn outcome;EAIActionRepeat callback;int calls;
    CAIStateGroup_BuyFromSeller():count(0),outcome(AI_ACTION_NULL),callback(ACTION_REPEAT_UNTIL_FINISHED),calls(0){}
    const CMessageEvent* poll(const CThingCreatureBase& seller){return GetSellersReply(&seller);}
};

const CMessageEvent* CAIStateGroup_BuyFromSeller::GetSellersReply(const CThingCreatureBase* seller)
{
    const CMessageEvent* pevent;
    if (AIAction_WaitForMessageCallback(ACTION_REPEAT_CALLBACK,
        GFFilter_Bind3(CMessageEventFilter_IsOfType(MESSAGE_EVENT_CONVERSATION_ANSWER_YES_OR_NO),
            CMessageEventFilter_IsOriginatedByThing(*seller), CMessageEventFilter_IsTargettedAtThing(GetCreature())),
        &pevent) == AI_ACTION_FINISHED)
        return pevent;
    return 0;
}

int main(){
    CAIStateGroup_BuyFromSeller state;CThingCreatureBase seller,stranger;
    for(unsigned i=0;i<4;++i){state.events[i].type=MESSAGE_EVENT_CONVERSATION_ANSWER_YES_OR_NO;state.events[i].origin=&seller;state.events[i].target=&state.buyer;}
    state.events[0].type=MESSAGE_EVENT_BUY_ITEM;state.events[1].origin=&stranger;state.events[2].target=&stranger;
    state.count=4;state.outcome=AI_ACTION_FINISHED;
    if(state.poll(seller)!=&state.events[3] || state.calls!=1 || state.callback!=ACTION_REPEAT_CALLBACK)return 1;
    state.count=3;if(state.poll(seller))return 2;
    state.count=4;
    const EAIActionReturn other[]={AI_ACTION_NULL,AI_ACTION_FAILED,AI_ACTION_IN_PROGRESS,AI_ACTION_TERMINATE_PROCESS};
    for(unsigned i=0;i<sizeof(other)/sizeof(other[0]);++i){state.outcome=other[i];if(state.poll(seller))return 3;}
    if(state.calls!=6)return 4;
    puts("00919a70_TEST PASS");return 0;
}
