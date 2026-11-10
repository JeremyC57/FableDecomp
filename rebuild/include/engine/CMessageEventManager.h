#ifndef FABLE_ENGINE_CMESSAGEEVENTMANAGER_H
#define FABLE_ENGINE_CMESSAGEEVENTMANAGER_H
#include "engine/EMessageEventType.h"
#include "stlport/fable_containers.h"
class CWorld;class CMessageEvent;class C3DVector;class CThing;
// Original Ego_r37bae: no base, World reference then std::list<CMessageEvent>.
// Native STLport list is4 bytes, so full manager is8 bytes.
class CMessageEventManager {
public:
 CMessageEvent* AddMessage(EMessageEventType,unsigned long,const C3DVector&,CThing*,CThing*,float,float,long);
private:
 const CWorld& World;
 std::list<CMessageEvent> Messages;
};
FABLE_STATIC_ASSERT(sizeof(CMessageEventManager)==8);
#endif
