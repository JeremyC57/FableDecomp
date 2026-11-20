#ifndef FABLE_ENGINE_CMAINTAINEDEVENT_H
#define FABLE_ENGINE_CMAINTAINEDEVENT_H
#include "engine/CBaseClass.h"
#include "engine/CInputEvent.h"
// Layout: Ego_r CMaintainedEvent size56, base CBaseClass+0 and Event+4; CMouse native event-list destructor confirms element.
class CMaintainedEvent:public CBaseClass { public: CInputEvent Event;
#ifdef FABLE_RETAIL_CMAINTAINEDEVENT_CTOR
    // agentNF182 (2026-11-20, row 0x009E45B0): the out-of-line constructor (Ego_r.pdb / FableWin.pdb ??0CMaintainedEvent@@QAE@XZ;
    // retail 0x009E45B0 stores vtable 0x0129C0E0 and runs ??0CInputEvent@@QAE@XZ 0x00A04410 on Event). Declaration only.
    CMaintainedEvent();
#endif
};
#endif
