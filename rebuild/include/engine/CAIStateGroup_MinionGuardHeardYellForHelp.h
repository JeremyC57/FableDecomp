#ifndef FABLE_ENGINE_CAISTATEGROUP_MINIONGUARDHEARDYELLFORHELP_H
#define FABLE_ENGINE_CAISTATEGROUP_MINIONGUARDHEARDYELLFORHELP_H
// Complete original owner from both original PDBs.
#include "engine/CAIStateGroupBase.h"
class CMessageEvent;
class CAIStateGroup_MinionGuardHeardYellForHelp:public CAIStateGroupBase{
public:
 virtual void Update();
 virtual bool IsRunnable();
 static CAIStateGroupBase * FABLE_FASTCALL Alloc();
 static CCharString FABLE_FASTCALL GetName();
private:
 const CMessageEvent * FindYellForHelpEvent();
public:
 CAIStateGroup_MinionGuardHeardYellForHelp(const CAIStateGroup_MinionGuardHeardYellForHelp & p0);
 CAIStateGroup_MinionGuardHeardYellForHelp();
 virtual ~CAIStateGroup_MinionGuardHeardYellForHelp();
 CAIStateGroup_MinionGuardHeardYellForHelp & operator=(const CAIStateGroup_MinionGuardHeardYellForHelp & p0);
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_MinionGuardHeardYellForHelp)==20);
#endif
