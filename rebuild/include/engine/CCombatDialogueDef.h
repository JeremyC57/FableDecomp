#ifndef FABLE_ENGINE_CCOMBATDIALOGUEDEF_H
#define FABLE_ENGINE_CCOMBATDIALOGUEDEF_H
#include "engine/CParentDefClassBase.h"
// Complete original Ego1b46a/FW1bf74; retail140bytes.
class CCombatDialogueDef:public CParentDefClassBase{
public:
 virtual void Copy(const CDefClassBase * p0);
 virtual unsigned long GetSizeofClass();
 static CDefClassBase * FABLE_FASTCALL Alloc();
 static CCharString FABLE_FASTCALL StatGetTypeName();
 float MinTimeBetweenSpeech;
 float MinTimeBetweenSpeechRepeat;
 float MinDelayAfterBattleCry;
 long EnemySighted;
 long RunForCover;
 long Hiding;
 long Ambushing;
 long WillSeenUsed;
 long Threat;
 long Encourage;
 long FoeKnockedDownByColleague;
 long MenLosing;
 long EnemyKnockedDown;
 long RangedCombatAiming;
 long RangedCombatAimingBlocked;
 long Injured;
 long ReactionToDecapitation;
 long ColleagueInjured;
 long Scared;
 long Flee;
 virtual void Transfer(CPersistContext & p0);
 CCombatDialogueDef(const CCombatDialogueDef & p0);
 CCombatDialogueDef();
 virtual ~CCombatDialogueDef();
 CCombatDialogueDef & operator=(const CCombatDialogueDef & p0);
};
FABLE_STATIC_ASSERT(sizeof(CCombatDialogueDef)==140);
#endif
