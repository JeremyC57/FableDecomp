#ifndef FABLE_ENGINE_CTCBOASTINGCROWDCONTROL_H
#define FABLE_ENGINE_CTCBOASTINGCROWDCONTROL_H
// Layout: complete original members from both PDB type records; retail STLport maps use12bytes.
// Evidence: 00783a30/NAMING_EVIDENCE.md; opt-in preserves the previous default.
#include "engine/CTCBase.h"
#include "engine/CQuestManager.h"
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_map.h"
class CThingMarker;class CThingVillage;class CThingAICreature;class CGroupMemberData;class C3DVector;class CTCCreatureGroupBoastingCrowd;
class CTCBoastingCrowdControl:public CTCBase {
public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType();
 virtual ETCInterfaceType GetInterfaceType()const;
 static CTCBase* FABLE_FASTCALL Alloc(CThing&);
 static CCharString FABLE_FASTCALL StatGetClassTypeName();
 virtual CCharString GetClassTypeName()const;
 CTCBoastingCrowdControl(const CTCBoastingCrowdControl&);
 CTCBoastingCrowdControl(CThing&);
 virtual bool IsComponentSerialisable()const;
 virtual bool HasFrameUpdate()const;
 virtual void OnSetInLimbo(bool);
 virtual void OnRemove();
 virtual void OnKill();
 virtual void OnSerialise(CPersistContext&,bool);
 void Startup();
 void Shutdown();
 bool IsActive()const;
 void OnHeroApproachPodiumArea();
 void OnHeroEnterPodiumArea();
 void OnHeroLeavePodiumArea();
 void OnHeroBoast(const CQuestManager::CBoast*);
 void OnHeroAttackCrowdMember();
 void OnNoBoastsRemain();
 bool GetCrowdGroup(CThing&,CTCCreatureGroupBoastingCrowd**)const;
 void OnKillCrowd(CThing&);
private:
 bool GetCrowd(CThing&,CThing**,CTCCreatureGroupBoastingCrowd**,bool);
 bool SpawnCrowdMember(const CThing&,const CThingMarker&,CThingVillage&);
 bool Active;
 std::map<unsigned __int64,CIntelligentPointer<CThing>,std::less<unsigned __int64>,std::allocator<std::pair<unsigned __int64 const ,CIntelligentPointer<CThing> > > > HeroCrowdThings;
 bool CrowdSizesSuppressed;
public:
 virtual ~CTCBoastingCrowdControl();
 CTCBoastingCrowdControl& operator=(const CTCBoastingCrowdControl&);
};
FABLE_STATIC_ASSERT(sizeof(CTCBoastingCrowdControl)==32);
#endif
