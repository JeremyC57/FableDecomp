
#include "engine/CAIStateGroup_MinionKidCalledToMother.h"  // retyped onto the PDB layout; byte parity re-verified
class CMessageEvent;
struct CAIStateGroup_MinionKidCalledToMother_Methods : CAIStateGroup_MinionKidCalledToMother {
    CMessageEvent* FindCallHomeEvent();
    bool IsRunnable();
};

bool CAIStateGroup_MinionKidCalledToMother_Methods::IsRunnable()
{
    return FindCallHomeEvent() != 0;
}