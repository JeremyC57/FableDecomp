#ifndef FABLE_ENGINE_CTCCOMBATLEADER_H
#define FABLE_ENGINE_CTCCOMBATLEADER_H
#include "engine/CTCBase.h"
// Complete original Ego2fac3/FW326a0 owner20bytes; native setters+c/10.
class CTCCombatLeader:public CTCBase{
public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_COMBAT_LEADER;}
 virtual ETCInterfaceType GetInterfaceType() const;
 static CTCBase * FABLE_FASTCALL Alloc(CThing & p0);
 static CCharString FABLE_FASTCALL StatGetClassTypeName();
 virtual CCharString GetClassTypeName() const;
 CTCCombatLeader(const CTCCombatLeader & p0);
 CTCCombatLeader(CThing & p0);
 void SetFrameOfLastEventReaction(unsigned long frame){mLastEventReactionFrame=frame;}
 unsigned long GetFrameOfLastEventReaction();
 void SetLastEventReaction(unsigned long reaction){mLastReaction=reaction;}
 unsigned long GetLastEventReaction();
private:
 unsigned long mLastEventReactionFrame;
 unsigned long mLastReaction;
public:
 virtual ~CTCCombatLeader();
 CTCCombatLeader & operator=(const CTCCombatLeader & p0);
};
FABLE_STATIC_ASSERT(sizeof(CTCCombatLeader)==20);
#endif
