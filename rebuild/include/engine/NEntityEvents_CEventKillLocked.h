#ifndef FABLE_ENGINE_NENTITYEVENTS_CEVENTKILLLOCKED_H
#define FABLE_ENGINE_NENTITYEVENTS_CEVENTKILLLOCKED_H
#include "engine/NEntityEvents_CEventHitBase.h"
#include "engine/CIntelligentPointer.h"
namespace NEntityEvents {
class CEventKillLocked : public CEventBase {
public:
    CEventKillLocked(const CThing*);
    virtual EEventType GetType() const;
    virtual long GetLifeTime() const;
private:
    CIntelligentPointer<const CThing> PKiller;
};
}
#endif
