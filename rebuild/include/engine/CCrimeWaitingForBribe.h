#ifndef FABLE_ENGINE_CCRIMEWAITINGFORBRIBE_H
#define FABLE_ENGINE_CCRIMEWAITINGFORBRIBE_H
#include "engine/EOpinionDeedType.h"
// Ego_d 0x253d7 and Ego_r layout, eight bytes in the tc_guard deque.
class CCrimeWaitingForBribe {
public:
    long DeedHandle;
    EOpinionDeedType Type;
};
#endif
