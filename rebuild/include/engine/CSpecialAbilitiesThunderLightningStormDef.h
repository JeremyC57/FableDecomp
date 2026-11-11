#ifndef FABLE_ENGINE_CSPECIALABILITIESTHUNDERLIGHTNINGSTORMDEF_H
#define FABLE_ENGINE_CSPECIALABILITIESTHUNDERLIGHTNINGSTORMDEF_H
// Complete original owner from both original PDBs and native offsets.
#include "engine/CParentDefClassBase.h"
#include "engine/CDefString.h"
class CSpecialAbilitiesThunderLightningStormDef:public CParentDefClassBase{
public:
 virtual void Copy(const CDefClassBase * p0);
 virtual unsigned long GetSizeofClass();
 static CDefClassBase * FABLE_FASTCALL Alloc();
 static CCharString FABLE_FASTCALL StatGetTypeName();
 long NumBeams;
 float StartRadius;
 float EndRadius;
 float SkyBeamStartTime;
 float MainBeamsStartTime;
 float BeamRotationStartSpeed;
 float BeamRotationEndSpeed;
 float TotalLifetime;
 float Damage;
 CDefString InitSound;
 CDefString LoopingSound;
 virtual void Transfer(CPersistContext & p0);
 CSpecialAbilitiesThunderLightningStormDef(const CSpecialAbilitiesThunderLightningStormDef & p0);
 CSpecialAbilitiesThunderLightningStormDef();
 virtual ~CSpecialAbilitiesThunderLightningStormDef();
 CSpecialAbilitiesThunderLightningStormDef & operator=(const CSpecialAbilitiesThunderLightningStormDef & p0);
};
FABLE_STATIC_ASSERT(sizeof(CSpecialAbilitiesThunderLightningStormDef)==104);
#endif
