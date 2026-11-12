#ifndef FABLE_ENGINE_CTCDEADSHOTINTHINGTRACKER_H
#define FABLE_ENGINE_CTCDEADSHOTINTHINGTRACKER_H
// Layout and names: both original PDBs; donor026fd4f0 and native007c1000.
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
class CTCDeadShotInThingTracker:public CTCBase {public:
 void SetThingStuckIn(const CThing&);
private:CIntelligentPointer<const CThing> PThingStuckIn;bool HasThingStuckIn;
};
FABLE_STATIC_ASSERT(sizeof(CTCDeadShotInThingTracker)==24);
#endif
