#ifndef FABLE_ENGINE_CGROUPMEMBERDATA_H
#define FABLE_ENGINE_CGROUPMEMBERDATA_H
#include "engine/CIntelligentPointer.h"
class CThingAICreature;
// Layout: complete original Ego2e6ef/FW311c6,size20; native data/ctor stores agree.
class CGroupMemberData {
public:
 CGroupMemberData(const CGroupMemberData& other):PMember(other.PMember),IsActive(other.IsActive),JoinedFrame(other.JoinedFrame),DeactivatedFrame(other.DeactivatedFrame){}
 CGroupMemberData(const CThingAICreature* member,bool active,unsigned long joined,unsigned long deactivated):PMember(member),IsActive(active),JoinedFrame(joined),DeactivatedFrame(deactivated){}
 CIntelligentPointer<const CThingAICreature> PMember;
 bool IsActive;
 unsigned long JoinedFrame;
 unsigned long DeactivatedFrame;
 ~CGroupMemberData(){}
 CGroupMemberData& operator=(const CGroupMemberData&);
};
FABLE_STATIC_ASSERT(sizeof(CGroupMemberData)==20);
#endif
