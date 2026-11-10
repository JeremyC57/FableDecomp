#ifndef FABLE_ENGINE_CTCCREATUREGROUP_H
#define FABLE_ENGINE_CTCCREATUREGROUP_H
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
