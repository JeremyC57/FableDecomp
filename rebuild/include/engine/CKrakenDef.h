#ifndef FABLE_ENGINE_CKRAKENDEF_H
#define FABLE_ENGINE_CKRAKENDEF_H
#include "engine/CSubDefClassBase.h"
#include "stlport/fable_containers.h"
class CPersistContext;
// Layout: complete original Ego3168d/FW3433f; retail124 after vector reduction.
class CKrakenDef:public CSubDefClassBase {
public:
 virtual void Copy(const CDefClassBase * p0);
 virtual unsigned long GetSizeofClass();
 static CDefClassBase * FABLE_FASTCALL Alloc();
 static CCharString FABLE_FASTCALL StatGetTypeName();
 float HeightOffset;
 float PoolRadius;
 float HeadRiseDelay;
 float HeadIdleTime;
 long LowStrikesBeforeHigh;
 long LowStrikeChargeLoops;
 long LowStrikeAttackLoops;
 long HighStrikeAttackLoops;
 long RoarLoops;
 float DelayBeforeLowBeam;
 float DelayBeforeHighBeam;
 float BeamExplosionRadius;
 float BeamTrackSpeed;
 float TargetingTrackSpeed;
 float BeamMouthDistLow;
 float BeamMouthDistHigh;
 long TicksBetweenBeamExplosions;
 std::vector<long,std::allocator<long> > TentacleDefIndex;
 float TentacleMaxRiseDelay;
 virtual void Transfer(CPersistContext & p0);
 CKrakenDef(const CKrakenDef & p0);
 CKrakenDef();
 virtual ~CKrakenDef();
 CKrakenDef & operator=(const CKrakenDef & p0);
};
FABLE_STATIC_ASSERT(sizeof(CKrakenDef)==124);
#endif
