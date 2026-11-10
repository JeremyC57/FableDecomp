#ifndef FABLE_ENGINE_CMESSAGEEVENTFILTER_ISMYDEADLEADER_H
#define FABLE_ENGINE_CMESSAGEEVENTFILTER_ISMYDEADLEADER_H
#include "stlport/fable_containers.h"
class CThingCreatureBase;class CMessageEvent;
// Original Ego_r complete type37d00: unary_function base and private const reference.
class CMessageEventFilter_IsMyDeadLeader:public std::unary_function<CMessageEvent,bool> {
 const CThingCreatureBase& Creature;
public:
 CMessageEventFilter_IsMyDeadLeader(const CThingCreatureBase& creature):Creature(creature){}
 bool operator()(const CMessageEvent& test_event)const;
};
#endif
