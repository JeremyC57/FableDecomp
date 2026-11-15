#ifndef FABLE_ENGINE_CTCGOSSIP_H
#define FABLE_ENGINE_CTCGOSSIP_H
// Both original complete PDB classes and native component accesses; native size0x18.
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ETCInterfaceType.h"
class CThingAICreature;
class CTCGossip:public CTCBase {
public:
 CTCGossip(CThing&);
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_GOSSIP;}
 static CTCBase* FABLE_FASTCALL Alloc(CThing&);
 static CCharString FABLE_FASTCALL StatGetClassTypeName();
 virtual ETCInterfaceType GetInterfaceType()const;
 virtual CCharString GetClassTypeName()const;
 CIntelligentPointer<CThingAICreature> Gossipee;
 bool MyTurnToSpeak;
};
FABLE_STATIC_ASSERT(sizeof(CTCGossip)==0x18);
#endif
