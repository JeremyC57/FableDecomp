#ifndef FABLE_ENGINE_NENTITYEVENTS_CEVENTAPOLOGYRECEIVED_H
#define FABLE_ENGINE_NENTITYEVENTS_CEVENTAPOLOGYRECEIVED_H
#include "engine/NEntityEvents_EEventType.h"
#include "engine/NEntityEvents_CEventExpressionPerformedTo.h"
namespace NEntityEvents {
class CEventApologyReceived : public CEventBase {
public:
    CEventApologyReceived(bool accepted) : Accepted(accepted) {}
    virtual EEventType GetType() const { return StatGetType(); }
    static EEventType FABLE_FASTCALL StatGetType() { return EVENT_APOLOGY_RECEIVED; }
private:
    bool Accepted;
};
}
#endif
