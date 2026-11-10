#ifndef FABLE_ENGINE_CTCCREATUREGROUP_H
#define FABLE_ENGINE_CTCCREATUREGROUP_H
// Layout: complete original members from both PDB type records; retail STLport maps use12bytes.
// Evidence: 00783a30/NAMING_EVIDENCE.md; opt-in preserves the previous default.
#ifdef FABLE_RETAIL_CTCBOASTINGCROWDCONTROL
#include "engine/CTCBase.h"
#include "engine/CQuestManager.h"
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_map.h"
class CThingMarker;class CThingVillage;class CThingAICreature;class CGroupMemberData;class C3DVector;class CTCCreatureGroupBoastingCrowd;
class CTCCreatureGroup:public CTCBase {
public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType();
 virtual ETCInterfaceType GetInterfaceType()const;
 static CTCBase* FABLE_FASTCALL Alloc(CThing&);
 static CCharString FABLE_FASTCALL StatGetClassTypeName();
 virtual CCharString GetClassTypeName()const;
 CTCCreatureGroup(const CTCCreatureGroup&);
 CTCCreatureGroup(CThing&);
 void SetShutdownOnEmpty(bool);
 virtual bool AddNewMember(const CThingAICreature*);
 virtual bool RemoveMember(const CThingAICreature*);
 virtual bool DeactivateMember(const CThingAICreature*);
 virtual bool ActivateMember(const CThingAICreature*);
 bool IsMember(const CThingAICreature*)const;
 bool IsActiveMember(const CThingAICreature*)const;
 bool GetMemberData(const CThingAICreature*,const CGroupMemberData*)const;
 unsigned long GetCount()const;
 unsigned long GetActiveCount()const;
 const CThingAICreature* GetNearestMember(const C3DVector&)const;
 const CThingAICreature* GetNearestActiveMember(const C3DVector&)const;
 const CThingAICreature* GetNearestOtherMember(const CThingAICreature*)const;
 const CThingAICreature* GetNearestOtherActiveMember(const CThingAICreature*)const;
protected:
 unsigned long MemberCount;
 unsigned long ActiveMemberCount;
 bool ShutdownOnEmpty;
 std::map<unsigned __int64,CGroupMemberData,std::less<unsigned __int64>,std::allocator<std::pair<unsigned __int64 const ,CGroupMemberData> > > Members;
private:
 void ShutdownGroupIfEmpty();
public:
 virtual ~CTCCreatureGroup();
 CTCCreatureGroup& operator=(const CTCCreatureGroup&);
};
FABLE_STATIC_ASSERT(sizeof(CTCCreatureGroup)==36);
#else
// Retail opt-in (Codex pass18): Ego_r type record has MemberCount+0c,
// ActiveMemberCount+10, ShutdownOnEmpty+14, Members+18. Original map occupies
// twelve bytes with retail STLport instead of donor sixteen. No member data is
// accessed by the nearest-group comparator.
#include "engine/CTCBase.h"
class C3DVector; class CThingAICreature;
class CTCCreatureGroup : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_CREATURE_GROUP; }
    const CThingAICreature* GetNearestMember(const C3DVector&) const;
private:
    unsigned long MemberCount;
    unsigned long ActiveMemberCount;
    bool ShutdownOnEmpty;
    unsigned char PaddingBeforeMembers[3];
    unsigned char Members[12]; // std::map<unsigned __int64,CGroupMemberData>
};
#endif
#endif
