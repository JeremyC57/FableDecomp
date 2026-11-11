#ifndef FABLE_ENGINE_CMAINTAINEDEVENT_H
#define FABLE_ENGINE_CMAINTAINEDEVENT_H
#include "engine/CBaseClass.h"
#include "engine/CInputEvent.h"
// Layout: Ego_r CMaintainedEvent size56, base CBaseClass+0 and Event+4; CMouse native event-list destructor confirms element.
class CMaintainedEvent:public CBaseClass { public: CInputEvent Event; };
#endif
