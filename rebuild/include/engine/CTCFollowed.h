#ifndef FABLE_ENGINE_CTCFOLLOWED_H
#define FABLE_ENGINE_CTCFOLLOWED_H
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
class CThingAICreature;
// Both original complete PDB records: full data and CTCBase inheritance.
// Retail constructor006d4e10: four-byte lists, then two counters and the vector.
class CTCFollowed:public CTCBase {
public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_FOLLOWED;}
 virtual ETCInterfaceType GetInterfaceType()const;
 static CTCBase* FABLE_FASTCALL Alloc(CThing&);
 static CCharString FABLE_FASTCALL StatGetClassTypeName();
 virtual CCharString GetClassTypeName()const;
 CTCFollowed(const CTCFollowed&);
 CTCFollowed(CThing&);
 virtual bool HasFrameUpdate()const;
 virtual void FrameUpdate();
 bool IsCreatureValidFollower(const CThingAICreature&,bool*)const;
 void AddFollower(CThing&,bool);
 void RemoveFollower(CThing&);
 void RemoveAllFollowers();
 void QuitAIFollower(CThingAICreature&);
 void GetFollowers(std::vector<CThing*>&);
 bool IsFollower(const CThing*)const;
 bool IsAIFollower(const CThingAICreature*)const;
protected:
 long GetNumFollowers()const{return Followers.size();}
 void CullNullFollowers();
 void AddEmoteIcon(CThing&);
 void RemoveEmoteIcon(CThing&);
 std::list<CIntelligentPointer<CThing> > Followers;
 std::list<CIntelligentPointer<CThingAICreature> > AIFollowers;
 long FrameLastIdleReaction,FramesSincePlayerMoved;
 C3DVector LastPlayerPosition;
public:
 virtual ~CTCFollowed();
 CTCFollowed& operator=(const CTCFollowed&);
};
FABLE_STATIC_ASSERT(sizeof(CTCFollowed)==0x28);
#endif
