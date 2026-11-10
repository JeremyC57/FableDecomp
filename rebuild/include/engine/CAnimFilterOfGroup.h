#ifndef FABLE_ENGINE_CANIMFILTEROFGROUP_H
#define FABLE_ENGINE_CANIMFILTEROFGROUP_H
#include "engine/CAnimFilterBase.h"
#include "fable_string.h"
class CAnimFilterOfGroup:public CAnimFilterBase {
public:
 virtual bool operator()(const CAnimationEntry&)const;
 virtual CAnimFilterBase* Clone()const;
 virtual ~CAnimFilterOfGroup();
 CCharString Group;
 unsigned long GroupCRC;
};
FABLE_STATIC_ASSERT(sizeof(CAnimFilterOfGroup)==12);
#endif
