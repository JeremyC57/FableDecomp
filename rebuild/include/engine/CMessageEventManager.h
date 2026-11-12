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
#ifdef FABLE_RETAIL_CMESSAGEEVENTMANAGER_ADDFROMDEF
 // Nested opt-in (wf17, row 0x00958840): Ego_r.pdb ?AddMessage@CMessageEventManager@@QAEPAVCMessageEvent@@ABVCMessageEventDef@@ABVC3DVector@@PAVCThing@@2J@Z
 // (landed 0x00687730). Declaration only.
 CMessageEvent* AddMessage(const class CMessageEventDef& def,const C3DVector& pos,CThing* originator,CThing* target,long extraData);   // 0x00687730
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
#if defined(FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDRECENT_DECL) && !defined(FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDRECENT)
 // Opt-in (wf17, row 0x007E2A60 CTCFineDialogue::GlobalUpdate): the same member template, declaration only, for a TU that calls
 // the COMDAT out of line (FindMostRecentMessage<CMessageEventFilter_IsOfType> 0x004AB8B0: `push filter ; mov ecx,manager ; call`).
 template<class TFilter> const CMessageEvent* FindMostRecentMessage(const TFilter& filter) const;
#endif
#ifdef FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDALL
 // Opt-in (wf13, row 0x00492973 CSubtitleRenderer::SetText): Ego_r.pdb member template
 // ??$FindAllMessages@VCMessageEventFilter_IsOfType@@@CMessageEventManager@@QBE?BJABVCMessageEventFilter_IsOfType@@AAV?$list@PBVCMessageEvent@@V?$allocator@PBVCMessageEvent@@@std@@@std@@@Z
 // (const member, `const long` result, the found events appended to a list<const CMessageEvent*>), COMDAT 0x00493EAB.
 // Declaration only; a row whose retail TU compiled the body defines it.
 template<class TFilter> const long FindAllMessages(const TFilter& filter, std::list<const CMessageEvent*>& found) const;
#endif
private:
 const CWorld& World;
 std::list<CMessageEvent> Messages;
};
FABLE_STATIC_ASSERT(sizeof(CMessageEventManager)==8);
#endif
