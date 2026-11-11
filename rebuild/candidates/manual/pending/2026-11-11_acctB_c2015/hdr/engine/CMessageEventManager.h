#ifndef FABLE_ENGINE_CMESSAGEEVENTMANAGER_H
#define FABLE_ENGINE_CMESSAGEEVENTMANAGER_H
#include "engine/EMessageEventType.h"
#include "stlport/fable_containers.h"
class CWorld;class CMessageEvent;class C3DVector;class CThing;
// Original Ego_r37bae: no base, World reference then std::list<CMessageEvent>.
// Native STLport list is4 bytes, so full manager is8 bytes.
class CMessageEventManager {
public:
#ifdef FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE
 void RemoveMessage(const CMessageEvent*);
#endif
 CMessageEvent* AddMessage(EMessageEventType,unsigned long,const C3DVector&,CThing*,CThing*,float,float,long);
#ifdef FABLE_RETAIL_CMESSAGEEVENTMANAGER_ADDDEFMESSAGE
 // Opt-in (chunk 2015, row 0x0097FEA0): Ego_r.pdb / FableWin.pdb
 // ?AddMessage@CMessageEventManager@@QAEPAVCMessageEvent@@ABVCMessageEventDef@@ABVC3DVector@@PAVCThing@@2J@Z, the def overload, out of
 // line in retail at 0x00687730 (landed; it reads the def's radii +0x3C / +0x40 and lifespan +0x44). Declaration only.
 CMessageEvent* AddMessage(const class CMessageEventDef& def,const C3DVector& position,CThing* originator,CThing* target,long extra_data);   // 0x00687730
#endif
#ifdef FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDRECENT
 // Opt-in (w22 agent1016, row 0x00934E50): Ego_r.pdb / FableWin.pdb member template
 // ??$FindMostRecentMessage@<filter>@CMessageEventManager@@QBEPBVCMessageEvent@@ABV<filter>@@Z (one COMDAT per filter type in retail:
 // 0x00934E50, 0x00939C60, 0x00939CB0, ...). Body: the landed family K034/K035 walk (0x00939CB0) -- the first (newest) queued message
 // the filter accepts, else null. Needs the complete CMessageEvent (engine/CMessageEvent.h, FABLE_RETAIL_CMESSAGEEVENT) at instantiation.
#ifdef FABLE_RETAIL_CMESSAGEEVENTMANAGER_ORIGINAL_LOCAL
 // Original donor02b69940 uses a const event reference.
 template<class TFilter> const CMessageEvent* FindMostRecentMessage(const TFilter& filter_func) const {
  for (std::list<CMessageEvent>::const_iterator it = Messages.begin(); it != Messages.end(); ++it) {
   const CMessageEvent& event = *it;
   if (filter_func(event))
    return &event;
  }
  return 0;
 }
#else
 template<class TFilter> const CMessageEvent* FindMostRecentMessage(const TFilter& filter) const {
  for (std::list<CMessageEvent>::const_iterator it = Messages.begin(); it != Messages.end(); ++it) {
   const CMessageEvent* message = &*it;
   if (filter(*message))
    return message;
  }
  return 0;
 }
#endif
#endif
private:
 const CWorld& World;
 std::list<CMessageEvent> Messages;
};
FABLE_STATIC_ASSERT(sizeof(CMessageEventManager)==8);
#endif
