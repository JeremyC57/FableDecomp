#ifndef FABLE_ENGINE_CAISTATEGROUP_WORKBASE_H
#define FABLE_ENGINE_CAISTATEGROUP_WORKBASE_H
#include "engine/CAIStateGroupBase.h"
class CThing;
// Complete original Ego_r0x535ab, same native20byte base, no own members.
template<class Base> class CAIStateGroup_WorkBase:public Base {
protected:
 bool IsInSchoolHours()const;
 bool IsInWorkHours()const;
 bool StayInShop()const;
 bool HasSeller();
 void AcquireSeller();
 bool BuildingHasVolume(const CThing*)const;
};
#endif
