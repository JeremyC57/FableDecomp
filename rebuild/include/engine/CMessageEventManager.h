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
#ifdef FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDRECENT
 // Opt-in (w22 agent1016, row 0x00934E50): Ego_r.pdb / FableWin.pdb member template
 // ??$FindMostRecentMessage@<filter>@CMessageEventManager@@QBEPBVCMessageEvent@@ABV<filter>@@Z (one COMDAT per filter type in retail:
 // 0x00934E50, 0x00939C60, 0x00939CB0, ...). Body: the landed family K034/K035 walk (0x00939CB0) -- the first (newest) queued message
 // the filter accepts, else null. Needs the complete CMessageEvent (engine/CMessageEvent.h, FABLE_RETAIL_CMESSAGEEVENT) at instantiation.
 template<class TFilter> const CMessageEvent* FindMostRecentMessage(const TFilter& filter) const {
  for (std::list<CMessageEvent>::const_iterator it = Messages.begin(); it != Messages.end(); ++it) {
   const CMessageEvent* message = &*it;
   if (filter(*message))
    return message;
  }
  return 0;
 }
#endif
private:
 const CWorld& World;
 std::list<CMessageEvent> Messages;
};
FABLE_STATIC_ASSERT(sizeof(CMessageEventManager)==8);
#endif
