#ifndef FABLE_ENGINE_CTCCREATUREGROUPTAG_H
#define FABLE_ENGINE_CTCCREATUREGROUPTAG_H
#ifdef FABLE_RETAIL_KID_EVADE_PASS23
#include "engine/CTCCreatureGroup.h"
// Layout: Ego_r original VCTCCreatureGroupTag, TaggerUID+28,TaggedFrame+30,size38.
// FableWin original type and donor02b68970 calls confirm the const GetTagger member.
class CTCCreatureGroupTag:public CTCCreatureGroup {
public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType();
 virtual ETCInterfaceType GetInterfaceType()const;
 static CTCBase* FABLE_FASTCALL Alloc(CThing&);
 static CCharString FABLE_FASTCALL StatGetClassTypeName();
 virtual CCharString GetClassTypeName()const;
 CTCCreatureGroupTag(const CTCCreatureGroupTag&);
 CTCCreatureGroupTag(CThing&);
 bool SetTagger(const CThingAICreature*);
 const CThingAICreature* GetTagger()const;
 unsigned long GetTaggedFrame()const;
 virtual bool RemoveMember(const CThingAICreature*);
 virtual bool DeactivateMember(const CThingAICreature*);
private:
 unsigned __int64 TaggerUID;
 unsigned long TaggedFrame;
 bool FindNewTagger();
public:
 virtual ~CTCCreatureGroupTag();
 CTCCreatureGroupTag& operator=(const CTCCreatureGroupTag&);
};
FABLE_STATIC_ASSERT(sizeof(CTCCreatureGroupTag)==0x38);
#endif
#endif
