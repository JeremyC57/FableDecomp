#ifndef FABLE_ENGINE_CHITLOCATIONDEF_H
#define FABLE_ENGINE_CHITLOCATIONDEF_H
#include "engine/CParentDefClassBase.h"
#include "engine/CPhysicalPrimitiveInit.h"
#include "engine/EActionRegisteredType.h"
enum EHitLocationFlags;enum EHitLocationCodelet;
class CHitLocationDef:public CParentDefClassBase {
public:
 virtual void Copy(const CDefClassBase * p0);
 virtual unsigned long GetSizeofClass();
 static CDefClassBase * FABLE_FASTCALL Alloc();
 static CCharString FABLE_FASTCALL StatGetTypeName();
 CPhysicalPrimitiveInit PhysicalPrimitive;
 long Armour;
 EActionRegisteredType HitResponseAction;
 EActionRegisteredType HitResponseToEpicSpellsAction;
 EActionRegisteredType HitKnockdownAction;
 EActionRegisteredType HitKnockdownDeathAction;
 EActionRegisteredType BlockResponseAction;
 long HitDamageEffect;
 long HitBlockedEffect;
 long VulnerableEffect;
 EHitLocationFlags Flags;
 bool PlayGetHitResponse;
 bool DecapitateOnHit;
 bool DefendableAgainstShots;
 bool DefendableAgainstMelee;
 long Priority;
 bool Default;
 bool EnableOnCreate;
 EHitLocationCodelet HitLocationCodelet;
 virtual void Transfer(CPersistContext & p0);
 CHitLocationDef(const CHitLocationDef & p0);
 CHitLocationDef();
 virtual ~CHitLocationDef();
 CHitLocationDef & operator=(const CHitLocationDef & p0);
};
FABLE_STATIC_ASSERT(sizeof(CHitLocationDef)==124);
#endif
