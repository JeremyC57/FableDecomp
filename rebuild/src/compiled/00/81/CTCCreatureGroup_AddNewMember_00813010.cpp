// CTCCreatureGroup::AddNewMember -- insert a living AI creature if absent and advance the group count
// Retail: 00813010,238 bytes; virtual thiscall,const AI creature pointer.
// Leaf: ?AddNewMember@CTCCreatureGroup@@UAE_NPBVCThingAICreature@@@Z
// Flags: none
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBOASTINGCROWDCONTROL
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/CThingAICreature.h"
#include "engine/CGroupMemberData.h"
#include "engine/CTCCreatureGroup.h"
#include "engine/CWorld.h"
#include "engine/ThingAlive.h"
bool CTCCreatureGroup::AddNewMember(const CThingAICreature* member)
{
 if(IS_THING_DEAD(member))return false;
 std::map<unsigned __int64,CGroupMemberData>::iterator it_member=Members.find(member->GetUniqueID());
 if(it_member==Members.end()) {
  Members.insert(std::make_pair(member->GetUniqueID(),CGroupMemberData(member,false,PeekWorld().GetFrame(),0)));
  ++MemberCount;
 }
 return true;
}