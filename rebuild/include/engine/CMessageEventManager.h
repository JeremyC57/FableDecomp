#ifndef FABLE_ENGINE_CMESSAGEEVENTMANAGER_H
#define FABLE_ENGINE_CMESSAGEEVENTMANAGER_H
#include "engine/EMessageEventType.h"
#include "stlport/fable_containers.h"
class CWorld;class CMessageEvent;class C3DVector;class CThing;
// Original Ego_r37bae: no base, World reference then std::list<CMessageEvent>.
// Native STLport list is4 bytes, so full manager is8 bytes.
class CMessageEventManager {
#ifdef FABLE_RETAIL_CMESSAGEEVENTMANAGER_CTOR
 // Opt-in (chunk 2051, row 0x004A6E30 CWorld::Init): Ego_r.pdb ??0CMessageEventManager@@QAE@ABVCWorld@@@Z, retail 0x00687510. Declaration only.
public:
 CMessageEventManager(const CWorld& world);                     // 0x00687510
#endif
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
#ifdef FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDRECENT_DECL_B
 // Opt-in (w22 agent2007, row 0x006DEEE0 CTCTrophy::GlobalUpdate): the member template declared only, so a caller binds the
 // out-of-line COMDAT (CMessageEventFilter_IsOfType: 0x004AB8B0, landed) without instantiating the body here.
 template<class TFilter> const CMessageEvent* FindMostRecentMessage(const TFilter& filter) const;
#endif
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
