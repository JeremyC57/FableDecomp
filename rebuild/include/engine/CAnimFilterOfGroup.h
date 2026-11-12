#ifndef FABLE_ENGINE_CANIMFILTEROFGROUP_H
#define FABLE_ENGINE_CANIMFILTEROFGROUP_H
#include "engine/CAnimFilterBase.h"
#include "fable_string.h"
#ifdef FABLE_RETAIL_CANIMFILTEROFGROUP_INLINE_CTOR
#include "engine/CCRC.h"
#endif
class CAnimFilterOfGroup:public CAnimFilterBase {
public:
 virtual bool operator()(const CAnimationEntry&)const;
 virtual CAnimFilterBase* Clone()const;
#ifdef FABLE_RETAIL_CANIMFILTEROFGROUP_INLINE_CTOR
 // Retail opt-in (wf7, row 0x00847DF0 CCreatureAction_UseLamp::Initialise): the (const CCharString&) constructor (out-of-line copy
 // 0x0073E8F0) and the destructor as retail expands them in place for a stack filter: vptr 0x01267568, Group default-constructed then
 // assigned, GroupCRC = the SOURCE string's CRC (CCharString::GetCRC, inline under FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE);
 // the destructor resets the vptr to CAnimFilterBase 0x0122FFCC after ~Group.
 CAnimFilterOfGroup(const CCharString& group)
 {
  Group = group;
#ifdef FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
  GroupCRC = group.GetCRC();
#else
  GroupCRC = CCRC::Calc(0, group.GetAsCharArray(), group.GetLength());
#endif
 }
 // no user-declared destructor: retail destroys a stack filter without re-storing this class's vptr (implicit destructor).
#else
 virtual ~CAnimFilterOfGroup();
#endif
 CCharString Group;
 unsigned long GroupCRC;
};
FABLE_STATIC_ASSERT(sizeof(CAnimFilterOfGroup)==12);
#endif
